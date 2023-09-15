// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
//
// This program only needs to handle arguments that satisfy
// R0 >= 0, R1 >= 0, and R0*R1 < 32768.

// Put your code here.

@sum
M = 0 //sum = 0



@R0
D=M
@ZERO
D; JEQ	//check if R0 has value 0 and if yes, then jump to ADD
@R1
@ZERO
D; JEQ	//check if R1 has value 0 and if yes, then jump to ADD



(LOOP)
@R1
D = M;
@ADD
D ; JEQ

@R0
D = M; //D= R0
@sum 
M = D+M  //sum = sum + R0

@R1
M = M-1;

@LOOP
0; JMP



(ZERO)
@sum
M= 0 ;


(ADD)
@sum
D = M 	//D = R0*R1
@R2
M = D 	// R2 = R0 * R1



