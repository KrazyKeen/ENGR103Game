#include <Adafruit_CircuitPlayground.h>

volatile bool measure[8][2];
volatile bool measureCompare[8][2];
volatile bool check;
volatile float time;
volatile float bpm = 60.0;
volatile bool flagLeft;
volatile bool flagRight;


void setup() {
  // put your setup code here, to run once:
  CircuitPlayground.begin();
  Serial.begin(9600);
  //randomSeed(analogRead(0));
  attachInterrupt(digitalPinToInterrupt(5), bFlag1, CHANGE); //interupt for buttons
  attachInterrupt(digitalPinToInterrupt(4), bFlag2, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
  check = 1;
  generateMeasure();
  //testing for loop to set it all to right button and every beat
  /*
  for(int i =0; i<=8; i++){
    for(int j = 0; j<=1; j++){
      measure[i][j] = 1;
    }
  }
  */
  CircuitPlayground.playTone(400, 200); // start beep
  for(int i =0; i<8; i++){ //plays tones based on measure
    CircuitPlayground.setPixelColor(i-1,0,0,0);
    CircuitPlayground.setPixelColor(i,125,125,0);
    if(measure[i][0]==1){
      if(measure[i][1]==1){
        CircuitPlayground.playTone(1080, 50);
      }
    if(measure[i][1]==0){
      CircuitPlayground.playTone(700, 50);
    }
    delay(beatToTime()-50);
    }
    if(measure[i][0]==0){
      delay(beatToTime());
    }
  }
  CircuitPlayground.playTone(300, 200); // end beat
  delay(1000);
  CircuitPlayground.playTone(300, 200);
  //comparison
  for(int i = 0; i<8; i++){
    CircuitPlayground.setPixelColor(i-1,0,0,0);
    CircuitPlayground.setPixelColor(i,0,0,255);
    delay(beatToTime());
    if(flagLeft || flagRight){
      measureCompare[i][0] = 1;
      if(flagLeft == 1){
        measureCompare[i][1] = 0;
      }
      if(flagRight == 1){
        measureCompare[i][1] = 1;
      }
      flagLeft = 0;
      flagRight = 0;
    }
    else{
      measureCompare[i][0] = 0;
      measureCompare[i][1] = measure[i][1];
      }
  }
  //check comparison
  Serial.println("start");
  for(int i =0; i<8; i++){
    Serial.print("Original: ");
    Serial.print(measure[i][0]);
    Serial.print(measure[i][1]);
    Serial.print("   Played: ");
    Serial.print(measureCompare[i][0]);
    Serial.print(measureCompare[i][1]);
    Serial.print("   check: ");
    Serial.println(check);
    if(measure[i][0] != measureCompare[i][0]){
      check = false;
    }
    if(measure[i][1] != measureCompare[i][1]){
      check = false;
    }
  }
  //outputs based on comparisons
  if(check ==1){
    for(int c =0; c<=10; c++){
      CircuitPlayground.setPixelColor(c,0,255,0);
  }
  delay(100);
    for(int c =0; c<=10; c++){
      CircuitPlayground.setPixelColor(c,0,0,0);
  }
  bpm+=10;
  }
  if(check ==0){
    for(int c =0; c<=10; c++){
      CircuitPlayground.setPixelColor(c,255,0,0);
  }
  delay(100);
    for(int c =0; c<=10; c++){
      CircuitPlayground.setPixelColor(c,0,0,0);
  }
  }
  for(int i =0; i<8; i++){
    for(int j = 0; j<=2; j++){
      measure[i][j] = 0;
    }
  }
}

void generateMeasure(){
  for(int i= 0; i<8; i++){
    //for(int j = 0; j<2; j++){
      measure[i][0] = random(0,2);
      if(measure[i][0] == 1){
        measure[i][1] = random(0,2);
      }
      else{
      measure[i][1] = 0;
      }
    }
  }

float beatToTime(){
  time = (60.0/bpm)*(500);
  //Serial.println(time);
  return time;

}

void bFlag1(){
  flagLeft = 1;
  //CircuitPlayground.playTone(700, 50);
}

void bFlag2(){
  flagRight = 1;
  //CircuitPlayground.playTone(1080, 50);
}