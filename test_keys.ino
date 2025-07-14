#include "opl3.h"



void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 pinMode(LED_BUILTIN, OUTPUT);
 pinMode(9, OUTPUT);
 pinMode(31, OUTPUT);
 pinMode(8, INPUT);
 
pinMode(BUTTON_KNOB, INPUT);
pinMode(BUTTON_SHIFT, INPUT);
pinMode(BUTTON_STEP, INPUT);

 digitalWrite(9, HIGH); 
 
 opl3_init();
 opl3_reset();
 set_instrument(0);
 set_instrument(1);
 set_instrument(2);
// tempo = get_tempo();
// set_interrupt(tempo<<2);
 //sei();
 
//opl3_playkey(0);
//digitalWrite(YM_IC, LOW);

//PORTC = 0xaa;

}


void set_interrupt(int temp){
 int aux;
 cli(); 
 TCCR1A = 0;// set entire TCCR1A register to 0
 TCCR1B = 0;// same for TCCR1B
 TCNT1  = 0;//initialize counter value to 0
 // set compare match register for 1hz increments
 aux = 915900 / (temp) - 1;
 OCR1A = aux;
 
 // turn on CTC mode
 TCCR1B |= (1 << WGM12);
 // Set CS10 and CS12 bits for 1024 prescaler
 TCCR1B |= (1 << CS12) | (1 << CS10);  
 // enable timer compare interrupt
 TIMSK1 |= (1 << OCIE1A);
 
 sei();
}

void run(){

  char modeButton = digitalRead(BUTTON_KNOB); //MODE: D8, PD5
  if(modeButton == LOW){
  mod_octave= get_octave();
  mod_sustain = get_pot4();
  mod_release = get_pot5();
  carrier_sustain = get_pot2();
  carrier_release = get_pot3();
  //
  }else{
  curr_octave= get_octave();
  mod_attack = get_pot4();
  mod_decay = get_pot5();
  carrier_attack = get_pot2();
  carrier_decay = get_pot3();
  }
  set_adsr(0,CARRIER,carrier_attack, carrier_decay, carrier_sustain, carrier_release);
  set_adsr(0,MOD,mod_attack, mod_decay, mod_sustain, mod_release);    

  set_adsr(1,CARRIER,carrier_attack, carrier_decay, carrier_sustain, carrier_release);
  set_adsr(1,MOD,mod_attack, mod_decay, mod_sustain, mod_release); 

  set_adsr(2,CARRIER,carrier_attack, carrier_decay, carrier_sustain, carrier_release);
  set_adsr(2,MOD,mod_attack, mod_decay, mod_sustain, mod_release); 
  
  modeButton = digitalRead(BUTTON_SHIFT);
  if(modeButton == LOW){
digitalWrite(9, HIGH); 
   M =  (analogRead(A1));
 // print_key(M);
  if(M > 594 && M < 598){
     opl3_send(REG_RL_FMF_SYN, 0xf1);
    opl3_send(REG_RL_FMF_SYN+ 3, 0xf1);
   // Serial.println("MODE A");
}
  if(M > 611 && M < 615){
    opl3_send(REG_RL_FMF_SYN, 0xf0);
    opl3_send(REG_RL_FMF_SYN+ 3, 0xf0);//Serial.println("Bb4");
    //Serial.println("MODE B");
}  
  }
  else{
    digitalWrite(9, LOW);
    //Serial.println(tempo);  
  M =  (analogRead(A1));
 //opl3_clrKey(); 
 if(M > 88 && M < 690){
   digitalWrite(31, HIGH); 
   //Serial.println(M);
   //print_key(M);
   //play_min_chord(curr_octave,M);
   play_maj_chord(curr_octave,M);
   //play_key(0,curr_octave,M);//opl3_playkey(0);
   key_state = 1;
  }else{
digitalWrite(31, LOW); 
 if(key_state){
  opl3_clrKey(0);
  opl3_clrKey(1); 
  opl3_clrKey(2);  
  key_state = 0;}
  
}
 }
}


boolean toggle0 = 0;
boolean toggle1 = 0;
boolean toggle2 = 0;

ISR(TIMER1_COMPA_vect){

 tick_counter +=1;
 int clk_div = tick_counter>>1;
 digitalWrite(13,clk_div & 0x1);

 
 
}





void loop() {

/*new_tempo= get_tempo();
if(new_tempo != tempo){
    tempo = new_tempo;
  //  set_interrupt(tempo*4);
  }
*/
run();
  
}
