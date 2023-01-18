#!/bin/bash

cd /home/gfwilki/SC22_powerstudy/testing/velocTesting
(echo "scratch=/local_scratch/pbs.${PBS_JOBID}/" && cat veloc-x.cfg) > veloc-2.cfg
(echo "scratch=/local_scratch/pbs.${PBS_JOBID}/" && cat veloc-y.cfg) > veloc-4.cfg

for h in 1 2; do
for i in {1..2}; do
    for j in 64 80 120 128 160; do
        for k in {0..10}; do
            rm -rf /dev/shm/veloc* /fastscratch/gfwilki/veloc* /local_scratch/pbs.${PBS_JOBID}/veloc* /scratch1/gfwilki/veloc*
            mpirun -np $j -N 40 ./rand veloc-$i.cfg veloc-weak.txt $h VELOC-async
        done
    done
done
done

for h in 1 2; do
    for j in 64 80 120 128 160; do
        for k in {0..10}; do
            rm -rf /dev/shm/veloc* /fastscratch/gfwilki/veloc* /local_scratch/pbs.${PBS_JOBID}/veloc* /scratch1/gfwilki/veloc*
            mpirun -np $j -N 40 ./rand veloc-sync-1.cfg veloc-weak.txt $h VELOC-sync
        done
    done
done
