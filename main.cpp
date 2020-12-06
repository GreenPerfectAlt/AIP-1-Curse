#include <iostream>
#include <cstdio>
#include <conio.h> 
#include <string>
#include <Windows.h>
#include <iomanip>
#define BLOCK cout<<" ============================================================================="<<endl;
#define CLS system("cls");
#define PAUSE system("pause");
#define CLEAR cin.clear(); cin.ignore(cin.rdbuf()->in_avail());
#define TOUCH puts("Для продолжения нажмите клавишу");
#pragma warning(disable : 4996)
//---обявления---//
using std::cout;
using std::cin;
using std::endl;
//---константы---//
const int MENUITEMS = 12;
const char WIDTH30 = 30;
const char WIDTH4 = 5;
const char WIDTH10 = 10;
const char MENUDOWN = MENUITEMS - 1;
const int MENUVIEWITEMS = 4;
const char* arrMenu[MENUITEMS] = {//пункты меню
         "Организация --> 1",
         "Просмотр --> 2" ,
		 "Обработка таблицы --> 3",
		 "Поиск записи --> 4",
         "Добавление  записи --> 5" ,
         "Удаление записи --> 6",
         "Изменение записи--> 7",
         "Сортировка  таблицы --> 8",
         "Очистка таблицы --> 9",
         "Сохранение --> 10",
         "Выход  --> 0",
         //Навзание работы
         "Курсовая работа по дисциплине «Информатика и программирование»\n\nСтруктура записей входного файла имеет следующий вид: табельный номер, Ф.И.О. (30 символов), год рождения, пол(булевская переменная), профессия(10 символов), стаж работы,разряд рабочего, номер цеха, номер участка, сумма заработной платы.\n"
};


//---типы и глобальные переменные---//
enum TABLE
{
    VIEW,//"Просмотр --> 2" ,
    MOD,//"Обработка таблицы --> 3",
	
    SCN,//"Поиск записи --> 4",
    ADD,//"Добавление  записи --> 5" ,
    DEL,//"Удаление записи --> 6",
    ALT,//"Изменение записи--> 7",
	
    SORTUP,//"Сортировка  таблицы --> 8",ПО ВОЗВРАСТАНИЮ
    SORTDOWN,//"Сортировка  таблицы --> 8",ПО УБЫВАНИЮ
    CLR//"Очистка таблицы --> 9",
};

//структура - рабочий
struct WORK
{
    char
	key[4],//табельный номер
        year[4],//год рождения
    profession[10];//профессия
    unsigned char FIO[30];//ФИО 
	bool gender;//половая принадлежность
	unsigned short 
	work_experience,//стаж работы
	working_class,//разряд рабочего
	manufactory_number,//номер цеха
	site_number,//номер участка
	salary;//сумма заработной платы
};

//структура - двунаправленный список
struct LIST {          //элемент 2- направленного списка 
	WORK newnode;          //данные о телефоне 
	LIST* left;          //указатель на элемент слева 
	LIST* right;          //указатель на элемент справа
};

//переменные
bool SORT = 0;
int amountOfElements = 0;
int menuLabel = 1;
unsigned short sumWorkingClass[3][6] = {};
char* name_file;
LIST* go,*back;
WORK newWorker;
WORK emptyWorker = { {"" },{""},{" "},{""},
	{""},{},{},{},{},{} };
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);//дескриптор консоли,для работы с динамическим меню
////---прототипы функций---//

LIST* table_operations(LIST* go, TABLE TAG);
WORK creation();
LIST* organization(LIST* go);
LIST* addnode(WORK key, LIST* go);
LIST* cleaning(LIST* go);
LIST* readfile(LIST* go);
LIST* delnode(LIST* go, WORK newWorker);
int writing_to_file(FILE*write_file,LIST* go);
char menu();
void newBuf();
bool search(LIST* go,char*key[]);
int view(LIST* go, TABLE SORT,int i, int position);
int output(const WORK& newWorker, int i);
bool search(LIST* go, char* key, bool i, TABLE tag);
//---тестируемые ф-ции


