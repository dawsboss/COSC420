#include<stdio.h>
#include "BigIntegerSingleFile.cpp"

/*
 * 1. gcc -c the .c files
 * 2. g++ -c the .cpp files
 * 3. g++ the .o's together into one executable
 */

int main(){
    BigInteger* zero = makeBigIntStr("0");
    BigInteger* one = makeBigIntStr("1");
    BigInteger* ten = makeBigIntStr("10");
    BigInteger* a = makeBigIntStr("999999999999999");
    BigInteger* b = makeBigIntStr("999999999999999");

    //printf("a*b = %s\n", c_str(c_mult(a,b)));
    c_mult(a,b);
    printf("a*b = %s\n", c_str(a));



    puts("Sub");
    //printf("a-1 = %s\n", c_str(c_sub(a,one)));
    c_sub(a,one);

    printf("a = %s\n", c_str(a));


    puts("mod jawn");
    
    printf("a = %s\n", c_str(a));
    printf("a%10 =%s\n", c_str(c__mod(a,ten)));
    
    while((c_eqeq(c__mod(a,ten), zero)) == 1){
        puts("ree");
        c_div(a,ten);
    }
    
    puts("Print");
    printf("a = %s\n", c_str(a));

    c_pp(a);

    printf("a = %s\n", c_str(a));

    char str[256];
    unsigned long long x = 989;
    sprintf(str, "%llu",x);

    printf("String: %s", str);
    
    //del(a);
    //del(zero);
    //del(one);
    //del(ten);
    //del(b);
    //free(&a)'
    return 0;
}














//
