/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			Севастопольский государственный университет 
				Кафедра «Информационные системы»
			Программа для работы с базой данных о рабочих


										Текст программы	разработал
										   Студент гр. ИC/б-19-2-о
														Трушин Д.А

							 2020 год
 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Программа работает с базой данных о рабочих цеха.
В качестве структуры используется структура с информацией о рабочем,
а также организующий двунаправленный список,построенный по принципу
бинарного дерева.

Структура записей входного файла имеет следующий вид: табельный номер,
Ф.И.О. (30 символов),\год рождения, пол (булевская переменная),
профессия(10 символов),стаж работы, разряд рабочего, номер цеха, номер
участка,сумма заработной платы.

Основные функции программы:

-   Чтение базы данных из файла
-   Организация списка
-   Просмотр структуры записей с возможностью скроллинга
-   Обработка структуры записей по заданию
-   Поиск записи
-   Добавление в список
-   Удаление из списка
-   Изменение в записи
-   Сортировка по убыванию или по возрастанию
-   Очистка списка
-   Сохранение в текстовый файл
-   Выход и бекап


Вариант задания 9. Утверждено 08.09.2020
Среда программирования Visual Studio C++ version 16.7.7
Дата последней коррекции: 09.12.2020.
Версия 1.0

 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include <iostream>
#include <fstream>
#include <cstdio>
#include <conio.h> 
#include <string>
#include <Windows.h>
#include <iomanip>
#define BLOCK cout<<" ============================================================================= "<<endl
#define CLS system("cls")
#define PAUSE system("pause")
#define CLEAR cin.clear(); cin.ignore(cin.rdbuf()->in_avail())//очистка буфера ввода
#pragma warning(disable : 4996)//отключение предупреждения для Visual Studio

 //---------------------------обявления--------------------------//
using std::ofstream;
using std::cout;
using std::cin;
using std::endl;

//---------------------------константы--------------------------//

const char *BASEFILE = "BASEFILE";  //имя файла с базой данных				
const int  MENUITEMS = 12;           //количество пунктов меню + шапка
const char WIDTH1 = 5;              //ширина для табельного номер(КЛЮЧ),года рождения
const char WIDTH2 = 10;             //ширина в 10 символов для профессии
const char WIDTH3 = 30;             //ширина в 30 для ФИО
const char WIDTHF = 25;;            //размер названия файла
const char MENUDOWN = MENUITEMS - 1;//последний пункт меню
const int  MENUVIEWITEMS = 4;        //кол-во отображаемых элементов в структуре записей
const int  NOWYEAR = 2020;           //текущий год
const char* arrMenu[MENUITEMS] = {  //пункты меню
         "Организация",
         "Просмотр" ,
		 "Обработка структуры записей и сохранение",
		 "Поиск записи",
         "Добавление  записи" ,
         "Удаление записи",
         "Изменение записи",
         "Сортировка  структуры записей",
         "Очистка структуры записей",
         "Сохранение в текстовый файл",
         "Выход  --> ESC",
         //Навзание работы
         "Курсовая работа по дисциплине «Информатика и программирование»\n\nСтруктура записей входного файла имеет следующий вид: табельный номер, Ф.И.О. (30 символов), год рождения, пол(булевская переменная), профессия(10 символов), стаж работы,разряд рабочего, номер цеха, номер участка, сумма заработной платы.\n"};

//--------------------------типы и глобальные переменные--------------------------//

enum TABLE{
    VIEW,    //"Просмотр --> 2" ,
    MOD,     //"Обработка структуры записей --> 3",для текстового 
    SCN,     //"Поиск записи --> 4",
    ADD,     //"Добавление  записи --> 5" ,
    DEL,     //"Удаление записи --> 6",
    ALT,     //"Изменение записи--> 7",
    SORTUP,  //"Сортировка  структуры записей --> 8",ПО ВОЗВРАСТАНИЮ
    SORTDOWN,//"Сортировка  структуры записей --> 8",ПО УБЫВАНИЮ
    CLR,};   //"Очистка структуры записей --> 9",

struct WORK{                  //структура - рабочий
    char
	key[WIDTH1],              //табельный номер
        year[WIDTH1],         //год рождения
    profession[WIDTH2];       //профессия
    unsigned char FIO[WIDTH3];//ФИО 
	bool gender;              //половая принадлежность
	unsigned short 
	work_experience,          //стаж работы
	working_class,            //разряд рабочего
	manufactory_number,       //номер цеха
	site_number,              //номер участка
	salary;};                 //сумма заработной платы

struct LIST {       //структура - двунаправленный список      
	WORK newnode;   //данные о рабочем 
    LIST* left;     //указатель на элемент с меньшим табельным номером 
    LIST* right;};  //указатель на элемент с большим табельным номером 

bool SORT = 0;                              //направленность сортировки
int amountOfElements = 0,                   //кол-во элементов в структуре
	menuLabel = 1;                          //текущая позиция индикатора выбора в меню
unsigned short sumWorkingClass[3][6] = {};  //массив рабочих по разрядам(3) и стажу(6)
char* name_file = new char[WIDTHF];            //имя  файла
char* buf = new char[WIDTHF];                  //временная переменная для записи строки
LIST* go,*back;                             //указатели вперед и назад
WORK newWorker;                             //новый рабочий
WORK emptyWorker = {                         //Используемая для вывода пустых значений в таблице стуктура
	{"~~~" },{"~~~"},{""},{""},
	{-1},{0},{0},{0},{0},{0}};

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);//дескриптор консоли,для работы с динамическим меню

//--------------------------прототипы функций--------------------------//