//===Главная функция===/
int main()
 {
    //---русский язык---/
    setlocale(LC_ALL, "rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    go = readfile(go);
    /*чтение из файла или создание файла
     * Введите название файла из которого прочитатб / создание файла
     * инициализация всех данных через ф-цию
     */
    while(true){
        CLS; CLEAR;
        switch (menu())
        {

        case 1:
        {
            go = organization(go);
        }break;
        case 2:
        {
            go = table_operations(go, VIEW);
        }break;
        case 3:
        {
            go = table_operations(go, MOD);
        }break;
        case 4:
        {
            CLS;
            go = table_operations(go, SCN);
            PAUSE
        }break;
        case 5:
        {
            go = table_operations(go, ADD);
        }break;
        case 6:
        {
            go = table_operations(go, DEL);
        }break;
        case 7:
        {
            go = table_operations(go, ALT);
        }break;
        case 8:
        {
            if (SORT == true) 
                go = table_operations(go, SORTUP);
            else if ((SORT == false))
                go = table_operations(go, SORTDOWN);
        }break;
        case 9:{
            amountOfElements = 0;
            SORT = 0;
            go = cleaning(go);
            puts("Таблица очищенна"); PAUSE;
        }break;
        case 10:
        {
            CLS;
            char* buf = new char;
            cout << "Введите название файла для сохранения (если пусто,то текущий \" "<< name_file << "\" )\n"; CLEAR;
            cin.getline(buf, 255);
            if (strcmp(buf, "")) strcpy(name_file, buf);
            FILE* write_file = fopen(name_file, "w");
            writing_to_file(write_file, go);
            fclose(write_file);
            cout << "Сохраненно в файл /" << name_file << endl;
            system("pause");
        }break;
        case 11:
        {
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));
            puts("Программа завершена");
            return 0;
        }
        default: {
        }break;
        }
    }
}


//---функции чтения и сохранения---//

//чтение из файла
LIST* readfile(LIST* go) {
    name_file = new char;
    bool create_file = NULL;
    FILE* read_file;
    while (1) {
        cout << "Загрузка файла : введите название файла в текущей папке \n";
        cin >> name_file;
        read_file = fopen(name_file, "r");
    	
        if (!read_file) {
            cout << "\nФайл " << name_file << " не найден в текущей папке\n";
            cout << "Хотите создать?(1/0) : ";
            cin >> create_file;
        	
            if (create_file == 0)
            {
                cout << "\nВыберете другой файл\n";
                BLOCK;
                continue;
            }
            else
            {
                read_file = fopen(name_file, "a"); fclose(read_file); amountOfElements = 0; return go;
            }

			}else {
            /*----чтение записей из файла----*/
            do
            {
                create_file = fread(&newWorker, sizeof(WORK), 1, read_file); //чтение структуры из файла
                if (create_file < 1)
                    break;             //если n<1, то конец
                go = addnode(newWorker, go); //создаем первый элемент
                amountOfElements++;
            } while (1);
			PAUSE;
            fclose(read_file);
            return go;
        }
    }
}

//сохранение в файл
int writing_to_file(FILE*write_file, LIST* go)
{
    if (go)
    {
        fwrite(go, sizeof(WORK), 1, write_file); //запись в файл;
        writing_to_file(write_file, go->left);      //запись левого поддерева
        writing_to_file(write_file, go->right);     //запись правого поддерева
        
    }
    return 0;
}


//====функции работы со списком====//

