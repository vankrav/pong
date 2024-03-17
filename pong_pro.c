#include <ncurses.h>  // Подключение библиотеки для интератива
#include <stdio.h>  // Подключение стандартной библиотеки ввода-вывода

#define SIZE_X 80  // Определение константы размера поля по горизонтали
#define SIZE_Y 25  // Определение константы размера поля по вертикали
#define WIN 21

/*-------------ОТРИСОВКА-------------*/

int draw(int ball_x, int ball_y, int p1_racket, int p2_racket, int player_1_score,
         int player_2_score);  // Прототип функции отрисовки игрового поля

/*---------ОТРИСОВКА В ПОЛЕ---------*/
// только 1 printw может выполниться
// возвращают 1 если напечатал и 0 в обратном случае

int racket(int x, int y, int player, int racket_position);  // Функция отрисовки ракетки, player {0, 1}
int ball(int x, int y, int ball_x, int ball_y);  // Функция отрисовки мяча
int score(int x, int y, int player_1_score, int player_2_score);  // Функция отрисовки счета
int sep_line(int x);  // Функция отрисовки разделительной линии
void space();         // Функция отрисовки пустого пространства

/*---------ОТРИСОВКА ВНЕ ПОЛЕ---------*/

int player_win(int player_1_score, int player_2_score,
               int win);  // Функция объявления победителя, player {1, 2}

/*---------ЛОГИКА---------*/

int racket_move(int p1_racket, int p2_racket, char input_char);  // Функция для перемещения ракетки

/*------------------------------------MAIN------------------------------------*/

int main() {
    /*------------------------------------------------------------------------*/
    initscr();
    curs_set(0);
    timeout(100);

    /*---------СЧЕТ---------*/
    int player_1_score = 0;  // Инициализация счета первого игрока
    int player_2_score = 0;  // Инициализация счета второго игрока

    /*---------МЯЧ----------*/

    int ball_vector_x = -1;  // Направление движения мяча по горизонтали (-1 - влево, 1 - вправо)
    int ball_vector_y = 1;  // Направление движения мяча по вертикали (-1 - вниз, 1 - вверх)

    int ball_x = SIZE_X / 2;  // Начальная позиция мяча по горизонтали
    int ball_y = SIZE_Y / 2;  // Начальная позиция мяча по вертикали

    /*---------РАКЕТКИ----------*/
    // Ракетки располагаются посередине
    int p1_racket = SIZE_Y / 2;  // Начальная позиция первой ракетки
    int p2_racket = SIZE_Y / 2;  // Начальная позиция второй ракетки

    char move = 0;  // Переменная для хранения ввода пользователя

    /*------------------------------------------------------------------------*/

    draw(ball_x, ball_y, p1_racket, p2_racket, player_1_score,
         player_2_score);  // Вызов функции отрисовки игрового поля

    while (1) {  // Основной игровой цикл

        move = getch();

        if (!draw(ball_x, ball_y, p1_racket, p2_racket, player_1_score, player_2_score))
            break;  // Повторный вызов функции отрисовки игрового поля

        if (move != '\n') {  // Игнорирование нажатия Enter
            int move_res = racket_move(p1_racket, p2_racket,
                                       move);  // Вызов функции перемещения ракетки и сохранение результата
            if ((move == 'a' || move == 'z') &&
                move_res != 0)  // Проверка на корректный ввод и изменение позиции ракетки
            {
                p1_racket = move_res;  // Обновление позиции первой ракетки
            }
            if ((move == 'm' || move == 'k') &&
                move_res != 0) {  // Проверка на корректный ввод и изменение позиции ракетки
                p2_racket = move_res;  // Обновление позиции второй ракетки
            }

            if (ball_x <= 0) {  // Проверка на прохождение мяча за левую границу поля
                player_2_score += 1;  // Увеличение счета второго игрока
                // Сброс позиций ракеток и мяча
                p1_racket = SIZE_Y / 2;
                p2_racket = SIZE_Y / 2;

                ball_x = 0;
                ball_y = SIZE_Y / 2 - 1;
                // Сброс векторов движения мяча
                ball_vector_x = 1;
                ball_vector_y = 1;
            }

            if (ball_x > SIZE_X - 1) {  // Проверка на прохождение мяча за правую границу поля
                player_1_score += 1;  // Увеличение счета первого игрока
                // Сброс позиций ракеток и мяча
                p1_racket = SIZE_Y / 2;
                p2_racket = SIZE_Y / 2;
                ball_x = SIZE_X - 1;
                ball_y = SIZE_Y / 2 + 1;
                // Сброс векторов движения мяча
                ball_vector_x = -1;
                ball_vector_y = -1;
            }

            if ((ball_x == 1) && (ball_vector_x == -1) &&
                ((ball_y == p1_racket) || (ball_y == p1_racket - 1) ||
                 (ball_y == p1_racket + 1))) {  // Проверка на отбивание мяча первой ракеткой
                ball_vector_x *= -1;  // Изменение направления движения мяча по горизонтали
            }
            if ((ball_x == SIZE_X - 2) && (ball_vector_x == 1) &&
                ((ball_y == p2_racket) || (ball_y == p2_racket - 1) ||
                 (ball_y == p2_racket + 1))) {  // Проверка на отбивание мяча второй ракеткой
                ball_vector_x *= -1;  // Изменение направления движения мяча по горизонтали
            }

            if (ball_y == SIZE_Y - 1) {  // Проверка на удар мяча о нижнюю границу поля
                ball_vector_y = -1;  // Изменение направления движения мяча по вертикали
            }
            if (ball_y == 0) {  // Проверка на удар мяча о верхнюю границу поля
                ball_vector_y = 1;  // Изменение направления движения мяча по вертикали
            }

            ball_x += ball_vector_x;  // Обновление позиции мяча по горизонтали
            ball_y += ball_vector_y;  // Обновление позиции мяча по вертикали
        }
    }
    while (1) {
        move = getch();
        if (move == 'q') break;
    }

    return 0;  // Возвращение 0 при успешном завершении программы
}

