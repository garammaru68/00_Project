/*//this �׽�Ʈ
#include "Example.h"
using namespace std;

int main()
{
	Calc calc;
	calc.Add(8).Sub(4).Mul(5);
	cout << calc.GetValue() << endl;
}*/
/////////////////////////////////////////////////////////////////////////////
/*//#pragma pack �׽�Ʈ
#include <stdio.h>
#include <stddef.h>   // offsetof ��ũ�ΰ� ���ǵ� ��� ����

#pragma pack(push, 1)    // 1����Ʈ ũ��� ����
struct PacketHeader {
	char flags;    // 1����Ʈ
	int seq;       // 4����Ʈ
};
#pragma pack(pop)        // ���� ������ ���� ����(�⺻��)�� �ǵ���

int main()
{
	printf("%d\n", offsetof(struct PacketHeader, flags));    // 0
	printf("%d\n", offsetof(struct PacketHeader, seq));      // 1

	return 0;
}*/