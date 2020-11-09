#include "Example.h"
using namespace std;

int main()
{
	Calc calc;
	calc.Add(8).Sub(4).Mul(5);
	cout << calc.GetValue() << endl;
}