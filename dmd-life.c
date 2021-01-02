#include "SPI.h"      
#include "DMD.h" 
#include "TimerOne.h"
#include "Arial_black_16.h" 
// you can remove the fonts if unused
#include "SystemFont5x7.h"
#define DISPLAYS_ACROSS 1   
#define DISPLAYS_DOWN 1       
/* change these values if you have more than one DMD connected */
DMD dmd(DISPLAYS_ACROSS,DISPLAYS_DOWN);

void ScanDMD()
{ 
  dmd.scanDisplayBySPI();
}

void setup()
{
   Timer1.initialize( 5000 );           
/*period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.*/

   Timer1.attachInterrupt( ScanDMD );  
/*attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()*/

   dmd.clearScreen( true );            
/* true is normal (all pixels off), false is negative (all pixels on) */

  Serial.begin(9600);

  Serial.println("setup complete.");

  show(28,16);
  delay(2000);

}

int iteration = 0;
int generations = 0;
int last = 0;
int delayLoop = 50;
unsigned char  univ[32][16] =
{
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0},
{0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0},
{0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0},
{0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0},
{0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
{0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0},
{0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
{0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
{0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
{0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

 
void show(int w, int h){
 for (int x = 0; x < w; x++){
  for (int y = 0; y < h; y++){
      int sh=(univ[x][y]==1);
      dmd.writePixel( x + 2, y, GRAPHICS_NORMAL, sh );
    }
  }  
}

int evolve(int w, int h){
  unsigned newar[w][h];
 
  for (int x = 0; x < w; x++){
    for (int y = 0; y < h; y++){
      int n = 0;
      for (int x1 = x - 1; x1 <= x + 1; x1++)
        for (int y1 = y - 1; y1 <= y + 1; y1++)
          if (univ[(x1 + w) % w][(y1 + h) % h])
            n++;
      if (univ[x][y]) n--;
      newar[x][y] = (n == 3 || (n == 2 && univ[x][y]));
    }
  }
 
  int living = 0;
  for (int x = 0; x < w; x++){
    for (int y = 0; y < h; y++){
      univ[x][y] = newar[x][y];
      living = living + newar[x][y];
    }
  }
  return living;
}

void randomWorld(int w, int h) {
      for (int x = 0; x < w; x++){
        for (int y = 0; y < h; y++){
          univ[x][y] = random(0, 100)>65 ? 1 : 0;
        }
      }
      delayLoop = random(0,50);
  }
  
void game(int w, int h) {
  show(w, h);
  int living = evolve(w, h);
  if (living == last) {
      int x = random(0,w-5);
      int y = random(0,h-5);
      univ[y+2][x+1] = 1;
      univ[y+1][x+2] = 1;
      univ[y][x+2] = 1;
      univ[y][x+1] = 1;
      univ[y][x] = 1;
  }
  last = living;
  if (living > 0) {
    generations = random(10,50);
    if (random(0,20) == 0) {
      int x = random(0,w-5);
      int y = random(0,h-5);
      univ[y][x] = 1;
    }
  } else {
    if (generations > 0) {
      generations = generations - 1;
    }
    if (generations == 0) {
      randomWorld(w,h);
    }
  }
}

void loop() {
  iteration = iteration + 1;
  game(28,16); // this doesn't work for more than x = 28
  delay(delayLoop);
  delayLoop = delayLoop + 5 - random(0,10);
  if (delayLoop <0 || delayLoop > 250) {
    delayLoop = random(0,250);
  }
}

