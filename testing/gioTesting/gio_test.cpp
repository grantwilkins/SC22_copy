#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>
#include <string.h>
#include <mpi.h>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <cmath>

#include "/home/gfwilki/software/genericio/GenericIO.h"
#include "papi.h"

#undef NDEBUG
#include <assert.h>

using namespace gio;
#define MAX_powercap_EVENTS 64

int main(int argc, char *argv[]) {
    

    int EventSet = PAPI_NULL;
    long long *values;
    int num_events=0;
    int code;
    char event_names[MAX_powercap_EVENTS][PAPI_MAX_STR_LEN];
    char event_descrs[MAX_powercap_EVENTS][PAPI_MAX_STR_LEN];
    char units[MAX_powercap_EVENTS][PAPI_MIN_STR_LEN];
    int data_type[MAX_powercap_EVENTS];
    int r,i;
    int retval = 0;

    const PAPI_component_info_t *cmpinfo = NULL;
    PAPI_event_info_t evinfo;
    long long before_time,after_time;
    double elapsed_time;


    if(argc != 5){
        std::cout << "Required inputs not provided! USAGE: " << argv[0] << "</PATH/TO/CKPT/CKPT_FILE.gio> <FILE_FOR_STATS>.txt <SIZE in GB> <CKPT_METHOD_ID>" << std::endl;
        exit(2);
    }
    std::size_t PROC_SIZE = 1<<30;
    std::string filename = argv[2];
    PROC_SIZE*=atoi(argv[3]);
    std::string strategy = argv[4];



    /* MPI INIT */
    int provided, N, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &N);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);    

    
    /* PAPI INIT */
    assert(PAPI_library_init( PAPI_VER_CURRENT ) == PAPI_VER_CURRENT);

    /* PAPI FIND POWERCAP CID */

    int numcmp = PAPI_num_components();
    int cid, powercap_cid = -1;
    for( cid=0; cid<numcmp; cid++ ) 
    {
        cmpinfo = PAPI_get_component_info(cid);
        assert(cmpinfo != NULL);
        if ( strstr( cmpinfo->name,"powercap" ) ) 
        {
            powercap_cid=cid;
            break;
        }
    }
    assert(cid != numcmp);
    /* PAPI CREATE EVENTSET */
    assert(PAPI_create_eventset(&EventSet) == PAPI_OK); 

    /* PAPI FIND ALL EVENTS */
    code = PAPI_NATIVE_MASK;
    r = PAPI_enum_cmp_event( &code, PAPI_ENUM_FIRST, powercap_cid );
    while ( r == PAPI_OK ) {
        assert(PAPI_event_code_to_name( code, event_names[num_events] ) == PAPI_OK);

        assert( PAPI_get_event_info( code,&evinfo ) == PAPI_OK);

        strncpy( event_descrs[num_events],evinfo.long_descr,sizeof( event_descrs[0] )-1 );
        strncpy( units[num_events],evinfo.units,sizeof( units[0] )-1 );
        // buffer must be null terminated to safely use strstr operation on it below
        units[num_events][sizeof( units[0] )-1] = '\0';
        data_type[num_events] = evinfo.data_type;
        retval = PAPI_add_event( EventSet, code );

        if ( retval != PAPI_OK )
            break; /* We've hit an event limit */
        num_events++;
        
        r = PAPI_enum_cmp_event( &code, PAPI_ENUM_EVENTS, powercap_cid );
    }

    // Values is where we will have all of our different
    // information about energy
    values= (long long *)calloc( num_events,sizeof( long long ) );


    /* PAPI STARTS COLLECTING*/
    
    ssize_t total = N * PROC_SIZE;
    if(rank == 0)
        std::cout << "total checkpoint size = " << (double)total / (1<<30) << " GB" << std::endl;
    char *ptr = (char *)malloc(PROC_SIZE * sizeof(char));
    assert(ptr != NULL);
    memset(ptr, (char)rank, PROC_SIZE);
    MPI_Barrier(MPI_COMM_WORLD);

    //set up GIO 
    GenericIO::setNaturalDefaultPartition();
    unsigned Method = GenericIO::FileIOPOSIX;
    char *mpiName = argv[1];
    GenericIO GIO(MPI_COMM_WORLD, mpiName, Method);
    GIO.setNumElems(PROC_SIZE);
    //ptr = (char *)realloc(ptr,PROC_SIZE + GIO.requestedExtraSpace());
    GIO.addVariable("rank", ptr, GenericIO::VarHasExtraSpace);
    assert(PAPI_start(EventSet) == PAPI_OK);
    double start = MPI_Wtime();
    GIO.write();
    double end = MPI_Wtime() - start;

    //PAPI STOP
    assert(PAPI_stop(EventSet, values) == PAPI_OK);

    double max_total, avg_total;
    MPI_Reduce(&end, &max_total, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&end, &avg_total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    avg_total /= N;

    if(rank == 0) {
        //PAPI
        double cpu_total_energy = 0.0, dram_total_energy = 0.0, 
            cpu_avg_power = 0.0, dram_avg_power = 0.0;
        for( i=0; i<num_events; i++ ) {
                //std::cout << event_names[i] << " " << values[i]/1.0e6 << std::endl;
            if ( strstr( event_names[i],"ENERGY_UJ" ) ) {
                if ( data_type[i] == PAPI_DATATYPE_UINT64 ) {
                    if(strstr(event_names[i], "SUBZONE")) 
                        dram_total_energy+=values[i];
                    else
                        cpu_total_energy+=values[i];                    
                    /*printf( "%-45s%-20s%4.6f J (Average Power %.1fW)\n",
                            event_names[i], event_descrs[i],
                            ( double )values[i]/1.0e6,
                            ( ( double )values[i]/1.0e6 )/maxFlushTime);*/
               }
           }
        }
        cpu_total_energy /= 1.0e6;
        dram_total_energy /= 1.0e6;
        cpu_avg_power = cpu_total_energy/avg_total;
        dram_avg_power = dram_total_energy/avg_total;
        //statistics print in the following order: STRATEGY, CHKPT PHASE, NUM_PROCS, TOTAL_CHKPT_SIZE, AVG TIME TO WRITE, MAX TIME TO WRITE, AVG THROUGHPUT, MIN THROUGHPUT
        std::ofstream outputfile;
        outputfile.open(filename.c_str(), std::ofstream::out | std::ofstream::app);
        outputfile << strategy << "," << mpiName << "," << N << "," 
                   << total/(1<<30) << "," << avg_total << "," << max_total << "," 
                   << (total / (1<<30)) / avg_total << "," << (total / (1<<30)) / max_total << ","
                   << cpu_total_energy << "," << dram_total_energy << "," 
                   << cpu_avg_power << "," << dram_avg_power <<"\n";
        outputfile.close();
    }

    MPI_Finalize();
    return 0;
}
