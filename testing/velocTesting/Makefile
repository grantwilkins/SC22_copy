CC = gcc

MPICC = mpicc
MPICXX = mpicxx

MPI_CFLAGS := -g -O0 $(OPENMPFLAG) $(G99FLAG)
MPI_CFLAGS := -g -O0 -DPAPI_NUM_COMP=3 $(OPENMPFLAG)
MPI_CPPFLAGS := $(BASE_CPPFLAGS)

VELOC_INC=/home/gfwilki/software/veloc/include
VELOC_LIB=/home/gfwilki/software/veloc/lib64
PAPI_INC=/home/gfwilki/papi/src/install/include 
PAPI_INC2 =/home/gfwilki/papi/src
PAPI_LIB=/home/gfwilki/papi/src/libpapi.a

all: rand rand-strong

rand: rand.cpp
	mpic++ $(MPI_CFLAGS) -I$(VELOC_INC) -I$(PAPI_INC) -I$(PAPI_INC2) -std=c++11 rand.cpp -o rand -L$(VELOC_LIB) $(PAPI_LIB) -lveloc-client -lpapi

rand-strong: rand-strong.cpp
	mpic++ $(MPI_CFLAGS) -I$(VELOC_INC) -I$(PAPI_INC) -I$(PAPI_INC2) -std=c++11 rand-strong.cpp -o rand-strong -L$(VELOC_LIB) $(PAPI_LIB) -lveloc-client -lpapi

clean:
	rm -f rand rand-strong