/*------------------------------------LOGIC------------------------------------*/

// функция движения ракетки (ориентир - средняя черта)
int racket_move(int p1_racket, int p2_racket, char input_char) {
    // вводим наши буквы и сохраняем в переменную.
    if ((input_char == 'a') &&
        (p1_racket - 2) >= 0) {  // Проверка на возможность движения первой ракетки вверх
        p1_racket--;       // Движение ракетки вверх
        return p1_racket;  // Возвращение новой позиции ракетки
    }

    if ((input_char == 'z') &&
        (p1_racket + 2) < SIZE_Y) {  // Проверка на возможность движения первой ракетки вниз
        p1_racket++;       // Движение ракетки вниз
        return p1_racket;  // Возвращение новой позиции ракетки
    }

    if ((input_char == 'k') &&
        (p2_racket - 2) >= 0) {  // Проверка на возможность движения второй ракетки вверх
        p2_racket--;       // Движение ракетки вверх
        return p2_racket;  // Возвращение новой позиции ракетки
    }

    if ((input_char == 'm') &&
        (p2_racket + 2) < SIZE_Y) {  // Проверка на возможность движения второй ракетки вниз
        p2_racket++;       // Движение ракетки вниз
        return p2_racket;  // Возвращение новой позиции ракетки
    }

    return 0;  // Возвращение 0, если движение невозможно
}

/*------------------------------------DRAW------------------------------------*/

int draw(int ball_x, int ball_y, int p1_racket, int p2_racket, int player_1_score, int player_2_score) {
    int valid = 1;

    clear();

    printw(" Player 1");                                // Вывод надписи "Player 1"
    for (int i = 0; i < SIZE_X - 16; i++) printw(" ");  // Вывод пробелов между надписями игроков
    printw("Player 2\n");                               // Вывод надписи "Player 2"

    printw(" ");  // Вывод пробела перед верхней границей поля
    for (int i = 0; i < SIZE_X; i++) printw("-");  // Вывод верхней границы поля
    printw("\n");                                  // Переход на новую строку

    for (int y = 0; y < SIZE_Y; y++) {
        // Цикл по вертикали поля
        printw("|");                          // Вывод левой границы поля
        for (int x = 0; x < SIZE_X; x++) {    // Цикл по горизонтали поля
            if (!ball(x, y, ball_x, ball_y))  // Попытка отрисовки мяча
                if (!racket(x, y, 0, p1_racket) && !racket(x, y, 1, p2_racket))  // Попытка отрисовки ракеток
                    if (!score(x, y, player_1_score, player_2_score))  // Попытка отрисовки счета
                        if (!sep_line(x))  // Попытка отрисовки разделительной линии
                            space();  // Отрисовка пустого пространства
        }
        printw("|\n");  // Вывод правой границы поля и переход на новую строку
    }

    printw(" ");  // Вывод пробела перед нижней границей поля
    for (int i = 0; i < SIZE_X; i++) printw("-");  // Вывод нижней границы поля
    printw("\n");                                  // Переход на новую строку

    valid = !player_win(player_1_score, player_2_score, WIN);

    return valid;
}

