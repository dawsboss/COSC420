# Project 1 - Problem 160 - 60%
## Factorial trailing digits
#### FOr any N, let f(N) be the last five digits before the trailing zeros in N!
#### For exsample
    9! = 362880 so f(9)=36288
    10! = 3628800 so f(10)=36288
    20! = 2432902008176640000 so f(20) = 17664

#### The goal for this project was to find the f(1,000,000,000,000) or the last 5 digits of the 1 trillion th factorial.

## Usage:
The best and final version of the code is int the file SingleFile, this was in reference to the BigInt library used. 
There is a Makefile used that will work so,
    
    make main
    ./main (OR) mpirun -n # ./main (OR) sbatch P160.sh
    

## Approach
Initially thought that this problem would be ver easy and that it would be the easiest 60% problem. What I failed to realize is that the 1 trillion th factorial is 10^10^13.06317213025609 . In other words it is a massive number and to get it would not only take a ton of computations. What I did not take into consideration was how I was going to do the computations. The computations for factorial in it's self is extremely simple. Just multiply by the number you are taking and by all the numbers that follow it down to one. But for a computer this is hard because a computer can only hold such a large number. My end result turned into me using a Big Int library that turned the number into a string and did operations on it in c plus plus. Each proecessor gets different chunks of the factorial that is to be caculated and since multiplication is comunitive I multiply them all together in the end. Also as the multiplications are happening in each processor the results are being modded and stripped of all trailing zeros. This adds mroe time for one compuation but would aid in the fight for overflow and memory usage. 


## The porblems:
#### Problem 1: MPI Reduce
My initial thought for this problem was that I would ahve each node calculate a chunk of the factroial and they would bring it all together using MPI Reduce(). This worked for small cases but once I tried some big boy number I began to overflow. So then moved to a system that I would use a gather instead of a reduce so I could control the overflows.

#### Problem 2: Primative data types are bad news
Also when I initial started I used ints and quickly realized my flaw so I switch to allicing all my stuff to 'boi' which was an unsigned long long. Which young Grant though "no way this can ever overflow" which for some of the big boy numbers it didn;t and when I would try larger factorials (100000!, no where near what I needed) it would give me the wrong numbers. Which indicated I was overflowing
#### Problem 3: The overflowing saga pt1.
To combat the overflowing pandemic I started by copnsulting with my professor where we cam up with an idea of a function I would "easily" implement. It involved modulous. I have been using the whole time so I thought it was a great idea. The overflow was most likely coming from when i would do (A\*B)%C where the multiplication would overflow before I would mod it. SO the question is could I turn that into (A%C \* B%C)%C and sneak by. I would be checking to see if the two number would overflow at all if I multiplied and compare them to their limits of their data types and the whole nine yards. But this turned out rather dificult and it would give me garbage back. After 2-3 days of that I scrapped the idea and moved on. I saved the code if you wish to check it out. It is in the Template.c file.
#### Problem 4: Overflow saga pt4.
Now that we have failed at an overflow functions and lost 3 days. I decided on the next approach being to find a Big Int library. Young Grant ocne again looking at the solution scoffed and said "Oh that will be easy" young Grant was once again mistaken. This event would lead young naive Grant on a expedition lasting 2 weeks of searching the internet and Github, annoying his professor, and sheading a few tears. I went through 4 libraries all from diffrent style of rpgramming and all having their own quarks. I was introduced to my currently library rather early but was brushed off because it was in cpp and I didn;lt want to mess woith converting :/ I went form library to library and they all had ranging rroblems, some wouldn't compile on their own test files, some called themselves "Big int libraries" or "Big digit libraries" and could only hold 32 bits or 9 characters of space.... Not very big big int libraries. Then one day i looked back at the cpp file and realized it had everyhting, unilmited sotrage (dependent on your RAM), all the functions you could want, multiple constructors. All for the low flat fee of converting to c. The got to work and my professor admitely gave me a stepping stone then I it was off the the races. I made wrapper function after wrapper function for this code. I got all the functionality to work and eveything and it began working ons amll numbers after some playing and a few more days of work. Then problem 5 hit...
#### Problem 5: Memory monster
I created a memory hungery monster. This thing would eat memory so fast because young Grant said "memory isn't an issue." SO young Grant decided to leave the the program to do it's thing. I ran it on the cluster and ate and leaked 4 GB of RAM. So new thought. I will free my memory and be a good boy, this took about .5 - 1 days. Where I got my program to leak either nothing ro 12 bytes (for printing). 
#### Problem 6: The unknown
I called the problem the unknown because I am not sure of the problem yet. I ran my code that was wokring for all these cases and all above problem were solved and Ig ot back anwser for 1 trillion factorial that was incorrect. Proplexed I messed wiht it some more and the decided to run the code on some big cases. Let it be known the 1 trillion case took mnore than 9 hours. So I tried it one 1 billion and I noticed that it got the last 5 digits right but if I look past that up the list it had an error zone where it got so many number right up to a point then they started going wring. SO I thought this may have been an issue with the 1 trillion so i changed the number I was moding by so that the error zone would be father up the chain from ym last 5 digits. Then I ran the code on 10 bilion and I also got the wright answer. This was about ~6.5 hours fo runnung. So as I am writing this I re ran the 1 trillion factorial with mod of 10^20 in hopes that I moved it far enough that I will get my answer.  
#### Problem 6 continued...
It is the next day and I went to see fi my code was done and we have hit 20.5 hours. I was nervous of this so I went to do some more testing and I discovered something alarming. The code isn;t wokring, the code is actually messing up somewhere with taking the mod... I added 10^20 for the mod so we could "move the error away" from the last 5 digits of the number. But there seems to be a problem or there is to much going on for the BigInt Library. I have to settle with using 10^15. Althought still massive I have been running my coce on the 10^20 for 20.5 hours and the project is due in 12.5 hours. We may not be able to get an answer by then but we know if it is not working where to put some work into. The mod dunction for the BigInt cpp file needs to be able to handle larger numbers. 
 








