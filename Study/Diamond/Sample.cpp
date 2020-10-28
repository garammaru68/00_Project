#include <iostream>

using namespace std;

void main()
{
	while (1)
	{
		int Num;
		cout << "¼ýÀÚ : ", cin >> Num;

		for (int iRow = 0; iRow < Num; iRow++)
		{
			for (int iCol = 0; iCol < Num - iRow; iCol++)
			{
				cout << " ";
			}
			for (int iStar = 0; iStar < iRow * 2 + 1; iStar++)
			{
				cout << "*";
			}
			cout << endl;
		}
		for (int iRow = Num; iRow >= 0; iRow--)
		{
			for (int iCol = 0; iCol < Num - iRow; iCol++)
			{
				cout << " ";
			}
			for (int iStar = 0; iStar < iRow * 2 + 1; iStar++)
			{
				cout << "*";
			}
			cout << endl;
		}
	}
}