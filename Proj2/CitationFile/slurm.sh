#!/bin/sh

#SBATCH --job-name=RyanCitationGenius
##SBATCH --nodes=25
##SBATCH --ntasks-per-node=10
#SBATCH --ntasks=1
#SBATCH --mem=4gb
#SBATCH --time=120:50:20
#SBATCH --output=out/%j.log

module load mpi/mpich-3.2-x86_64

##mpicc ~/Ryan/COSC420/

mpirun /mnt/GrantDawson/public/Ryan/COSC420/Project2arXivSearchEngine/CitationFile/writematrix
