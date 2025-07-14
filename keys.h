

#define KEY_A3  0x01
#define KEY_Bb3 0x02
#define KEY_B3  0x03
#define KEY_C4  0x04
#define KEY_Cs4 0x05
#define KEY_D4  0x06
#define KEY_Eb4 0x07
#define KEY_E4  0x08
#define KEY_F4  0x09
#define KEY_Fs4 0x0a
#define KEY_G4  0x0b
#define KEY_Gs4 0x0c
#define KEY_A4  0x0d
#define KEY_Bb4 0x0e
#define KEY_B4  0x0f
#define KEY_C5  0x10
#define KEY_Cs5 0x11
#define KEY_D5  0x12
#define KEY_Eb5 0x13
#define KEY_E5  0x14

#define BUTTON_KNOB 2
#define BUTTON_SHIFT 3
#define BUTTON_STEP 30

int M;
int curr_octave;
char key_state = 0;

int tempo=0;
int new_tempo;

unsigned char tick_counter=0;

unsigned char mod_octave;
unsigned char mod_attack;
unsigned char mod_decay;
unsigned char mod_sustain;
unsigned char mod_release;

unsigned char carrier_attack;
unsigned char carrier_decay;
unsigned char carrier_sustain;
unsigned char carrier_release;



unsigned char decode_key(int val){

if(val > 98 && val < 105){
 return(KEY_A3);
}

if(val > 180 && val < 188){
  return(KEY_Bb3);
}
  
if(val > 249 && val < 257){
  return(KEY_B3);
}

if(val > 308 && val < 316){
  return(KEY_C4);
}

if(val > 359 && val < 367){
  return(KEY_Cs4);
}

if(val > 403 && val < 411){
  return(KEY_D4);
}  

if(val > 441 && val < 449){
  return(KEY_Eb4);
}

if(val > 475 && val < 484){
  return(KEY_E4);
}

if(val > 506 && val < 514){
  return(KEY_F4);
}

if(val > 534 && val < 541){
  return(KEY_Fs4);
}

if(val > 557 && val < 565){
  return(KEY_G4);
}

if(val > 579 && val < 587){
  return(KEY_Gs4);
}

if(val > 599 && val < 607){
  return(KEY_A4);
}

if(val > 617 && val < 625){
  return(KEY_Bb4);
}

if(val > 634 && val < 642){
  return(KEY_B4);
}

if(val > 649 && val < 657){
  return(KEY_C5);
}

if(val > 664 && val < 672){
  return(KEY_Cs5);
}

if(val > 677 && val < 685){
  return(KEY_D5);
}

if(val > 689 && val < 697){
  return(KEY_Eb5);
}

if(val > 700 && val < 708){
  return(KEY_E5);
}

}



void print_key(int val){  

if(val > 88 && val < 92){
  Serial.println("A3");
}

if(val > 166 && val < 170){
  Serial.println("Bb3");
}
  
if(val > 232 && val < 238){
  Serial.println("B3");
}

if(val > 288 && val < 292){
  Serial.println("C4");
}

if(val > 337 && val < 341){
  Serial.println("C#4");
}

if(val > 380 && val < 384){
  Serial.println("D4");
}  

if(val > 417 && val < 421){
  Serial.println("Eb4");
}

if(val > 451 && val < 455){
  Serial.println("E4");
}

if(val > 481 && val < 485){
  Serial.println("F4");
}

if(val > 508 && val < 512){
  Serial.println("F#4");
}

if(val > 532 && val < 536){
  Serial.println("G4");
}

if(val > 554 && val < 558){
  Serial.println("G#4");
}

if(val > 575 && val < 579){
  Serial.println("A4");
}

if(val > 594 && val < 598){
  Serial.println("Bb4");
}

if(val > 611 && val < 615){
  Serial.println("B4");
}

if(val > 627 && val < 631){
  Serial.println("C5");
}

if(val > 641 && val < 645){
  Serial.println("C#5");
}

if(val > 655 && val < 659){
  Serial.println("D5");
}

if(val > 668 && val < 672){
  Serial.println("Eb5");
}

if(val > 680 && val < 684){
  Serial.println("E5");
}
 
}


int get_octave(){
 int val =  (analogRead(A0));
 if(val >= 0 && val < 128)return(0);
 else{
      if(val > 127 && val < 256)return(1);
      else{
           if(val > 255 && val < 384)return(2);
           else{
                if(val > 383 && val < 512)return(3);
                else
                   if(val > 511 && val < 640)return(4);
                   else{
                     if(val > 639 && val < 768)return(5);
                      else{
                        if(val > 767 && val < 896)return(6);
                         else{
                            if(val > 895 && val < 1024)return(7);
                            else return(7);
                         }
                          
                      }
                   }
           }
      }
      }
}

int get_tempo(){
//unsigned int val = 15625 /((((analogRead(A2) / 5) + 64)<<2)) -1;
unsigned int val = ((analogRead(A2) / 5) + 64);  
 //Serial.println(val);
 return(val);
}

int get_pot2(){
 return(analogRead(A3)>>5);
}

int get_pot3(){
 return(analogRead(A4)>>5);
}

int get_pot4(){
 return(analogRead(A5)>>5);
}


int get_pot5(){
 return(analogRead(A6)>>5);
}



