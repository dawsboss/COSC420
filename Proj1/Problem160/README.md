# Project 1 - Problem 160 - 60%
## Factorial trailing digits
#### FOr any N, let f(N) be the last five digits before the trailing zeros in N!
#### For exsample
    9! = 362880 so f(9)=36288
    10! = 3628800 so f(10)=36288
    20! = 2432902008176640000 so f(20) = 17664

#### The goal for this project was to find the f(1,000,000,000,000) or the last 5 digits of the 1 trillionth factorial.

## Usage:
First of all, the file with the final answer is "SingleFile." 

The best and final version of the code is int the file SingleFile. This was about the BigInt library used. 
There is a Makefile used that will work so,
    
    make main
    ./main (OR) mpirun -n # ./main (OR) sbatch P160.sh
    

## Analysis of the code:
There is a time function in the code, so the code will print how long it took when ran.
But I will analyze the operation it takes. 

NOTE: I will be ignoring the CPP files operation. I used this from one else.

#### Main:
The main it's self/excluding the factorial function is extremely simple. All it needs to do is give an unsigned long long number to factorial, receive a big Int and then print. So all in all, a constant amount of fo work.

#### Factorial:
    The number of operation is 22 + ( Fac/worldSize * 5 * 3*numofTrailingZeros ) + 4 + GatherAll + 3 + 2*worldSize + 1 + GatherV + worldsize*4*numofTrailingZeros + 1 + worldSize + 6

This is so this may seem randomly made, but I went through the code line by line. I was unsure if we needed a direct counter, but the number will change based on the node you will have. There will be more computation done if you have mode nodes not from the factorial calculation but from the setup and clean up of the numbers. The last loop of collecting won't run if there is on the processor, but the more there are, the more nodes it needs to grab numbers from.
## Approach:
Initially thought that this problem would be elementary and would be the most straightforward 60% problem. What I failed to realize is that the 1 trillionth factorial is 10^10^13.06317213025609 . In other words, it is a massive number, and to get it would not only take a ton of computations. What I did not take into consideration was how I was going to do the calculations. The computations for factorial in its self is straightforward. Just multiply by the number you are taking and by all the numbers that follow it down to one. But for a computer, this is hard because a computer can only hold such a large number. My end result turned into me using a Big Int library that turned the number into a string and did operations on it in c plus. Each processor gets different chunks of the factorial that is to be calculated. Since multiplication is communitive, I multiply them all together in the end. Also, as the multiplications are happening in each processor, the results are being modded and stripped of all trailing zeros. This adds more time for one computation but would aid in the fight for overflow and memory usage.


