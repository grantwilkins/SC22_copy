#!/bin/bash

cd /home/gfwilki/SC22_powerstudy/testing/velocTesting
(echo "scratch=/local_scratch/pbs.${PBS_JOBID}/" && cat veloc-x.cfg) > veloc-2-strong.cfg
(echo "scratch=/local_scratch/pbs.${PBS_JOBID}/" && cat veloc-y.cfg) > veloc-4-strong.cfg

for h in 40 80 160; do
for i in {1..4}; do
    for j in 1 2 4 8 16 32 40; do
        for k in {0..10}; do
            rm -rf /dev/shm/veloc* /fastscratch/gfwilki/veloc* /local_scratch/pbs.${PBS_JOBID}/veloc* /scratch1/gfwilki/veloc*
            mpirun -np $j ./rand-strong veloc-$i-strong.cfg stats-strong-async.txt $h -1
        done
    done
done
done

for h in 40 80 160; do
for i in 1 2; do
    for j in 1 2 4 8 16 32 40; do
        for k in {0..10}; do
            rm -rf /dev/shm/veloc* /fastscratch/gfwilki/veloc* /local_scratch/pbs.${PBS_JOBID}/veloc* /scratch1/gfwilki/veloc*
            mpirun -np $j ./rand-strong veloc-sync-$i.cfg stats-strong-sync.txt $h -2
        done
    done
done
done
