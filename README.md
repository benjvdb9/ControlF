# ControlF

Program meant to look for a string of words in a large text. Written in C and threaded as much as possible.

## How to use

Nothing special here:
	gcc Threading.c
	./a.out

Once the program launched you will be asked the amount of threads to create, the file to read, the file to write the results into and what to look for. FOr example:  
	Threads: 3  
	Input:	 text.txt
	Output:	 o.txt
	Find:	 BURR	

## Important Files

There are only two important files in this project: Find.h, which acts as a bit of a library with functions and structs to use and Threading.c which is the main code that can be split up into multiple threads.

The other files are:
	Find.c: The main code commented but without threading  
	SysCall.c: Test code to play around with system calls  
	text.txt: Main text to probe containing 4109 chars in 147 lines  
	test.txt: Text used for test containing 4 lines of 5 characters for a total of 20 chars. Splitting this in 3 threads and searching for "This" proves that matches stuck between two threads will still be found  
	Sources.txt: Links to the Sources where major bits of code were inspired from  
	notes.txt: Random notes taken while working on the project, possibly highlighting flaws in the program  

## Output Illustration
![Output Illustration](/Illustration.png)
