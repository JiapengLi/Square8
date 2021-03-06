/*
  szmf heart

  loovee
  2014-3-20

  for more information, please refer to
  http://www.seeedstudio.com/wiki/index.php?title=Sidekick_Advanced_Kit&uselang=en
  
  About the pin connect, refer to http://arduino.cc/en/Tutorial/RowColumnScanning
  
  button connect to A0
  
*/

#include <LED_Matrix.h>
#include <LED_Matrix_dfs.h>
#include <MsTimer2.h>

const int pinBtn = A0;


int speed = 0;


// 0 for normal
// 1 for 45 angle
int heartType = 0;

int dir = DIR_RIGHT;

int delay_time1 = 200;
int delay_time2 = 1000;

// matrix of big heart
unsigned char big_heart[8] = {

0b00000000,
0b01100110,
0b11111111,
0b11111111,
0b11111111,
0b01111110,
0b00111100,
0b00011000,
};

// matrix of small heart
unsigned char small_heart[8] = {

0b00000000,
0b00000000,
0b00100100,
0b01111110,
0b01111110,
0b00111100,
0b00011000,
0b00000000,
};

unsigned char big_heart_45[8] = 
{
0b00000000,
0b01111110,
0b01111110,
0b01111110,
0b01111110,
0b01111000,
0b01111000,
0b00000000,
};

unsigned char small_heart_45[8] = 
{
0b00000000,
0b00000000,
0b00111000,
0b00111000,
0b00110000,
0b00000000,
0b00000000,
0b00000000,

};




/*

#define DIR_NORMAL      0x00
#define DIR_LEFT        0x01
#define DIR_RIGHT       0x02
#define DIR_DOWN        0x03

*/

void speedCnt()
{
    speed++;
    if(speed>4)speed=0;
    
    delay_time2 = (5-speed)*200;
}

void typeCnt()
{
    heartType = 1-heartType;
    
    if(heartType == 1)                  // 45 angle
    {
        dir = DIR_DOWN;
        matrix.setDirDisp(dir);
        matrix.dispMatrix(big_heart_45);
    }
    else
    {
        dir = DIR_RIGHT;
        matrix.setDirDisp(dir);
        matrix.dispMatrix(big_heart);
    }
    
    
}


// matrix of empty

unsigned char full[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
unsigned char empty[8] = {0, 0, 0, 0, 0, 0, 0, 0};

int state = 1;

void checkBtnAndDelay(int ms)
{
    
    for(int i=0; i<ms; i++)
    {
        delay(1);

        if(!digitalRead(pinBtn))
        {
            delay(10);
            
            long timer_t = millis();
            if(!digitalRead(pinBtn))
            {
                
                if(0 == heartType)
                {
                    matrix.dispMatrix(big_heart);
                }
                else
                {
                    matrix.dispMatrix(big_heart_45);
                }
                
                
                int flg_setype = 0;
                while(!digitalRead(pinBtn))
                {
                    if((millis()-timer_t)>1000)         // more than 1s
                    {
                        typeCnt();
                        timer_t = millis();
                        
                        flg_setype = 1;

                    }
                }
            
                if(flg_setype)return;
                speedCnt();

            }
            
            Serial.println("btn on");
            Serial.print("state = ");
            Serial.println(state);
            
            return;
        }
    }
}
long tiemr_tmp = 1;


void setup()
{
    matrix.begin();
    matrix.setDirDisp(dir);
    Serial.begin(115200);
    pinMode(pinBtn, INPUT);
    digitalWrite(pinBtn, HIGH);
    Serial.println("hello world");
}


void loop()
{
   /* matrix.dispMatrix(big_heart);
    checkBtnAndDelay(200);
        
    matrix.dispMatrix(small_heart);
    checkBtnAndDelay(delay_time2);*/
    
    if(0 == heartType)
    {
        matrix.dispMatrix(big_heart);
    }
    else
    {
        matrix.dispMatrix(big_heart_45);
    }
    
    checkBtnAndDelay(150);
    
    if(0 == heartType)   
    {
        matrix.dispMatrix(small_heart);
    }
    else
    {
        matrix.dispMatrix(small_heart_45);
    }
    
    checkBtnAndDelay(50);
    matrix.dispMatrix(empty);
    checkBtnAndDelay(delay_time2);
            
    if(0 == heartType)   
    {
        matrix.dispMatrix(small_heart);
    }
    else
    {
        matrix.dispMatrix(small_heart_45);
    }
    checkBtnAndDelay(80);

    
}
