#include "keys.h"

#define YM_IC 4
#define YM_A0 5
#define YM_A1 6
#define YM_WR 7
#define YM_CS 10

#define YM_D0 22
#define YM_D1 23
#define YM_D2 24
#define YM_D3 25
#define YM_D4 26
#define YM_D5 27
#define YM_D6 28
#define YM_D7 29

#define REG_TVSKF 0x20
#define REG_KSL_OL 0x40
#define REG_AT_DC 0x60
#define REG_ST_RL 0x80
#define REG_FNUML 0xA0
#define REG_KEY_BL_FNUMH 0xB0
#define REG_RL_FMF_SYN 0xC0
#define REG_WAVE 0xE0

#define CARRIER 3
#define MOD     0

byte oplRegisters[256];

const unsigned char INSTRUMENT_ELPIANO1[12] PROGMEM = { 0x00, 0x01, 0x40, 0xF1, 0x50, 0x00, 0x06, 0x01, 0x04, 0xD2, 0x7C, 0x00 };


void opl3_init(){

 pinMode(YM_IC, OUTPUT);
 pinMode(YM_A0, OUTPUT);
 pinMode(YM_A1, OUTPUT);
 pinMode(YM_WR, OUTPUT);
 pinMode(YM_CS, OUTPUT);
 DDRC =  B11111111;
 
 digitalWrite(YM_IC, HIGH); 
 digitalWrite(YM_WR, HIGH);
 digitalWrite(YM_CS, HIGH);
 digitalWrite(YM_A0, LOW);
 digitalWrite(YM_A1, LOW);    
 
}




void opl3_write(unsigned char dat){
 
 PORTC = dat;
 delayMicroseconds(500);
 digitalWrite(YM_CS, LOW);
 digitalWrite(YM_WR, LOW);
 delayMicroseconds(500);
 digitalWrite(YM_WR, HIGH); 
 digitalWrite(YM_CS, HIGH);
 delayMicroseconds(500);
}

void opl3_send( unsigned char addr, unsigned char data){
  
digitalWrite(YM_A0, LOW);
digitalWrite(YM_A1, LOW);
opl3_write(addr);
digitalWrite(YM_A0, HIGH);
opl3_write(data);
digitalWrite(YM_A0, LOW);
}

void opl3_reset(){
int i;
 
  digitalWrite(YM_IC, LOW);
  delay(1);
  digitalWrite(YM_IC, HIGH);
  delay(1);
  /*for(int i = 0; i < 256; i ++) {
   
    opl3_send(i, 0x00);
  } */ 
}


void set_adsr(char channel, char mod_carr, unsigned char attack,unsigned char decay,unsigned char sustain,unsigned char releas){
  
  opl3_send(REG_AT_DC+ mod_carr + channel, (attack << 4) | decay );
  opl3_send(REG_ST_RL+ mod_carr + channel, (sustain << 4) | releas );
}

const unsigned char INSTRUMENT_ELORGAN1[12] PROGMEM = { 0x00, 0xB2, 0xCD, 0x91, 0x2A, 0x02, 0x09, 0xB1, 0x80, 0x91, 0x2A, 0x01 };


void set_instrument(char channel){
  
  opl3_send(REG_TVSKF + channel, 0x02);
  opl3_send(REG_KSL_OL + channel, 0x22);
  opl3_send(REG_AT_DC + channel, 0xf2);
  opl3_send(REG_ST_RL + channel, 0x13);
  opl3_send(REG_WAVE + channel, 0);
  opl3_send(REG_RL_FMF_SYN + channel, 0x0e);

  opl3_send(REG_TVSKF + channel + 3, 0x02);
  opl3_send(REG_KSL_OL + channel + 3, 0x00);
  opl3_send(REG_AT_DC+ channel + 3, 0xf5);
  opl3_send(REG_ST_RL+ channel+ 3, 0x43);
  opl3_send(REG_WAVE+ channel+ 3, 0);
  opl3_send(REG_RL_FMF_SYN+ channel+ 3, 0x0e);
  
}


//void opl3_keyOnOff(unsigned char channel, int on_off){}

const unsigned int noteFNumbers[12] = {
        0x156, 0x16B, 0x181, 0x198, 0x1B0, 0x1CA,
        0x1E5, 0x202, 0x220, 0x241, 0x263, 0x287
      };

//void opl3_setFreq(unsigned char channel, unsigned char block, unsigned char freq){}

void opl3_playkey(char channel, unsigned char octave, unsigned char key){
unsigned char low_f = noteFNumbers[key] & 0x0ff;
unsigned char kbf = 0x20 | (octave << 2) | (noteFNumbers[key] >> 8);
unsigned char kbf2 = 0x20 | ((octave-1) << 2) | (noteFNumbers[key] >> 8);

  opl3_send(REG_FNUML + channel, low_f);
  opl3_send(REG_KEY_BL_FNUMH+ channel, kbf2);

  opl3_send(REG_FNUML+ channel +3, low_f);
  opl3_send(REG_KEY_BL_FNUMH+ channel +3, kbf);
  
} 




