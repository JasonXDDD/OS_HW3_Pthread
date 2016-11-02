// Coding by JasonXDDD
// 2016/11/2

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

typedef struct item{
  char c;
  int loc;
  int y;
} item;

int start_x = 10, start_y = 10;
item queue[10];
int queue_tail = -1,  //producer: every i sec create item 
    queue_start = 0; //comsumer: user type an correct word
int count = 0;
char correctChar;

//fix Race Condition: Peterson’s solution
int flag[2] = {0}; //flag1: comsumer, flag2: producer
int turn = 0;



//gotoxy function for linux
void gotoxy(int x,int y){
  printf("%c[%d;%df",0x1B,y,x);
}
void setTextxy(int x, int y, char *text){
  gotoxy(x, y);
  printf("%s", text);
}

//init map
void drawMap(){
  int y = start_y;

  setTextxy(start_x, y-1, "hello word!! 請使用大寫字母!! 按ENTER輸入!!");
  setTextxy(start_x, y, "XXXXXXXXXXXXXXX");
  y++;
  
  int i;
  for(i = 0; i < 10; i++){
    gotoxy(start_x-3, y);
    printf("%2d ", i);
    setTextxy(start_x, y, "   ---   ---   ");
    y++;
  }
  setTextxy(start_x, y, "XXXXXXXXXXXXXXX");
  
}
 

//draw every change
void drawContent(){
  int i = queue_start;
  int drawCount = 0;

  drawMap();

  while(drawCount < count){
    int y = start_y + queue[i].y;

    switch(queue[i].loc){
      case 0:
        gotoxy(start_x, y);
        printf(" %c ---   ---   ", queue[i].c);
        break;
      case 1:
        gotoxy(start_x, y);
        printf("   --- %c ---   ", queue[i].c);
        break;
      case 2:
        gotoxy(start_x, y);
        printf("   ---   --- %c ", queue[i].c);
        break;
    }

    // gotoxy(start_x + 18, y);
    // printf("queue[%d], char: %c, start: %d, tail: %d, count: %d, correctChar: %c", i, queue[i].c, queue_start, queue_tail, count, correctChar);
    
    i = (i+1)%10;
    drawCount++;
  }
  gotoxy(start_x, start_y+20);  
}



item createItem(){
  item data;
  data.c = (char)(rand()%26)+65;
  data.loc = rand()%3;
  data.y = 1;

  return data;
}

void nextStep(){
  int i = queue_start;
  int tempCount = 0;

  while(tempCount < count){
    queue[i].y ++;

    i = (i+1)%10;
    tempCount++;
  }
}


int isFull(){
  if(count < 10) return 0;
  else return 1;
}

int isAns(char c){
  if(c == correctChar) return 1;
  else return 0;
}

void *userKeyIn(){
  fflush(stdin);
  char keyIn;

  //do comsumer
  while(1){
    gotoxy(start_x, start_y+20); 
    scanf(" %c", &keyIn);
    
    if(isAns(keyIn) == 1){
      //fix Race Condition
      flag[0] = 1;
      turn = 1;
      while(flag[1] == 1 && turn == 1);

      //critical section
      queue_start = (queue_start+1)%10;
      correctChar = queue[queue_start].c;
      count--;
      drawContent();

      flag[0] = 0;
    }

    if(keyIn == '0'){
      count=99;
      break;
    }
  }

  pthread_exit(0);
}


int main (){
  pthread_t ptd;
  pthread_attr_t ptd_attr;

  srand(time(NULL));
  system("clear");
  drawMap();

  pthread_attr_init(&ptd_attr);
  pthread_create(&ptd, &ptd_attr, userKeyIn, NULL);
  
  //do producer
  while(isFull() != 1){
    sleep(1);    
    fflush(stdout);

    //fix Race Condition
    flag[1] = 1;
    turn = 0;
    while(flag[0] == 1 && turn == 0);

    //critical section
    nextStep();
    queue_tail = (queue_tail+1)%10;
    queue[queue_tail] = createItem();
    count++;
    drawContent();

    if(queue_tail == queue_start && count == 1)
      correctChar = queue[queue_tail].c;
  
    flag[1] = 0;      
  }

  if(isFull() == 1) exit(0);

  pthread_join(ptd, NULL);
  exit(0);
}