#include <iostream>


int main(){

	/*12.1 Find the mistake(s) in the following code:*/
	unsigned int i;
	for (i = 100; i >= 0; --i)
		printf("%d\n", i);
	/*First, %u should be used when printing unsigned integers. Secondly, this for
	loop will iterate infinetly. When i = 0, the next decrement will cause i to be UINT_MAX.
	In other words, it wont be negative, therefore, always being >= 0.*/


	/*12.2 You are given the source to an application which crashes when it is run. After
	running it ten times in a debugger, you find it never crashes in the same place.
	The application is single threaded, and uses only the C standard library. What
	programming errors could be causing this crash? How would you test each one?*/

	/*First. There could be some kind of external input being given to the program.
	Depending on the input, the program will crash in different moments. This input could be a
	something entered by the user (as in a 'fread'). The input could also be something that when
	read would always have different values (as in a random number generator), or the reading of
	files of a folder in a way that each program launch would request a different file (due to some
	disk-stored counter increment).

	Second. The problem could also happen due to heap corruption (as in memory leak). So when we try to 
	allocated some memory region (using a malloc), which could land on different memory places at 
	different runs, the crash could	happen.	*/

	/*12.3 We have the following method used in a chess game: boolean canMoveTo(int x, int y). 
	This method is part of the Piece class and returns whether or not the piece can
	move to position (x, y). Explain how you would test this method
	...
	First:  test negative coordinates, for only x, only y, and both.
	Second: do the same, but for out of bound coordinates, sinc the chess board has limited size.
	Third:  test positions where there are pieces in it (enemy ones assuming that piece can be killed), and 
			friendly ones (assuming it can't be killed, thus the movement should not be possible).
	Fourth: test positions where there are no pieces in it.
	Fifth:  repeat the above steps for all kinds of pieces (given they move in different ways).
	*/

	/*12.4 How would you load test a webpage without using any test tools?
	Writting my own software to simulated many, many users.*/

	/*
	12.5 How would you test a pen?
	First, by whom is the pen used?
	What kind of pen?
	How often is it used?
	What is the purpose of its use? Write on paper? Write on leather? Write on fabric?


	/*12.6 How would you test an ATM in a distributed banking system?
	
	Testing all possible operations offered by the ATM, in different situations.

	For example, money withdraw:
	Test with people who has money, has no money, has a lot of money on their account.

	Money transfer:
	Test between valid accounts and between invalid ones.
	*/



	return 0;
}