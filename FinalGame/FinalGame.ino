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
    CircuitPlayground.setPixelColor(i-1,0,0,0); //earses last pixel
    CircuitPlayground.setPixelColor(i,125,125,0); //sets next pixel
    if(measure[i][0]==1){ //checks if the array is one or zero and then plays a tone based on the the second array if true
      if(measure[i][1]==1){
        CircuitPlayground.playTone(1080, 50);
      }
    if(measure[i][1]==0){
      CircuitPlayground.playTone(700, 50);
    }
    delay(beatToTime()-50); //waits one beat
    }
    if(measure[i][0]==0){ //if false, just wait
      delay(beatToTime());
    }
  }
  CircuitPlayground.playTone(300, 200); // end beat
  delay(1000);
  CircuitPlayground.playTone(300, 200);
  //comparison
  for(int i = 0; i<8; i++){
    CircuitPlayground.setPixelColor(i-1,0,0,0); //rotates pixels by 1
    CircuitPlayground.setPixelColor(i,0,0,255);
    delay(beatToTime());
    if(flagLeft || flagRight){ //waits for the beat to be over then evaluates if the buttons were pressed.
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
      measureCompare[i][1] = measure[i][1]; //if no buttons were pressed it sets the seconed array to whatever it is in the original measure. This is just in case a tone is generated with out a play indicator in the first array.
      }
  }
  //check comparison
  Serial.println("start");
  for(int i =0; i<8; i++){
    Serial.print("Original: "); //prints the data of the original array and the new, played array
    Serial.print(measure[i][0]);
    Serial.print(measure[i][1]);
    Serial.print("   Played: ");
    Serial.print(measureCompare[i][0]);
    Serial.print(measureCompare[i][1]);
    Serial.print("   check: ");
    Serial.println(check);
    if(measure[i][0] != measureCompare[i][0]){ //checks if the original and played are the same
      check = false;
    }
    if(measure[i][1] != measureCompare[i][1]){
      check = false;
    }
  }
  //outputs based on comparisons
  if(check ==1){ //lights up red or green based on how you did and palys a tone
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
  for(int i =0; i<8; i++){ //resets the original measure
    for(int j = 0; j<=2; j++){
      measure[i][j] = 0;
    }
  }
}

void generateMeasure(){
  //generates a new meaasure by filling the first array with true or false if a tone should be played on that beat. Then if true, generates the tone in the second array. 
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
  //convertes the bpm to miliseconds
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