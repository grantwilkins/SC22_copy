#!/bin/bash

cd /home/gfwilki/SC22_powerstudy/testing/velocTesting
(echo "scratch=/local_scratch/pbs.${PBS_JOBID}/" && cat veloc-x.cfg) > veloc-2-strong.cfg
#(echo "scratch=/local_scratch/pbs.${PBS_JOBID}/" && cat veloc-y.cfg) > veloc-4-strong.cfg

for h in 40 80; do
for i in {1..2}; do
    for j in 1 2 4 8 16 32 40; do
        for k in {0..10}; do
            rm -rf /dev/shm/veloc* /fastscratch/gfwilki/veloc* /local_scratch/pbs.${PBS_JOBID}/veloc* /scratch1/gfwilki/veloc*
            mpirun -np $j ./rand-strong veloc-$i-strong.cfg veloc-strong.txt $h VELOC-async
        done
    done
done
done

for h in 40 80; do
    for j in 1 2 4 8 16 32 40; do
        for k in {0..10}; do
            rm -rf /dev/shm/veloc* /fastscratch/gfwilki/veloc* /local_scratch/pbs.${PBS_JOBID}/veloc* /scratch1/gfwilki/veloc*
            mpirun -np $j ./rand-strong veloc-sync-1.cfg veloc-strong.txt $h VELOC-sync
        done
    done
done

