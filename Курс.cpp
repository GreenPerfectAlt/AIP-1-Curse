#include <stdio.h>
#include <conio.h>
#include <locale>
#include <Windows.h>
#include <iomanip>//��� ����������� ������ ��� ������ �-��� setw()
#include <iostream>//��� ����������� ������ ��� ������ �-��� setw()
#pragma warning(disable : 4996)
using namespace std;

const	char width = 12;
int MenuLabel = 1;
struct STUDENT
{
	char Sname[width],
		Fname[width], Tname[width];
	char rating[3];
};

struct T
{
	STUDENT data;
	T* right;
	T* left;
};

T* top;
int i;
int lessons[3];
T* addtree(T* top, const STUDENT& newnode);  //���������� ���� 
char menu();               //����������� ���� 
T* organization(T* top);
T* cleaning(T* top);
void view(T* top);  //��������  �������� ����� 
//������ ����� ������� 
void otobr(T* top, int otstup);
int writing_to_file(FILE* FileVedomost, T* top);    //������ � ���� 
int reading_from_file(FILE* FileVedomost, T*& top);  //������ �� ����� 
void printMenu(int MenuLabel);
STUDENT creation();            //���� ������ 

int main() {
	setlocale(LC_ALL, "rus"); SetConsoleCP(1251); SetConsoleOutputCP(1251);//���.��
	STUDENT z;
	top = NULL;
	while (1) {
		switch (menu()) {
		case 1: {
			top = organization(cleaning(top));
			system("pause");
		}break;
		case 2: {
			system("cls");
			if (!top) { puts("������� ������"); system("pause"); break; }
			i = 0;
			puts(" =====================================================================");
			puts("|  � |  �������   |   ���      |  ��������  |           ������        |");
			puts("|====|============|============|============|=========================|");
			puts("|    |            |            |            |������|�������|����������|");
			puts("|====|============|============|============|=========================|");
			view(top);
			_getch(); }
				break;
		case 3: {
			otobr(top, 1);
			system("pause");
		}break;
		case 4: {
			system("cls");
			if (top)
				addtree(top, creation());
			else top = addtree(top, creation());
		}break;
		case 5: {
			FILE* FileVedomost = fopen("vedomost", "w+");
			writing_to_file(FileVedomost, top);
			puts("  ===================================================================");
			puts(" |                        ���������� 100%...                         |");
			puts("  ===================================================================");
			system("pause");
		}break;
		case 6: {
			FILE* FileVedomost = fopen("vedomost", "r+");
			if (!FileVedomost) { fopen("vedomost", "w"); }
			reading_from_file(FileVedomost, top);
			fclose(FileVedomost);
			puts("  ===================================================================");
			puts(" |                        �������� 100%...                           |");
			puts("  ===================================================================");
			system("pause");
		}  break;
		case 7: {
			top = cleaning(top);
			puts("  ===================================================================");
			puts(" |                        ������� 100%...                            |");
			puts("  ===================================================================");
			system("pause");
		}break;
		case 8: {
			system("cls");
			if (!top) { puts("������� ������"); system("pause"); break; }
			i = 0;
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 5));
			puts("      ����������� ���������� � ������������ ����������� ������ 5.");
			puts(" =====================================================================");
			puts("|  � |  �������   |   ���      |  ��������  |           ������        |");
			puts("|====|============|============|============|=========================|");
			puts("|    |            |            |            |������|�������|����������|");
			puts("|====|============|============|============|=========================|");
			for (int i = 0; i < 3; i++)lessons[i] = 0;
			view(top);;
			if (lessons[0] > lessons[1] and lessons[0] > lessons[2]) puts("\n            �� ������ ������ ����� ������ 5.\n");
			else if (lessons[1] > lessons[2] and lessons[1] > lessons[0])puts("\n            �� ������� ������ ����� ������ 5.\n");
			else if (lessons[2] > lessons[1] and lessons[2] > lessons[0]) puts("\n            �� ���������� ������ ����� ������ 5.\n");
			else puts("\n            ����� ���������� ���.\n");
			system("pause");
		}break;
		case 9: {
			return 0;
		}
		default:
			break;
		}
	}
}


char menu() {
	char item;
	printMenu(MenuLabel);
	while ((item = _getch()) != 13) {
		if (item != -32) { fflush(stdin); item = 0; continue; }
		item = _getch();
		system("color F0");
		if (item == 80) {
			if (MenuLabel == 9) {
				MenuLabel = 1;
				printMenu(MenuLabel);
			}
			else
			{
				MenuLabel++; printMenu(MenuLabel);
			}
		}
		if (item == 72) {
			if (MenuLabel == 1) { MenuLabel = 9; printMenu(MenuLabel); }
			else { MenuLabel--; printMenu(MenuLabel); }
		}
		if (item == 27) {
			printMenu(9);
			return 9;
		}
	}
	return MenuLabel;
}

