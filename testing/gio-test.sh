#!/bin/bash

cd /home/gfwilki/SC22_powerstudy/testing/gioTesting
#(echo "scratch=/local_scratch/pbs.${PBS_JOBID}/" && cat veloc-x.cfg) > veloc-2.cfg
#(echo "scratch=/local_scratch/pbs.${PBS_JOBID}/" && cat veloc-y.cfg) > veloc-4.cfg

for h in 1 2 4; do
#for i in {1..4}; do
    for j in 1 2 4 8 16 32 40; do
        for k in {0..10}; do
            rm -rf /fastscratch/gfwilki/file.gio /scratch1/gfwilki/file.gio
            mpirun -np $j ./gio_rand /fastscratch/gfwilki/file.gio stats-weak-gio.txt $h 1
            #sleep 1
        done
    done
done
#done
for h in 1 2 4; do
for j in 1 2 4 8 16 32 40; do
    for k in {0..10}; do
        rm -rf /fastscratch/gfwilki/file.gio /scratch1/gfwilki/file.gio
        mpirun -np $j ./gio_rand /scratch1/gfwilki/file.gio stats-weak-gio.txt $h 1
        #sleep 1
    done
done
done

for h in 40 80 160; do
for j in 1 2 4 8 16 32 40; do
    for k in {0..10}; do
        rm -rf /fastscratch/gfwilki/file.gio /scratch1/gfwilki/file.gio
        mpirun -np $j ./gio_rand_strong /fastscratch/gfwilki/file.gio stats-strong-gio.txt $h 1
        #sleep 1
    done
done
done

for h in 40 80 160; do
for j in 1 2 4 8 16 32 40; do
    for k in {0..10}; do
        rm -rf /fastscratch/gfwilki/file.gio /scratch1/gfwilki/file.gio
        mpirun -np $j ./gio_rand_strong /scratch1/gfwilki/file.gio stats-strong-gio.txt $h 1
        #sleep 1
    done
done
done
