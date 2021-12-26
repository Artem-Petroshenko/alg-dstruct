#include "pch.h"
#include "LabD.h"

/*
IDE: Visual Studio 2019 Community Edition
OS: Windows 10 Home, Version 20H2, Build 19042.1288
PC configuration:
	CPU: Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz 2.59 GHz
	Motherboard: TIMI TM1806
	RAM: 16 GB, DDR4, 2400 MHz
	SSD: SSD M.2 1TB
*/

/*
Lab D, Variant 26 (placement of signs of arithmetic operations):
Results:
	Full algorythm time: 88375 ms
	Total run time: 88376 ms
	Memory Costs: About 1 MB
*/

TEST(Stress_Test, Full_algorythm) {
	const char* Input = "stress_test_input.txt";
	const char* Output = "stress_test_output.txt";
	Algorithm(Input, Output);
}