//---создание записи
WORK creation()
	{
    CLEAR; BLOCK;
    bool j = false;
    while (j == false) {
        CLEAR;
        if (!(strcmp(newWorker.key, ""))) {
            cout << "Введите табельный номер(4 цифры c 0001 номера) или Enter для прекращения записи:";
            cin.getline(newWorker.key, 5);
            if (!(strcmp(newWorker.key, ""))) {
                return newWorker;
            }
            for (int i = 0; i < 4; i++)
            {
                if (!((newWorker.key[i] > 47) and (newWorker.key[i]) <= 57)) {
                    j = true;
                    break;
                }
            }
            if (j == true) {
                j = false;
                cout << "!Введите корректное значение!\n";
                strcpy_s(newWorker.key, "");
            	continue;
            }
            else break;
        }
        else {
            cout << "Табельный номер введен " << newWorker.key[0] << newWorker.key[1] <<
                newWorker.key[2] <<
                newWorker.key[3] << endl; _getch_nolock(); TOUCH; break;
        }
    }
    while (j == false) {
        cout << "Введите Ф.И.О (30 символов:Иванов Иван Иванович,без чисел и пробела в конце): \n";
        byte space = 0;
        CLEAR;
        fgets((char*)newWorker.FIO, WIDTH30, stdin);
        for (int i = 0; i < WIDTH30; i++)
        {

        	if((space>=1)and(newWorker.FIO[i]== newWorker.FIO[i-1]==0x20)){
                j = true;
                break;
            }
            if ((!((newWorker.FIO[i] > 191) and (newWorker.FIO[i]) <= 255))){
                if (newWorker.FIO[i] == 0x20) {
                    space++; continue;
                }
            	if((newWorker.FIO[i]==10)and(space==2)and(newWorker.FIO[i-1] > 191) and (newWorker.FIO[i-1] <= 255))
            	{
                    newWorker.FIO[i] = NULL;
                    break;
            	}
                j = true;
                break;
            }
        }
        if (j == true) {
            j = false;
            cout << "!Введите корректное значение!\n";
            continue;
        }
        else break;
    }
    while (1){
        cout << "Введите год рождения(1960-2000гг): ";
        CLEAR;
        cin.getline(newWorker.year, WIDTH4);
        if (((newWorker.year[0] == 49) and (newWorker.year[1] == 57) and (((newWorker.year[2] >= 54) and ((newWorker.year[2] < 58)))))) 
            break;
        if (((newWorker.year[0] == 50) and (newWorker.year[1] == 48) and ((newWorker.year[2] == 48) and (newWorker.year[3] == 48)))) 
             break;
        if (newWorker.year[4] != 0)
            break;
        cout << "!Введите корректное значение!\n";
        continue;
    }

    	cout << "Введите пол (не 0 - женщина) : ";
        CLEAR;
        cin >> newWorker.gender;
    	cout << "Введите профессию (10 символов) : ";
        CLEAR;
        cin.getline(newWorker.profession, WIDTH10);
    while (1) {
        cout << "Введите стаж работы : [2-42]";
        CLEAR;
        cin >> newWorker.work_experience;
        if ((newWorker.work_experience < 2) or (newWorker.work_experience > 42))
            cout << "!Введите корректное значение!\n";
        else break;
    }
    while (1) {
        cout << "Введите разряд рабочего [1-3]: ";
        CLEAR;
        cin >> newWorker.working_class;
        if ((newWorker.working_class < 1) or (newWorker.working_class > 3))
            cout << "!Введите корректное значение!\n";
        else break;
    }
        while (1) {
            cout << "Введите номер цеха[1-100] : ";
            CLEAR;
            cin >> newWorker.manufactory_number;
            if (((newWorker.manufactory_number >= 1) and (newWorker.manufactory_number <= 100)))
                break;
            cout << "!Введите корректное значение!\n";
        }
        while (1) {
            cout << "Введите номер участка[1-100] : ";
            CLEAR;
            cin >> newWorker.site_number;
            if (((newWorker.site_number >= 1) and (newWorker.site_number <= 100)))
                break;
            cout << "!Введите корректное значение!\n";
        }
        while (1) {
            cout << "Введите заработную плату([15-100] в тыс.рублей) : ";
            CLEAR;
            cin >> newWorker.salary; ;//сумма заработной платы
            if (((newWorker.salary >= 15) and (newWorker.salary <= 100)))
                break;
            cout << "!Введите корректное значение!\n";
        }
       
    return newWorker;
}

