#include "Algorithm.h"

int Mul(int x, int y, int mod)
{
	return x * y % mod;
}

int GCD(int a, int b)
{
	if (a < b)
		swap(a, b);

	while (b != 0)
	{
		int r = a % b;
		a = b;
		b = r;
	}
	return a;
}

int LCM(int a, int b)
{
	return a * b / GCD(a, b);
}
int Ceil(int a, int b)
{
	return (a % b == 0) ? (a / b) : (a / b + 1);
}
int Ceil(double n)
{
	if (n - (int)(n) > 0)
		return (int)n + 1;
	else
		return n;
}

int Pow(int a, int b)
{
	int res = 1;
	while (b > 0)
	{
		if (b & 1)
			res = (res * a);
		b = b >> 1;
		a = (a * a);
	}
	return res;
}
int Pow(int a, int b, int p)
{
	int res = 1;
	a = a % p;
	while (b > 0)
	{
		if (b & 1)
			res = Mul(res, a, p);
		b = b >> 1;
		a = Mul(a, a, p);
	}
	return res;
}

vector<string> split(string input, initializer_list<char> check)
{
	vector<string> answer;

	input += *check.begin();

	string temp = "";
	for (int i = 0; i < input.size(); i++)
	{
		bool flag = false;
		for(char c : check)
			if (input[i] == c)
			{
				flag = true;
				break;
			}

		if (flag && temp.size() == 0)
			continue;
		if (flag && temp.size() > 0)
		{
			answer.push_back(temp);
			temp = "";
		}
		else
		{
			temp += input[i];
		}
	}
	return answer;
}

unsigned int StringToUint(string num)
{
	unsigned int number = 0;
	for (int i = 0; i < num.size(); i++)
	{
		int n = num[i] - '0';
		number *= 10;
		number += n;
	}
	return number;
}

string UintToString(unsigned int num)
{
	if (num == 0)
	{
		return "0";
	}
	string temp = "";
	int c = (int)log10(num);
	for (int i = 0; i <= c; i++)
	{
		int n = num % 10;
		temp += (n + '0');
		num /= 10;
	}
	reverse(temp.begin(), temp.end());
	return temp;
}

__int64 StringToInt64(string num)
{
	__int64 number = 0;
	for (int i = 0; i < num.size(); i++)
	{
		int n = num[i] - '0';
		number *= 10;
		number += n;
	}
	return number;
}

string Int64ToString(__int64 num)
{
	if (num == 0)
	{
		return "0";
	}
	string temp = "";
	int c = (int)log10(num);
	for (int i = 0; i <= c; i++)
	{
		int n = num % 10;
		temp += (n + '0');
		num /= 10;
	}
	reverse(temp.begin(), temp.end());
	return temp;
}