void printMenu(int MenuLabel) {
	string arrMenu[9] = { "1 - �������� ������ (���� ������ � ����������).","2 - �������� ������ (����� ������ � ��������� ����).","3 - ����������� ������ (����� ��������� ������).","4 - ���������� ������ �������� � ������.","5 - ���������� ������ � ����.","6 - �������� ������ �� ����� � �������� ��������� ������ �� �� ������.","7 - �������� ���� ��������� ������.","8 - ���������� ���������� � ������������ ����������� ������ 5.","9 - �����." };
	system("cls");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 5));
	puts("  ====================================================================");
	puts(" |        ����������� � ������� ��������� ����� � ����.               |");
	puts(" |         Enter ��� ������.Escape - ������� �����                    |");
	puts("  ====================================================================");
	SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));
	for (int i = 0; i < 9; i++) {
		if (i == (MenuLabel - 1)) {
			SetConsoleTextAttribute(hConsole, (WORD)((1 << 4) | 15));
			cout << arrMenu[i] << endl;
			continue;
		}
		SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));
		cout << arrMenu[i] << endl;
	}
	puts(" =====================================================================");
}

T* organization(T* top) {
	system("cls");
	STUDENT z;
	puts("����� ������������� �������� ��������� ,��������� �������� ������");
	if (top) {
		puts("������ ��� ��������,�� ������ ������������?(0-��)");
		if (_getch() == '0') { delete top; top = NULL; puts("������ ��������,������� �������"); return 0; }
	}
	while (1) {
		z = creation();
		if (strcmp(z.Sname, "*") != 0) top = addtree(top, z);
		else return top;
	}
}


T* addtree(T* top, const STUDENT& newnode) {

	if (!top)            //���� ��������� �� ������ �����, 
	{
		top = new T;      //�� �������� ������ ��� ���� 
		if (!top) {
			puts("�� ������� ������");
			return NULL;    //����� ���� ������ �� �������� 
		}
		top->data = newnode;    //������ ������ � ���� 
		top->left = NULL;     //��������� ���������� 
		top->right = NULL;
	}
	else                //����� 
		if (strcmp(top->data.Sname, newnode.Sname) > 0)  //���������� �������� � 
			top->left = addtree(top->left, newnode);  //��������� � �����  
						  //��������� 
		else
			top->right = addtree(top->right, newnode);//��� ������ ��������� 
	return top;         //���������� ��������� �� ������ ������ 
}


STUDENT creation() {
	STUDENT z;
	char buf[10], item;
	short i = 0;
	while (1) {
		/*       cin.clear();
			   cin.ignore(cin.rdbuf()->in_avail());*/
		puts("\n������� �������");
		gets_s(z.Sname, width);
		if ((z.Sname[0] == '*') and (z.Sname[1] == '\0'))
			return z;
		puts("���");
		gets_s(z.Fname, width);
		puts("��������");
		gets_s(z.Tname, width);
		puts("������� ������ : |������|�������|����������|\n");
		do {
			while (((item = _getch()) > '5') or (item < '1'));
			if (z.rating[i] = item)
				i++;
			printf_s("|%c", item);
		} while (i < 3);
		printf_s("|\n");
		return z;
	}
}


������� ������ ������ �������
void output(const STUDENT& z) {
	printf("|%3d)|%12s|%12s|%12s|     %-5c|    %-3c|   %-3c|\n", i, z.Sname, z.Fname, z.Tname, z.rating[0], z.rating[1], z.rating[2]);
	puts("|====|============|============|============|=========================|");
	for (int j = 0; j < 3; j++) {
		if (z.rating[j] == '5')
			lessons[j]++;
	}
}

//3 - ��������
void view(T* top) {
	if (top) {
		view(top->left);    i++; output(top->data);      //����� ������ ��������� 
		view(top->right);      //����� ������� ��������� 

	}
}


void otobr(T* top, int otstup)
{
	if (top)
	{
		otstup += 3;         //������ �� ���� ������ 
		otobr(top->right, otstup);    //����� ������� ��������� 
		//����� �������� �������, ���������������� ���� 

		cout << setw(otstup) << "(" << top->data.Sname << ")" << endl;
		otobr(top->left, otstup);     //����� ������ ��������� 
	}
}

int writing_to_file(FILE* FileVedomost, T* top) {
	if (top)
	{
		fwrite(top, sizeof(STUDENT), 1, FileVedomost);//������ � ����;
		writing_to_file(FileVedomost, top->left);    //������ ������ ��������� 
		writing_to_file(FileVedomost, top->right);    //������ ������� ��������� 
		fclose(FileVedomost);
	}
	return 0;
}


int reading_from_file(FILE* FileVedomost, T*& top)//������ �� ����� ��� �������
{
	STUDENT z; bool n = false;
	top = 0;
	do {
		n = fread(&z, sizeof(STUDENT), 1, FileVedomost);//������ ��������� �� ����� 
		if (n < 1) break;//���� n<1, �� ����� 
		top = addtree(top, z);//������� ������ ������� 
	} while (1);
	return 0;
}

T* cleaning(T* top)
{
	if (top) {
		if (top->left)
		{
			cleaning(top->left);
		}
		if (top->right)
		{
			cleaning(top->right);
		}
	}
	delete top;
	return top = NULL;
}