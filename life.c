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
    printf("show\n");
  int (*univ)[w] = u;
  char newstr[260] = "";
  int index = 0;
  for (int y = 0; y < h; y++){
    for (int x = 0; x < w; x++){
      int sh=(univ[y][x]==1);
      setDot(x,y,sh); 
      if (sh == 0) {
      newstr[index] = ' ';
      } else {
          newstr[index] = 'O';
      }
      index ++;
    }
  }  
  printf("%s\n",newstr);
}

void evolve(void *u, int w, int h){
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
 
  for (int y = 0; y < h; y++){
    for (int x = 0; x < w; x++){
      univ[y][x] = newar[y][x];
    }
  }
}

void game(int w, int h) {
  unsigned univ[h][w];
  for (int x = 0; x < w; x++){
    for (int y = 0; y < h; y++){
      univ[y][x] = arandom(0, 100)>65 ? 1 : 0;
    }
  }
  int sc=0;
  while (1) {
    show(univ, w, h);
    evolve(univ, w, h);
    delay(150);
    sc++;if(sc>150)break;
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