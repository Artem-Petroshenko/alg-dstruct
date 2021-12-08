#include "LabD.h"

//getting numbers from the file
bool Input(const char* Filename, int* A, int* B) {
	FILE* F = fopen(Filename, "r");
	if (!F)
		return false;
	fscanf(F, "%i", A);
	fscanf(F, "%i", B);
	fclose(F);
	return true;
}

//Calculating number of digits to get the size for massive(look next func)
int NumLen(int A) {
	int i = 0;
	int tmp = A;
	while (tmp)
	{
		i++;
		tmp /= 10;
	}
	return i;
}

//Transformation of a number to a massive of it's digits 
int* NumtoMass(int A) {
	int* Mass = (int*)malloc(sizeof(int) * NumLen(A));
	if (!Mass)
		return NULL;
	int tmp = A;
	int i = 1;
	while (tmp)
	{
		Mass[NumLen(A) - i] = tmp % 10;
		tmp /= 10;
		i++;
	}
	return Mass;
}

//Creation of massive filled with 0
int* ZeroMass(int size) {
	int* Mass = (int*)malloc(sizeof(int) * size);
	if (!Mass)
		return NULL;
	for (int i = 0; i < size; i++)
		Mass[i] = 0;
	return Mass;
}

//calculation of expression length
int ExpLen(int* Nums, int* Ops, int sizeNums, int sizeOps) {
	int Len = sizeNums;
	for (int i = 0; i < sizeOps; i++)
		if (Ops[i])
			Len++;
	return Len;
}

//Check if symbol is digit
bool isDigit(char ch) {
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
bool isEmpty(Stack_t* S) {
	return !S->Top;
}

//Pushing new element to stack
bool Push(Stack_t* S, int Num) {
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
	if (isEmpty(S))
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
int Top(Stack_t* S) {
	return S->Top->Data;
}

//Function transforms regular expression into reverse polish notation and calculates gets it's result
int* RPN(int* Nums, int* Ops, int sizeNums, int sizeOps) {
	Stack_t* Result = StackInit();
	if (!Result)
		return NULL;
	Stack_t* Opers = StackInit();
	if (!Opers)
	{
		free(Result);
		return NULL;
	}
	char* Expression = GetExp(Nums, Ops, sizeNums, sizeOps);
	if (!Expression)
	{
		free(Result);
		free(Opers);
		return NULL;
	}
	char* ptr = Expression;
	while (*ptr != '\0')
	{
		if (isDigit(*ptr))
		{

			int Num = 0;
			while (isDigit(*ptr))
			{
				Num = Num * 10 + (*ptr - '0');
				ptr++;
			}
			if (!Push(Result, Num))
			{
				free(Result);
				free(Opers);
				free(Expression);
				return NULL;
			}
		}
		else
		{
			if (!isEmpty(Opers))
			{
				if (*ptr == '+' || *ptr == '-')
					while (!isEmpty(Opers))
					{
						if (!Calculate(Result, Top(Opers)))
						{
							free(Result);
							free(Opers);
							free(Expression);
							return NULL;
						}
						Pop(Opers);
					}
				else
					while (!isEmpty(Opers) && (Top(Opers) == '*' || Top(Opers) == '/'))
					{
						if (!Calculate(Result, Top(Opers)))
						{
							free(Result);
							free(Opers);
							free(Expression);
							return NULL;
						}
						Pop(Opers);
					}
			}
			if (!Push(Opers, *ptr))
			{
				free(Result);
				free(Opers);
				free(Expression);
				return NULL;
			}
			ptr++;
		}
	}
	while (!isEmpty(Opers))
	{
		if (!Calculate(Result, Top(Opers)))
		{
			free(Result);
			free(Opers);
			free(Expression);
			return NULL;
		}
		Pop(Opers);
	}
	int ExpRes = Top(Result);
	Pop(Result);
	free(Result);
	free(Opers);
	free(Expression);
	return &ExpRes;
}

//Function for calculation of one operation
bool Calculate(Stack_t* S, int operation) {
	int Res = 0, A, B;
	switch (operation)
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
int Perebor(const int B, int* Nums, int* Ops, int sizeNums, int sizeOps, int m, const char* Filename) {
	if (m >= sizeOps)
	{
		if(RPN(Nums, Ops, sizeNums, sizeOps))
			if (B == *RPN(Nums, Ops, sizeNums, sizeOps))
			{
				FILE* F = fopen(Filename, "a");
				if (!F)
					return 0;
				fputs(GetExp(Nums, Ops, sizeNums, sizeOps), F);
				fprintf(F, "\n");
				fclose(F);
				return 1;
			}
	}
	else
		for (int j = 0; j < 5; j++)
		{
			Ops[m] = j;
			if (Perebor(B, Nums, Ops, sizeNums, sizeOps, m + 1, Filename))
				return 1;
		}
	return 0;
}

//Function gets expression string
char* GetExp(int* Nums, int* Ops, int sizeNums, int sizeOps) {
	int Len = ExpLen(Nums, Ops, sizeNums, sizeOps) + 1;
	char* String = (char*)malloc(sizeof(char) * Len);
	if (!String)
		return NULL;
	String[0] = Nums[0] + '0';
	int j = 0;
	for (int i = 1; i <= sizeOps; i++)
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

int Algorythm(const int A, const int B, const char* fileoutput) {
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
	if (!Perebor(B, Nums, Ops, size, size - 1, 0, fileoutput))
	{
		FILE* F = fopen(fileoutput, "a");
		if (!F)
		{
			free(Nums);
			free(Ops);
			return -1;
		}
		fprintf(F, "0\n");
		fclose(F);
	}
	free(Nums);
	free(Ops);
	return 0;
}