//ф-ции для работы с элементами структуры
WORK creation_element();                                         //запись нового рабочего
LIST* add_element(WORK key, LIST* go);//добавление в структуру
bool search_element(LIST* go, char* key, bool i, TABLE tag);    //его поиск в ней
int moving_elements(LIST* go, TABLE SORT, int i, int position); //перемещие по бинарному дереву
int show_elements(const WORK& newWorker, int i);                //поэлементный вывод в консоль
LIST* delete_element(LIST* go, WORK newWorker);                 //удаление из структуры
//ф-ции для работы со структурой данных и таблицей
LIST* organization_table(LIST* go);         //создание структуры
LIST* cleaning_table(LIST* go);             //очистка структуры
LIST* operations_table(LIST* go, TABLE TAG);//ф-ция для работы с операциями над таблицей 
//ф-ции для работы с базой данных и текстовыми файлами
LIST* reading_from_file(LIST* go);                             //чтение из файла
int writing_to_base(FILE* write_file, LIST* go);               //запись в бинарный файл
void writing_to_txt(ofstream& write_file, LIST* go, TABLE tag);//запись в тектовый файл
//ф-ции для работы с динамическим меню
char scrolling_menu();         //перемещение по меню
void print_menu(int menuLabel);//вывод пунктов меню в консоль
//глобальные ф-ции для работы с изменяемыми хар-ми 
void global_clsSumWorkingClass();              //количество рабочих
void global_back(const LIST* go, TABLE tag);   //указатель на предыдущий элемент
void global_addSumWorkingClass(const WORK newWorker);
//==========================Главная функция============================//

