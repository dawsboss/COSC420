#include<stdio.h>
#include<stdlib.h>

/*
 * Think about what happens when the CPU has to add 1.0e5 and 1.0e-5
 * Have to align the exponents (one way):
 * 100000.000000
 * 000000.000001
 * =============
 * 100000.000001 <- Not enough digits in mantissato hold this!
 */


float epsilonEstimate(float x){
    float tmp = x;
    while(x+tmp != x){
        tmp /= 2;
    }
    return tmp;
}


int main(int argc, char** argv){
    float x = 1;
    printf("Estimate x = %7.7f | epsilon = %7.7f\n", x, epsilonEstimate(x));  
    x = 100;
    printf("Estimate x = %7.7f | epsilon = %7.7f\n", x, epsilonEstimate(x));  
    x = 10000;
    printf("Estimate x = %7.7f | epsilon = %7.7f\n", x, epsilonEstimate(x));  
    x = 100000;
    printf("Estimate x = %7.7f | epsilon = %7.7f\n", x, epsilonEstimate(x));  
    x = 1000000;
    printf("Estimate x = %7.7f | epsilon = %7.7f\n", x, epsilonEstimate(x));  
      
    x = 100.0;
    printf("Try %7.7f + %7.7f = %7.7f\n", x, epsilonEstimate(x), x+epsilonEstimate(x));
}