//---поиск элемента в таблице

bool search(LIST* go, char* key,bool i,TABLE tag)
{
    bool equal = false;
    if (go)
    {
        for (int i = 0; i < 4; i++)
        {
            if (go->newnode.key[i] != key[i])
                equal = true;
            if (equal) break;
        }
    	if(!equal){
            if (tag == ALT) {
                newWorker = creation();
                go->newnode = newWorker;
            }
            i++;
            return i;
        }
       i = search(go->right, key,i,tag);
       i = search(go->left, key,i,tag);  //обход правого поддерева
    }
    return i;
}

//---организация записей
LIST* organization(LIST* go)
{
    CLS; CLEAR;
    cout << "Создание таблицы\n\n";
    if ((go))
    {
        puts("Таблица уже созданна,не хотите перезаписать?(1-да)");
        if (cin.get() == '1')
        {
            go = cleaning(go);
            puts("Таблица удалена"); TOUCH;
        }
        else { TOUCH; return go; }
    }
    if(sumWorkingClass)
    {
        for (unsigned i = 0; i < 3; i++)
            for (unsigned j = 0; j < 6; j++)
                sumWorkingClass[i][j] = 0;
    }
    amountOfElements = 0;
    while (1)
    {
        strcpy(newWorker.key, "");
        newWorker = creation();
        if ((strcmp(newWorker.key, ""))) {
           go = addnode(newWorker, go);
            if (!(strcmp(go->newnode.key, "000"))) {
            	continue;
            }
            back = new LIST;
            back->newnode = go->newnode;
            back->left = go->left;
            back->right = back->right;
            amountOfElements++;
        }
        else {
            cout << "\nЗапись окончена\n";
            return go;
        }
    }
}


//---очистка таблицы
LIST* cleaning(LIST* go)
{
    amountOfElements = 0;
    if (go)
    {
        if (go->left)
        {
            cleaning(go->left);
        }
        if (go->right)
        {
            cleaning(go->right);
        }
    }
    else return NULL;
    delete go;
    return go = NULL;
}

//---функции работы с таблицей