int main(){
    //---русский язык---/
    setlocale(LC_ALL, "rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

	go = reading_from_file(go);
    while(true){//меню
        CLS; CLEAR;
        switch (scrolling_menu()){//возвращает символ
        case 1:{
            go = organization_table(go);}break;
        case 2:{
            go = operations_table(go, VIEW);}break;
        case 3:{
            go = operations_table(go, MOD);}break;
        case 4:{
            CLS;
            go = operations_table(go, SCN);
            PAUSE;}break;
        case 5:{
            go = operations_table(go, ADD);}break;
        case 6:{
            go = operations_table(go, DEL);}break;
        case 7: {
            go = operations_table(go, ALT); }break;
        case 8:{
            if (SORT == true) //если отсортированно по убыванию
                go = operations_table(go, SORTUP);
            else if ((SORT == false))//если отсортированно по возврастанию
                go = operations_table(go, SORTDOWN);}break;
        case 9:{
            SORT = false;
            go = cleaning_table(go);
            puts("Структура записей очищенна"); PAUSE;}break;
        case 10:{
            CLS;
            ofstream view_table_file;
            puts("Введите название файла для сохранения структуры записей"); CLEAR;
            cin.getline(buf, WIDTHF);
            if (strcmp(buf, ""))
                view_table_file.open(buf, std::ios::out, std::ios::trunc);
            else {
                strcpy(buf, "default_text_table.txt");
                view_table_file.open(buf, std::ios::out, std::ios::trunc);}
            writing_to_txt(view_table_file,go, VIEW);
            view_table_file.close();
            printf("Сохраненно в файл /%s\n", buf);
            PAUSE;}break;
        case 11:{
            CLS;
            FILE* base_file = fopen(BASEFILE, "w");
            writing_to_base(base_file, go);
            fclose(base_file);
            puts("База данных сохраненна");
            puts("Не хотите скопировать её в другой файл?(1/0 -->ENTER)");
            if (cin.get() == 49) {
                puts("Введите название файла : "); CLEAR;
                    cin.getline(name_file, WIDTHF);
            		FILE* write_file = fopen(name_file, "w");
                    writing_to_base(write_file, go);}
			PAUSE;return 0;}
        default: {
        }break;}}}


//--------------------------ф-ции чтения и сохранения--------------------------//

LIST* reading_from_file(LIST* go) {//чтение из файла
    strcpy(name_file, BASEFILE);
    bool create_file = NULL;
    puts("Загрузка базы данных из файла \"BASEFILE\"");
    FILE* base_file = fopen(BASEFILE, "r");
    FILE* read_file;
    while (!(base_file)or(getc(base_file) == EOF)){
        puts("База данных пустая");
        puts("Хотите загрузить из другого файла?(1/0 -->ENTER) : ");
        cin >> create_file;
        if (create_file == 0) {
            puts( "Будет создана пустая база данных");
            amountOfElements = 0;
            BLOCK;PAUSE;
            return go;}
        else {
            fseek(base_file, 0, SEEK_SET);
            puts( "Введите название файла : ");CLEAR;
            cin >> name_file;
            read_file = fopen(name_file, "a+t");
            do{
                create_file = fread(&newWorker, sizeof(WORK), 1, read_file); //чтение структуры из файла
                if (create_file < 1)
                    break;//если n<1, то конец
                go = add_element(newWorker, go); //создаем первый элемент;
            } while (1);
            if (!go) { puts("Файл пустой,попробуйте ещё раз"); continue; }
            FILE* base_file = fopen(BASEFILE, "w");
            writing_to_base(base_file, go);
            fclose(base_file);
            puts( "Данные успешно сохранились в базу");
            fclose(read_file);PAUSE;return go;}}
        /*----чтение записей из файла----*/
		fseek(base_file, 0, SEEK_SET);
        do {
            create_file = fread(&newWorker, sizeof(WORK), 1, base_file); //чтение структуры из файла
            if (create_file < 1)
                break;//если n<1, то конец
            go = add_element(newWorker, go); //создаем первый элемент
        } while (1);;
        puts( "Данные успешно  загруженны");;
        fclose(base_file);
        PAUSE;
        return go;}


int writing_to_base(FILE*write_file, LIST* go){//сохранение в бинарный файл
    if (go){
        fwrite(go,sizeof(WORK), 1, write_file);
        writing_to_base(write_file, go->left);      //запись левого поддерева
        writing_to_base(write_file, go->right);}     //запись правого поддерева
    return 0;}

void writing_to_txt(ofstream& write_file,LIST*go, TABLE tag) {
    if (tag == MOD) {
        write_file << "	Вид выходной структуры записей для варианта 9\n"
            << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        write_file << "~                          Количество рабочих по разрядам    ~\n"
            << "~СТАЖ РАБОТЫ~~~~~~~~~~~~~~~~~~~~~~ ВСЕГО~\n"
            << "~                           ~     1      ~     2      ~     3     ~      0   ~\n"
            << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
            << "~До 6" << std::setw(20) << "~" << std::setw(6) << sumWorkingClass[0][0] << std::setw(6) << "~" << std::setw(6) << sumWorkingClass[1][0] << std::setw(7) << "~" << std::setw(6) << sumWorkingClass[2][0] << std::setw(7) << "~" << std::setw(7) << sumWorkingClass[2][0] + sumWorkingClass[1][0] + sumWorkingClass[0][0] << std::setw(6) << "~\n"
            << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
            << "~C 6 до 11" << std::setw(13) << "~" << std::setw(6) << sumWorkingClass[0][1] << std::setw(6) << "~" << std::setw(6) << sumWorkingClass[1][1] << std::setw(7) << "~" << std::setw(6) << sumWorkingClass[2][1] << std::setw(7) << "~" << std::setw(7) << sumWorkingClass[2][1] + sumWorkingClass[1][1] + sumWorkingClass[0][1] << std::setw(6) << "~\n"
            << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
            << "~С 11 до 16" << std::setw(11) << "~" << std::setw(6) << sumWorkingClass[0][2] << std::setw(6) << "~" << std::setw(7) << sumWorkingClass[1][2] << std::setw(6) << "~" << std::setw(7) << sumWorkingClass[2][2] << std::setw(6) << "~" << std::setw(7) << sumWorkingClass[2][2] + sumWorkingClass[1][2] + sumWorkingClass[0][2] << std::setw(6) << "~\n"
            << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
            << "~До 16 до 21" << std::setw(9) << "~" << std::setw(6) << sumWorkingClass[0][3] << std::setw(6) << "~" << std::setw(7) << sumWorkingClass[1][3] << std::setw(6) << "~" << std::setw(7) << sumWorkingClass[2][3] << std::setw(6) << "~" << std::setw(7) << sumWorkingClass[2][3] + sumWorkingClass[1][3] + sumWorkingClass[0][3] << std::setw(6) << "~\n"
            << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
            << "~До 21 до 25" << std::setw(9) << "~" << std::setw(6) << sumWorkingClass[0][4] << std::setw(6) << "~" << std::setw(7) << sumWorkingClass[1][4] << std::setw(6) << "~" << std::setw(7) << sumWorkingClass[2][4] << std::setw(6) << "~" << std::setw(7) << sumWorkingClass[2][4] + sumWorkingClass[1][4] + sumWorkingClass[0][4] << std::setw(6) << "~\n"
            << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
            << "~Свыше 25" << std::setw(12) << "~" << std::setw(6) << sumWorkingClass[0][5] << std::setw(6) << "~" << std::setw(7) << sumWorkingClass[1][5] << std::setw(6) << "~" << std::setw(7) << sumWorkingClass[2][5] << std::setw(6) << "~" << std::setw(7) << sumWorkingClass[2][5] + sumWorkingClass[1][5] + sumWorkingClass[0][5] << std::setw(6) << "~\n"
            << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";}
    else if(tag == VIEW){
        write_file << "Структура записей входного файла имеет следующий вид: табельный номер, Ф.И.О.(30 символов), год рождения, пол (булевская переменная), профессия (10 символов),стаж работы, разряд рабочего, номер цеха, номер участка, сумма заработной платы." << endl
            <<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        write_file.fill('~');
        writing_to_txt(write_file,go,ADD);
    }else if(tag == ADD){
        if (!(go)) return;
        if (!(SORT))
            writing_to_txt(write_file, go->left, ADD);
        else if (SORT)
            writing_to_txt(write_file, go->right, ADD);
    write_file << "~" << go->newnode.key[0] << go->newnode.key[1] << go->newnode.key[2] << go->newnode.key[3] << "~" << go->newnode.year[0] << go->newnode.year[1] << go->newnode.year[2] << go->newnode.year[3] << "~" << std::left << std::setw(10) << go->newnode.profession  << "~" << std::left << std::setw(15) << go->newnode.FIO<<  "~стаж работы " << std::left << std::setw(4) << go->newnode.work_experience  << "~разряд рабочего " << std::left << std::setw(2) << newWorker.working_class << "~номер цеха " << std::left << std::setw(4) << newWorker.manufactory_number << "~номер участка " << std::left << std::setw(3) << go->newnode.site_number << "~зарплата " << std::left << std::setw(6) << go->newnode.salary << "~";
        if (go->newnode.gender == 0) write_file << std::right << std::setw(1) << "Мужчина~"; else write_file <<std::right<<std::setw(1)<< "Женщина~";
        write_file
    	<< "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        if (!(SORT))
            writing_to_txt(write_file, go->right, ADD);
        else if (SORT)
            writing_to_txt(write_file, go->left, ADD);}}

//====функции работы со списком====//

void global_clsSumWorkingClass(){
    if (sumWorkingClass)
    {
        for (unsigned i = 0; i < 3; i++)
            for (unsigned j = 0; j < 6; j++)
                sumWorkingClass[i][j] = 0;}}


void global_back(const LIST*go,TABLE tag){
    back = new LIST;
    back->newnode = go->newnode;
    back->left = go->left;
    back->right = back->right;
    (tag == ADD) ? amountOfElements++ : amountOfElements--;}

void global_addSumWorkingClass(const WORK newWorker) {
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
}

WORK creation_element(){
    CLEAR; BLOCK;
    bool j = false;
    while (j == false) {
        CLEAR;
        if (!(strcmp(newWorker.key, ""))) {
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 2));
            puts( "Введите табельный номер(4 цифры c 0001 номера) или Enter для прекращения записи");
            cin.getline(newWorker.key, WIDTH1);
            if (!(strcmp(newWorker.key, ""))) {
                return newWorker;}
            for (int i = 0; i < 4; i++){
                if (!((newWorker.key[i] > 47) and (newWorker.key[i]) <= 57)) {
                    j = true;
                    break;}
                if (i == 3)
                    newWorker.key[4] = '\0';}
            int key = (newWorker.key[0]- '0') + (newWorker.key[1]- '0') + (newWorker.key[2]- '0') +( newWorker.key[3]- '0');
            if (!key) j = true;
            if (j == true) {
                j = false;
                SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 12));
                puts( "!Введите корректное значение!");
                SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));
                strcpy_s(newWorker.key, "");
            	continue;}
            else break;}
        else {
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));
           cout << "Табельный номер введен " << newWorker.key[0] << newWorker.key[1] <<
                newWorker.key[2] <<
                newWorker.key[3] << endl; PAUSE; break;}}
      while (j == false) {
          SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));
        puts( "Введите Ф.И.О (30 символов:Иванов Иван Иванович,без чисел и пробела в конце): ");
        byte space = 0;
        CLEAR;
        fgets((char*)newWorker.FIO, WIDTH3, stdin);
        for (int i = 0; i < WIDTH3; i++){
            if ((newWorker.FIO[0] == 0x20) and (newWorker.FIO[i - 1]==0x20)and(newWorker.FIO[i] != 0x20)) { j = true; break; }
            if (space>2){ j = true; break; }
        	if((space>=1)and(newWorker.FIO[i]== newWorker.FIO[i-1]==0x20)){j = true;break;}
            if ((!((newWorker.FIO[i] > 191) and (newWorker.FIO[i]) <= 255))){
                if (newWorker.FIO[i] == 0x20) {space++; continue;}
            	if((newWorker.FIO[i]=='\n')and(space>=2)and(newWorker.FIO[i-1] > 191) and (newWorker.FIO[i-1] <= 255)){
                    newWorker.FIO[i] = NULL;break;}
            	
                j = true;break;}}
        if (j == true) {
            j = false;
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 12));
            puts( "!Введите корректное значение!");
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));
            continue;}
        else break;}
    while (1) {
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 6));
        puts( "Введите год рождения(1960-2000гг): ");
        CLEAR;
        cin.getline(newWorker.year, WIDTH1);
        if (((newWorker.year[0] == 49) and (newWorker.year[1] == 57) and (((newWorker.year[2] >= 54) and ((newWorker.year[2] < 58)))))) 
            break;
        if (((newWorker.year[0] == 50) and (newWorker.year[1] == 48) and ((newWorker.year[2] == 48) and (newWorker.year[3] == 48)))) 
             break;
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 12));
        puts( "!Введите корректное значение!\n");
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));
        continue;}
    SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));
    	puts( "Введите пол (не 0 - женщина) : ");
        CLEAR;
        cin >> newWorker.gender;
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 12));
    	puts( "Введите профессию (10 символов) : ");
        CLEAR;
        cin.getline(newWorker.profession, WIDTH2);
    while (1){
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 2));
        int work_experience = ((newWorker.year[0] - '0') * 10e2 + (newWorker.year[1] - '0') * 10e1 + (newWorker.year[2] - '0') * 10e0 + (newWorker.year[3] - '0') * 10e-1 + 18);
        cout << "Введите стаж работы : [2-" << NOWYEAR - work_experience<<"]";
        CLEAR;
        cin >> newWorker.work_experience;
