#include <math.h>
#include <stdio.h>


#define SIZE_X 80
#define SIZE_Y 25

/*-------------ОТРИСОВКА-------------*/

void draw(int ball_x, int ball_y, int p1_racket, int p2_racket, int player_1_score, int player_2_score ); // отрисовка игрового поля

/*---------ОТРИСОВКА В ПОЛЕ---------*/
//только 1 printf может выполниться
//возвращают 1 если напечатал и 0 в обратном случае

int racket(int x, int y, int player, int racket_position); //player {0, 1}
int ball(int x, int y, int ball_x, int ball_y);
int score(int x, int y, int player_1_score, int player_2_score); 
int sep_line(int x, int y);
void space();




/*---------ОТРИСОВКА ВНЕ ПОЛЕ---------*/

void player_win(int player); //0 1


int main() {

    /*---------СЧЕТ---------*/
    int player_1_score = 20;
    int player_2_score = 12;

    /*---------МЯЧ----------*/

    int ball_state = 0; 

    /*
    1 - летит влево-вверх
    2 - летит влево
    3 - летит влево-вниз

    4 - летит вправо-вверх
    5 - летит право
    6 - летит право-вниз
    */

   int ball_x = SIZE_X / 2;
   int ball_y = SIZE_Y / 2;

   /*---------РОКЕТКИ----------*/
   // ракетки по середине
    int p1_racket = SIZE_Y / 2; 
    int p2_racket = SIZE_Y / 2; 
    

    draw(ball_x, ball_y, p1_racket, p2_racket, player_1_score, player_2_score );
  return 0;
}



void draw(int ball_x, int ball_y, int p1_racket, int p2_racket, int player_1_score, int player_2_score ) {

  printf("\n\n");
  printf(" Player 1");
  for (int i = 0; i < SIZE_X - 16; i++) printf(" ");
  printf("Player 2\n");

  printf(" ");
  for (int i = 0; i < SIZE_X; i++) printf("⎯");
  printf("\n");


  for (int y = 0; y < SIZE_Y; y++) {
    printf("|");
    for (int x = 0; x < SIZE_X; x++) {
        if(!ball(x, y, ball_x, ball_y))
            if(!racket(x, y, 0, p1_racket) && !racket(x, y, 1, p2_racket))
                if(!score(x, y, player_1_score, player_2_score))
                    if(!sep_line(x, y))
                        space();
        
    }
    printf("|\n");
  }


  printf(" ");
  for (int i = 0; i < SIZE_X ; i++) printf("⎯");
  printf("\n");

}

void space() {
    printf(" ");
}

int sep_line(int x, int y) {
    if(x == SIZE_X / 2) {
        printf("|");
        return 1;
    }

    return 0;
}

int racket(int x, int y, int player, int racket_position) {
    racket_position = racket_position <= 1 ? 1 : racket_position;
    racket_position = racket_position >= SIZE_Y - 2 ? SIZE_Y - 2 : racket_position;
    if(!player) {
        if(x == 0 && (y == racket_position || y == racket_position + 1 || y == racket_position - 1)) {
            printf("|");
            return 1;
        }
        else
            return 0;
        
    }
    else {
        if(x == SIZE_X - 1 && (y == racket_position || y == racket_position + 1 || y == racket_position - 1)){
             printf("|");
            return 1;
        }
        else
            return 0;
    }
    
    return 0;
}


int ball(int x, int y, int ball_x, int ball_y) {
    if(x == ball_x && y == ball_y) {
        printf("*");
        return 1;
    }
    return 0;
}

int score(int x, int y, int player_1_score, int player_2_score) {

    if(y == 0 && x == SIZE_X / 2 - 3 ) {
        printf("%d", player_1_score / 10);
        return 1;
    }
    if(y == 0 && x == SIZE_X / 2 - 2 ) {
        printf("%d", player_1_score % 10);
        return 1;
    }
    if(y == 0 && x == SIZE_X / 2 + 3 ) {
        printf("%d", player_2_score % 10);
        return 1;
    }
    if(y == 0 && x == SIZE_X / 2 + 2 ) {
        printf("%d", player_2_score / 10);
        return 1;
    }
    return 0;
    
}