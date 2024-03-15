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
int sep_line(int x);
void space();


/*---------ОТРИСОВКА ВНЕ ПОЛЕ---------*/

void player_win(int player); //1 2

/*---------ЛОГИКА---------*/

int logic(char command, int *ball_x, int *ball_y, int *ball_vector_x, int *ball_vector_y, int *p1_racket, int *p2_racket, int *player_1_score, int *player_2_score, int *restart);
int logic_ball(int *ball_x, int *ball_y, int *ball_vector_x, int *ball_vector_y, int *p1_racket, int *p2_racket, int *player_1_score, int *player_2_score);
int logic_racket(char command, int *p1_racket, int *p2_racket);

/*------------------------------------MAIN------------------------------------*/

int main() {

    /*---------СЧЕТ---------*/
    int player_1_score = 20;
    int player_2_score = 19;
    int restart = 0;
    /*---------МЯЧ----------*/

    int ball_vector_x = 0; // 0 - влево 1 - вправо
    int ball_vector_y = 0; // 0 - вниз 1 - вверх
    

   int ball_x = SIZE_X / 2;
   int ball_y = SIZE_Y / 2;

   /*---------РАКЕТКИ----------*/
   // ракетки по середине
   int p1_racket = SIZE_Y / 2; 
   int p2_racket = SIZE_Y / 2; 

   char command = 0;

    draw(ball_x, ball_y, p1_racket, p2_racket, player_1_score, player_2_score );

    while(scanf("%c", &command) && player_1_score < 21 && player_2_score < 21) {

        
        logic(command, &ball_x, &ball_y, &ball_vector_x, &ball_vector_y, &p1_racket, &p2_racket, &player_1_score, &player_2_score, &restart);
        draw(ball_x, ball_y, p1_racket, p2_racket, player_1_score, player_2_score );
    }
  return 0;
}

/*------------------------------------LOGIC------------------------------------*/

int logic(char command, int *ball_x, int *ball_y, int *ball_vector_x, int *ball_vector_y, int *p1_racket, int *p2_racket, int *player_1_score, int *player_2_score, int *restart) {
    logic_racket(command, p1_racket, p2_racket);
    logic_ball(ball_x, ball_y, ball_vector_x, ball_vector_y, p1_racket, p2_racket, player_1_score, player_2_score);
        
    return 1;
}

int logic_ball(int *ball_x, int *ball_y, int *ball_vector_x, int *ball_vector_y, int *p1_racket, int *p2_racket, int *player_1_score, int *player_2_score) {
    if(*ball_y == SIZE_Y - 1 || *ball_y == 0)
        *ball_vector_y = !(*ball_vector_y);
    if(*ball_x == SIZE_X - 1 && (*ball_y == *p2_racket - 1 || *ball_y == *p2_racket || *ball_y == *p2_racket + 1))
        *ball_vector_x = !(*ball_vector_x);
    else if(*ball_x == SIZE_X) {
        *player_1_score += 1;
        *ball_x = SIZE_X / 2;
        *ball_y = SIZE_Y / 2;
    }
    if(*ball_x == 1 && (*ball_y == *p1_racket - 1 || *ball_y == *p1_racket || *ball_y == *p1_racket + 1))
        *ball_vector_x = !(*ball_vector_x);
    else if(*ball_x == 0) {
        *player_2_score += 1;
        *ball_x = SIZE_X / 2;
        *ball_y = SIZE_Y / 2;
    }

    if(*ball_vector_x)  *ball_x += 1;
    else *ball_x -= 1;
    if(*ball_vector_y)  *ball_y += 1;
    else *ball_y -= 1;

    return 1;
}

int logic_racket(char command, int *p1_racket, int *p2_racket) {
    if(command == 'a' || command == 'A') {
        *p1_racket -= 1;
        *p1_racket = *p1_racket <= 1 ? 1 : *p1_racket;
    }
    if(command == 'z' || command == 'Z') {
        *p1_racket += 1;
        *p1_racket = *p1_racket >= SIZE_Y - 2 ? SIZE_Y - 2 : *p1_racket;
    }
    if(command == 'k' || command == 'K') {
        *p2_racket -= 1;
        *p2_racket = *p2_racket <= 1 ? 1 : *p2_racket;
    }
    if(command == 'm' || command == 'M') {
        *p2_racket += 1;
        *p2_racket = *p2_racket >= SIZE_Y - 2 ? SIZE_Y - 2 : *p2_racket;
    }
    return 1;
}

// int *ball_vector_x, int *ball_vector_y

/*------------------------------------DRAW------------------------------------*/


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
                    if(!sep_line(x))
                        space();
        
    }
    printf("|\n");
  }



  printf(" ");
  for (int i = 0; i < SIZE_X ; i++) printf("⎯");
  printf("\n");

  if(player_1_score >= 21)  player_win(1);
  if(player_2_score >= 21)  player_win(2);

}

void space() {
    printf(" ");
}

int sep_line(int x) {
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
        printf("o");
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

void player_win(int player) {
    printf("PLAYER %d WIN\n", player);
}