;        ;
        if ((newWorker.work_experience < 2) or (newWorker.work_experience > 42)or(work_experience+ newWorker.work_experience > NOWYEAR)) {
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 12));
            puts( "!Введите корректное значение!");
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));}
        else break;}
    while (1) {
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 6));
        puts( "Введите разряд рабочего [1-3]: ");
        CLEAR;
        cin >> newWorker.working_class;
        if ((newWorker.working_class < 1) or (newWorker.working_class > 3)) {
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 12));
            puts( "!Введите корректное значение!");
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));}
        else break;}
        while (1) {
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 2));
            puts( "Введите номер цеха[1-100] : ");
            CLEAR;
            cin >> newWorker.manufactory_number;
            if (((newWorker.manufactory_number >= 1) and (newWorker.manufactory_number <= 100)))
                break;
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 12));
            puts( "!Введите корректное значение!");
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));}
        while (1) {
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 12));
            puts( "Введите номер участка[1-100] : ");
            CLEAR;
            cin >> newWorker.site_number;
            if (((newWorker.site_number >= 1) and (newWorker.site_number <= 100)))
                break;
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 6));
            puts( "!Введите корректное значение!");
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));}
        while (1) {
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 13));
            puts( "Введите заработную плату([15-100] в тыс.рублей) : ");
            CLEAR;
            cin >> newWorker.salary;//сумма заработной платы
            if (((newWorker.salary >= 15) and (newWorker.salary <= 100)))
                break;
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 12));
            puts( "!Введите корректное значение!\n");
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));}
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));
        puts("Запись осуществленна");
    return newWorker;}

