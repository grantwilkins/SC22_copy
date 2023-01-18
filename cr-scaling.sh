#!/bin/bash

cd /home/gfwilki/SC22_powerstudy/testing/velocTesting
(echo "scratch=/local_scratch/pbs.${PBS_JOBID}/" && cat veloc-x.cfg) > veloc-2.cfg
#(echo "scratch=/local_scratch/pbs.${PBS_JOBID}/" && cat veloc-y.cfg) > veloc-4-strong.cfg

for h in 1 2; do
for i in {1..2}; do
    for j in 64 80 120 128 160; do
        for k in {0..10}; do
            let n=$j/2
            rm -rf /dev/shm/veloc* /fastscratch/gfwilki/veloc* /local_scratch/pbs.${PBS_JOBID}/veloc* /scratch1/gfwilki/veloc*
            mpirun -np $j -N $n ./rand veloc-$i.cfg scaling-veloc-weak.txt $h VELOC-async
        done
    done
done
done

for h in 1 2; do
    for j in 64 80 120 128 160; do
        for k in {0..10}; do
            let n=$j/2
            rm -rf /dev/shm/veloc* /fastscratch/gfwilki/veloc* /local_scratch/pbs.${PBS_JOBID}/veloc* /scratch1/gfwilki/veloc*
            mpirun -np $j -N $n ./rand veloc-sync-1.cfg scaling-veloc-weak.txt $h VELOC-sync
        done
    done
done


#STRONG SCALING VELOC

for h in 40 80; do
for i in {1..2}; do
    for j in 64 80 120 128; do
        for k in {0..10}; do
            let n=$j/2
            rm -rf /dev/shm/veloc* /fastscratch/gfwilki/veloc* /local_scratch/pbs.${PBS_JOBID}/veloc* /scratch1/gfwilki/veloc*
            mpirun -np $j -N $n ./rand-strong veloc-$i.cfg scaling-veloc-strong.txt $h VELOC-async
        done
    done
done
done

for h in 40 80; do
    for j in 64 80 120 128; do
        for k in {0..10}; do
            let n=$j/2
            rm -rf /dev/shm/veloc* /fastscratch/gfwilki/veloc* /local_scratch/pbs.${PBS_JOBID}/veloc* /scratch1/gfwilki/veloc*
            mpirun -np $j -N $n ./rand-strong veloc-sync-1.cfg scaling-veloc-strong.txt $h VELOC-sync
        done
    done
done



cd /home/gfwilki/SC22_powerstudy/testing/gioTesting

for h in 1 2; do
    for j in 64 80 120 128; do
        for k in {0..10}; do
            let n=$j/2
            rm -rf /fastscratch/gfwilki/file.gio /scratch1/gfwilki/file.gio
            mpirun -np $j -N $n ./gio_rand /scratch1/gfwilki/file.gio scaling-gio-weak.txt $h GenericIO
        done
    done
done



for h in 40 80; do
    for j in 64 80 120 128; do
        for k in {0..10}; do
            let n=$j/2
            rm -rf /fastscratch/gfwilki/file.gio /scratch1/gfwilki/file.gio
            mpirun -np $j -N $n ./gio_rand_strong /scratch1/gfwilki/file.gio scaling-gio-strong.txt $h GenericIO
        done
    done
done