#include <Arduino.h>

#define LEDARRAY_D 9
#define LEDARRAY_C 8
#define LEDARRAY_B 7
#define LEDARRAY_A 6
#define LEDARRAY_G 5
#define LEDARRAY_DI 4
#define LEDARRAY_CLK 3
#define LEDARRAY_LAT 2

unsigned char Display_Buffer[2];

unsigned int Word1[32];

unsigned univ[16][16] = 
{
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

int iteration = 0;
int generations = 0;
int WIDTH = 16;
int HEIGHT = 16;
int offset = 0;

void setup() {
  pinMode(LEDARRAY_D, OUTPUT); 
  pinMode(LEDARRAY_C, OUTPUT);
  pinMode(LEDARRAY_B, OUTPUT);
  pinMode(LEDARRAY_A, OUTPUT);
  pinMode(LEDARRAY_G, OUTPUT);
  pinMode(LEDARRAY_DI, OUTPUT);
  pinMode(LEDARRAY_CLK, OUTPUT);
  pinMode(LEDARRAY_LAT, OUTPUT);
  Serial.begin(9600);
}

void randomWorld() {
 for (int x = 0; x < WIDTH; x++){
    for (int y = 0; y < HEIGHT; y++){
      univ[y][x] = random(0, 100)>65 ? 1 : 0;
    }
  }
}

void SceneToWord(){
  int i, k, key = 0;
  unsigned int value;
  for(i = 0; i < WIDTH; i++){
    for(k = 0; k < HEIGHT; k++){
      int v = 1;
      if (univ[i][k] == 1) {
          v = 0;
      }
      if(i < 8){
        value = v << (7 - i);
        Word1[15 - k] += value;
      } else {
        value = v << (15 - i);
        Word1[31 - k] += value;
      }
    }
  }
}


void Display(unsigned int dat[])  {
  unsigned char i;

  offset = (offset + 1) % 16;

  for(int row = 0 ; row < 16 ; row++ )
  {
    int i = (row + offset) % 16;
    digitalWrite(LEDARRAY_G, HIGH);   
    
    Display_Buffer[0] = dat[i];   
    Display_Buffer[1] = dat[i+16];

    Send(Display_Buffer[1]);
    Send(Display_Buffer[0]);

    digitalWrite(LEDARRAY_LAT, HIGH);          
    delayMicroseconds(1);
  
    digitalWrite(LEDARRAY_LAT, LOW);
    delayMicroseconds(1);

    Scan_Line(i);             

    digitalWrite(LEDARRAY_G, LOW);
    
    delayMicroseconds(100);   
  } 
}


void Scan_Line( unsigned int m) { 
  switch(m)
  {
    case 0:     
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW);          
      break;
    case 1:         
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH);     
      break;
    case 2:         
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW);     
      break;
    case 3:         
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH);    
      break;
    case 4:
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW);     
      break;
    case 5:
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH);    
      break;
    case 6:
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW);    
      break;
    case 7:
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH);     
      break;
    case 8:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW);     
      break;
    case 9:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH);    
      break;  
    case 10:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW);    
      break;
    case 11:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH);     
      break;
    case 12:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW);    
      break;
    case 13:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH);     
      break;
    case 14:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW);     
      break;
    case 15:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH);    
      break;
    default : break;  
  }
}

void Send( unsigned int dat) {
  unsigned char i;
  digitalWrite(LEDARRAY_CLK, LOW);
  delayMicroseconds(1);;  
  digitalWrite(LEDARRAY_LAT, LOW);
  delayMicroseconds(1);;

  for( i = 0 ; i < 8 ; i++ )
  {
    if( dat&0x01 )
    {
      digitalWrite(LEDARRAY_DI, HIGH);  
    }
    else
    {
      digitalWrite(LEDARRAY_DI, LOW);
    }

    delayMicroseconds(1);
    digitalWrite(LEDARRAY_CLK, HIGH);         
      delayMicroseconds(1);
    digitalWrite(LEDARRAY_CLK, LOW);
      delayMicroseconds(1);   
    dat >>= 1;
      
  }     
}

int evolve(){
  unsigned newar[WIDTH][HEIGHT];
 
  for (int x = 0; x < WIDTH; x++){
    for (int y = 0; y < HEIGHT; y++){
      int n = 0;
      for (int x1 = x - 1; x1 <= x + 1; x1++)
        for (int y1 = y - 1; y1 <= y + 1; y1++)
          if (univ[(x1 + WIDTH) % WIDTH][(y1 + HEIGHT) % HEIGHT])
            n++;
      if (univ[x][y]) n--;
      newar[x][y] = (n == 3 || (n == 2 && univ[x][y]));
    }
  } 
 
  int living = 0;
  for (int x = 0; x < WIDTH; x++){
    for (int y = 0; y < HEIGHT; y++){
      univ[x][y] = newar[x][y];
      living = living + newar[x][y];
    }
  }
  return living;
}

void game() {
  int living = evolve();
  if (living > 0) {
    generations = 100;
    if (random(0,20) == 0) {
      int x = random(0,WIDTH);
      int y = random(0,HEIGHT);
      univ[x][y] = 1;
    }
  } else {
    generations = generations - 1;
    if (generations <=0) {
        randomWorld();
    }
  }
}

void loop() {
  clearWord();
  SceneToWord();
  Display(Word1);
  iteration = iteration + 1;
  if (iteration == 8) {
    game();
    iteration = 0;
  }
}

void clearWord(){
  for(int i = 0; i < 32; i++)
    Word1[i] = 0;
}