bool search_element(LIST* go, char* key, bool i, TABLE tag){
        bool equal = false;
        if (go){
            for (int j = 0;j < 4;j++){
                if (go->newnode.key[j] != key[j])
                    equal = true;
                if (equal) break;}
            if (!equal) {
                if (tag == ALT) {
                    newWorker = creation_element();
                    go->newnode = newWorker;}
                return true;}
            i = search_element(go->right, key, i, tag);
            i = search_element(go->left, key, i, tag); }
        return i;}

LIST* organization_table(LIST* go){
    CLS; CLEAR;
    FILE* read_file;
    puts( "Создание структуры записей");
    if ((go)){
    	puts("Структура записей уже создана, не хотите перезаписать? (1 - да) ");
        if (cin.get() == '1') {
            go = cleaning_table(go);
            puts("Структура записей удалена");}
        else { PAUSE; return go; }
    }else puts("Структура пустая");
    
    puts( "Хотите загрузить из файла или создать с нуля?(1 - с файла)");
    CLEAR;
    if (cin.get() == '1') {
        char* name_file = new char[WIDTHF];
        bool create_file = false;
        CLEAR;
        while ((read_file = fopen(name_file, "r")) == NULL) {
            puts("Введите имя файла(Enter для выхода из программы) : ");
            cin.getline(name_file, WIDTHF);
            if (!strcmp(name_file, "")) {
                puts("\nЗапись окончена");
                PAUSE;
                return go;
            }
            read_file = fopen(name_file, "r");
            if (getc(read_file) == EOF) {
                puts("Файл пустой");
                read_file = fopen("", "r");
                puts("\nЗапись окончена");
                return go;
                PAUSE;
            }
            else {
                fseek(read_file, 0L, SEEK_SET); break;
            }
        }/* Перейти в начало файла */
        do {
            create_file = fread(&newWorker, sizeof(WORK), 1, read_file); //чтение структуры из файла
            if (create_file < 1)
                break;//если n<1, то конец
            go = add_element(newWorker, go); //создаем первый элемент
            global_back(go, ADD);
        } while (1);
        return go;
    }
    else global_clsSumWorkingClass();
    amountOfElements = 0;
    while (1) {
        strcpy(newWorker.key, "");
        newWorker = creation_element();
        if ((strcmp(newWorker.key, ""))) {
            go = add_element(newWorker, go);
            if (!(strcmp(go->newnode.key, "000"))) {
                continue;}
            global_back(go,ADD);}
        else {
            puts( "\nЗапись окончена");
            return go;}}}

//---очистка структуры записей
LIST* cleaning_table(LIST* go){
    SORT = false;
    global_clsSumWorkingClass();
    amountOfElements = 0;
    if (go){
        if (go->left){
            cleaning_table(go->left);}
        if (go->right)
            cleaning_table(go->right);}
    else return NULL;
    delete go;
    return go = NULL;}

//---функции работы с структурой записей

