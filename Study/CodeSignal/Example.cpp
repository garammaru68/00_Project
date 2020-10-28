#include "Example.h"

using namespace std;

void main()
{

	//// 아케이드 3번 문제
	//while (1)
	//{
	//	int year;
	//	cin >> year;

	//	//int century = ceil((double)year / 100);
	//	//cout << "Century : " << century << endl;

	//	int century = year / 100;
	//	year % 100 == 0 ? century : century += 1;
	//	cout << century;
	//	break;
	//}

	//// 아케이드 4번 문제
	//while (1)
	//{
	//	string Input;
	//	cin >> Input;
	//	if (checkPalindrome(Input) == true)
	//	{
	//		cout << "진실" << endl;
	//	}
	//	else cout << "거짓" << endl;
	//	break;
	//}

	//// 아케이드 6번 문제
	//// 나의 풀이
	//int arryStatues[] = { 6,2,9,5 };
	//int smallest = arryStatues[0];
	//int largest = arryStatues[0];
	//for (int i = 0; i < sizeof(arryStatues) / sizeof(int); i++)
	//{
	//	if (smallest > arryStatues[i])
	//	{
	//		if (largest < smallest)
	//		{
	//			largest = smallest;
	//		}
	//		smallest = arryStatues[i];
	//	}
	//	else
	//	{
	//		if (largest < arryStatues[i])
	//		{
	//			largest = arryStatues[i];
	//		}
	//	}
	//}

	//int count = 0;
	//bool exist;
	//for (int i = smallest; i < largest; i++)
	//{
	//	for (int j = 0; j < sizeof(arryStatues) / sizeof(int); j++)
	//	{
	//		exist = false;
	//		if (i == arryStatues[j])
	//		{
	//			exist = true;
	//			break;
	//		}
	//	}
	//	if (exist == false) count++;
	//}
	//cout << count << endl;

	//// 정답(함수)
	//int makeArrayConsecutive2(std::vector<int> statues)
	//{
	//	int total = 0;
	//	sort(statues.begin(), statues.end());
	//	total = statues.back() - statues.front() + 1;
	//	return total - statues.size();
	//}

	// 아케이드 7번 문제
	vector<int> test;
	test.push_back(1);
	test.push_back(1);
	test.push_back(3);
	test.push_back(5);
	test.push_back(3);

	almostIncreasingSequence(test);
}

// 아케이드 4번 문제(함수)
bool checkPalindrome(std::string inputString)
{
	string word = inputString;
	reverse(inputString.begin(), inputString.end());

	if (word == inputString) return true;
	else return false;
}

//bool almostIncreasingSequence(std::vector<int> sequence)
//{
//	bool Check;
//	int count = 0;
//	std::vector<int>::iterator viSequence;
//
//	for (viSequence = sequence.begin(); viSequence != sequence.end(); viSequence++)
//	{
//		if (viSequence > sequence.begin() + 1)
//		{
//			sequence.erase();
//			count++;
//		}
//		if (count >= 2) Check = false;
//		else Check = true;
//	}
//
//	return Check;
//}

// 아케이드 7번 문제
//bool almostIncreasingSequence(std::vector<int> vSequence)
//{
//	bool Check;
//	int count = 0;
//	int length = vSequence.size();
//
//	for (int i = 0; i < length - 1; i++)
//	{
//		if (vSequence[i] > vSequence[i + 1])
//		{
//			count++;
//		}
//		for (int j = i + 1; j < length; j++)
//		{
//			if (vSequence[i] >= vSequence[j])
//			{
//				count++;
//			}
//		}
//		if (count >= 2) Check = false;
//		else Check = true;
//	}
//	return Check;
//}

//
bool almostIncreasingSequence(std::vector<int> vSequence)
{
	bool Check;
	int count = 0;
	int length = vSequence.size();

	for (int i = 0; i < length - 1; i++)
	{
		if (vSequence[i] > vSequence[i + 1])
		{
			count++;
		}
		for (int j = i + 1; j < length; j++)
		{
			if (vSequence[i] == vSequence[j])
			{
				vSequence.erase(vSequence.begin() + j);
				length = vSequence.size();
				count++;
				j--;
			}
		}
		if (count >= 2) Check = false;
		else Check = true;
	}
	return Check;
}