#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//gotoxy function for linux
void gotoxy(int x,int y){
  printf("%c[%d;%df",0x1B,y,x);
}
void setTextxy(int x, int y, char text[]){
  gotoxy(x, y);
  printf("%s", text);
}

void drawMap(int start_x, int start_y){
  setTextxy(start_x, start_y, "hello world");
  start_y++;
  setTextxy(start_x, start_y, "XXXXXXXXXXXXXXX");
  start_y++;
  
  int i;
  for(i = 0; i < 10; i++){
    gotoxy(start_x-3, start_y);
    printf("%2d ", i);
    setTextxy(start_x, start_y, "XXX   XXX   XXX");
    start_y++;
  }
  setTextxy(start_x, start_y, "XXXXXXXXXXXXXXX");
  
}
 

void drawText(int x, int y, char c){  
}

int main (){
  pthread_t thd;
  pthread_attr_t thd_attr;


  system("clear");
  drawMap(10, 10);

  pthread_attr_init(&thd_attr);


}