## The porblems:
#### Problem 1: MPI Reduce
My initial thought for this problem was that I would have each node calculate a factorial chunk, and they would bring it all together using MPI Reduce(). This worked for small cases, but I began to overflow once I tried some significant boy numbers. So then moved to a system that I would use a gather instead of a reduction to control the overflows.
#### Problem 2: Primitive data types are bad news
When I initially started, I used ints and quickly realized my flaw, so I switch to aliasing all my stuff to 'boi' which was an unsigned long long. Which young Grant, though, "no way this can ever overflow," which for some of the big boy numbers it didn't, and when I would try larger factorials (100000!, nowhere near what I needed), it would give me the wrong numbers. Which indicated I was overflowing
#### Problem 3: The overflowing saga pt1.
To combat the overflowing pandemic, I started by consulting with my professor. We came up with an idea of a function I would "easily" implement. It involved modulus. I have been using the whole time, so I thought it was a great idea. The overflow was most likely coming from when I would do (A*B)%C where the multiplication would overflow before I would mod it. SO the question is could I turn that into (A%C * B%C)%C and sneak by. I would check to see if the two numbers would overflow if I multiplied and compare them to the limits of their data types and the whole nine yards. But this turned out rather tricky, and it would give me garbage back. After 2-3 days of that, I scrapped the idea and moved on. I saved the code if you wish to check it out. It is in the Template.c file.
#### Problem 4: Overflow saga pt4.
Now that we have failed at an overflow function and lost 3 days. I decided on the next approach being to find a Big Int library. Young Grant, once again looking at the solution, scoffed and said, "Oh, that will be easy" young Grant was once again mistaken. This event would lead naive young Grant on an expedition lasting 2 weeks of searching the internet and Github, annoying his professor, and shedding a few tears. I went through 4 libraries, all from a different programming style and all having their own quarks. I was introduced to my current library relatively early but was brushed off because it was in CPP and I didn;lt want to mess with converting :/ I went form library to library, and they all had ranging problems. Some wouldn't compile on their own test files. Some called themselves "Big int libraries" or "Big digit libraries" and could only hold 32 bits or 9 characters of space... Not very big int libraries. Then one day, I looked back at the CPP file and realized it had everything, unlimited storage (dependent on your RAM), all the functions you could want, multiple constructors. All for the low flat fee of converting to c. The got to work, and my professor admittedly gave me a stepping stone, then I was off the the races. I made wrapper function after wrapper function for this code. I got all the functionality to work and everything, and it began working on all numbers after some playing and a few more days of work. Then problem 5 hits...
#### Problem 5: Memory monster
I created a memory hungry monster. This thing would eat memory so fast because young Grant said, "memory isn't an issue." SO young Grant decided to leave the program to do its thing. I ran it on the cluster and ate and leaked 4 GB of RAM. Such a new thought. I will free my memory and be a good boy. This took about .5 - 1 days. Where I got my program to leak either nothing or 12 bytes (for printing).
#### Problem 6: The unknown
I called the problem the unknown because I am not sure of the problem yet. I ran my code working for all these cases, and all the above issues were solved—Ig to back answer for 1 trillion factorial that was incorrect. Perplexed, I messed with it some more and decided to run the code on some significant cases. Let it be known the 1 trillion cases took more than 9 hours. So I tried it one 1 billion, and I noticed that it got the last 5 digits right. Still, if I look past that up the list, it had an error zone where it got so many numbers right up to a point then they started going wring. SO I thought this may have been an issue with the 1 trillion, so I changed the number I was moving by so that the error zone would be father up the chain from my last 5 digits. Then I ran the code on 10 billion, and I also got the right answer. This was about ~6.5 hours fo running. So as I am writing this, I re-ran the 1 trillion factorial with the mod of 10^20 in hopes that I moved it far enough that I will get my answer.
#### Problem 6 continued...
The next day, I went to see if my code was done, and we have hit 20.5 hours. I was nervous about this, so I went to do some more testing, and I discovered something alarming. The code isn't working, the code is actually messing up somewhere with taking the mod... I added 10^20 for the mod to "move the error away" from the last 5 digits of the number. But there seems to be a problem, or there is too much going on for the BigInt Library. I have to settle with using 10^15. Although still massive, I have been running my code on the 10^20 for 20.5 hours, and the project is due in 12.5 hours. We may not be able to get an answer by then, but we know if it is not working where to put some work into. The mod function for the BigInt CPP file needs to be able to handle larger numbers. 

Another addition to be made to the continuation is that I found two causes of error. One being from MPi and the other is from the Big Int library.

MPI_Gather and other functions take int* so only int arrays, so I cannot have a variety of unsinged long longs, which I imagine are screwing up some overflow when I do 1 trillion factorial.

BigInt library modulus is another cause. I said this already, but I am repeating it with more time under my belt and experimentations that the mod doesn't work for large enough numbers. I would imagine this is where that small error is coming from that I mention throughout this readme. In smaller numbers are leaked in, but the error grows fast when the number gets big enough. This would have to be in the CPP, and the way to developer implemented their modulus function fo the BgiInt type. With more time, I could have tried and solved this problem or made something to work a little more that could be dumbed down than bit counting. 
 
### Solution to problems:

On the second to last day, I found some interesting readings that I wish I would have seen earlier. I am sure this would give me correct answers and increase speeds. 

https://www.nctm.org/tmf/library/drmath/view/71768.html

This is the basic, but then someone else also posted to the same forum where the conversation continued and shed more light on a solution.

https://www.nctm.org/tmf/library/drmath/view/78186.html

Putting these two together, I am sure I could have a better solution than this brute force method I came up with. Ignoring numbers crossed my mind, but I never took the thought seriously enough, nor did I realize there were so many numbers that I could have "skipped" or at least reduced. It was also interesting to see patterns that arise from this reduction. It was sad to see so late in the project that I could be doing something better and not have the time to implement.

## Results:
My end results left me with a program that I knew worked for 10 billion factorial, but the 1 trillion could not be tested in time. Since the code could work for 10 billion, I would assume it could work for 1 trillion. I started running the code for 1 trillion at 12:00pm on Oct 11.  






