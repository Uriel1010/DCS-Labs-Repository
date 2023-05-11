#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "func.h"
#define M 10 
#define N 10
#define CEIL 100 
#define max(x,y) x>y ? x : y  // MACRO function


char** menu[] = {
	" --------------------- Menu --------------------------------",
	" -----------------------------------------------------------",
	"1) Create two 10x10 matrices Mat1,Mat2 using the next expression:",
	"   for i,j = {0-9}: Mat[i][j] = (offset + i * 10 + j) % 100",
	"2) Compute and show the materix Mat1+Mat2",
	"3) Compute and show the maximal diagonal element between Mat1 and Mat2",
	"4) Compute and show the the maximal Trace between matrices Mat1 and Mat2",
	"5) Copy mat1 right diagonals to mat2 left diagonals",
	"6) Compute and show the transposed of a given Matrix",
	"7) Show a given Matrix",
	"8) Show menu",
	"9) exit",
	NULL
};

int main() {
	int Mat1[M][M], Mat2[M][M], auxMat[M][M];
	int mat1Trace, mat2Trace, maxDiag1, maxDiag2, sele=0, offset = 0,offset2 = 0;
	char Selector = '0', ch, str[3],m3[5],m1[]="Mat1",m2[]="Mat2";
	show_menu(menu);
	while (1) {
		printf("Choose a menu clause number: ");
		Selector = getchar();
		getchar(); // get the enter key
		switch (Selector) {
		case '0':
			break;

		case '1':
			do {
				printf("In order to creat Mat1 please insert an offset value between 0-100: ");
				gets(str);
				sscanf_s(str, "%d ", &offset);
				if (offset >= 0 && offset <= 100) break;

			} while (1);
			FillMatrix(Mat1, offset);
			do {
				printf("In order to creat Mat2 please insert an offset value between 0-100: ");
				gets(str);
				sscanf_s(str, "%d", &offset2);
				if (offset2 >= 0 && offset2 <= 100) break;

			} while (1);
			FillMatrix(Mat2, offset2);
			break;

		case '2':
			MatAdd(Mat1, Mat2, auxMat);
			show_matrix(auxMat);
			break;

		case '3':
			maxDiag1 = DiagonalsMax(Mat1);
			maxDiag2 = DiagonalsMax(Mat2);
			if (maxDiag2 >= maxDiag1) {
				printf("mat2");
			}else {
				printf("mat1");
			}
			printf(" has the maximal diagonal element value : %d\n", max(maxDiag1, maxDiag2));
			break;
			//delete
			mat1Trace = ComputeTrace(Mat1);
			printf("The mat Trace value = %d\n", mat1Trace);
			break;

		case '4':
			mat1Trace = ComputeTrace(Mat1);
			mat2Trace = ComputeTrace(Mat2);
			if (mat2Trace >= mat1Trace) {
				printf("mat2");
			}
			else {
				printf("mat1");
			}
			printf(" has the maximal Trace value: %d\n", max(mat1Trace, mat2Trace));
			break;
			//delete
			Transpose(Mat1, auxMat);
			show_matrix(auxMat);
			break;

		case '5':
			copyDiag(Mat1,Mat2);
			break;

		case '6':
			do {
				printf("Insert the wanted matrix [Mat1 or Mat2]: ");
				scanf("%s", m3);
				/*if (m3[4] == '\0' && m3[2] == 't' && m3[1] == 'a' && m3[0] == 'M' && (m3[3] == '2' || m3[3] == '1')) {
					break;
				}*/
				while ((getchar()) != '\n');
				if (stringCompare(m1, m3) || stringCompare(m2, m3)) {
					break;
				}
			} while (1);
			if (m3[3] == '1') {
				Transpose(Mat1, auxMat);
			}
			else {
				Transpose(Mat2, auxMat);
			}
			show_matrix(auxMat);
			break;
			//delete
			show_menu(menu);
			
			break;

		case '7':
			do {
				printf("Insert the wanted matrix [Mat1 or Mat2]: ");
				scanf("%s", m3);
				while ((getchar()) != '\n');
				if (stringCompare(m1, m3) || stringCompare(m2, m3)) {
					break;
				}
			} while (1);
			if (m3[3] == '1') {
				show_matrix(Mat1);
			}
			else {
				show_matrix(Mat2);
			}
			
			break;
			
		case '8':
			show_menu(menu);
			break;
		case '9':
			return 0;
			break;
		default:
			while ((getchar()) != '\n');
			printf("Try againn, You have inserted wrong number\n");
			break;


		}
	}
}