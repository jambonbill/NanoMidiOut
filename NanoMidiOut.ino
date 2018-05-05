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
  byte a=0xB0;// Control change
  byte b=30;// CCNUM (i like this one)
  byte c=127;// MAXIMUM POWER !!!  
  // https://arduino.stackexchange.com/questions/41684/midiusb-why-is-the-command-put-twice
  //midiEventPacket_t event = {0|a>>4, a, b, c};
  //MidiUSB.sendMIDI(event);
  //MidiUSB.flush();//send NOW
}

void onBar(){//every 4 beats...
  byte a=0xB0;// Control change
  byte b=31;// CCNUM (i like this one)
  byte c=127;// MAXIMUM POWER !!!  
  //midiEventPacket_t event = {0|a>>4, a, b, c};
  //MidiUSB.sendMIDI(event);
  //MidiUSB.flush();//send NOW
}

void onTick(){
  byte a=248;
  //midiEventPacket_t event = {0|a>>4, a, 0, 0};//i must try to get rid of the zeros
  //MidiUSB.sendMIDI(event);
  //MidiUSB.flush();//send NOW
  //Serial.write(a);
  //Serial.write(pitch);
  //Serial.write(velocity);
  midiSerial.write(248);
}

void onStart(){
  byte a=250;
  Serial.write(a);
  //Serial.write(pitch);
  //Serial.write(velocity);
  midiSerial.write(250);
}

void onStop(){
  byte a=252;
  Serial.write(a);
  //Serial.write(pitch);
  //Serial.write(velocity);
  midiSerial.write(252);
}