LIST* operations_table(LIST* go, TABLE TAG){
    CLS;
    char itemOfTable = 0;
    int positionOfElement = 1;
    if (!(go) and ((TAG != ADD))) {
        puts("Структура записей не созданна");
        PAUSE; return go;}
    else if (TAG != MOD) {
        while (1){
            CLS;
            puts( "Структура записей входного файла имеет следующий вид: ");
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));
            BLOCK;
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0)); cout << "|";
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 2)); cout << "ключ";
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0)); cout << "|";
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 6));  cout << " год";
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0)); cout << "|";
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 12));    cout << " профессия";
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0)); cout << "|";
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));  cout << "ФИО   ";;
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0)); cout << "|";
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 2)); cout << "стаж";
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0)); cout << "|";
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 6));   cout << "разряд";
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0)); cout << "|";
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 2));  cout << "номер цеха";
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0)); cout << "|";
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 12));  cout << "номер участка";
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0)); cout << "|";
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 13));  cout << "зарплата";
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0)); cout << "|";
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));    cout << "пол";
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0)); cout << "|\n";
            BLOCK;
            if (!(positionOfElement > amountOfElements)) {
                if (SORT == false)
                    moving_elements(go, SORTUP, 0, positionOfElement);
                else
                    moving_elements(go, SORTDOWN, 0, positionOfElement);}
            else {
                positionOfElement = 1;
                if (SORT == false)
                    moving_elements(go, SORTUP, 0, positionOfElement);
                else
                    moving_elements(go, SORTDOWN, 0, positionOfElement);}
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));//белый
            CLEAR;
            puts("\nПеремещение --> стрелочки  вверх и вниз\nЗапуск команды  --> Enter\nДля выхода --> ESC");
            SetConsoleTextAttribute(hConsole, (WORD)((3 << 4) | 14));//            
            SORT ? puts("Отсортировано по убыванию") : puts("Отсортировано по возврастанию");
            SetConsoleTextAttribute(hConsole, (WORD)((5 << 4) | 14));//красный
            CLEAR;
            SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));//красный
            itemOfTable = _getch_nolock();
            if (itemOfTable == 27) {
                SetConsoleTextAttribute(hConsole, (WORD)((4 << 4) | 14));//красный
                puts("Нажата клавиша ESC - операция завершена");
                SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));//красный
                Sleep(1500);
                return go;}
            else if (itemOfTable == 13) {
                switch (TAG){
                case VIEW: {
                    continue; }
                case SORTDOWN:case SORTUP:
                {SetConsoleTextAttribute(hConsole, (WORD)((12 << 4) | 14));
                    SORT ? puts("Операция : сортировка по возврастанию") : puts("Операция : сортировка по убыванию");
                    SORT ? SORT = false : SORT = true;
                    SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));
                    Sleep(1000); continue;}break;
                case SCN: {
                    puts("Операция : поиск работника по табельному номеру");
                    SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 2));
                    puts("Введите табельный номер(0000-9999) : ");
                    SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));
                    CLEAR;
                    cin.getline(newWorker.key, 5);
                    if (search_element(go, newWorker.key, 0, VIEW)) {
                        BLOCK;
                        cout << "Элемент " << newWorker.key << " записан в структуре записей\n";
                        BLOCK; PAUSE;}
                    else {
                        BLOCK;
                        cout << "Элемент " << newWorker.key << " не записан в структуре записей\n";
                        BLOCK; PAUSE;}break;}
                case ADD: {
                    int i = 0;
                    puts("Операция : добавление работника по табельному номеру");
                    puts("Введите табельный номер(0000-9999) : ");
                    SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 2));
                    cin.getline(newWorker.key, 5);
                    SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));
                    bool j = false;
                    for (int i = 0; i < 4; i++){
                        if ((!((newWorker.key[i] > 47) and (newWorker.key[i]) <= 57)) or (!strcmp(newWorker.key, "0000"))) {
                            puts( "Неверный ввод");
                            j = true;break;}}
                    if (j) { PAUSE;continue; };
                    newWorker.key[4] = NULL;
                    if (go) {
                        if ((search_element(go, newWorker.key, 0, VIEW))){
                            BLOCK;
                            cout << "Элемент " << newWorker.key << " записан в структуре записей\n";
                            BLOCK; PAUSE;continue;}
                        else if (!(strcmp(newWorker.key, ""))){
                            puts("Отмена записи данного табельного номера"); PAUSE; continue;}
                        else {
                            cout << "Элемент    " << newWorker.key << " не записан в структуре записей\n"; Sleep(1000);
                            if (amountOfElements == 9999){
                                puts("Превышен лимит записей (0001-9999)"); continue;}
                            puts("Будет произведенна запись"); Sleep(2000);}}
                    BLOCK; CLS;
                    go = add_element((newWorker = creation_element()), go);
                    if (!(strcmp(go->newnode.key, "000"))) {
                        continue;}
                        global_back(go,ADD);
                    amountOfElements++; }
                        break;
                case DEL:{
                    puts("Операция : удаление работника по табельному номеру");
                    if ((go->right == NULL) and (go->left == NULL)){
                        puts("В структуре записей остался последний элемент");
                        PAUSE;
                        continue;}
                    puts("Введите табельный номер(0000-9999) : ");
                    SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 12));
                    cin.getline(newWorker.key, 5);
                    SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));
                    if (!(search_element(go, newWorker.key, 0, VIEW))){
                        BLOCK;
                        cout << "Элемент " << newWorker.key << " не записан в структуре записей\n";
                        BLOCK; PAUSE;}
                    else if (!(strcmp(newWorker.key, ""))){
                        puts("Отмена записи данного табельного номера"); PAUSE; continue;}
                    else {
                        cout << "Элемент " << newWorker.key << " записан в структуре записей\n";
                        puts("Будет произведененно удаление");
                        go = delete_element(go, newWorker);
                        global_back(go,DEL);}}
                break;
                case ALT: {
                    puts("Операция : изменение данных работника по табельному номеру");
                    puts("Введите табельный номер(0000-9999) : ");
                    SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 1));
                    cin.getline(newWorker.key, 5);
                    SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));
                    if (!(search_element(go, newWorker.key, 0, VIEW))){
                        BLOCK;
                        cout << "Элемент " << newWorker.key << " не записан в структуре записей\n";
                        BLOCK; PAUSE;}
                    else if (!(strcmp(newWorker.key, ""))){
                        puts("Отмена записи данного табельного номера"); PAUSE; continue;}
                    else {
                        cout << "Элемент " << newWorker.key << " записан в структуре записей\n";
                        puts("Будет произведененно изменение");
                        search_element(go, newWorker.key, 0, ALT);}}
                        break;
                default:
                    break;}}
            itemOfTable = _getch_nolock();
            if (itemOfTable == 80) {
                if (positionOfElement >= amountOfElements) {
                    positionOfElement = 1; continue;}
                if (positionOfElement + MENUVIEWITEMS >= amountOfElements)
                    positionOfElement = 1;
                else { positionOfElement += MENUVIEWITEMS; }}
            else if (itemOfTable == 72) {
                if (positionOfElement <= 1) {
                    positionOfElement = amountOfElements - MENUVIEWITEMS; continue;}
                if (positionOfElement - MENUVIEWITEMS <= 0) {
                    positionOfElement = 1; continue;}
                positionOfElement -= MENUVIEWITEMS;}
            else {
                itemOfTable = 0;
                continue;}}}
    else{//tag == MOD,изменение структуры записейицы
        BLOCK;
        cout << "\t\tВид выходной структуры записейицы для варианта 9" << std::setw(30) << endl; BLOCK;
        cout << "|" << std::setw(20) << "|" << std::setw(5) << "\tКоличество рабочих по разрядам" << std::setw(5) << "|" << std::setw(21) << "|\n" <<
            "|" << "    СТАЖ РАБОТЫ" << std::setw(5) << "|" << std::setw(5) << "=====================================" << "|" << "      ВСЕГО" << std::setw(10) << "|\n"
            << "|" << std::setw(20) << "|" << std::setw(6) << "1" << std::setw(6) << "|" << std::setw(7) << "2" << std::setw(6) << "|" << std::setw(7) << "3" << std::setw(6) << "|" << std::setw(21) << "|\n"
            << " =============================================================================\n"
            << "|До 6" << std::setw(16) << "|" << std::setw(6) << sumWorkingClass[0][0] << std::setw(6) << "|" << std::setw(7) << sumWorkingClass[1][0] << std::setw(6) << "|" << std::setw(7) << sumWorkingClass[2][0] << std::setw(6) << "|" << std::setw(9) << sumWorkingClass[2][0] + sumWorkingClass[1][0] + sumWorkingClass[0][0] << std::setw(12) << "|\n"
            << " =============================================================================\n"
            << "|C 6 до 11" << std::setw(11) << "|" << std::setw(6) << sumWorkingClass[0][1] << std::setw(6) << "|" << std::setw(7) << sumWorkingClass[1][1] << std::setw(6) << "|" << std::setw(7) << sumWorkingClass[2][1] << std::setw(6) << "|" << std::setw(9) << sumWorkingClass[2][1] + sumWorkingClass[1][1] + sumWorkingClass[0][1] << std::setw(12) << "|\n"
            << " =============================================================================\n"
            << "|С 11 до 16" << std::setw(10) << "|" << std::setw(6) << sumWorkingClass[0][2] << std::setw(6) << "|" << std::setw(7) << sumWorkingClass[1][2] << std::setw(6) << "|" << std::setw(7) << sumWorkingClass[2][2] << std::setw(6) << "|" << std::setw(9) << sumWorkingClass[2][2] + sumWorkingClass[1][2] + sumWorkingClass[0][2] << std::setw(12) << "|\n"
            << " =============================================================================\n"
            << "|До 16 до 21" << std::setw(9) << "|" << std::setw(6) << sumWorkingClass[0][3] << std::setw(6) << "|" << std::setw(7) << sumWorkingClass[1][3] << std::setw(6) << "|" << std::setw(7) << sumWorkingClass[2][3] << std::setw(6) << "|" << std::setw(9) << sumWorkingClass[2][3] + sumWorkingClass[1][3] + sumWorkingClass[0][3] << std::setw(12) << "|\n"
            << " =============================================================================\n"
            << "|До 21 до 25" << std::setw(9) << "|" << std::setw(6) << sumWorkingClass[0][4] << std::setw(6) << "|" << std::setw(7) << sumWorkingClass[1][4] << std::setw(6) << "|" << std::setw(7) << sumWorkingClass[2][4] << std::setw(6) << "|" << std::setw(9) << sumWorkingClass[2][4] + sumWorkingClass[1][4] + sumWorkingClass[0][4] << std::setw(12) << "|\n"
            << " =============================================================================\n"
            << "|Свыше 25" << std::setw(12) << "|" << std::setw(6) << sumWorkingClass[0][5] << std::setw(6) << "|" << std::setw(7) << sumWorkingClass[1][5] << std::setw(6) << "|" << std::setw(7) << sumWorkingClass[2][5] << std::setw(6) << "|" << std::setw(9) << sumWorkingClass[2][5] + sumWorkingClass[1][5] + sumWorkingClass[0][5] << std::setw(12) << "|\n"
            << " =============================================================================\n";}
    ofstream processed_table_file;
    puts( "Введите название файла для сохранения структуры записей"); CLEAR;
    cin.getline(buf, WIDTHF);
    if (strcmp(buf, ""))
        processed_table_file.open(buf, std::ios::out, std::ios::trunc);
    else
        processed_table_file.open("default_processed_table.txt", std::ios::out, std::ios::trunc);
    writing_to_txt(processed_table_file, go, MOD);
    printf("Сохраненно в файл /%s\n", buf);
    processed_table_file.close();PAUSE; return go;}

