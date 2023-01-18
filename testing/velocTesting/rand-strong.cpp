#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>
#include <string>
#include <mpi.h>
#include <fstream>
#include <iostream>
#include "/home/gfwilki/software/veloc/include/veloc.h"
// this examples uses asserts so they need to be activated
#undef NDEBUG
#include <assert.h>
extern "C" {
#include "papi.h"
}

#define MAX_powercap_EVENTS 64
size_t PROC_SIZE = 1<<30;

int main(int argc, char *argv[]) {

    int EventSet = PAPI_NULL, EventSet2 = PAPI_NULL;
    long long *values, *values2;
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


    if(argc != 5)
    {
        std::cout << "Required inputs not provided! USAGE: " << argv[0] << "<CONFIG>.cfg <FILE_FOR_STATS>.txt GB MULTIPLIER <CKPT_METHOD_ID>" << std::endl;
        exit(2);
    }
    std::string filename = argv[2];
    PROC_SIZE*=atoi(argv[3]);
    std::string strategy = argv[4];

    // FILE I/O FOR LOCATIONS
    std::string temp_line, scratch_loc, persistent_loc;
    int start_str = 0, end_str = 0;
    std::ifstream cfg (argv[1]);
    assert(cfg.is_open());
    getline(cfg,temp_line);
    scratch_loc = temp_line.substr(8, temp_line.length() - 1);
    getline(cfg,temp_line);
    persistent_loc = temp_line.substr(11, temp_line.length() - 1);
    cfg.close();
 
    /* MPI INIT */
    int provided, N, rank;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    assert(provided == MPI_THREAD_MULTIPLE);
    MPI_Comm_size(MPI_COMM_WORLD, &N);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    /* PAPI INIT */
    //printf("%d\n", PAPI_VER_CURRENT);
    //printf("%d\n", PAPI_library_init(PAPI_VER_CURRENT));
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
    values2 = (long long*) calloc(num_events, sizeof(long long));

    /* PAPI STARTS COLLECTING*/
        
    ssize_t total =  PROC_SIZE;
    if(rank == 0)
        std::cout << "total checkpoint size = " << (double)total / (1<<30) << " GB" << std::endl;
    char *ptr = (char *)malloc(PROC_SIZE * sizeof(char) / N);
    assert(ptr != NULL);
    memset(ptr, (char)rank, PROC_SIZE / N);
    MPI_Barrier(MPI_COMM_WORLD);
    
    std::cout << "initializing VELOC..." << std::endl;
    if(VELOC_Init(MPI_COMM_WORLD, argv[1]) != VELOC_SUCCESS) 
    {
		printf("Error initializing VELOC! Aborting...\n");
		exit(2);
	}
	std::cout << "done..start mem protect..." << std::endl;
	VELOC_Mem_protect(rank, ptr, PROC_SIZE/N, sizeof(char));
	MPI_Barrier(MPI_COMM_WORLD);
    std::cout << "done...start flush..." << std::endl;
    assert(PAPI_start(EventSet) == PAPI_OK);
    double start = MPI_Wtime();
    assert(VELOC_Checkpoint("veloc_test", N) == VELOC_SUCCESS);
    double local_end = MPI_Wtime() - start;
    assert(PAPI_read(EventSet,values2) == PAPI_OK);
    assert(VELOC_Checkpoint_wait() == VELOC_SUCCESS);
    double flush_end = MPI_Wtime() - start;
    std::cout << "done" << std::endl;
    //PAPI STOP
    assert(PAPI_stop(EventSet, values) == PAPI_OK);

    double avgLocalTime, maxLocalTime, avgFlushTime, maxFlushTime;
    MPI_Reduce(&local_end, &avgLocalTime, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&flush_end, &avgFlushTime, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    avgLocalTime /= N;
    avgFlushTime /= N;
    MPI_Reduce(&local_end, &maxLocalTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&flush_end, &maxFlushTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    

    if(rank == 0)
    {
        //PAPI
        double cpu_total_energy_local = 0.0, dram_total_energy_local = 0.0,
            cpu_avg_power_local=0.0, dram_avg_power_local = 0.0;
        double cpu_total_energy_flush = 0.0, dram_total_energy_flush = 0.0, 
            cpu_avg_power_flush = 0.0, dram_avg_power_flush = 0.0;
        for( i=0; i<num_events; i++ ) {
            if ( strstr( event_names[i],"ENERGY_UJ" ) ) {
                if ( data_type[i] == PAPI_DATATYPE_UINT64 ) {
                    if(strstr(event_names[i], "SUBZONE")) {
                        dram_total_energy_flush+=values[i];
                        dram_total_energy_local+=values2[i];
                    }
                    else {
                        cpu_total_energy_flush+=values[i]; 
                        cpu_total_energy_local+=values2[i];
                    }
                    /*printf( "%-45s%-20s%4.6f J (Average Power %.1fW)\n",
                            event_names[i], event_descrs[i],
                            ( double )values[i]/1.0e6,
                            ( ( double )values[i]/1.0e6 )/maxFlushTime);*/
               }
           }
        }
        cpu_total_energy_flush /= 1.0e6;
        cpu_total_energy_local /= 1.0e6;
        dram_total_energy_flush /= 1.0e6;
        dram_total_energy_local /= 1.0e6;
        //cpu_total_energy_flush -= dram_total_energy_flush;
        //cpu_total_energy_local -= dram_total_energy_local;
        cpu_avg_power_local = cpu_total_energy_local / avgLocalTime;
        cpu_avg_power_flush = cpu_total_energy_flush/avgFlushTime;
        dram_avg_power_local = dram_total_energy_local/avgLocalTime;
        dram_avg_power_flush = dram_total_energy_flush/avgFlushTime;
        //statistics print in the following order: STRATEGY, CHKPT PHASE, NUM_PROCS, TOTAL_CHKPT_SIZE, AVG TIME TO WRITE, MAX TIME TO WRITE, AVG THROUGHPUT, MIN THROUGHPUT
        std::ofstream outputfile;
        outputfile.open(filename.c_str(), std::ofstream::out | std::ofstream::app);
        outputfile << strategy << ",local," << scratch_loc << "," << persistent_loc << "," <<  N << "," 
                << total/(1<<30) << "," << avgLocalTime << "," << maxLocalTime << "," 
                << (total / (1<<30)) / avgLocalTime << "," << (total / (1<<30)) / maxLocalTime << ","
                << cpu_total_energy_local << "," << dram_total_energy_local << "," 
                << cpu_avg_power_local << "," << dram_avg_power_local <<"\n";
        outputfile << strategy << ",flush," << scratch_loc << "," << persistent_loc <<","<< N << "," 
                << total/(1<<30) << "," << avgFlushTime << "," << maxFlushTime << "," 
                << (total / (1<<30)) / avgFlushTime << "," << (total / (1<<30)) / maxFlushTime << ","
                << cpu_total_energy_flush << "," << dram_total_energy_flush << "," 
                << cpu_avg_power_flush << "," << dram_avg_power_flush <<"\n";
        outputfile.close();
    
    

    


    
    }

    free(ptr);
    VELOC_Finalize(0); // no clean up
    MPI_Finalize();
    return 0;
}
