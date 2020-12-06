#include <stdio.h>
#include <conio.h>
#include <locale>
#include <Windows.h>
#include <iomanip>//для отображения дерева при помощи ф-ции setw()
#include <iostream>//для отображения дерева при помощи ф-ции setw()
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
T* addtree(T* top, const STUDENT& newnode);  //добавление узла 
char menu();               //отображение меню 
T* organization(T* top);
T* cleaning(T* top);
void view(T* top);  //просмотр  значений узлов 
//дерева слева направо 
void otobr(T* top, int otstup);
int writing_to_file(FILE* FileVedomost, T* top);    //запись в файл 
int reading_from_file(FILE* FileVedomost, T*& top);  //чтение из файла 
void printMenu(int MenuLabel);
STUDENT creation();            //ввод данных 

int main() {
	setlocale(LC_ALL, "rus"); SetConsoleCP(1251); SetConsoleOutputCP(1251);//рус.яз
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
			if (!top) { puts("Очередь пустая"); system("pause"); break; }
			i = 0;
			puts(" =====================================================================");
			puts("|  № |  Фамилия   |   Имя      |  Отчество  |           Оценки        |");
			puts("|====|============|============|============|=========================|");
			puts("|    |            |            |            |Физика|История|Математика|");
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
			puts(" |                        Сохранение 100%...                         |");
			puts("  ===================================================================");
			system("pause");
		}break;
		case 6: {
			FILE* FileVedomost = fopen("vedomost", "r+");
			if (!FileVedomost) { fopen("vedomost", "w"); }
			reading_from_file(FileVedomost, top);
			fclose(FileVedomost);
			puts("  ===================================================================");
			puts(" |                        Загрузка 100%...                           |");
			puts("  ===================================================================");
			system("pause");
		}  break;
		case 7: {
			top = cleaning(top);
			puts("  ===================================================================");
			puts(" |                        Очистка 100%...                            |");
			puts("  ===================================================================");
			system("pause");
		}break;
		case 8: {
			system("cls");
			if (!top) { puts("Очередь пустая"); system("pause"); break; }
			i = 0;
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 5));
			puts("      Определение дисциплины с максимальным количеством оценок 5.");
			puts(" =====================================================================");
			puts("|  № |  Фамилия   |   Имя      |  Отчество  |           Оценки        |");
			puts("|====|============|============|============|=========================|");
			puts("|    |            |            |            |Физика|История|Математика|");
			puts("|====|============|============|============|=========================|");
			for (int i = 0; i < 3; i++)lessons[i] = 0;
			view(top);;
			if (lessons[0] > lessons[1] and lessons[0] > lessons[2]) puts("\n            По Физике больше всего оценок 5.\n");
			else if (lessons[1] > lessons[2] and lessons[1] > lessons[0])puts("\n            По Истории больше всего оценок 5.\n");
			else if (lessons[2] > lessons[1] and lessons[2] > lessons[0]) puts("\n            По Математике больше всего оценок 5.\n");
			else puts("\n            Такой дисциплины нет.\n");
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
	string arrMenu[9] = { "1 - Создание дерева (ввод данных с клавиатуры).","2 - Просмотр дерева (вывод данных в табличном виде).","3 - Отображение дерева (вывод структуры дерева).","4 - Добавление нового элемента в дерево.","5 - Сохранение данных в файл.","6 - Загрузка данных из файла и создание бинарного дерева на их основе.","7 - Удаление всех элементов дерева.","8 - Определить дисциплину с максимальным количеством оценок 5.","9 - Выход." };
	system("cls");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 5));
	puts("  ====================================================================");
	puts(" |        Перемещение с помощью стрелочек вверх и вниз.               |");
	puts(" |         Enter для выбора.Escape - быстрый выход                    |");
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
	puts("Будет организованно создание ведомости ,используя бинарное дерево");
	if (top) {
		puts("Дерево уже созданно,не хотите перезаписать?(0-да)");
		if (_getch() == '0') { delete top; top = NULL; puts("Дерево удаленно,нажмите клавишу"); return 0; }
	}
	while (1) {
		z = creation();
		if (strcmp(z.Sname, "*") != 0) top = addtree(top, z);
		else return top;
	}
}


T* addtree(T* top, const STUDENT& newnode) {

	if (!top)            //если находимся на уровне листа, 
	{
		top = new T;      //то выделить память под узел 
		if (!top) {
			puts("Не хватает памяти");
			return NULL;    //выход если память не выделена 
		}
		top->data = newnode;    //запись данных в узел 
		top->left = NULL;     //обнуление указателей 
		top->right = NULL;
	}
	else                //иначе 
		if (strcmp(top->data.Sname, newnode.Sname) > 0)  //сравниваем значение в 
			top->left = addtree(top->left, newnode);  //добавляем в левое  
						  //поддерево 
		else
			top->right = addtree(top->right, newnode);//или правое поддерево 
	return top;         //возвращаем указатель на корень дерева 
}


STUDENT creation() {
	STUDENT z;
	char buf[10], item;
	short i = 0;
	while (1) {
		/*       cin.clear();
			   cin.ignore(cin.rdbuf()->in_avail());*/
		puts("\nВведите фамилию");
		gets_s(z.Sname, width);
		if ((z.Sname[0] == '*') and (z.Sname[1] == '\0'))
			return z;
		puts("Имя");
		gets_s(z.Fname, width);
		puts("Отчество");
		gets_s(z.Tname, width);
		puts("Введите оценки : |Физика|История|Математика|\n");
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


Функция вывода данных ученика
void output(const STUDENT& z) {
	printf("|%3d)|%12s|%12s|%12s|     %-5c|    %-3c|   %-3c|\n", i, z.Sname, z.Fname, z.Tname, z.rating[0], z.rating[1], z.rating[2]);
	puts("|====|============|============|============|=========================|");
	for (int j = 0; j < 3; j++) {
		if (z.rating[j] == '5')
			lessons[j]++;
	}
}

//3 - Просмотр
void view(T* top) {
	if (top) {
		view(top->left);    i++; output(top->data);      //обход левого поддерева 
		view(top->right);      //обход правого поддерева 

	}
}


void otobr(T* top, int otstup)
{
	if (top)
	{
		otstup += 3;         //отступ от края экрана 
		otobr(top->right, otstup);    //обход правого поддерева 
		//вывод значения фамилии, соответствующего узла 

		cout << setw(otstup) << "(" << top->data.Sname << ")" << endl;
		otobr(top->left, otstup);     //обход левого поддерева 
	}
}

int writing_to_file(FILE* FileVedomost, T* top) {
	if (top)
	{
		fwrite(top, sizeof(STUDENT), 1, FileVedomost);//запись в файл;
		writing_to_file(FileVedomost, top->left);    //запись левого поддерева 
		writing_to_file(FileVedomost, top->right);    //запись правого поддерева 
		fclose(FileVedomost);
	}
	return 0;
}


int reading_from_file(FILE* FileVedomost, T*& top)//чтение из файла при запуске
{
	STUDENT z; bool n = false;
	top = 0;
	do {
		n = fread(&z, sizeof(STUDENT), 1, FileVedomost);//чтение структуры из файла 
		if (n < 1) break;//если n<1, то конец 
		top = addtree(top, z);//создаем первый элемент 
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