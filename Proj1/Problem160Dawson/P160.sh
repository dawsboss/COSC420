#!/bin/sh

#SBATCH --job-name=GrantProject1Problem160
##SBATCH --nodes=25
##SBATCH --ntasks-per-node=10
#SBATCH --ntasks=100
#SBATCH --mem=2gb
#SBATCH --time=05:05:00
#SBATCH --output=out/%j.log

module load mpi/mpich-3.2-x86_64

mpicc ~/Ryan/COSC420/Project-1-ProjectEuler/Problem160/Template.c

mpirun ~/Ryan/COSC420/Project-1-ProjectEuler/Problem160/a.out