LIST* table_operations(LIST*go,TABLE TAG)
{
    CLS;
    char itemOfTable = 0;
    int positionOfElement = 1;
    if (!(go)and((TAG!=ADD)and(TAG!=SCN))) {
        puts("Таблица не созданна");
        PAUSE; return go;
    }
	else if(TAG != MOD){
        while (1)
        {
            CLS;BLOCK;
            cout << "Структура записей входного файла имеет следующий вид: табельный номер, Ф.И.О. \n(30 символов), год рождения, пол (булевская переменная), профессия (10 символов), стаж работы, разряд рабочего, номер цеха, номер участка, сумма заработной платы." << endl; BLOCK; BLOCK;
            if (!(positionOfElement > amountOfElements)) {
                if (SORT == false)
                    view(go, SORTUP, 0, positionOfElement);
                else
                    view(go, SORTDOWN, 0, positionOfElement);
            }
            else {
                positionOfElement = 1;
                if (SORT == false)
                    view(go, SORTUP, 0, positionOfElement);
                else
                    view(go, SORTDOWN, 0, positionOfElement);
            }
            BLOCK;CLEAR;
            puts("Перемещение --> стрелочки  вверх и вниз");
            puts("Запуск команды  --> Enter");
            puts("Для выхода --> ESC");
            SORT ? puts("Отсортировано по убыванию") : puts("Отсортировано по возврастанию");
        	BLOCK;
            TOUCH;; CLEAR; BLOCK;
            itemOfTable = _getch_nolock();
            if (itemOfTable == 27) {
                puts("Нажата клавиша ESC - операция завершена");
                Sleep(1000);
                return go;
            }
            else if (itemOfTable == 13) {
                switch (TAG)
                {
                case VIEW:{
                    continue; }
                case SORTDOWN:case SORTUP:
                {
                  SORT ? puts("Операция : сортировка по возврастанию"): puts("Операция : сортировка по убыванию");
                  SORT ? SORT=false: SORT=true;
                 Sleep(1000); continue;
                }break;
                case SCN: {
                    puts("Операция : поиск работника по табельному номеру");
                    puts("Введите табельный номер(0000-9999) : ");
                    CLEAR;
                	cin.getline(newWorker.key, 5);
                    if (search (go, newWorker.key,0, VIEW))
                    {
                        BLOCK;
                        cout << "Элемент " << newWorker.key << " записан в таблице\n";
                        BLOCK; TOUCH;;
                    }
                    else 
                    {
                        BLOCK;
                        cout << "Элемент " << newWorker.key << " не записан в таблице\n";
                        BLOCK; TOUCH;;
                    }break;
                }
                case ADD :
                {
                    int i = 0;
                    puts("Операция : добавление работника по табельному номеру");
                    puts("Введите табельный номер(0000-9999) : ");
                    cin.getline(newWorker.key, 5);
                    bool j = false;
                    for (int i = 0; i < 4; i++)
                    {
                        if ((!((newWorker.key[i] > 47) and (newWorker.key[i]) <= 57))or(!strcmp(newWorker.key, "0000"))) {
                            cout << "Неверный ввод\n";
                        	
                            j = true;
                            break;
                        }
                    }
                    if (j) { TOUCH; _getchar_nolock(); continue; };
                    newWorker.key[4] = NULL;
                    if (go) {
                        if ((i = search(go, newWorker.key, 0, VIEW)) == true)
                        {
                            BLOCK;
                            cout << "Элемент " << newWorker.key << " записан в таблице\n";
                            BLOCK; TOUCH;;
                        }
                        else if (!(strcmp(newWorker.key, "")))
                        {
                            puts("Отмена записи данного табельного номера"); TOUCH; _getch_nolock(); continue;
                        }
                        else {
                            cout << "Элемент    " << newWorker.key << " не записан в таблице\n"; Sleep(2000);
                            if (amountOfElements == 9999)
                            {
                                puts("Превышен лимит записей (0001-9999)"); continue;
                            }
                            puts("Будет произведенна запись"); Sleep(1000);
                        }
                    }
                        BLOCK; CLS;
                        newWorker = creation();
                        go = addnode(newWorker, go);
                        if (!(strcmp(go->newnode.key, "000"))) {
                            continue;
                        }
                        TOUCH;
                        back = new LIST;
                        back = go;
                        back->newnode = go->newnode;
                        back->left = go->left;
                        back->right = go->right;	
                        amountOfElements++;
                    }
	                break;
                 case DEL:
                 {
                     puts("Операция : удаление работника по табельному номеру");
                     puts("Введите табельный номер(0000-9999) : ");
                     cin.getline(newWorker.key, 5);
                     if (!(search(go, newWorker.key, 0, VIEW)))
                     {
                         BLOCK;
                         cout << "Элемент " << newWorker.key << " не записан в таблице\n";
                         BLOCK; TOUCH;;
                     }
                     else if (!(strcmp(newWorker.key, "")))
                     {
                         puts("Отмена записи данного табельного номера"); TOUCH; _getch_nolock(); continue;
                     }
                     else {
                         cout << "Элемент " << newWorker.key << " записан в таблице\n";
                         puts("Будет произведененно удаление");
                         go = delnode(go, newWorker);
                         back = new LIST;
                         back->newnode = go->newnode;
                         back->left = go->left;
                         back->right = back->right;
                         amountOfElements--;
                     }
                 }
                    break;
                case ALT:{
                    puts("Операция : изменение данных работника по табельному номеру");
                    puts("Введите табельный номер(0000-9999) : ");
                    cin.getline(newWorker.key, 5);
                    if (!(search(go, newWorker.key, 0, VIEW)))
                    {
                        BLOCK;
                        cout << "Элемент " << newWorker.key << " не записан в таблице\n";
                        BLOCK; TOUCH;;
                    }
                    else if (!(strcmp(newWorker.key, "")))
                    {
                        puts("Отмена записи данного табельного номера"); TOUCH; _getch_nolock(); continue;
                    }
                    else {
                        cout << "Элемент " << newWorker.key << " записан в таблице\n";
                        puts("Будет произведененно изменение");
                        search(go, newWorker.key, 0, ALT);
                    }
                }
                    break;
                default:
                    break;
                }
            }
            itemOfTable = _getch_nolock();
            if (itemOfTable == 80) {
                if (positionOfElement >= amountOfElements) {
                    positionOfElement = 1; continue;
                }
                if (positionOfElement + MENUVIEWITEMS >= amountOfElements)
                    positionOfElement = 1;
                else { positionOfElement += MENUVIEWITEMS; }
            }
            else if (itemOfTable == 72) {
                if (positionOfElement <= 1) {
                    positionOfElement = amountOfElements - MENUVIEWITEMS; continue;
                }
                if (positionOfElement - MENUVIEWITEMS <= 0) {
                    positionOfElement = 1; continue;
                }
                positionOfElement -= MENUVIEWITEMS;
            }
            else{
                itemOfTable = 0;
                continue;
            }
        }
		}
    else//tag == MOD,изменение таблицы
    {
    BLOCK;
        cout <<"\t\tВид выходной таблицы для варианта 9" << std::setw(30)<<endl; BLOCK;
    cout << "|" << std::setw(20) << "|" << std::setw(5) << "\tКоличество рабочих по разрядам" << std::setw(5) << "|" << std::setw(21) << "|\n" <<
        "|" << "    СТАЖ РАБОТЫ" << std::setw(5) << "|" << std::setw(5) << "=====================================" << "|" << "      ВСЕГО" << std::setw(10) << "|\n"
        << "|" << std::setw(20) << "|" << std::setw(6) << "1" << std::setw(6) << "|" << std::setw(7) << "2" << std::setw(6) << "|" << std::setw(7) << "3" << std::setw(6) << "|" << std::setw(21) << "|\n"
        << " =============================================================================\n"
        << "|До 6" << std::setw(16) << "|" << std::setw(6) << sumWorkingClass[0][0] << std::setw(6) << "|" << std::setw(7)<< sumWorkingClass[1][0]<< std::setw(6)<< "|" << std::setw(7)<< sumWorkingClass[2][0] << std::setw(6) << "|" << std::setw(9) << sumWorkingClass[2][0] + sumWorkingClass[1][0]+ sumWorkingClass[0][0] <<std::setw(12) << "|\n"
        << " =============================================================================\n"
        << "|C 6 до 11" << std::setw(11) << "|" << std::setw(6) << sumWorkingClass[0][1] << std::setw(6) << "|" << std::setw(7) << sumWorkingClass[1][1] << std::setw(6) << "|" << std::setw(7)<< sumWorkingClass[2][1] << std::setw(6) << "|" << std::setw(9) << sumWorkingClass[2][1] + sumWorkingClass[1][1] + sumWorkingClass[0][1] << std::setw(12)<<"|\n"
        << " =============================================================================\n"
        << "|С 11 до 16" << std::setw(10) << "|" << std::setw(6) << sumWorkingClass[0][2] << std::setw(6) << "|" << std::setw(7) << sumWorkingClass[1][2] << std::setw(6) << "|" << std::setw(7) << sumWorkingClass[2][2] << std::setw(6) << "|" << std::setw(9) << sumWorkingClass[2][2] + sumWorkingClass[1][2] + sumWorkingClass[0][2] << std::setw(12) << "|\n"
        << " =============================================================================\n"
        << "|До 16 до 21" << std::setw(9) << "|" << std::setw(6) << sumWorkingClass[0][3] << std::setw(6) << "|" << std::setw(7) << sumWorkingClass[1][3] << std::setw(6) << "|" << std::setw(7) << sumWorkingClass[2][3] << std::setw(6) << "|" << std::setw(9) << sumWorkingClass[2][3] + sumWorkingClass[1][3] + sumWorkingClass[0][3] << std::setw(12) << "|\n"
        << " =============================================================================\n"
        << "|До 21 до 25" << std::setw(9) << "|" << std::setw(6) << sumWorkingClass[0][4] << std::setw(6) << "|" << std::setw(7) << sumWorkingClass[1][4] << std::setw(6) << "|" << std::setw(7) << sumWorkingClass[2][4] << std::setw(6) << "|" << std::setw(9) << sumWorkingClass[2][4] + sumWorkingClass[1][4] + sumWorkingClass[0][4] << std::setw(12) << "|\n"
        << " =============================================================================\n"
        << "|Свыше 25" << std::setw(12) << "|" << std::setw(6) << sumWorkingClass[0][5] << std::setw(6) << "|" << std::setw(7) << sumWorkingClass[1][5] << std::setw(6) << "|" << std::setw(7) << sumWorkingClass[2][5] << std::setw(6) << "|" << std::setw(9) << sumWorkingClass[2][5] + sumWorkingClass[1][5] + sumWorkingClass[0][5] << std::setw(12) << "|\n"
        << " =============================================================================\n";}TOUCH; _getchar_nolock();}

