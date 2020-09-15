﻿#include "glut.h"
#include <Windows.h> 
#include <iostream>
#include <string>
using namespace std;
int posi, posj;//вспомогательные индексы
int score = 0;//кол-во клеток из выбранной ячейки в конечную, игнорируя при этом финальную.
int help_i, help_j;//вспомогательные индексы
int razn_i = 0, razn_j = 0;//вычисление разницы клеток между выбранной ячейкой и финишной
bool inst = 0;// логическая переменная, нужна для решения ситуаций: "Выводить инструкцию или нет?"
float x_1 = 0.0f, y_1 = 500.0f;//позиция первого символа
float helpx, helpy;//вспомогательные координаты
int n1=0, n2=0;//n1 - кол-во столбцов матрице сетки, n2 - кол-во строк в матрице сетки
int** mas1 = 0;//матрица сетки
double** mas2 = 0;//матрица расстояний активной ячейки со смежными (3x3)
double** mas3 = 0;//матрица эвристических приближений активной точки со смежными (3х3) 
double** mas_sum = 0;//матрица сумм соответсвующих расстояний и эвристических приближений(3х3) 
int** mas_index = 0;//матрица номеров активной точки со смежными(3х3)
int active_i, active_j = 0;//индекс активной ячейки
int min_dist = 0;//минимальное расстояние из активной точки в смежные с ней.
int start_i, start_j;//номер строки и столбца старта
int finish_i, finish_j;//номер строки и столбца финиша
int prepyat_i, prepyat_j;//номер строки и столбца препятсвия
int help1 = 0;
int start_help, finish_help;
string resh = "";//строка, в которой будет храниться итоговый путь.
int pyt = 0;//длина кратчайшего пути
int min_rast = 0;//
int min_i=0, min_j = 0;//индексы минимального значения в матрице
bool f = 0;//логическая переменная, помогающая вовремя завершить работу программы при вычисленном результате
void reshape(int w, int h)//функция для изменения размеров окна GLUT
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
 
}
void writeText(string text, int x, int y)//функция, отвечающая за вывод строк в окно GLUT 
{

    glColor3f(1.0, 1.0, 0.0);//цвет символов
    glRasterPos2i(x, y);//позиция вывода символов
    int len = text.length();//вычисление кол-во символов в строке
    for (int i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);//функция выводящая символы в окно GLUT
    }
   glFlush();//эта функция приостанавливает выполнение программы, пока все не будет выведено на экран 
}
void writeText(char text, int x, int y)//функция, отвечающая за вывод строк в окно GLUT 
{

    glColor3f(1.0, 1.0, 0.0);
    glRasterPos2i(x, y);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text);
    glFlush();
}
void draw_grid()//отрисовка сетки в окне GLUT
{
    x_1 = 20.0f;//координата первой точки по x
    y_1 = 500.0f;//координата первой точки по y
    for (int i = 0; i < n2; i++)
    {
        for (int j = 0; j < n1; j++)
        {
            glBegin(GL_QUADS);//отрисовка квадрата
            if (mas1[i][j] == 0)
                glColor3f(1.0, 0, 0);
            if (mas1[i][j] == 1)
                glColor3f(0.5, 0.5, 0.5);
            if (mas1[i][j] == 2)
                glColor3f(0.0, 1.0, 0);
            if (mas1[i][j] == 3)
                glColor3f(0.0, 0.0, 1.0);
            if (mas1[i][j] == 4)
                glColor3f(1.0, 0, 1.0);
            glVertex2f(x_1 + 5.0f, y_1 + 5.0f);//
            glVertex2f(x_1 + 5.0f, y_1 + 50.0f);//
            glVertex2f(x_1 + 50.0f, y_1 + 50.0f);// выбор координат квадрата
            glVertex2f(x_1 + 50.0f, y_1 + 5.0f);//
            glEnd();
            //center_x1 = (x_1 + 5.0f + x_1 + 50.0f) / 2;
            //center_y1 = (y_1 + 5.0f + y_1 + 50.0f) / 2;
            glFlush();
            if (i == 0)
            {
                helpy = y_1 + 55.0f;
                helpx = x_1 + 27.0f;
                help1 = j + 1;
                writeText(to_string(help1), helpx, helpy);
            }
            x_1 += 50.0f;//сдвиг по x
        }
        x_1 = 20.0f;
        helpy = y_1 + 30.0f;//сдвиг по y для индексов
        helpx = x_1 - 12.0f;//сдвиг по x для индексов
        help1 = i + 1;
        writeText(to_string(help1), helpx, helpy);//вывод индексов в окно GLUT
        y_1 -= 50.0f;//сдвиг по y

    }
}
template <typename T>
void clear_mas(T**& mas, int n1, int n2)//очистка массива
{
    for (int i = 0; i < i; i++)
        delete[] mas[i];       /// удаление массива

    delete[] mas;
    mas = new T * [n1];    ///пересоздание массива
    for (int i = 0; i < n1; i++)
    {
        mas[i] = new T[n2];
    }
    for (int i = 0; i < n1; i++)
    {
        for (int j = 0; j < n2; j++)
        {
            mas[i][j] = 0;
        }
    }
}
template <typename T>
void print_mas(int key,T** mas, int n_i, int n_j)//печать определенного массива
{
    cout << "####################################################" << endl;
    switch (key)//выбор нужного массива
    {
    case 1:
        cout << "Массив сетки (0 - свободные ячейки, 1- препятствия и закрытые ячейки , 2 - старт, 3 - финиш): " << endl;
        break;
    case 2:
        cout << "Массив расстояний с ближайшими точками: " << endl;
        break;
    case 3:
        cout << "Массив эвристических приближений: " << endl;
        break;
    case 4:
        cout << "Массив ближайших индексов: " << endl;
        break;
    case 5:
        cout << "Массив суммы эврестической функции и расстония: " << endl;
        break;
    }
    for (int i = 0; i < n_i; i++)//прохождение по строкам матрицы
    {
        for (int j = 0; j < n_j; j++)//прохождение по столбцам матрицы
        {
            cout << mas[i][j] << ' ';
        }
        cout << endl;
    }
    cout << "####################################################" << endl;
}
void instruction()//функция печати инструкции
{
    cout << "####################################################" << endl;
    cout << "Данная программа вычисляет кратчайщее расстояние для двух заданных пунктов.\n";
    cout<<"Фиолетовым обозначены ячейки, по которым двигалась активная ячейка.\n";
    cout << "Красным цветом обозначены свободные ячейки, серым препятсвия.\n";
    cout << "Зеленым цветом обозначена активная ячейка, первоначально являющаяся стартом.\n";
    cout << "Синим цветом обозначена ячейка финиша.\n";
    cout << "Для правильной работы программы следует выполнять действия в следуещем порядке:\n";
    cout << "1) Расстановка препятсвий. \n";
    cout << "2) Выбор начальной и финишной точек. \n";
    cout << "3) Запуск алгоритма A Star. \n";
    cout << "####################################################" << endl;
}
void Astar()//алгоритм, основанный на А стар
{
    active_i = start_i - 1;// присвоение индексам активной ячейки индексов начальной точки
    active_j = start_j - 1;//
    f = (active_i != finish_i - 1) || (active_j != finish_j - 1);//логическая переменная отвечающая за условие:"находится активная точка находится на финише или нет?"
    while (f == true)//проверка условия 
    {

        mas2[1][1] = 0;//обнуление активной точки в матрицах
        mas3[1][1] = 0;//
        if (active_i - 1 < 0)//проверка исключительных ситуаций для активной ячейки
        {
            for (int i = 0; i < 3; i++)//замена неудовлетворяющих ячеек на -1 в соответсвующих матрицах
            {
                mas2[0][i] = -1;
                mas3[0][i] = -1;
                mas_index[0][i] = -1;
            }
        }
        if (active_j - 1 < 0) // проверка исключительных ситуаций для активной ячейки
        {
            for (int i = 0; i < 3; i++)//замена неудовлетворяющих ячеек на -1 в соответсвующих матрицах
            {
                mas2[i][0] = -1;
                mas3[i][0] = -1;
                mas_index[i][0] = -1;
            }
        }
        if (active_j + 1 >= n1) // проверка исключительных ситуаций для активной ячейки
        {
            for (int i = 0; i < 3; i++)//замена неудовлетворяющих ячеек на -1 в соответсвующих матрицах
            {
                mas2[i][2] = -1;
                mas3[i][2] = -1;
                mas_index[i][2] = -1;
            }
        }
        if (active_i + 1 >= n2)// проверка исключительных ситуаций для активной ячейки
        {
            for (int i = 0; i < 3; i++)//замена неудовлетворяющих ячеек на -1 в соответсвующих матрицах
            {
                mas2[2][i] = -1;
                mas3[2][i] = -1;
                mas_index[2][i] = -1;
            }
        }
        int c = 1;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (mas2[i][j] != -1)
                {
                    if (i == 0)
                    {
                        if (active_j > 0)
                        {
                            mas_index[i][j] = (active_i) * 10 + active_j - 1 + c;//заполнение первой строки матрицы индексов для активной точки
                            c++;
                        }
                        else
                        {
                            mas_index[i][j] = (active_i) * 10 + active_j + c;//заполнение первой строки матрицы индексов для активной точки
                            c++;
                        }
                    }
                    if (i == 1)
                    {
                        if (active_j > 0)
                        {
                            mas_index[i][j] = (active_i + 1) * 10 + active_j - 1 + c;//заполнение второй строки матрицы индексов для активной точки
                            c++;
                        }
                        else
                        {
                            mas_index[i][j] = (active_i + 1) * 10 + active_j + c;//заполнение второй строки матрицы индексов для активной точки
                            c++;
                        }
                    }
                    if (i == 2)
                    {
                        if (active_j > 0)
                        {
                            mas_index[i][j] = (active_i + 2) * 10 + active_j - 1 + c;//заполнение третьей строки матрицы индексов для активной точки
                            c++;
                        }
                        else
                        {
                            mas_index[i][j] = (active_i + 2) * 10 + active_j + c;//заполнение третьей строки матрицы индексов для активной точки
                            c++;
                        }
                    }
                    if (i == 1 || j == 1)
                    {
                        mas2[i][j] = min_dist;//заполнение матрицы расстояний для точек, которые лежат не на диагонали с активной точкой
                    }
                    else
                    {
                        mas2[i][j] = min_dist * 1.4;//заполнение матрицы расстояний для точек, которые лежат на диагонали с активной точкой
                    }
                    help1 = mas_index[i][j];
                    help_j = mas_index[i][j] % 10;
                    help_i = mas_index[i][j] / 10;
                    score = 0;
                    while (help_i < finish_i)//выравнивание активной точки с финишной по строкам
                    {
                        help_i++;
                        score++;
                    }
                    while (help_i > finish_i)//выравнивание активной точки с финишной по строкам
                    {
                        help_i--;
                        score++;
                    }
                    while (help_j < finish_j)//выравнивание активной точки с финишной по столбцам
                    {
                        help_j++;
                        score++;
                    }
                    while (help_j > finish_j)//выравнивание активной точки с финишной по столбцам
                    {
                        help_j--;
                        score++;
                    }
                    mas3[i][j] = score * min_dist;//заполнение матрицы эвристических функций
                }
            }
            c = 1;
        }
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if(finish_i == (mas_index[i][j] / 10) && finish_j == (mas_index[i][j] % 10))
               {
                    mas3[i][j] = 0;//эвристическая функция для финальной точки = 0 !
                }
                mas_sum[i][j] = mas2[i][j] + mas3[i][j];//заполнение матрицы суммы активной точки
            }
        }
        mas2[1][1] = 0;//обнуление активной точки в соответсвующих матрицах
        mas3[1][1] = 0;//
        mas_sum[1][1] = 0;//
        mas_index[1][1] = 0;//
        int sum_index = 0;
        for (int i = 0; i < n2; i++)
        {
            for (int j = 0; j < n1; j++)
            {
                if (mas1[i][j] == 1)
                {
                    sum_index = (i + 1) * 10 + j + 1;
                    for (int k = 0; k < 3; k++)
                    {
                        for (int z = 0; z < 3; z++)
                        {
                            if (sum_index == mas_index[k][z])//обработка исключительных ситуаций
                            {
                                mas2[k][z] = -1;
                                mas3[k][z] = -1;
                     
                                mas_sum[k][z] = -1;
                                mas_index[k][z] = -1;
                            }
                        }
                    }
                }
            }
        }
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (mas_sum[i][j] > 0)
                {
                    min_rast = mas_sum[i][j];//минимальны элемент в матрице сумм
                    min_i = mas_index[i][j] / 10;//вычисление строки минимального  в матрице сетки
                    min_j = mas_index[i][j] % 10;//вычисление столбца минимального элемента в матрице сетки
                    posi = i;//строка минимального числа в матрице индексов
                    posj = j;//столбец минимального числа в матрице индексов
                }
            }
        }
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (mas_sum[i][j] > 0 && mas_sum[i][j] < min_rast)//вычисление минимального элемента
                {
                            min_rast = mas_sum[i][j];
                            min_i = mas_index[i][j] / 10;
                            min_j = mas_index[i][j] % 10;
                            posi = i;
                            posj = j;
                }
            }
        }
        pyt += mas2[posi][posj];//запись пройденного пути в
        mas1[active_i][active_j] = 4;//обозначение пройденной ячейки
        resh += to_string((active_i + 1) * 10 + active_j + 1)+"->";//запись резултата
        active_i = min_i - 1;//присвоение индексам активной ячейки индексов минимального элемента
        active_j = min_j - 1;//присвоение индексам активной ячейки индексов минимального элемента
        mas1[active_i][active_j] = 2;//смена активной ячейки
        print_mas(1, mas1, n2, n1);//печать массивов
        print_mas(2, mas2, 3, 3);//
        print_mas(3, mas3, 3, 3);//
        print_mas(4, mas_index, 3, 3);//
        print_mas(5, mas_sum, 3, 3);//
        Sleep(1000);//задержка в 1 секунду(для наглядности)
        draw_grid();//отрисовка обновленной сетки
        if (min_i == finish_i && min_j == finish_j)//если достигнут финал, то прекратить выполнение программы
        {
            f = false;
            resh += to_string(finish_i * 10 + finish_j);
        }
        clear_mas(mas2, 3, 3);//очистка массивов
        clear_mas(mas3, 3, 3);//
        clear_mas(mas_index, 3, 3);//
        clear_mas(mas_sum, 3, 3);//
    }
    cout << "Кратчайший путь из ячейки " << start_i * 10 + start_j << " в ячейку "<<finish_i*10+finish_j<<" = " << pyt << endl;
    cout << "Сам путь:  " << resh << endl;;
    writeText("Shortest way: "+resh, 0, 25);
}
void display()//функция вывода сетки в окно GLUT, инструкции и опционального меню в консоль
{
    if (inst == 0)//если инструкция не выведена
    {
        writeText("The instruction is printed to the console ", 0, 5);//сообщение о том, что инструкция выведена в консоль
        instruction();//печать инструкции в консоль
        draw_grid();//отрисовка первоначальной сетки
        inst = 1;
    }
    int r = 0;
    do 
    {
        cout << "####################################################" << endl;
        cout << "0. Вывести инструкцию по использованию программы."<<endl;
        cout << "1. Добавить препятствие. " << endl;
        cout << "2. Выбрать начальную(старт) и конечные(финиш) точки." << endl;
        cout << "3. Вывести матрицу свободных ячеек и препятсвий." << endl;
        cout << "4. Запустить алгоритм A Star." << endl;
        cout << "5. Очистить сетку(убрать препятствия, старт и финиш)." << endl;
        cout << "6. Завершить работу программы." << endl;
        cout << "Ваш выбор: ";
        cin >> r;//выбор нужного действия
        cout << "####################################################" << endl;
        switch (r)//выполнение соответсвующего ключа
        {
        case 0://ключ 0
            instruction();//печать инструкции
            break;
        case 1://ключ 1
            cout << "Введите номер строки: ";
            cin >> prepyat_i;
            cout << "Введите номер стобца: ";
            cin >> prepyat_j;
            mas1[prepyat_i-1][prepyat_j-1] = 1;//на матрице сетки появилось препятсвие
             draw_grid();//отрисовка сетки с препятствиями 
            break;
        case 2:// ключ 2
            cout << "Введите номер строки(start): ";
            cin >> start_i;
            cout << "Введите номер стобца(start): ";
            cin >> start_j;
            mas1[start_i-1][start_j-1] = 2;//на матрице появлась активная ячейка, либо ячейка старта
            cout << "Введите номер строки(finish): ";
            cin >> finish_i;
            cout << "Введите номер стобца(finish): ";
            cin >> finish_j;
            mas1[finish_i-1][finish_j-1] = 3;// на матрице сетки появилась финишная ячейка
            draw_grid();//отрисовка сетки с активной и финишной ячейками
            break;
        case 3:// ключ 3
            print_mas(1,mas1, n2, n1);
            print_mas(2,mas2, 3, 3);
            print_mas(3,mas3, 3, 3);
            print_mas(4,mas_index, 3, 3);
            print_mas(5,mas_sum, 3, 3);
            break;
        case 4:// ключ 4
            Astar();
            break;
        case 5:// ключ 5
            clear_mas(mas1, n2, n1);
            clear_mas(mas2, 3, 3);
            clear_mas(mas3, 3, 3);
            clear_mas(mas_index, 3, 3); 
            pyt = 0;//обнуление конечного расстояния
            draw_grid();//отрисовка очищенной сетки
            break;
        case 6:// ключ 6
            exit(0);//завершение работы программы
            break;
        }
    } while (r != 6);
}
void matrix_create()//выбор размера сетки, минимального расстояния между смежными ячейками и создание нужных матриц
{
    min_dist = 0;
    while (n2 <= 0 || n2 > 9)
    {
        cout << "Введите количество строк (Макс. 9) = ";
        cin >> n2;//введение кол-во строк сетки и соотвествующей матрицы
    }
    while (n1 <= 0 || n1 > 9)
    {
        cout << "Введите количество столбцов (Макс. 9) = ";
        cin >> n1;//введение кол-во столбцов сетки и соотвествующей матрицы
    }
    while (min_dist <= 0)
    {
        cout << "Введите минимальное расстояние между ячеейками: ";
        cin >> min_dist;//введение миним. расстояния между смежными клетками
    }
    clear_mas(mas1, n2, n1);//создание матрицы сетки
    clear_mas(mas2, 3, 3);// создание матрицы расстояний между активной клеткой(0) и смежными 
    clear_mas(mas3, 3, 3);// создание матрицы эвристических приближений у смежных клеток с активной
    clear_mas(mas_index, 3, 3);//создание матрицы индексов между активной клеткой(0) и смежными ей
    clear_mas(mas_sum, 3, 3);//создание матрицы сумм эвристических приближений и расстояний
}
int main(int argc, char* argv[])//основная функция программы
{
    setlocale(LC_ALL, "rus");//задает локаль, которая будет использоваться текущей программой, в нашем случае нужна для вывода русских символов на консоль
    matrix_create();//создание нужных матриц
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//установливает для окна режим отображения информации
        glutInitWindowPosition(-5, 150);// функция, задающая позицию окна GLUT
        glutInitWindowSize(500, 700);// функция, задающая размеры окна GLUT
        glutCreateWindow("A Star Algorithm");// Эта команда создаёт окно с заголовком, который вы укажете в качестве параметра
        glutReshapeFunc(reshape); //это функция устанавливает другую функцию в вашей программе, которая будет ответственна за изменение размеров окна
        glutDisplayFunc(display); // Эта функция устанавливает функцию в вашей программе, которая будет отвечать за перерисовку окна.
        glutMainLoop();//Для того, чтобы перейти в главный цикл GLUT
    return 0;
}