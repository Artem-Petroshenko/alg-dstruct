#include "LabD.h"

//getting numbers from the file
bool Input(const char* Filename, long long* A, long long* B) {
	FILE* F = fopen(Filename, "r");
	if (!F)
		return false;
	fscanf(F, "%lli", A);
	fscanf(F, "%lli", B);
	fclose(F);
	return true;
}

//Calculating number of digits to get the size for massive(look next func)
int NumLen(long long A) {
	int i = 0;
	long long tmp = A;
	while (tmp)
	{
		i++;
		tmp /= 10;
	}
	return i;
}

//Transformation of a number to a massive of it's digits 
int* NumtoArray(long long A) {
	int* Array = (int*)malloc(sizeof(int) * NumLen(A));
	if (!Array)
		return NULL;
	long long tmp = A;
	int i = 1;
	while (tmp)
	{
		Array[NumLen(A) - i] = tmp % 10;
		tmp /= 10;
		i++;
	}
	return Array;
}

//Creation of massive filled with 0
int* ZeroArray(int Size) {
	int* Array = (int*)malloc(sizeof(int) * Size);
	if (!Array)
		return NULL;
	for (int i = 0; i < Size; i++)
		Array[i] = 0;
	return Array;
}

//calculation of expression length
int ExpLen(int* Nums, int* Ops, int SizeNums, int SizeOps) {
	int Len = SizeNums;
	for (int i = 0; i < SizeOps; i++)
		if (Ops[i])
			Len++;
	return Len;
}

//Function gets expression string
char* GetExp(int* Nums, int* Ops, int SizeNums, int SizeOps) {
	int Len = ExpLen(Nums, Ops, SizeNums, SizeOps) + 1;
	char* String = (char*)malloc(sizeof(char) * Len);
	if (!String)
		return NULL;
	String[0] = Nums[0] + '0';
	int j = 0;
	for (int i = 1; i <= SizeOps; i++)
	{
		j++;
		switch (Ops[i - 1])
		{
		case 0:
			String[j] = Nums[i] + '0';
			break;
		case 1:
			String[j] = '+';
			j++;
			String[j] = Nums[i] + '0';
			break;
		case 2:
			String[j] = '-';
			j++;
			String[j] = Nums[i] + '0';
			break;
		case 3:
			String[j] = '*';
			j++;
			String[j] = Nums[i] + '0';
			break;
		case 4:
			String[j] = '/';
			j++;
			String[j] = Nums[i] + '0';
			break;
		}
	}
	String[Len - 1] = '\0';
	return String;
}

//Check if symbol is digit
bool IsDigit(char ch) {
	return ch >= '0' && ch <= '9';
}

//Stack initialisation
Stack_t* StackInit(void) {
	Stack_t* S = (Stack_t*)malloc(sizeof(Stack_t));
	if (!S)
		return NULL;
	S->Top = NULL;
	return S;
}

//Check if stack is empty
bool IsEmpty(Stack_t* S) {
	return !S->Top;
}

//Pushing new element to stack
bool Push(Stack_t* S, long long Num) {
	Node_t* Element = (Node_t*)malloc(sizeof(Node_t));
	if (!Element)
		return false;
	Element->Data = Num;
	Element->Next = S->Top;
	S->Top = Element;
	return true;
}

//poping an element from top of the stack
bool Pop(Stack_t* S) {
	if (IsEmpty(S))
		return false;
	else
	{
		Node_t* ToFree = S->Top;
		S->Top = S->Top->Next;
		free(ToFree);
		return true;
	}
}

//getting the value of the top of the stack
long long Top(Stack_t* S) {
	return S->Top->Data;
}