//---функции работы с структурой записей
int show_elements(const WORK& newWorker, int i) {
    SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0)); cout << "|";
    if (!strcmp(newWorker.key, "~~~")) {
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 2)); cout << "    ";}
    else { SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 2)); cout << newWorker.key[0] << newWorker.key[1] << newWorker.key[2] << newWorker.key[3]; }
    SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0)); cout << "|";
    if (!strcmp(newWorker.year, "~~~")) {
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 2)); cout << "    ";}
    else{
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 6)); cout << newWorker.year[0] << newWorker.year[1] << newWorker.year[2] << newWorker.year[3];}
    SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0)); cout << "|";
    SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 12)); cout << std::setw(10) << newWorker.profession;
    SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0)); cout << "|";
    SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));  cout << std::setw(30) << newWorker.FIO;
    SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0)); cout << "|";
    if (newWorker.work_experience == 0) {
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 2)); cout << "    ";}
    else {
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 2)); cout << std::setw(4) << newWorker.work_experience;}
    SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0)); cout << "|";
    if (newWorker.working_class == 0) {
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 2)); cout << "  ";}
    else {
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 6)); cout << std::setw(2) << newWorker.working_class;}
    SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0)); cout << "|";
    if (newWorker.manufactory_number == 0) {
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 2)); cout << "    ";}
    else {
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 2)); cout << std::setw(4) << newWorker.manufactory_number;}
    SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0)); cout << "|";
    if (newWorker.manufactory_number == 0) {
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 2)); cout << "   ";}
    else {
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 12)); cout << std::setw(3) << newWorker.site_number;}
    SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0)); cout << "|";
    if (newWorker.manufactory_number == 0) {
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 2)); cout << "      ";}
    else {
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 13)); cout << std::setw(6) << newWorker.salary;}
    SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0)); cout << "|";
    if (newWorker.gender == 0) {
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 6)); cout << "М";
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0)); cout << "|\n";}
    else{
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 12)); cout << "Ж";
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0)); cout << "|\n";}
    SetConsoleTextAttribute(hConsole, (WORD)((2 << 4) | 0));
    SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));
    BLOCK; return i++;}

