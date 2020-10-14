#!/bin/sh

#SBATCH --job-name=RosiakProject1Prob282
#SBATCH --nodes=8
#SBATCH --ntasks-per-node=20        # will run with -n 150 tasks
#SBATCH --mem=4gb
#SBATCH --time=00:15:00      # Time limit in the form hh::mm::ss
#SBATCH --output=out/%j.log

module load mpi/mpich-3.2-x86_64

mpirun ~/Repositories/COSC420/Project-1-ProjectEuler/Problem282Rosiak/main # note: don't need the -n argument, etc, reads from above
