#include <math.h>
#include <stdio.h>
#include <ncurses.h>

#define SIZE_X 80
#define SIZE_Y 25

/*-------------ОТРИСОВКА-------------*/

void draw(int ball_x, int ball_y, int p1_racket, int p2_racket, int player_1_score, int player_2_score ); // отрисовка игрового поля

/*---------ОТРИСОВКА В ПОЛЕ---------*/
//только 1 printw может выполниться
//возвращают 1 если напечатал и 0 в обратном случае

int racket(int x, int y, int player, int racket_position); //player {0, 1}
int ball(int x, int y, int ball_x, int ball_y);
int score(int x, int y, int player_1_score, int player_2_score); 
int sep_line(int x);
void space();


/*---------ОТРИСОВКА ВНЕ ПОЛЕ---------*/

void player_win(int player); //1 2

/*---------ЛОГИКА---------*/

int racket_move(int p1_racket, int p2_racket, char input_char);

















/*------------------------------------MAIN------------------------------------*/

int main(int argc,char *argv[]) {

    initscr();
    int TERM_X, TERM_Y;
    getmaxyx(stdscr, TERM_Y,  TERM_X);

    /*------------------------------------------------------------------------*/

    /*---------СЧЕТ---------*/
    int player_1_score = 0;
    int player_2_score = 0;

    /*---------МЯЧ----------*/

    int ball_vector_x = -1; // -1 - влево 1 - вправо
    int ball_vector_y = 1; // -1 - вниз 1 - вверх
    

   int ball_x = SIZE_X / 2;
   int ball_y = SIZE_Y / 2;

   /*---------РАКЕТКИ----------*/
   // ракетки по середине
   int p1_racket = SIZE_Y / 2; 
   int p2_racket = SIZE_Y / 2; 

   char move = 0;

    /*------------------------------------------------------------------------*/




    draw(ball_x, ball_y, p1_racket, p2_racket, player_1_score, player_2_score );

    while(player_1_score < 21 && player_2_score < 21) {
        move = getch();
        if (move != '\n') {        //чтобы нажатие Enter не считалось за действие
        int move_res = racket_move(p1_racket, p2_racket, move);  //записываем в переменную move_res новую позицию ракетки
    if ((move == 'a' || move == 'z') &&
       move_res != 0)  //проверка на корректный ввод. Переменная не может равняться 0, тк
                      //учитывается верхняя граница поля
    {
       p1_racket = move_res;
   }
    if ((move == 'm' || move == 'k') && move_res != 0) {
      p2_racket = move_res;
  }

    if (ball_x == 0) {  //если мячик проходит за ракетку по левому краю
        player_2_score = player_2_score + 1;  // прибавь очко
        p1_racket = SIZE_Y / 2;
        p2_racket = SIZE_Y / 2;
        ball_x = 0;  //мячик ставим перед ракеткой
        ball_y = SIZE_Y / 2 - 1;
        ball_vector_x = 1;  //вектора обнуляются
        ball_vector_y = 1;
      }

     if (ball_x == SIZE_X - 1) {
        player_1_score = player_1_score + 1;  //тоже самое для правого края
        p1_racket = SIZE_Y / 2;
        p2_racket = SIZE_Y / 2;
        ball_x = SIZE_X - 1;
        ball_y = SIZE_Y / 2 + 1;
        ball_vector_x = -1;
        ball_vector_y = -1;
     }

     if ((ball_x == 0) && (ball_vector_x == -1) && ((ball_y == p1_racket) || (ball_y == p1_racket - 1) || (ball_y == p1_racket + 1))) {  //если мячик отбивается
                ball_vector_x = ball_vector_x * -1;
    }
     if ((ball_x == SIZE_X - 1) && (ball_vector_x == 1) && ((ball_y == p2_racket) || (ball_y == p2_racket - 1) || (ball_y == p2_racket - 2))) {
         ball_vector_x = ball_vector_x * -1;  //поменяй вектор
     }

     if (ball_y == SIZE_Y - 1) {  //если ударилось об нижнее поле
         ball_vector_y = -1;
     }
     if (ball_y == 0) {  //если ударилось об верхнее
        ball_vector_y = 1;
     }

     ball_x = ball_x + ball_vector_x;  // двигаем мячик на 1 по 2 направлениям
     ball_y = ball_y + ball_vector_y;

    
  
  }
   draw(ball_x, ball_y, p1_racket, p2_racket, player_1_score, player_2_score );
}
        
    
    
  return 0;
}