void opl3_clrKey(char channel){
opl3_send(REG_KEY_BL_FNUMH+ channel, 0);
opl3_send(REG_KEY_BL_FNUMH+ channel+3, 0);  
} 

void play_min_chord(unsigned char octave, int val){  

if(val > 88 && val < 92){
  opl3_playkey(0,octave - 1, 9);
  opl3_playkey(1,octave - 1, 1);
  opl3_playkey(2,octave - 1, 4);
}

if(val > 166 && val < 170){
  opl3_playkey(0,octave - 1, 10);
  opl3_playkey(1,octave - 1, 2);
  opl3_playkey(2,octave - 1, 5);
}
  
if(val > 232 && val < 238){
  opl3_playkey(0,octave - 1, 11);
  opl3_playkey(1,octave - 1, 3);
  opl3_playkey(2,octave - 1, 6);
}

if(val > 288 && val < 292){
  opl3_playkey(0,octave,0);
  opl3_playkey(1,octave,3);
  opl3_playkey(2,octave,7);
}

if(val > 337 && val < 341){
  opl3_playkey(0,octave,1);
  opl3_playkey(1,octave,4);
  opl3_playkey(2,octave,8);
}

if(val > 380 && val < 384){
  opl3_playkey(0,octave,2);
  opl3_playkey(1,octave,5);
  opl3_playkey(2,octave,9);
}  

if(val > 417 && val < 421){
  opl3_playkey(0,octave,3);
  opl3_playkey(1,octave,6);
  opl3_playkey(2,octave,10);
}

if(val > 451 && val < 455){
  opl3_playkey(0,octave,4);
  opl3_playkey(1,octave,7);
  opl3_playkey(2,octave,11);
}

if(val > 481 && val < 485){
  opl3_playkey(0,octave,5);
  opl3_playkey(1,octave,8);
  opl3_playkey(2,octave +1,0);
}

if(val > 508 && val < 512){
  opl3_playkey(0,octave,6);
  opl3_playkey(1,octave,9);
  opl3_playkey(2,octave+1,1);
}

if(val > 532 && val < 536){
  opl3_playkey(0,octave,7);
  opl3_playkey(1,octave,10);
  opl3_playkey(2,octave+1,2);
}

if(val > 554 && val < 558){
  opl3_playkey(0,octave,8);
  opl3_playkey(1,octave,11);
  opl3_playkey(2,octave+1,3);
}

if(val > 575 && val < 579){
  opl3_playkey(0,octave,9);
  opl3_playkey(1,octave+1,0);
  opl3_playkey(2,octave+1,4);
}

if(val > 594 && val < 598){
  opl3_playkey(0,octave,10);
  opl3_playkey(1,octave+1,1);
  opl3_playkey(2,octave+1,5);
}

if(val > 611 && val < 615){
  opl3_playkey(0,octave,11);
  opl3_playkey(1,octave+1,2);
  opl3_playkey(2,octave+1,6);
}

if(val > 627 && val < 631){
  opl3_playkey(0,octave+1,0);
  opl3_playkey(1,octave+1,3);
  opl3_playkey(2,octave+1,7);
}

if(val > 641 && val < 645){
  opl3_playkey(0,octave+1,1);
  opl3_playkey(1,octave+1,4);
  opl3_playkey(2,octave+1,8);
}

if(val > 655 && val < 659){
  opl3_playkey(0,octave+1,2);
  opl3_playkey(1,octave+1,5);
  opl3_playkey(2,octave+1,9);
}

if(val > 668 && val < 672){
  opl3_playkey(0,octave+1,3);
  opl3_playkey(1,octave+1,6);
  opl3_playkey(2,octave+1,10);
}

if(val > 680 && val < 684){
  opl3_playkey(0,octave+1,4);
  opl3_playkey(1,octave+1,7);
  opl3_playkey(2,octave+1,11);
}
 
}




