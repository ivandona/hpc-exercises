#!/bin/bash
#PBS -l select=2:ncpus=1:mem=2gb
#PBS -l walltime=00:10:00
#PBS -q short_cpuQ
#PBS -J 1-4

# Array of placement strategies
PLACEMENTS=("pack" "scatter" "pack:excl" "scatter:excl")

# Assign the placement strategy based on the job array index
PLACEMENT=${PLACEMENTS[$PBS_ARRAY_INDEX-1]}

# Modify select line to use the chosen placement strategy
#PBS -l place=$PLACEMENT

module load mpich-3.2
mpirun.actual -n 2 ./hpc-exercises/benchmark-p2p/benchmark-p2p
