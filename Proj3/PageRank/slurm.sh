#!/bin/sh

#SBATCH --job-name=WELOVEPAGERANK
#SBATCH --nodes=20
#SBATCH --ntasks-per-node=20
##SBATCH --ntasks=
#SBATCH --mem=3gb
#SBATCH --time=120:50:20
#SBATCH --output=out/%j.log

module load mpi/mpich-3.2-x86_64

##mpicc ~/Ryan/COSC420/

mpirun ~/Repositories/COSC420/Project3P2Continuation/PageRank/main
