#!/bin/bash

cd /home/gfwilki/SC22_powerstudy/testing/velocTesting
(echo "scratch=/local_scratch/pbs.${PBS_JOBID}/" && cat veloc-x.cfg) > veloc-2.cfg
(echo "scratch=/local_scratch/pbs.${PBS_JOBID}/" && cat veloc-y.cfg) > veloc-4.cfg

for h in 1 2 4; do
for i in {1..4}; do
    for j in 1 2 4 8 16 32 40; do
        for k in {0..10}; do
            rm -rf /dev/shm/veloc* /fastscratch/gfwilki/veloc* /local_scratch/pbs.${PBS_JOBID}/veloc* /scratch1/gfwilki/veloc*
            mpirun -np $j ./rand veloc-$i.cfg stats-weak-async.txt $h 0
        done
    done
done
done

for h in 1 2 4; do
for i in 1 2; do
    for j in 1 2 4 8 16 32 40; do
        for k in {0..10}; do
            rm -rf /dev/shm/veloc* /fastscratch/gfwilki/veloc* /local_scratch/pbs.${PBS_JOBID}/veloc* /scratch1/gfwilki/veloc*
            mpirun -np $j ./rand veloc-sync-$i.cfg stats-weak-sync.txt $h 1
        done
    done
done
done