//Function transforms regular expression into reverse polish notation and calculates gets it's result
long long RPN(int* Nums, int* Ops, int SizeNums, int SizeOps) {
	Stack_t* Result = StackInit();
	if (!Result)
		return LLONG_MIN;
	Stack_t* Opers = StackInit();
	if (!Opers)
	{
		free(Result);
		return LLONG_MIN;
	}
	char* Expression = GetExp(Nums, Ops, SizeNums, SizeOps);
	if (!Expression)
	{
		free(Result);
		free(Opers);
		return LLONG_MIN;
	}
	char* ptr = Expression;
	while (*ptr != '\0')
	{
		if (IsDigit(*ptr))
		{

			long long Num = 0;
			while (IsDigit(*ptr))
			{
				Num = Num * 10 + (*ptr - '0');
				ptr++;
			}
			if (!Push(Result, Num))
			{
				free(Result);
				free(Opers);
				free(Expression);
				return LLONG_MIN;
			}
		}
		else
		{
			if (!IsEmpty(Opers))
			{
				if (*ptr == '+' || *ptr == '-')
					while (!IsEmpty(Opers))
					{
						if (!Calculate(Result, Top(Opers)))
						{
							free(Result);
							free(Opers);
							free(Expression);
							return LLONG_MIN;
						}
						Pop(Opers);
					}
				else
					while (!IsEmpty(Opers) && (Top(Opers) == '*' || Top(Opers) == '/'))
					{
						if (!Calculate(Result, Top(Opers)))
						{
							free(Result);
							free(Opers);
							free(Expression);
							return LLONG_MIN;
						}
						Pop(Opers);
					}
			}
			if (!Push(Opers, *ptr))
			{
				free(Result);
				free(Opers);
				free(Expression);
				return LLONG_MIN;
			}
			ptr++;
		}
	}
	while (!IsEmpty(Opers))
	{
		if (!Calculate(Result, Top(Opers)))
		{
			free(Result);
			free(Opers);
			free(Expression);
			return LLONG_MIN;
		}
		Pop(Opers);
	}
	long long ExpRes = Top(Result);
	Pop(Result);
	free(Result);
	free(Opers);
	free(Expression);
	return ExpRes;
}

//Function for calculation of one operation
bool Calculate(Stack_t* S, int Operation) {
	long long Res, A, B;
	switch (Operation)
	{
	case '+':
		B = Top(S);
		Pop(S);
		A = Top(S);
		Pop(S);
		Res = A + B;
		break;
	case '-':
		B = Top(S);
		Pop(S);
		A = Top(S);
		Pop(S);
		Res = A - B;
		break;
	case '*':
		B = Top(S);
		Pop(S);
		A = Top(S);
		Pop(S);
		Res = A * B;
		break;
	case '/':
		B = Top(S);
		if (!B) // we can't divide by zero
			return false;
		Pop(S);
		A = Top(S);
		Pop(S);
		Res = A / B;
		break;
	}
	if (!Push(S, Res))
		return false;
	return true;
}

//realisation of enumeration
int Enumeration(long long B, int* Nums, int* Ops, int SizeNums, int SizeOps, int m, const char* Filename) {
	if (m >= SizeOps)
	{
		long long tmp = RPN(Nums, Ops, SizeNums, SizeOps);
		if (tmp != LLONG_MIN)
			if (B == tmp)
			{
				FILE* F = fopen(Filename, "w");
				if (!F)
					return 0;
				char* tmp = GetExp(Nums, Ops, SizeNums, SizeOps);
				if (!tmp) {
					fclose(F);
					return 0;
				}
				fputs(tmp, F);
				fprintf(F, "\n");
				free(tmp);
				fclose(F);
				return 1;
			}
		return 0;
	}
	else
		for (int j = 0; j < 5; j++)
		{
			Ops[m] = j;
			if (Enumeration(B, Nums, Ops, SizeNums, SizeOps, m + 1, Filename))
				return 1;
		}
	return 0;
}

int Algorithm(const char* FileInput, const char* FileOutput) {
	long long A, B;
	FILE* FInput = fopen(FileInput, "r");
	if (!FInput)
		return -1;
	Input(FileInput, &A, &B);
	int Size = NumLen(A);
	int* Nums = NumtoArray(A);
	if (!Nums)
		return -1;
	int* Ops = ZeroArray(Size - 1);
	if (!Ops)
	{
		free(Nums);
		return -1;
	}
	if (!Enumeration(B, Nums, Ops, Size, Size - 1, 0, FileOutput))
	{
		FILE* FOutput = fopen(FileOutput, "w");
		if (!FOutput)
		{
			free(Nums);
			free(Ops);
			return -1;
		}
		fprintf(FOutput, "0\n");
		fclose(FOutput);
	}
	free(Nums);
	free(Ops);
	return 0;
}