//---функции работы с таблицей
int output(const WORK& newWorker,int i){
    cout <<"|"<<newWorker.key[0] << newWorker.key[1] << newWorker.key[2] << newWorker.key[3] << "|" << newWorker.year[0] << newWorker.year[1] << newWorker.year[2] << newWorker.year[3] << "|" << std::setw(10)<< newWorker.profession << "|" << std::setw(30) << newWorker.FIO << "|" << std::setw(4) << newWorker.work_experience  << "|" << std::setw(2) << newWorker.working_class  << "|" << std::setw(4) << newWorker.manufactory_number << "|" << std::setw(3) << newWorker.site_number << "|" << std::setw(6) << newWorker.salary << "|";
    if (newWorker.gender == 0) cout << "М|\n"; else cout << "Ж|\n"; BLOCK; return i++;}

int view(LIST * go,TABLE SORT, int i ,int position)
{
	if(!go)    return i;
    if ((go) and (SORT == SORTUP))
        i = view(go->left, SORT, i, position);
    else if ((go) and (SORT == SORTDOWN))
        i = view(go->right, SORT, i, position);
        i++;
        if ((i >= position) and (i <= position + MENUVIEWITEMS)) {
            output(go->newnode, i); //обход левого поддерева
        }
        else if (i > position + MENUVIEWITEMS - 1) return position + MENUVIEWITEMS;
        if(i == amountOfElements)
        {
	        for(auto j = amountOfElements; j < (position + MENUVIEWITEMS);j++)
                output(emptyWorker, i); //запись пустых элементов
            return -9999;}
        if ((go) and (SORT == SORTUP))
			i = view(go->right, SORT, i, position);//обход правого поддерева
        else if ((go) and (SORT == SORTDOWN))
            i = view(go->left, SORT, i, position);}

