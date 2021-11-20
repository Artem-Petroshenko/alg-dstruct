#include "LabD.h"

// 0 - connect
// 1 - plus
// 2 - minus
// 3 - multiply
// 4 - divide

int main() {
	remove("output.txt");
	const char* fileinput = "input.txt";
	const char* fileoutput = "output.txt";
	int Flag = 0;
	int A, B;
	Input(fileinput, &A, &B);
	int size = NumLen(A);
	int* Nums = NumtoMass(A);
	if (!Nums)
		return -1;
	int* Ops = ZeroMass(size - 1);
	if (!Ops)
	{
		free(Nums);
		return -1;
	}
	Perebor(B, Nums, Ops, size, size - 1, 0, &Flag, fileoutput);
	if (!Flag)
	{
		FILE* F = fopen(fileoutput, "a");
		if (!F)
		{
			free(Nums);
			free(Ops);
			return;
		}
		fprintf(F, "0\n");
		fclose(F);
	}
	free(Nums);
	free(Ops);
	return 0;
}