#define _CRT_SECURE_NO_WARNINGS
#include "LinkedList.cpp"
#include <fstream>
#include <filesystem>

using namespace std;

enum MENU
{
	ADD = 1,
	DEL,
	MODI,
	ALDEL,
	SAVE,
	LOAD,
};

struct Gradecard
{
	int m_iNum;
	char m_cName[4];
	int m_iKor;
	int m_iEng;
	int m_iMat;
	int m_iTotal;
	float m_fAverage;
};

int iStudentNum = 0;

LinkedList<Gradecard> SetLink;

void Menu();
void Screen();
void Add();
void Del();
void Modify();
void AllDel();
void Save(const char* FileName);
void Load(const char* FileName);

void textcolor(int color_Gradecard)

{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_Gradecard);
}

int main()
{
	//system("mode con cols=39 lines=16");
	while (true)
	{
		system("cls");
		Screen();
		Menu();

		cout << endl << "메뉴 선택 : ";
		int Select;
		cin >> Select;
		if (Select == 0) break;

		switch (Select)
		{
		case ADD:
		{
			Add();
		}break;
		case DEL:
		{
			Del();
		}break;
		case MODI:
		{
			Modify();
		}break;
		case ALDEL:
		{
			AllDel();
		}break;
		case SAVE:
		{
			Save("NumCheck.txt");
		}break;
		case LOAD:
		{
			Load("NumCheck.txt");
		}break;
		}
	}
}

void Menu()
{
	cout << "====================================================" << endl;

	cout << "추가(1)  삭제(2)  수정(3)  올삭제(4)" << endl << "저장(5)  로드(6)  종료(0)";
}
void Screen()
{
	cout << "====================================================" << endl;
	textcolor(12);
	cout << "번호\t이름\t국어\t영어\t수학\t총점\t평균" << endl;
	textcolor(15);
	for (MNode<Gradecard>* Student = SetLink.n_Head->n_Next; Student != SetLink.n_Tail; Student = Student->n_Next)
	{
		cout << Student->m_Data->m_iNum << "\t"
			<< Student->m_Data->m_cName << "\t"
			<< Student->m_Data->m_iKor << "\t"
			<< Student->m_Data->m_iEng << "\t"
			<< Student->m_Data->m_iMat << "\t"
			<< Student->m_Data->m_iTotal << "\t"
			<< Student->m_Data->m_fAverage << endl;
	}
}
void Add()
{
	system("cls");
	Screen();

	Gradecard* Info;
	Info = new Gradecard;
	memset(Info, 0, sizeof(Gradecard));
	cout << "====================================================" << endl;
	cout << "이름 : ", cin >> Info->m_cName;
	cout << "국어 : ", cin >> Info->m_iKor;
	cout << "영어 : ", cin >> Info->m_iEng;
	cout << "수학 : ", cin >> Info->m_iMat;
	Info->m_iTotal = Info->m_iKor + Info->m_iEng + Info->m_iMat;
	Info->m_fAverage = Info->m_iTotal / 3.0f;

	if (&SetLink != NULL)
	{
		for (MNode<Gradecard>* FindLastNum = SetLink.n_Head->n_Next; FindLastNum != SetLink.n_Tail; FindLastNum = FindLastNum->n_Next)
		{
			if (FindLastNum->m_Data->m_iNum > iStudentNum)
			{
				iStudentNum = FindLastNum->m_Data->m_iNum;
			}
		}
	}
	iStudentNum++;
	Info->m_iNum = iStudentNum;

	MNode<Gradecard>* NewInfo;
	NewInfo = new MNode<Gradecard>;
	memset(NewInfo, 0, sizeof(MNode<Gradecard>));
	NewInfo->m_Data = Info;
	SetLink.LinkNode(SetLink.SortLink(Info), NewInfo);

}
void Del()
{
	system("cls");
	Screen();
	int iDeleteNum;
	cout << "====================================================" << endl;
	cout << "삭제 번호 : ", cin >> iDeleteNum;

	for (MNode<Gradecard>* FindInfo = SetLink.n_Head->n_Next; FindInfo != SetLink.n_Tail; FindInfo = FindInfo->n_Next)
	{
		if (FindInfo->m_Data->m_iNum == iDeleteNum)
		{
			SetLink.DelNode(FindInfo);
			break;
		}
	}
}
void Modify()
{
	system("cls");
	Screen();
	int iModifyNum;
	cout << "====================================================" << endl;
	cout << "수정 번호 : ", cin >> iModifyNum;

	for (MNode<Gradecard>* FindInfo = SetLink.n_Head->n_Next; FindInfo != SetLink.n_Tail; FindInfo = FindInfo->n_Next)
	{
		Gradecard* ModifyInfo = FindInfo->m_Data;
		if (ModifyInfo->m_iNum == iModifyNum)
		{
			cout << "수정할 학생 : " << ModifyInfo->m_iNum << "번\t" << ModifyInfo->m_cName << endl;
			cout << "이름 : ", cin >> ModifyInfo->m_cName;
			cout << "국어 : ", cin >> ModifyInfo->m_iKor;
			cout << "영어 : ", cin >> ModifyInfo->m_iEng;
			cout << "수학 : ", cin >> ModifyInfo->m_iMat;
			ModifyInfo->m_iTotal = ModifyInfo->m_iKor + ModifyInfo->m_iEng + ModifyInfo->m_iMat;
			ModifyInfo->m_fAverage = ModifyInfo->m_iTotal / 3.0f;
			break;
		}
	}
}
void AllDel()
{
	SetLink.AllDelNode();
}
void Save(const char* FileName)
{
	FILE* fSave = NULL;
	fSave = fopen(FileName, "w");
	for (MNode<Gradecard>* saveNode = SetLink.n_Head->n_Next; saveNode != SetLink.n_Tail; saveNode = saveNode->n_Next)
	{
		fwrite(saveNode->m_Data, 1, sizeof(Gradecard), fSave);
	}
	fclose(fSave);
}
void Load(const char* FileName)
{
	SetLink.AllDelNode();

	FILE* fLoad = NULL;
	fLoad = fopen(FileName, "r");
	while (1)
	{
		Gradecard* LoadData = 0;
		LoadData = new Gradecard;
		memset(LoadData, 0, sizeof(Gradecard));
		size_t size = fread(LoadData, 1, sizeof(Gradecard), fLoad);
		if (size == 0) break;

		MNode<Gradecard>* NewNum;
		NewNum = new MNode<Gradecard>;
		memset(NewNum, 0, sizeof(MNode<Gradecard>));
		NewNum->m_Data = LoadData;
		SetLink.LinkNode(SetLink.SortLink(LoadData), NewNum);
	}
	fclose(fLoad);
}