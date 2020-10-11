#!/bin/sh

#SBATCH --job-name=GrantProject1Problem160
#SBATCH --nodes=24
#SBATCH --ntasks-per-node=24
#SBATCH --mem=4gb
#SBATCH --time=48:25:00
#SBATCH --output=out/%j.log

module load mpi/mpich-3.2-x86_64

make main

mpirun ~/COSC420/Proj1/Problem160/SingleFile/main


