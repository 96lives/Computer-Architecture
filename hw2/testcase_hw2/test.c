//
// Created by ds-ubuntu on 18. 4. 6.
//
#include <stdio.h>
#include <string.h>
#include "pa2.h"

#define BYTE_FORMAT "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BITS(byte)  (byte & 0x80 ? '1' : '0'), \
  							(byte & 0x40 ? '1' : '0'), \
  							(byte & 0x20 ? '1' : '0'), \
  							(byte & 0x10 ? '1' : '0'), \
  							(byte & 0x08 ? '1' : '0'), \
  							(byte & 0x04 ? '1' : '0'), \
  							(byte & 0x02 ? '1' : '0'), \
  							(byte & 0x01 ? '1' : '0')

#define PRINT_TINYFP(t)		printf("tinyfp("BYTE_FORMAT"), ", BYTE_TO_BITS(t))

void printTFFile(FILE* fp, tinyfp tf) {
	fprintf(fp, ""BYTE_FORMAT"", BYTE_TO_BITS(tf));
}

void printWrong(FILE* fp, int add,  tinyfp a, tinyfp b, char rAns[255], char mAns[255]) {
	char oper = '*';
	if (add)
		oper = '+';
	printTFFile(fp, a);
	fprintf(fp, " %c ", oper);
	printTFFile(fp, b);
	fprintf(fp, " = %s , %s\n", rAns, mAns);
}


void myAddAnswer() {

	FILE *fp;
	fp = fopen("./my_add_answer.txt", "w+");
	// add
	tinyfp a = 0;
	while (1) {
		tinyfp b = 0;
		while (1) {
			printTFFile(fp, add(a, b));
			fprintf(fp, "\n");
			if (b == 255)
				break;
			++b;
		}
		if (a == 255)
			break;
		++a;
	}
	fclose(fp);
}

void myMulAnswer() {

	FILE *fp;
	fp = fopen("./my_mul_answer.txt", "w+");
	// add
	tinyfp a = 0;
	while (1) {
		tinyfp b = 0;
		while (1) {
			printTFFile(fp, mul(a, b));
			fprintf(fp, "\n");
			if (b == 255)
				break;
			++b;
		}
		if (a == 255)
			break;
		++a;
	}
	fclose(fp);
}

// if add 1, mul 0
// generates ./wrong_mul.txt or ./wrong_add.txt depending on add param
// Before running checkAnswers, run myAddanswer() or myMulAnswer()
// Ex) 00000000 * 00000001 = 00000000 , 00000001
// means that real answer is 00000000 but the program outputs 00000001
void checkAnswers(int add, char myAnsDir[255], char realAnsDir[255]) {

	FILE *maf;
	FILE *raf;
	maf = fopen(myAnsDir, "r");
	raf = fopen(realAnsDir, "r");

	FILE *wr;
	char wrMulDir [255] = "./wrong_mul.txt";
	char wrAddDir [255] = "./wrong_add.txt";
	if (add)
		wr = fopen(wrAddDir, "w+");
	else
		wr = fopen(wrMulDir, "w+");
	char ma[255];
	char ra[255];

	fprintf(wr, "Wrong List\n");
	tinyfp a = 0;
	while (1) {
		tinyfp b = 0;
		while (1) {
			fscanf(maf, "%s\n", ma);
			fscanf(raf, "%s\n", ra);
			if (strcmp(ma, ra) != 0)
				printWrong(wr, add, a, b, ra, ma);
			if (b == 255)
				break;
			++b;
		}
		if (a == 255)
			break;
		++a;
	}
	fclose(maf);
	fclose(raf);
	fclose(wr);

}

int main (){

	myAddAnswer();	// create my program answer testcases to "./my_add_answer.txt"
	myMulAnswer()  // create my program answer testcases to "./my_mul_answer.txt"
	char myAddAnsDir [255] = "./my_add_answer.txt";	// directory of the my answer
	char realAddAnsDir [255] = "./ds_add_answer.txt";	// directory of the existing comparison answer	
	char myMulAnsDir [255] = "./my_mul_answer.txt";	// directory of the my answer
	char realMulAnsDir [255] = "./ds_mul_answer.txt";	// directory of the existing comparison answer
	
	checkAnswers(1, myAddAnsDir, realAddAnsDir);	// 1 for add, 0 for multiplication
	checkAnswers(0, myMulAnsDir, realMulAnsDir);
	 // creates what I did wrong in "wrong_add.txt" or "wrong_mul.txt"

}