void play_maj_chord(unsigned char octave, int val){  

if(val > 88 && val < 92){
  opl3_playkey(0,octave - 1, 9);
  opl3_playkey(1,octave - 1, 1);
  opl3_playkey(2,octave - 1, 4);
}

if(val > 166 && val < 170){
  opl3_playkey(0,octave - 1, 10);
  opl3_playkey(1,octave - 1, 2);
  opl3_playkey(2,octave - 1, 5);
}
  
if(val > 232 && val < 238){
  opl3_playkey(0,octave - 1, 11);
  opl3_playkey(1,octave - 1, 3);
  opl3_playkey(2,octave - 1, 6);
}

if(val > 288 && val < 292){
  opl3_playkey(0,octave,0);
  opl3_playkey(1,octave,4);
  opl3_playkey(2,octave,7);
}

if(val > 337 && val < 341){
  opl3_playkey(0,octave,1);
  opl3_playkey(1,octave,5);
  opl3_playkey(2,octave,8);
}

if(val > 380 && val < 384){
  opl3_playkey(0,octave,2);
  opl3_playkey(1,octave,6);
  opl3_playkey(2,octave,9);
}  

if(val > 417 && val < 421){
  opl3_playkey(0,octave,3);
  opl3_playkey(1,octave,7);
  opl3_playkey(2,octave,10);
}

if(val > 451 && val < 455){
  opl3_playkey(0,octave,4);
  opl3_playkey(1,octave,8);
  opl3_playkey(2,octave,11);
}

if(val > 481 && val < 485){
  opl3_playkey(0,octave,5);
  opl3_playkey(1,octave,9);
  opl3_playkey(2,octave +1,0);
}

if(val > 508 && val < 512){
  opl3_playkey(0,octave,6);
  opl3_playkey(1,octave,10);
  opl3_playkey(2,octave+1,1);
}

if(val > 532 && val < 536){
  opl3_playkey(0,octave,7);
  opl3_playkey(1,octave,11);
  opl3_playkey(2,octave+1,2);
}

if(val > 554 && val < 558){
  opl3_playkey(0,octave,8);
  opl3_playkey(1,octave+1,0);
  opl3_playkey(2,octave+1,3);
}

if(val > 575 && val < 579){
  opl3_playkey(0,octave,9);
  opl3_playkey(1,octave+1,1);
  opl3_playkey(2,octave+1,4);
}

if(val > 594 && val < 598){
  opl3_playkey(0,octave,10);
  opl3_playkey(1,octave+1,2);
  opl3_playkey(2,octave+1,5);
}

if(val > 611 && val < 615){
  opl3_playkey(0,octave,11);
  opl3_playkey(1,octave+1,3);
  opl3_playkey(2,octave+1,6);
}

if(val > 627 && val < 631){
  opl3_playkey(0,octave+1,0);
  opl3_playkey(1,octave+1,4);
  opl3_playkey(2,octave+1,7);
}

if(val > 641 && val < 645){
  opl3_playkey(0,octave+1,1);
  opl3_playkey(1,octave+1,5);
  opl3_playkey(2,octave+1,8);
}

if(val > 655 && val < 659){
  opl3_playkey(0,octave+1,2);
  opl3_playkey(1,octave+1,6);
  opl3_playkey(2,octave+1,9);
}

if(val > 668 && val < 672){
  opl3_playkey(0,octave+1,3);
  opl3_playkey(1,octave+1,7);
  opl3_playkey(2,octave+1,10);
}

if(val > 680 && val < 684){
  opl3_playkey(0,octave+1,4);
  opl3_playkey(1,octave+1,8);
  opl3_playkey(2,octave+1,11);
}
 
}

void play_key(char channel,unsigned char octave, int val){  

if(val > 88 && val < 92){
  opl3_playkey(channel,octave - 1, 9);
}

if(val > 166 && val < 170){
  opl3_playkey(channel,octave - 1, 10);
}
  
if(val > 232 && val < 238){
  opl3_playkey(channel,octave-1, 11);
}

if(val > 288 && val < 292){
  opl3_playkey(channel,octave,0);
}

if(val > 337 && val < 341){
  opl3_playkey(channel,octave,1);
}

if(val > 380 && val < 384){
  opl3_playkey(channel,octave,2);
}  

if(val > 417 && val < 421){
  opl3_playkey(channel,octave,3);
}

if(val > 451 && val < 455){
  opl3_playkey(channel,octave,4);
}

if(val > 481 && val < 485){
  opl3_playkey(channel,octave,5);
}

if(val > 508 && val < 512){
  opl3_playkey(channel,octave,6);
}

if(val > 532 && val < 536){
  opl3_playkey(channel,octave,7);
}

if(val > 554 && val < 558){
  opl3_playkey(channel,octave,8);
}

if(val > 575 && val < 579){
  opl3_playkey(channel,octave,9);
}

if(val > 594 && val < 598){
  opl3_playkey(channel,octave,10);
}

if(val > 611 && val < 615){
  opl3_playkey(channel,octave,11);
}

if(val > 627 && val < 631){
  opl3_playkey(channel,octave+1,0);
}

if(val > 641 && val < 645){
  opl3_playkey(channel,octave+1,1);
}

if(val > 655 && val < 659){
  opl3_playkey(channel,octave+1,2);
}

if(val > 668 && val < 672){
  opl3_playkey(channel,octave+1,3);
}

if(val > 680 && val < 684){
  opl3_playkey(channel,octave+1,4);
}
 
}





