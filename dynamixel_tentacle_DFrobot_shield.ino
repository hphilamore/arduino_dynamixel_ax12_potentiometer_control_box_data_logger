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

const int slider0 = A0;
const int slider1 = A1;
//const int pot_pins[] = {A0, A1, A2};
//const int n_pins = 3;



//const int pot_pin0 = A0;         // input pin for potentiometer 
//const int pot_pin1 = A1;         // input pin for potentiometer 

//float slider_0, slider_1, slider_3;

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
//  delay(10000);
//  myservo.setVelocity(200);     // set velocity to 100(range:0-300) in Servo mode
//  myservo.write(1, 150); //ID:1  Pos:300  velocity:150
//  Serial.println("done");
//  delay(5000);
//  //myservo.setVelocity(50);      // set velocity to 100(range:0-300) in Servo mode
//
//  myservo.setVelocity(200);     // set velocity to 100(range:0-300) in Servo mode
//  myservo.write(2, 150); //ID:1  Pos:300  velocity:150
//  Serial.println("done");
//  delay(5000);
//  myservo.setVelocity(50);      // set velocity to 100(range:0-300) in Servo mode
  

  while(1){
//      myservo.write(servoNum, 300); //ID:1  Pos:0  velocity:150
//      delay(3000);
//      myservo.write(servoNum, 0); //ID:1  Pos:0  velocity:150
//      delay(3000);

//    for(int pin=1; pin<n_pins+1; pin++){
//      int pin_val = analogRead(pot_pins[pin]);
//      Serial.print(pin);
//      Serial.print(" ");
//      Serial.println(pin_val);
//      int servo_val = map(pin_val, pot_min, pot_max, servo_min, servo_max);
//      myservo.write(pin, servo_val); //ID:1  Pos:300  velocity:150
//      }

  Serial.print(analogRead(slider0));
  int servo_val = map(analogRead(slider0), pot_min, pot_max, servo_min, servo_max);  // Map value to full range of servo
  Serial.print(" ");
  Serial.print(servo_val);
  myservo.write(1, servo_val); //ID:1  Pos:300  velocity:150

  Serial.print("     ");

  Serial.print(analogRead(slider1));
  servo_val = map(analogRead(slider1), pot_min, pot_max, servo_min, servo_max);  // Map value to full range of servo
  Serial.print(" ");
  Serial.println(servo_val);
  myservo.write(2, servo_val); //ID:1  Pos:300  velocity:150
  
//  Serial.print(" ");
//  Serial.println(analogRead(pot_pin1));
//  slider_0 = analogRead(pot_pin0)
//
//   
//  int servo_val = map(pot_val, pot_min, pot_max, 0, 1023);  // Map value to full range of servo
  

//  serialEvent();
//  if (inputComplete) {
//    Serial.print("Your command is: "); Serial.println(inputCommand); Serial.println("");
//    controlServo(inputCommand);
//    // clear the command:
//    inputCommand = 0;
//    inputComplete = false;
//  }
 
}


//  controlServo('d');
//  controlServo('p');

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
//    case 'm':
//      myservo.rotate(servoNum, 150); //   Anti CW    ID:1  Velocity: 150_middle velocity  300_max
//      delay(2000);
//      myservo.rotate(servoNum, -150); //  CW     ID:1  Velocity: -150_middle velocity  -300_max
//      delay(2000);
//      myservo.rotate(servoNum, 0); //Stop
//      myservo.Reset(servoNum);    //Only Dynamixel AX need this instruction while changing working mode
//      //CDS55xx don't need this, it can switch freely between its working mode
//      break;
//
//    case 'n':
////      myservo.rotate(0, 150); //   Anti CW    ID:1  Velocity: 150_middle velocity  300_max
////      delay(2000);
////      myservo.rotate(0, -150); //  CW     ID:1  Velocity: -150_middle velocity  -300_max
////      delay(2000);
////      myservo.rotate(0, 0); //Stop
////      myservo.Reset(0);    //Only Dynamixel AX need this instruction while changing working mode
////      Serial.println("done_1");
//      myservo.rotate(2, 200); //   Anti CW    ID:1  Velocity: 150_middle velocity  300_max
//      delay(2000);
//      myservo.rotate(2, -200); //  CW     ID:1  Velocity: -150_middle velocity  -300_max
//      delay(2000);
//      myservo.rotate(2, 0); //Stop
//      myservo.Reset(2);    //Only Dynamixel AX need this instruction while changing working mode
//      Serial.println("done_2");
//      //CDS55xx don't need this, it can switch freely between its working mode
//      break;
      
    case 'r':
      myservo.Reset(servoNum);//Restore ID2 servo to factory Settings ( ID:1  Baud rate:1000000)
      break;
    case 'i':
      myservo.SetID(2,1);//ID:1   newID:2
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