LIST* addnode(WORK newWorker, LIST* go) {
    if (go == NULL)
    {
        go = new LIST;
        go->newnode = newWorker;
        go->left = NULL;
        go->right = NULL;
        return go;
    }
    else if ((newWorker.key[0]==go->newnode.key[0])and(newWorker.key[1] == go->newnode.key[1])and(newWorker.key[2] == go->newnode.key[2])and(newWorker.key[3] == go->newnode.key[3]))
    {
        BLOCK;
        cout << "Ошибка - табельный номер " << go->newnode.key[0] << go->newnode.key[1] << go->newnode.key[2] << go->newnode.key[3] << " уже записан, работник - " << go->newnode.FIO << endl;
        for (int i = 0; i < 3; i++)
            go->newnode.key[i] = '0';
        go->newnode.key[3] = '\0';
        return go;
    }
    else {

        if (newWorker.work_experience > 25)
            sumWorkingClass[newWorker.working_class - 1][5] ++;
        else if (newWorker.work_experience >= 21)
            sumWorkingClass[newWorker.working_class - 1][4] ++;
        else if (newWorker.work_experience >= 16)
            sumWorkingClass[newWorker.working_class - 1][3] ++;
        else if (newWorker.work_experience >= 11)
            sumWorkingClass[newWorker.working_class - 1][2] ++;
        else if (newWorker.work_experience >= 6)
            sumWorkingClass[newWorker.working_class - 1][1] ++;
        else if (newWorker.work_experience < 6)
            sumWorkingClass[newWorker.working_class - 1][0] ++;

    	
        for (int i = 0; i < 4; i++)
        {
            if (newWorker.key[i] < go->newnode.key[i]) {
                go->left = addnode(newWorker, go->left);
                return(go);
            }
            else if (newWorker.key[i] > go->newnode.key[i]) {
                go->right = addnode(newWorker, go->right);
                return(go);
            }
            else { continue; }
        }
    }
}


