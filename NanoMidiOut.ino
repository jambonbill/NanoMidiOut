// Jambonbill nano MIDI sync  Ver 0.1
// very simple LSDJ to MIDI SYNC (just what i need)
// Use LSDJ Nano Sync on Arduino Pro micro / Leonardo / etc

// Send midi Start/Stop and Ticks

#include <SoftwareSerial.h>

//software serial
SoftwareSerial midiSerial(2, 3);

int gnd_pin=7;// gnd
int beat_pin=8;// blink blink ?
int bar_pin=9;
int analog_pin=18;//LSDJ NANO INPUT

void setup(){   
  // Set MIDI baud rate:
  midiSerial.begin(31250);

  //unused
  pinMode(gnd_pin, OUTPUT);
  pinMode(beat_pin, OUTPUT);
  pinMode(bar_pin, OUTPUT);
  digitalWrite(gnd_pin,0);//GND
  digitalWrite(beat_pin,1);
  digitalWrite(bar_pin,1);
  delay(100);
  digitalWrite(beat_pin,0);
  digitalWrite(bar_pin,0);
  
}

bool play=0;
bool last=0;//last state
byte ticks=0;
byte beats=0;
int idle=0;

void loop() {
  
  int val = analogRead(analog_pin);
  
  bool tick=0;//clock state
  if (val>10) {
    tick=1;
  }
  
  if(tick!=last){    
    ticks++;
    if(play==0){
      play=1;
      start();
    }
    idle=0;
    onTick();
  } else {
    idle++;
  }

  if(ticks>=24){// Regular 4/4
  //if(ticks>=36){//BIG SWING
    onBeat();
    ticks=0;
    beats++;
  }
  
  if(beats>=4){
    onBar();
    beats=0;
  }

  // blink to the beat - to move to onBeat then ? //
  if(play&&ticks<2){
    digitalWrite(beat_pin,1);
  }else{
    digitalWrite(beat_pin,0);
  }

  if(play&&beats==0&&ticks<2){
    digitalWrite(bar_pin,1);
  }else{
    digitalWrite(bar_pin,0);
  }

  last=tick;  

  if(idle>2000){
    stop();
  }
}

void start(){
  play=1;//playing
  onStart();
}

void stop(){
  if(play==0)return;//already stoped
  ticks=0;
  beats=0;
  idle=0;
  play=0;  
  onStop();
}


void onBeat(){
  //
}

void onBar(){//every 4 beats...
  //
}

void onTick(){
  midiSerial.write(248);
}

void onStart(){
  midiSerial.write(250);
}

void onStop(){
  midiSerial.write(252);
}
