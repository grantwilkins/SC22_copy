#!/bin/bash

cd /home/gfwilki/SC22_powerstudy/testing/gioTesting
#(echo "scratch=/local_scratch/pbs.${PBS_JOBID}/" && cat veloc-x.cfg) > veloc-2.cfg
#(echo "scratch=/local_scratch/pbs.${PBS_JOBID}/" && cat veloc-y.cfg) > veloc-4.cfg


for h in 1 2; do
for j in 64 80 120 128 160; do
    for k in {0..10}; do
        rm -rf /fastscratch/gfwilki/file.gio /scratch1/gfwilki/file.gio
        mpirun -np $j -N 40 ./gio_rand /scratch1/gfwilki/file.gio gio-weak.txt $h GenericIO
        #sleep 1
    done
done
done



for h in 40 80; do
for j in 64 80 120 128 160; do
    for k in {0..10}; do
        rm -rf /fastscratch/gfwilki/file.gio /scratch1/gfwilki/file.gio
        mpirun -np $j -N 40 ./gio_rand_strong /scratch1/gfwilki/file.gio gio-strong.txt $h GenericIO
        #sleep 1
    done
done
done
