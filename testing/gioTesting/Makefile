MPIDIR = /home/gfwilki/software/genericio/mpi
CC = gcc

OPENMPFLAG = -fopenmp
MPICC = mpicc
MPICXX = mpicxx

MPI_CFLAGS := -g -O0 $(OPENMPFLAG) $(G99FLAG)
MPI_CFLAGS := -g -O0 -DPAPI_NUM_COMP=3 $(OPENMPFLAG)
MPI_CPPFLAGS := $(BASE_CPPFLAGS)

PAPI_INC=/home/gfwilki/papi/src/install/include 
PAPI_INC2 =/home/gfwilki/papi/src
PAPI_LIB=/home/gfwilki/papi/src/libpapi.a

BLOSC_O := \
	thirdparty/blosc/blosc.o \
	thirdparty/blosc/blosclz.o \
	thirdparty/blosc/shuffle.o \
	thirdparty/blosc/bitshuffle-generic.o \
	thirdparty/blosc/shuffle-generic.o \
	thirdparty/blosc/internal-complibs/zlib-1.2.8/gzwrite.o \
	thirdparty/blosc/internal-complibs/zlib-1.2.8/crc32.o \
	thirdparty/blosc/internal-complibs/zlib-1.2.8/inffast.o \
	thirdparty/blosc/internal-complibs/zlib-1.2.8/zutil.o \
	thirdparty/blosc/internal-complibs/zlib-1.2.8/infback.o \
	thirdparty/blosc/internal-complibs/zlib-1.2.8/deflate.o \
	thirdparty/blosc/internal-complibs/zlib-1.2.8/inflate.o \
	thirdparty/blosc/internal-complibs/zlib-1.2.8/gzread.o \
	thirdparty/blosc/internal-complibs/zlib-1.2.8/gzlib.o \
	thirdparty/blosc/internal-complibs/zlib-1.2.8/gzclose.o \
	thirdparty/blosc/internal-complibs/zlib-1.2.8/uncompr.o \
	thirdparty/blosc/internal-complibs/zlib-1.2.8/compress.o \
	thirdparty/blosc/internal-complibs/zlib-1.2.8/inftrees.o \
	thirdparty/blosc/internal-complibs/zlib-1.2.8/trees.o \
	thirdparty/blosc/internal-complibs/zlib-1.2.8/adler32.o \
	thirdparty/blosc/internal-complibs/lz4-1.7.2/lz4.o \
	thirdparty/blosc/internal-complibs/lz4-1.7.2/lz4hc.o \
	thirdparty/blosc/internal-complibs/zstd-0.7.4/legacy/zstd_v01.o \
	thirdparty/blosc/internal-complibs/zstd-0.7.4/legacy/zstd_v02.o \
	thirdparty/blosc/internal-complibs/zstd-0.7.4/legacy/zstd_v03.o \
	thirdparty/blosc/internal-complibs/zstd-0.7.4/legacy/zstd_v06.o \
	thirdparty/blosc/internal-complibs/zstd-0.7.4/legacy/zstd_v04.o \
	thirdparty/blosc/internal-complibs/zstd-0.7.4/legacy/zstd_v05.o \
	thirdparty/blosc/internal-complibs/zstd-0.7.4/compress/fse_compress.o \
	thirdparty/blosc/internal-complibs/zstd-0.7.4/compress/zstd_compress.o \
	thirdparty/blosc/internal-complibs/zstd-0.7.4/compress/huf_compress.o \
	thirdparty/blosc/internal-complibs/zstd-0.7.4/compress/zbuff_compress.o \
	thirdparty/blosc/internal-complibs/zstd-0.7.4/common/entropy_common.o \
	thirdparty/blosc/internal-complibs/zstd-0.7.4/common/xxhash.o \
	thirdparty/blosc/internal-complibs/zstd-0.7.4/common/zstd_common.o \
	thirdparty/blosc/internal-complibs/zstd-0.7.4/common/fse_decompress.o \
	thirdparty/blosc/internal-complibs/zstd-0.7.4/dictBuilder/zdict.o \
	thirdparty/blosc/internal-complibs/zstd-0.7.4/dictBuilder/divsufsort.o \
	thirdparty/blosc/internal-complibs/zstd-0.7.4/decompress/zstd_decompress.o \
	thirdparty/blosc/internal-complibs/zstd-0.7.4/decompress/huf_decompress.o \
	thirdparty/blosc/internal-complibs/zstd-0.7.4/decompress/zbuff_decompress.o \
	thirdparty/blosc/internal-complibs/snappy-1.1.1/snappy-c.o \
	thirdparty/blosc/internal-complibs/snappy-1.1.1/snappy.o \
	thirdparty/blosc/internal-complibs/snappy-1.1.1/snappy-sinksource.o \
	thirdparty/blosc/internal-complibs/snappy-1.1.1/snappy-stubs-internal.o \
	thirdparty/SZ/sz/src/ByteToolkit.o \
	thirdparty/SZ/sz/src/dataCompression.o \
	thirdparty/SZ/sz/src/DynamicIntArray.o \
	thirdparty/SZ/sz/src/iniparser.o \
	thirdparty/SZ/sz/src/CompressElement.o \
	thirdparty/SZ/sz/src/DynamicByteArray.o \
	thirdparty/SZ/sz/src/rw.o \
	thirdparty/SZ/sz/src/TightDataPointStorageI.o \
	thirdparty/SZ/sz/src/TightDataPointStorageD.o \
	thirdparty/SZ/sz/src/TightDataPointStorageF.o \
	thirdparty/SZ/sz/src/conf.o \
	thirdparty/SZ/sz/src/DynamicDoubleArray.o \
	thirdparty/SZ/sz/src/TypeManager.o \
	thirdparty/SZ/sz/src/dictionary.o \
	thirdparty/SZ/sz/src/DynamicFloatArray.o \
	thirdparty/SZ/sz/src/VarSet.o \
	thirdparty/SZ/sz/src/callZlib.o \
	thirdparty/SZ/sz/src/Huffman.o \
	thirdparty/SZ/sz/src/sz_float.o \
	thirdparty/SZ/sz/src/sz_double.o \
	thirdparty/SZ/sz/src/sz_int8.o \
	thirdparty/SZ/sz/src/sz_int16.o \
	thirdparty/SZ/sz/src/sz_int32.o \
	thirdparty/SZ/sz/src/sz_int64.o \
	thirdparty/SZ/sz/src/sz_uint8.o \
	thirdparty/SZ/sz/src/sz_uint16.o \
	thirdparty/SZ/sz/src/sz_uint32.o \
	thirdparty/SZ/sz/src/sz_uint64.o \
	thirdparty/SZ/sz/src/szd_uint8.o \
	thirdparty/SZ/sz/src/szd_uint16.o \
	thirdparty/SZ/sz/src/szd_uint32.o \
	thirdparty/SZ/sz/src/szd_uint64.o \
	thirdparty/SZ/sz/src/szd_float.o \
	thirdparty/SZ/sz/src/szd_double.o \
	thirdparty/SZ/sz/src/szd_int8.o \
	thirdparty/SZ/sz/src/szd_int16.o \
	thirdparty/SZ/sz/src/szd_int32.o \
	thirdparty/SZ/sz/src/szd_int64.o \
	thirdparty/SZ/sz/src/utility.o \
	thirdparty/SZ/sz/src/sz.o \
	thirdparty/SZ/sz/src/sz_float_pwr.o \
	thirdparty/SZ/sz/src/sz_double_pwr.o \
	thirdparty/SZ/sz/src/szd_float_pwr.o \
	thirdparty/SZ/sz/src/szd_double_pwr.o \
	thirdparty/SZ/sz/src/sz_double_ts.o \
	thirdparty/SZ/sz/src/sz_float_ts.o \
	thirdparty/SZ/sz/src/szd_double_ts.o \
	thirdparty/SZ/sz/src/szd_float_ts.o

MPI_BLOSC_O := $(addprefix $(MPIDIR)/,$(BLOSC_O))
GIO_O := /home/gfwilki/software/genericio/build/lib/libgenericio_mpi.a
all: gio_rand gio_rand_strong

gio_rand: $(GIO_O) $(MPI_BLOSC_O)
	$(MPICXX) $(MPI_CFLAGS) -I$(PAPI_INC) -I$(PAPI_INC2) -fopenmp -o gio_rand gio_test.cpp $(GIO_O) $(MPI_BLOSC_O) $(PAPI_LIB) -lpapi 

gio_rand_strong: $(GIO_O) $(MPI_BLOSC_O)
	$(MPICXX) $(MPI_CFLAGS) -I$(PAPI_INC) -I$(PAPI_INC2) -fopenmp -o gio_rand_strong gio_test_strong.cpp $(GIO_O) $(MPI_BLOSC_O) $(PAPI_LIB) -lpapi 

clean: 
	rm -f gio_rand*


