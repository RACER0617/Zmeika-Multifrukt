// Змейка Мультифрукт.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include <iostream>
#include <cstdlib> // Для функций rand() и srand()
#include <ctime>   // Для функции time()
#include <vector>
#include <windows.h>
#include <conio.h>
using namespace std;

// Константы размеров поля
const int MIN_WIDTH = 10;
const int MAX_WIDTH = 50;
const int MIN_HEIGHT = 10;
const int MAX_HEIGHT = 30;

// Константы типов фруктов
const int FRUIT_NORMAL = 0;
const int FRUIT_DOUBLE = 1;
const int FRUIT_HALF = 2;
const int FRUIT_DEATH = 3;
const int FRUIT_COUNT = 4;

// Переменные игрового поля и змейки
int width = 20;
int height = 20;
int fruitCount = 3;
int deadlyFruitCount = 1; // Количество ядовитых фруктов
vector<pair<int, int>> fruits;
vector<pair<int, int>> deadlyFruits;
int x, y;
int* tailX;
int* tailY;

// Переменные счета и направления змейки
int score;
int nTail;
enum eDirection
{
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};
eDirection dir;

// Переменная состояния игры
bool gameOver;

// Функция настройки игры
void Setup()
{
    setlocale(LC_ALL, "Russian");
    // Начальное состояние игры
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    score = 0;
    nTail = 0;
    tailX = new int[width * height];
    tailY = new int[width * height];

    // Инициализируем генератор случайных чисел
    srand(time(NULL));

    // Размещаем фрукты на поле
    fruits.clear();
    deadlyFruits.clear();
    for (int i = 0; i < fruitCount; i++)
    {
        int fruitX = rand() % width;
        int fruitY = rand() % height;
        fruits.push_back(make_pair(fruitX, fruitY));
    }

    // Размещаем ядовитые фрукты на поле
    for (int i = 0; i < deadlyFruitCount; i++)
    {
        int deadlyFruitX = rand() % width;
        int deadlyFruitY = rand() % height;
        deadlyFruits.push_back(make_pair(deadlyFruitX, deadlyFruitY));
    }
}


// Функция отрисовки игрового поля и змейки
void Draw()
{
    setlocale(LC_ALL, "Russian");
    system("cls");

    // Отрисовка верхней границы
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    // Отрисовка поля
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (j == 0)
                cout << "#";

            bool print = false;
            if (i == y && j == x) {
                cout << "O";
                print = true;
            }
            else
            {
                for (int k = 0; k < nTail; k++)
                {
                    if (tailX[k] == j && tailY[k] == i)
                    {
                        cout << "o";
                        print = true;
                    }
                }
            }

            if (!print)
            {
                bool isFruit = false;
                for (int k = 0; k < fruits.size(); k++)
                {
                    if (fruits[k].first == j && fruits[k].second == i)
                    {
                        switch (k)
                        {
                        case FRUIT_NORMAL:
                            cout << "F";
                            break;
                        case FRUIT_DOUBLE:
                            cout << "D";
                            break;
                        case FRUIT_HALF:
                            cout << "H";
                            break;
                        case FRUIT_DEATH:
                            cout << "X";
                            break;
                        }
                        isFruit = true;
                        break;
                    }
                }

                if (!isFruit)
                {
                    for (int k = 0; k < deadlyFruits.size(); k++)
                    {
                        if (deadlyFruits[k].first == j && deadlyFruits[k].second == i)
                        {
                            cout << "*";
                            isFruit = true;
                            break;
                        }
                    }
                }

                if (!isFruit)
                    cout << " ";
            }

            if (j == width - 1)
                cout << "#";
        }
        cout << endl;
    }

    // Отрисовка нижней границы
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    // Вывод счета игры
    cout << "Счет: " << score << endl;
}

// Функция обработки пользовательского ввода
void Input()
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'a':
            if (dir != RIGHT)
                dir = LEFT;
            break;
        case 'd':
            if (dir != LEFT)
                dir = RIGHT;
            break;
        case 'w':
            if (dir != DOWN)
                dir = UP;
            break;
        case 's':
            if (dir != UP)
                dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        }
    }
}