/*------------------------------------LOGIC------------------------------------*/


//функция движения ракетки (ориентир - средняя черта)
int racket_move(int p1_racket, int p2_racket, char input_char) {
    //вводим наши буквы и сохраняем в переменную.
    if ((input_char == 'a') && (p1_racket - 3) != 0) {  //учитываем верхние границы поля
        p1_racket--;
        return p1_racket;
    }

    if ((input_char == 'z') && (p1_racket + 2) != SIZE_Y) {  //учитываем нижние границы
        p1_racket++;
        return p1_racket;
    }

    if ((input_char == 'k') && (p2_racket - 3) != 0) {
        p2_racket--;
        return p2_racket;
    }

    if ((input_char == 'm') && (p2_racket + 2) != SIZE_Y) {
        p2_racket++;
        return p2_racket;
    }


    return 0;
}























/*------------------------------------DRAW------------------------------------*/


void draw(int ball_x, int ball_y, int p1_racket, int p2_racket, int player_1_score, int player_2_score ) {

  printw("\n\n");
  printw(" Player 1");
  for (int i = 0; i < SIZE_X - 16; i++) printw(" ");
  printw("Player 2\n");

  printw(" ");
  for (int i = 0; i < SIZE_X; i++) printw("⎯");
  printw("\n");


  for (int y = 0; y < SIZE_Y; y++) {
    printw("|");
    for (int x = 0; x < SIZE_X; x++) {
        refresh();
        if(!ball(x, y, ball_x, ball_y))
            if(!racket(x, y, 0, p1_racket) && !racket(x, y, 1, p2_racket))
                if(!score(x, y, player_1_score, player_2_score))
                    if(!sep_line(x))
                        space();
        
    }
    printw("|\n");
  }



  printw(" ");
  for (int i = 0; i < SIZE_X ; i++) printw("⎯");
  printw("\n");

  if(player_1_score >= 21)  player_win(1);
  if(player_2_score >= 21)  player_win(2);

}

void space() {
    printw(" ");
}

int sep_line(int x) {

    int valid = 0;

    if(x == SIZE_X / 2) {
        printw("|");
        valid = 1;
    }

    return valid;
}

int racket(int x, int y, int player, int racket_position) {

    int valid = 0;

    racket_position = racket_position <= 1 ? 1 : racket_position;
    racket_position = racket_position >= SIZE_Y - 2 ? SIZE_Y - 2 : racket_position;
    if(!player) {
        if(x == 0 && (y == racket_position || y == racket_position + 1 || y == racket_position - 1)) {
            printw("|");
            valid = 1;
        }
        else
            valid = 0;
        
    }
    else {
        if(x == SIZE_X - 1 && (y == racket_position || y == racket_position + 1 || y == racket_position - 1)){
             printw("|");
            valid = 1;
        }
        else
            valid = 0;
    }
    
    return valid;
}


int ball(int x, int y, int ball_x, int ball_y) {

    int valid = 0;


    if(x == ball_x && y == ball_y) {
        printw("@");
        valid = 1;
    }
    return valid;
}

int score(int x, int y, int player_1_score, int player_2_score) {

    int valid = 0;

    if(y == 0 && x == SIZE_X / 2 - 3 ) {
        printw("%d", player_1_score / 10);
        valid = 1;
    }
    if(y == 0 && x == SIZE_X / 2 - 2 ) {
        printw("%d", player_1_score % 10);
        valid = 1;
    }
    if(y == 0 && x == SIZE_X / 2 + 3 ) {
        printw("%d", player_2_score % 10);
        valid = 1;
    }
    if(y == 0 && x == SIZE_X / 2 + 2 ) {
        printw("%d", player_2_score / 10);
        valid = 1;
    }
    return valid;
    
}

void player_win(int player) {
    for (int i = 0; i < SIZE_X / 2 - 5; i++) printw(" ");
    printw("PLAYER %d WIN\n", player);
}