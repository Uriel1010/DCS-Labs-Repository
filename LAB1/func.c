#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "func.h"

void show_menu(char** strlst) {
	int i;
	for (i = 0; strlst[i]; i++)
		printf("%s\n", strlst[i]);
	return;
}
//---------------------------------------------------------------
//                  Trace Computation
//---------------------------------------------------------------
int ComputeTrace(int Mat[M][M]) {
	int Trace = 0, i;
	for (i = 0; i < M; i++) Trace += Mat[i][i];
	return Trace;
}
//---------------------------------------------------------------
//                  DiagonalsMax Computation
//---------------------------------------------------------------
int DiagonalsMax(int Mat[M][M]) {
	int Max = 0, i;
	for (i = 0; i < M; i++) {
		Max = max(Mat[i][i], Max);
		Max = max(Mat[i][N - 1 - i], Max);
	}
	return Max;
}
//---------------------------------------------------------------
//                  Matrix Transpose 
//---------------------------------------------------------------
void Transpose(int Mat1[M][M], int Mat2[M][M]) {
	int i, j;
	for (i = 0; i < M; i++) {
		for (j = 0; j < M; j++) {
			Mat2[i][j] = Mat1[j][i];
			Mat2[j][i] = Mat1[i][j];
		}
	}
}
//---------------------------------------------------------------
//                  Fill Matrix
//---------------------------------------------------------------
void FillMatrix(unsigned int Mat[M][M], int offset) {
	int i, j;
	for (i = 0; i < M; i++) {
		for (j = 0; j < M; j++) {
			Mat[i][j] = (offset + i * M + j) % CEIL;
		}
	}
}
//---------------------------------------------------------------
//                  show matrix
//---------------------------------------------------------------
void show_matrix(unsigned int Mat[M][M]) {
	int i, j;
	printf("[\n");
	for (i = 0; i < M; i++) {
		for (j = 0; j < M; j++)
			printf("%d, ", Mat[i][j]);
		printf("\n");
	}
	printf("]\n\n");
	return;
}
//---------------------------------------------------------------
//                  Matrixs Add
//---------------------------------------------------------------
void MatAdd(int Mat1[M][M], int Mat2[M][M], int Mat3[M][M]) {
	int i, j;
	for (i = 0; i < M; i++) {
		for (j = 0; j < M; j++) {
			Mat3[i][j] = Mat1[i][j]+ Mat2[i][j];
		}
	}
}
//---------------------------------------------------------------
//                  copy Diag
//---------------------------------------------------------------
void copyDiag(int Mat1[M][M], int Mat2[M][M]) {
	int i,tmp[M][M];
	for (i = 0; i < M; i++) {
		Mat2[i][i] = Mat1[i][M-i-1];
		Mat2[i][M - i - 1] = Mat1[i][i];
	}
}
//---------------------------------------------------------------
//                  string comparison
//---------------------------------------------------------------
int stringCompare(char str1[], char str2[])
{
	int i = 0;
	while (str1[i] == str2[i])
	{
		if (str1[i] == '\0' || str2[i] == '\0')
			break;
		i++;
	}
	if (str1[i] == '\0' && str2[i] == '\0')
		return 1;
	else
		return 0;
	}