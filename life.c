#include <stdio.h>
#include <stdlib.h>

int flib[16][16];

// remove these for Arduino
void delay(int n) {

}

int arandom(int min, int max) {
    return rand() % max;
}

void setDot(int x,int y, int isOn){
    flib[x][y] = isOn;
}
 
void show(void *u, int w, int h){
  int (*univ)[w] = u;
  int index = 0;
  for (int y = 0; y < h; y++){
    for (int x = 0; x < w; x++){
      int sh=(univ[y][x]==1);
      setDot(x,y,sh); 
      index ++;
    }
  }  
}

int evolve(void *u, int w, int h){
  unsigned (*univ)[w] = u;
  unsigned newar[h][w];
 
  for (int y = 0; y < h; y++){
    for (int x = 0; x < w; x++){
      int n = 0;
      for (int y1 = y - 1; y1 <= y + 1; y1++)
        for (int x1 = x - 1; x1 <= x + 1; x1++)
          if (univ[(y1 + h) % h][(x1 + w) % w])
            n++;
      if (univ[y][x]) n--;
      newar[y][x] = (n == 3 || (n == 2 && univ[y][x]));
    }
  } 
 
  int living = 0;
  for (int y = 0; y < h; y++){
    for (int x = 0; x < w; x++){
      univ[y][x] = newar[y][x];
      living = living + newar[y][x];
    }
  }
  return living;
}

void game(int w, int h) {
  unsigned univ[h][w];
  for (int x = 0; x < w; x++){
    for (int y = 0; y < h; y++){
      univ[y][x] = arandom(0, 100)>65 ? 1 : 0;
    }
  }
  int sc=0;
  int last = 0;
  while (1) {
    show(univ, w, h);
    int living = evolve(univ, w, h);
    printf("%d\n",living);
    delay(150);
    sc++;
    if (living == 0 || living == last) {
      for (int x = 0; x < w; x++){
        for (int y = 0; y < h; y++){
          univ[y][x] = arandom(0, 100)>65 ? 1 : 0;
        }
      }
    }
    if (arandom(0,10) == 0) {
      int x = arandom(0,16);
      int y = arandom(0,16);
      univ[y][x] = 1;
    }
    last = living;
  }
}

void loop() {
  game(16,16);//w,h
}


int main(void) {
    printf("Hello world\n");
    loop();
    return 0;
}