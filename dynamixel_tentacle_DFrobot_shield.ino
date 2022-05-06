/*
  # This Sample code is to test the Digital Servo Shield.
  # Editor : Leff, original by YouYou@DFRobot(Version1.0)
  # Date   : 2016-1-19
  # Ver    : 1.1
  # Product: Digital Servo Shield for Arduino

  # Hardwares:
  1. Arduino UNO
  2. Digital Servo Shield for Arduino
  3. Digital Servos( Compatible with AX-12,CDS55xx...etc)
  4. Power supply:6.5 - 12V

  # How to use:
  If you don't know your Servo ID number, please
  1. Open the serial monitor, and choose NewLine,115200
  2. Send command:'d',when it's finished, please close the monitor and re-open it
  3. Send the command according to the function //controlServo()//
*/

#include <SPI.h>
#include <ServoCds55.h>
ServoCds55 myservo;

int servoNum = 1;
char inputCommand ;             // a string to hold incoming data
boolean inputComplete = false;

const int sliders[] = {A1, A2, A3};
const int n_sliders = 3;
const int LEDs[] = {5, 6, 7};
const int n_LEDs = 3;

const int pot_max = 1023;
const int pot_min = 0;
const int servo_max = 300;
const int servo_min = 0;


void setup () {
  Serial.begin (115200);
  myservo.begin ();
//  pinMode(pot_pin0, INPUT);      // Input pin for slider/ potentiometer 
//  pinMode(pot_pin1, INPUT);      // Input pin for slider/ potentiometer 
}

void loop () {
  // setting up
  delay(10000);
  myservo.setVelocity(200);     // set velocity to 100(range:0-300) in Servo mode
  myservo.write(1, 150); //ID:1  Pos:300  velocity:150
  myservo.write(2, 150); //ID:1  Pos:300  velocity:150
  Serial.println("done");
  for(int i=0; i<n_LEDs; i++){
    digitalWrite(LEDs[i], HIGH);
  } 


  while(1){
    for(int s=0; s<n_sliders; s++){
      Serial.print(analogRead(sliders[s]));
      int servo_val = map(analogRead(sliders[s]), pot_min, pot_max, servo_min, servo_max);  // Map value to full range of servo
      Serial.print(" ");
      Serial.print(servo_val);
      Serial.print("    ");
      myservo.write(s+1, servo_val);
      }
    Serial.println();

  }

//  serialEvent();
//  if (inputComplete) {
//    Serial.print("Your command is: "); Serial.println(inputCommand); Serial.println("");
//    controlServo(inputCommand);
//    // clear the command:
//    inputCommand = 0;
//    inputComplete = false;
//  }

}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      inputComplete = true;
      break;
    }
    inputCommand += inChar;
  }
}

void controlServo(char val) {
  switch (val) {
    case 'p':
      myservo.write(1, 300); //ID:1  Pos:300  velocity:150
      delay(3000);
      myservo.write(1, 0); //ID:1  Pos:0  velocity:150
      break;
    case 'q':
      myservo.write(2, 300); //ID:1  Pos:300  velocity:150
      delay(3000);
      myservo.write(2, 0); //ID:1  Pos:0  velocity:150
      break;
    case 'v':
      myservo.setVelocity(200);// set velocity to 100(range:0-300) in Servo mode
      break;      
    case 'r':
      myservo.Reset(servoNum);//Restore ID2 servo to factory Settings ( ID:1  Baud rate:1000000)
      break;
    case 'i':
      myservo.SetID(1,2);//ID:1   newID:2
      break;
    case 'd':  //Reset servo to ID>>servoNum. If you don't know your Servo ID, please send "d".
      Serial.print("Please wait..");
      for (int buf = 0; buf < 255; buf++) {
        myservo.SetID(buf, servoNum);
        if (buf % 50 == 0) Serial.print(".");
      }
      delay(2000);
      Serial.println("");   Serial.println("Please close the monitor and re-open it to play your servo! ");
      break;
    default:
      Serial.println("Please give me an available instruction:");
      Serial.println("  Servo mode: p_Set position; v_Set velocity.");
      Serial.println("  Motor mode: m_Rotate; v_Set velocity.");
      Serial.println("  Others: r_Reset servo to factory settings; i_Change servo ID."); Serial.println("");
  }
}