//перемещение по элементам и вывод в консоль
int moving_elements(LIST* go, TABLE SORT, int i, int position){
    if (!go)    return i;
    if (SORT == SORTUP)
        i = moving_elements(go->left, SORT, i, position);
    else if (SORT == SORTDOWN)
        i = moving_elements(go->right, SORT, i, position);
    i++;
    if ((i >= position) and (i <= position + MENUVIEWITEMS)) {
        show_elements(go->newnode, i); }
    else if (i > position + MENUVIEWITEMS - 1) return position + MENUVIEWITEMS;
    if (i == amountOfElements){
        for (auto j = amountOfElements; j < (position + MENUVIEWITEMS-1); j++)
            show_elements(emptyWorker, i); //запись пустых элементов
        return -9999;}
    if (SORT == SORTUP)
        i = moving_elements(go->right, SORT, i, position);//обход правого поддерева
    else if (SORT == SORTDOWN)
        i = moving_elements(go->left, SORT, i, position);}



LIST* add_element(WORK newWorker, LIST* go) {
    
    if (go == NULL){
        go = new LIST;
        go->newnode = newWorker;
        go->left = NULL;
        go->right = NULL;

}
    else if ((newWorker.key[0]==go->newnode.key[0])and(newWorker.key[1] == go->newnode.key[1])and(newWorker.key[2] == go->newnode.key[2])and(newWorker.key[3] == go->newnode.key[3])){
        BLOCK;
        cout << "Ошибка - табельный номер " << go->newnode.key[0] << go->newnode.key[1] << go->newnode.key[2] << go->newnode.key[3] << " уже записан, работник - " << go->newnode.FIO << endl;
        for (int i = 0; i < 3; i++)
            go->newnode.key[i] = '0';
        go->newnode.key[3] = '\0';
        return go;}
    else {
        for (int i = 0; i < 4; i++){
            if (newWorker.key[i] < go->newnode.key[i]) {
                go->left = add_element(newWorker, go->left);
                return(go);}
            else if (newWorker.key[i] > go->newnode.key[i]) {
                go->right = add_element(newWorker, go->right);
                return(go);}     
            else { continue; }}}global_addSumWorkingClass(newWorker);
            global_back(go, ADD);
            return go;
}


LIST* delete_element(LIST* go, WORK newWorker) {
    if (go == NULL)
        return go;
    if ((newWorker.key[0] == go->newnode.key[0]) and (newWorker.key[1] == go->newnode.key[1]) and (newWorker.key[2] == go->newnode.key[2]) and (newWorker.key[3] == go->newnode.key[3])){
        LIST* tmp;
        if (go->right == NULL)
            tmp = go->left;
        else {
            LIST* ptr = go->right;
            if (ptr->left == NULL) {
                ptr->left = go->left;
                tmp = ptr;}
            else {
                LIST* pmin = ptr->left;
                while (pmin->left != NULL) {
                    ptr = pmin;
                    pmin = ptr->left;}
                ptr->left = pmin->right;
                pmin->left = go->left;
                pmin->right = go->right;
                tmp = pmin;}}
        delete go;
        return tmp;}
    for (int i = 0; i < 4; i++){
        if (newWorker.key[i] < go->newnode.key[i]) {
            go->left = delete_element(go->left, newWorker);
            return(go);}
        else if (newWorker.key[i] > go->newnode.key[i]) {
            go->right = delete_element(go->right, newWorker);
            return(go);}
        else { continue; }}
    return go;}

//----Вывод главного меню---///
void print_menu(int menuLabel){
    CLS;
    BLOCK;
    cout<<arrMenu[MENUITEMS-1]<<"Программа рассчитана на текущий "<<NOWYEAR<<" год\n";
    BLOCK;
    for (int i = 0; i < MENUITEMS-1; i++){
        if (i == (menuLabel - 1)){
            SetConsoleTextAttribute(hConsole, (WORD)((7 << 4) | 0));
            cout << arrMenu[i] << endl;
            continue;}
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));
        cout << arrMenu[i] << endl;}
    SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));
    BLOCK;}

//---динамическое управление через стрелочки
char scrolling_menu(){
	CLS;char item;
    print_menu(menuLabel);
    while ((item = _getch_nolock()) != 13){
        if ((item != -32) and (item != 27)){
            CLEAR;
            item = 0;
            continue;}
        item = _getch_nolock();
        switch (item){
        case 80:{
            if (menuLabel == MENUDOWN){
                menuLabel = 1;
                print_menu(menuLabel);}
            else{
                menuLabel++;
                print_menu(menuLabel);}
        }break;
        case 72:{
            if (menuLabel == 1){
                menuLabel = MENUDOWN;
                print_menu(menuLabel);}
            else{
                menuLabel--;
                print_menu(menuLabel);}
        }break;
        case 27:{
            print_menu(MENUDOWN);
            return MENUDOWN;}break;
        default: {
            continue; }
            break;} }	
    return menuLabel;}

