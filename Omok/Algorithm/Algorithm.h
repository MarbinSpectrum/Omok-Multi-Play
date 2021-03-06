#pragma once

#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <initializer_list>

using namespace std;

int Mul(int x, int y, int mod);
int GCD(int a, int b);
int LCM(int a, int b);
int Ceil(int a, int b);
int Ceil(double n);
int Pow(int a, int b);
int Pow(int a, int b, int p);
vector<string> split(string input, initializer_list<char> check);
__int64 StringToInt64(string num);
string UintToString(unsigned int num);
unsigned int StringToUint(string num);
string Int64ToString(__int64 num);