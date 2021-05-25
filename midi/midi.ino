#include <SoftwareSerial.h>
#include <MIDI.h>

#define VRx A2
#define VRy A1
#define button 9
#define led 8


boolean on = false;
int p1, p2, p1P, p2P;
int Jx, Jy, JxP, JyP;

MIDI_CREATE_DEFAULT_INSTANCE();

SoftwareSerial serialPin(2, 3); // RX, TX


void setup() {
  Serial.begin(31250); //default MIDI 31250
  serialPin.begin(38400);
  
  pinMode(button, INPUT);
  pinMode(led, OUTPUT);

  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT); 

  digitalWrite(13, LOW);
  
  MIDI.begin();
}

void loop() {
  if (digitalRead(button) == HIGH && !on) {
    MIDI.sendNoteOn(60, 64, 1);
    on = true;
    digitalWrite(led, HIGH);
  }
  else if (digitalRead(button) == LOW && on) {
    MIDI.sendNoteOff(60, 64, 1);
    on = false;
    digitalWrite(led, LOW);
  }

  updateValues();

  sendUpdate();
}

void updateValues() {
  p1P = p1; // previous value
  p1 = analogRead(A5)/8; // Divide by 8 to get range of 0-127 for midi
  analogPinMidiTX(1, p1, p1P); //TX value 
   
  p2P = p2; 
  p2 = analogRead(A4)/8;
  analogPinMidiTX(2, p2, p2P); 

  JxP = Jx;
  Jx = analogRead(VRx)/8;
  analogPinMidiTX(3, Jx, JxP);

  JyP = Jy;
  Jy = analogRead(VRy)/8;
  analogPinMidiTX(4, Jy, JyP);
}

void sendUpdate() {
  char data[]= {(char)p1, (char)p2, (char)Jx, (char)Jy};
  serialPin.write(data, 4);
}

void analogPinMidiTX(int chan, int curr, int prev)
{  
  // TX Value only if it has changed
  if(prev != curr)
  {
    MidiTX(176, chan, curr); // 176 = CC command, 1 = Which Control, val = value read from Potentiometer
  }
}

void MidiTX(unsigned char MESSAGE, unsigned char CONTROL, unsigned char VALUE) //Value on Midi Command
{
   Serial.write(MESSAGE);
   Serial.write(CONTROL);
   Serial.write(VALUE);
}