LIST* delnode(LIST* go, WORK newWorker) {
    if (go == NULL)
        return go;
    if ((newWorker.key[0] == go->newnode.key[0]) and (newWorker.key[1] == go->newnode.key[1]) and (newWorker.key[2] == go->newnode.key[2]) and (newWorker.key[3] == go->newnode.key[3])) {
        LIST* tmp;
        if (go->right == NULL)
            tmp = go->left;
        else {
            LIST* ptr = go->right;
            if (ptr->left == NULL) {
                ptr->left = go->left;
                tmp = ptr;
            }
            else {
                LIST* pmin = ptr->left;
                while (pmin->left != NULL) {
                    ptr = pmin;
                    pmin = ptr->left;
                }
                ptr->left = pmin->right;
                pmin->left = go->left;
                pmin->right = go->right;
                tmp = pmin;
            }
        }
        delete go;
        return tmp;
    }
    for (int i = 0; i < 4; i++)
    {
        if (newWorker.key[i] < go->newnode.key[i]) {
            go->left = delnode(go->left, newWorker);
            return(go);
        }
        else if (newWorker.key[i] > go->newnode.key[i]) {
            go->right = delnode(go->right, newWorker);
            return(go);
        }
        else { continue; }
    }
    return go;
}

//----Вывод главного меню---///
void printMenu(int menuLabel)
{
    system("color F0");
    CLS;
    BLOCK;
    cout<<arrMenu[MENUITEMS-1];
    BLOCK;
    for (int i = 0; i < MENUITEMS-1; i++)
    {
        if (i == (menuLabel - 1))
        {
            SetConsoleTextAttribute(hConsole, (WORD)((1 << 4) | 15));
            cout << arrMenu[i] << endl;
            continue;
        }
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));
        cout << arrMenu[i] << endl;
    }
    BLOCK;
}

//---динамическое управление через стрелочки
char menu()
{
    CLS;char item;
    printMenu(menuLabel);
    while ((item = _getch_nolock()) != 13)
    {
        if ((item != -32) and (item != 27))
        {
            CLEAR;
            item = 0;
            continue;
        }
        item = _getch_nolock();
        switch (item) {
        case 80:
        {
            if (menuLabel == MENUDOWN)
            {
                menuLabel = 1;
                printMenu(menuLabel);
            }
            else
            {
                menuLabel++;
                printMenu(menuLabel);
            }
        }break;
        case 72:
        {
            if (menuLabel == 1)
            {
                menuLabel = MENUDOWN;
                printMenu(menuLabel);
            }
            else
            {
                menuLabel--;
                printMenu(menuLabel);
            }
        }break;
        case 27:
        {
            printMenu(MENUDOWN);
            return MENUDOWN;
        }break;
        default:{
            continue;
        }
            break;
        }
    }
	
    return menuLabel;
}

