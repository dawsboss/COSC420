#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<stdbool.h>

int ackermann(int n, int m) {
    if (m == 0) {
        return n + 1;
    } else if (m > 0 && n == 0) {
        return ackermann(m - 1, 1);
    } else if (m > 0 && n > 0) {
        return ackermann(m - 1, ackermann(m, n - 1));
    }
    puts("This shouldnt happen");
    return 1;
}

int main(int argc, char** argv) {
    
    printf("A(1,0) = %d\n", ackermann(1,0));

    printf("A(2,2) = %d\n", ackermann(2,2));

    printf("A(3,4) = %d\n", ackermann(3,4));

    return 0;
}


