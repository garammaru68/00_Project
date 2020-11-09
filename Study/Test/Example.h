#include <iostream>

class Calc
{
private:
	int m_Value = 0;
public:
	Calc& Add(int value) { m_Value += value; return *this; }
	Calc& Sub(int value) { m_Value -= value; return *this; }
	Calc& Mul(int value) { m_Value *= value; return *this; }
	int GetValue() { return m_Value; }
};