void space() {
    printw(" ");  // Вывод пробела
}

int sep_line(int x) {
    int valid = 0;  // Инициализация переменной валидности

    if (x == SIZE_X / 2) {  // Проверка на позицию разделительной линии
        printw("|");        // Вывод разделительной линии
        valid = 1;          // Установка валидности в 1
    }

    return valid;  // Возвращение значения валидности
}

int racket(int x, int y, int player, int racket_position) {
    int valid = 0;  // Инициализация переменной валидности

    if (racket_position <= 1) {
        racket_position = 1;  // Корректировка позиции ракетки вверх
    }
    if (racket_position >= SIZE_Y - 2) {
        racket_position = SIZE_Y - 2;  // Корректировка позиции ракетки вниз
    }
    if (!player) {  // Проверка на первого игрока
        if (x == 0 && (y == racket_position || y == racket_position + 1 ||
                       y == racket_position - 1)) {  // Проверка на позицию ракетки первого игрока
            printw("|");                             // Вывод ракетки
            valid = 1;                               // Установка валидности в 1
        } else
            valid = 0;  // Установка валидности в 0

    } else {  // Проверка на второго игрока
        if (x == SIZE_X - 1 && (y == racket_position || y == racket_position + 1 ||
                                y == racket_position - 1)) {  // Проверка на позицию ракетки второго игрока
            printw("|");                                      // Вывод ракетки
            valid = 1;                                        // Установка валидности в 1
        } else
            valid = 0;  // Установка валидности в 0
    }

    return valid;  // Возвращение значения валидности
}

int ball(int x, int y, int ball_x, int ball_y) {
    int valid = 0;  // Инициализация переменной валидности

    if (x == ball_x && y == ball_y) {  // Проверка на позицию мяча
        printw("@");                   // Вывод мяча
        valid = 1;                     // Установка валидности в 1
    }
    return valid;  // Возвращение значения валидности
}

int score(int x, int y, int player_1_score, int player_2_score) {
    int valid = 0;  // Инициализация переменной валидности

    if (y == 0 && x == SIZE_X / 2 - 3) {  // Проверка на позицию счета первого игрока (десятки)
        printw("%d", player_1_score / 10);  // Вывод десятков счета первого игрока
        valid = 1;                          // Установка валидности в 1
    }
    if (y == 0 && x == SIZE_X / 2 - 2) {  // Проверка на позицию счета первого игрока (единицы)
        printw("%d", player_1_score % 10);  // Вывод единиц счета первого игрока
        valid = 1;                          // Установка валидности в 1
    }
    if (y == 0 && x == SIZE_X / 2 + 3) {  // Проверка на позицию счета второго игрока (единицы)
        printw("%d", player_2_score % 10);  // Вывод единиц счета второго игрока
        valid = 1;                          // Установка валидности в 1
    }
    if (y == 0 && x == SIZE_X / 2 + 2) {  // Проверка на позицию счета второго игрока (десятки)
        printw("%d", player_2_score / 10);  // Вывод десятков счета второго игрока
        valid = 1;                          // Установка валидности в 1
    }
    return valid;  // Возвращение значения валидности
}

int player_win(int player_1_score, int player_2_score, int win) {  // Функция объявления победителя

    int valid = 0;

    if (player_1_score == win) {
        for (int i = 0; i < SIZE_X / 2 - 5; i++) printw(" ");  // Вывод пробелов перед надписью победителя
        printw("PLAYER 1 WIN\n");  // Вывод надписи победителя
        valid = 1;
    }
    if (player_2_score == win) {
        for (int i = 0; i < SIZE_X / 2 - 5; i++) printw(" ");  // Вывод пробелов перед надписью победителя
        printw("PLAYER 2 WIN\n");  // Вывод надписи победителя
        valid = 1;
    }
    return valid;
}