// Функция обновления игры
void Logic()
{
    // Обновляем позицию хвоста змейки
    // Обновляем позицию хвоста змейки
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // Обновляем позицию головы змейки
    switch (dir)
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    }

    // Проверяем, находится ли голова змейки на поле
    if (x < 0 || x >= width || y < 0 || y >= height)
        gameOver = true;

    // Проверяем, находится ли голова змейки на фрукте
    for (int i = 0; i < fruits.size(); i++)
    {
        if (fruits[i].first == x && fruits[i].second == y)
        {
            // Увеличиваем счет игры и добавляем к змейке новый сегмент
            switch (i)
            {
            case FRUIT_NORMAL:
                score += 10;
                nTail++;
                break;
            case FRUIT_DOUBLE:
                score += 20;
                nTail *= 2;
                break;
            case FRUIT_HALF:
                score += 5;
                nTail /= 2;
                break;
            case FRUIT_DEATH:
                gameOver = true;
                break;
            }

            // Удаляем съеденный фрукт и добавляем новый на поле
            fruits.erase(fruits.begin() + i);
            int fruitX = rand() % width;
            int fruitY = rand() % height;
            fruits.push_back(make_pair(fruitX, fruitY));

            // Обновляем позиции ядовитых фруктов
            for (int j = 0; j < deadlyFruits.size(); j++)
            {
                int deadlyFruitX = rand() % width;
                int deadlyFruitY = rand() % height;
                deadlyFruits[j] = make_pair(deadlyFruitX, deadlyFruitY);
            }

            break;
        }
    }


    // Проверяем, находится ли голова змейки на ядовитом фрукте
    for (int i = 0; i < deadlyFruits.size(); i++)
    {
        if (deadlyFruits[i].first == x && deadlyFruits[i].second == y)
        {
            gameOver = true;  // Вызываем Game Over при съедении ядовитого фрукта
            break;
        }
    }

    // Проверяем, находится ли голова змейки на своем теле
    for (int i = 0; i < nTail; i++)
    {
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;
    }
}

// Главная функция игры
int main()
{
    // Ввод размеров игрового поля
    setlocale(LC_ALL, "Russian");
    cout << "Введите ширину (" << MIN_WIDTH << "-" << MAX_WIDTH << "): ";
    cin >> width;
    if (width < MIN_WIDTH || width > MAX_WIDTH)
    {
        cout << "Невозможнок значение. Выбран размер по умолчанию (20) \n";
        width = 20;
    }

    cout << "Введите высоту (" << MIN_HEIGHT << "-" << MAX_HEIGHT << "): ";
    cin >> height;
    if (height < MIN_HEIGHT || height > MAX_HEIGHT)
    {
        cout << "Невозможнок значение. Выбран размер по умолчанию (20)\n";
        height = 20;
    }

    // Ввод количества ядовитых фруктов
    int deadlyFruitCount;
    cout << "Введите количество ядовитых фруктов в поле (1-7): ";
    cin >> deadlyFruitCount;
    if (deadlyFruitCount < 1 || deadlyFruitCount > 7)
    {
        cout << "Невозможное значение. Выбрано количество по умолчанию (1) \n";
        deadlyFruitCount = 1;
    }

    // Настройка игры
    Setup();

    // Размещаем ядовитые фрукты на поле
    deadlyFruits.clear();
    for (int i = 0; i < deadlyFruitCount; i++)
    {
        int deadlyFruitX = rand() % width;
        int deadlyFruitY = rand() % height;
        deadlyFruits.push_back(make_pair(deadlyFruitX, deadlyFruitY));
    }

    // Основной цикл игры
    while (!gameOver)
    {
        // Обработка пользовательского ввода
        Input();

        // Обновление состояния игры
        Logic();

        // Отрисовка игрового поля и змейки
        Draw();

        // Задержка для стабилизации FPS
        Sleep(100);
    }

    // Отображение сообщения о конце игры и счета
    system("cls");
    cout << "Игра окончена!" << endl;
    cout << "Счет: " << score << endl;

    if (score >= 0 && score <= 5)
        cout << "Ачивка! Ты - микрочелик" << endl;
    else if (score > 5 && score <= 20)
        cout << "Ачивка! Нечего себе, да ты - уж обыкновенный" << endl;
    else if (score > 20 && score <= 60)
        cout << "Ачивка! Боже мой, да ты - Левантская гадюка!" << endl;
    else if (score > 60 && score <= 100)
        cout << "Ачивка! Нифига себе, да ты - Сурукуку" << endl;
    else if (score > 100 && score <= 150)
        cout << "Ачивка! Хмм, да ты - Черная мамба ~~~" << endl;
    else if (score > 150 && score <= 200)
        cout << "Ачивка! Хорош чел! Ты - Удав Обыкновенный" << endl;
    else if (score > 200 && score <= 250)
        cout << "Ачивка! О-ФИ-ГЕТЬ! Ваше высочество, вы - Королевскоя Кобра" << endl;
    else if (score > 250 && score <= 300)
        cout << "Ачивка! Охх тыж, гвоздь мне в C++! Да ты - Сетчатый питон!!!" << endl;
    else if (score > 300 && score <= 350)
        cout << "Ачивка! О господи, не ешь меня! Да ты Гигантская АНАКОНДА" << endl;
    else if (score > 350)
        cout << "Ачивка! ТВОЮ МАТЬ, ДА ТЫ ГОДЗИЛЛА!!!" << endl;

    // Очистка памяти
    delete[] tailX;
    delete[] tailY;

    return 0;
}
