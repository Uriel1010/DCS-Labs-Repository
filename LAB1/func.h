#define M 10 
#define N 10
#define CEIL 100 
#define max(x,y) x>y ? x : y  // MACRO function

void show_menu(char** strlst);
int ComputeTrace(int Mat[M][M]);
int DiagonalsMax(int Mat[M][M]);
void Transpose(int Mat1[M][M], int Mat2[M][M]);
void FillMatrix(unsigned int Mat[M][M], int);
void show_matrix(unsigned int Mat[M][M]);

void MatAdd(int Mat1[M][M], int Mat2[M][M], int Mat3[M][M]);

void copyDiag(int Mat1[M][M], int Mat2[M][M]);
