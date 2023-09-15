// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.

(LOOP1) 

	@SCREEN	
	D = A
	@addr
	M=D //addr holds base address of SCREEN memory map

	@color 
	M = 0 //Color register set to 0;
	
	//check value of keyboard
			
	@15
	D= A	// D=15(11...111)
	@KBD
	D = D&M;	//0 if nothing pressed
	
	@SET_WHITE
	D; JEQ	//if keyboard has value 0, then go to white 
	@SET_BLACK
	0; JMP	//else goto black
	
(SET_WHITE)
	
	@color 
	M = 0 //Color register set to 0;
	@SET_SCREEN
	0; JMP
		
(SET_BLACK)
	@color 
	M = -1 //Color register set to (1111.1111)


(SET_SCREEN)
	//check if we reach end of screen and if yes, then go back to beginning
	@KBD
	D=A;  
	@addr
	D = D-M; 
	@LOOP1
	D; JEQ
	
	//set the value of RAM[addr]=color for all pixels in screen map
	@color
	D = M; //D has value of color
	@addr 
	A = M;
	M = D ; //M refers to current memory map pixel and set to current color
	
	//increase addr by 1
	@addr
	M = M + 1
	
	@SET_SCREEN
	0 ; JMP
	

	







