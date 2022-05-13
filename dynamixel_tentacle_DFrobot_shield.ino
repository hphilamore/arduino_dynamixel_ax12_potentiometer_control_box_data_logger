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
// include the SD library:
#include <SD.h>

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

// change this to match your SD shield or module;
const int chipSelect = 4;

int servoNum = 1;
char inputCommand ;             // a string to hold incoming data
boolean inputComplete = false;

const int sliders[] = {A1, A2, A3};
const int n_sliders = 3;
const int LEDs[] = {5, 6, 7};
const int n_LEDs = 3;
const int button_LED = 8; 
const int button = 9; 

int slider_vals [3];
int servo_vals [3];

const int pot_max = 1023;
const int pot_min = 0;
const int servo_max = 300;
const int servo_min = 0;

bool button_status = LOW;
bool SD_present = HIGH;

//char filename[] = "file_0.txt";
String filename = "file_0.txt";

File myFile;


void setup () {
  Serial.begin (115200);
  myservo.begin ();
  pinMode(button_LED, OUTPUT);      // LED on button 
  pinMode(button, INPUT_PULLUP);      // LED on button 

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  Serial.print("Initializing SD card...");

  if (!SD.begin(5) or !card.init(SPI_HALF_SPEED, chipSelect) or !volume.init(card)) {
    Serial.println("initialization failed!");
    SD_present = LOW;
    //while (1);
    for (int i=0; i<5; i++){
      for(int i=0; i<n_LEDs; i++){
        digitalWrite(LEDs[i], HIGH);
      }
      delay(500);
      for(int i=0; i<n_LEDs; i++){
        digitalWrite(LEDs[i], LOW);
      }
      delay(500);
    }

    if (!card.init(SPI_HALF_SPEED, chipSelect)) {
      Serial.println("card not detected!");
      delay(500);
    }

    if (!volume.init(card)) {
      Serial.println("FAT 16/32 partition not detected!");
      delay(500);
    }
  }

  else{Serial.println("initialization done.");}

//  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
//    Serial.println("card not detected!");
//    delay(500);
//  }
//
//  if (!volume.init(card)) {
//    Serial.println("FAT 16/32 partition not detected!");
//    delay(500);
//  }

  

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
    //digitalWrite(button_LED, LOW);
  } 


  while(1){
    //myFile = SD.open("test.txt", FILE_WRITE);
    
    for(int i=0; i<n_sliders; i++){
      Serial.print(analogRead(sliders[i]));
      slider_vals[i] = analogRead(sliders[i]);
      int servo_val = map(analogRead(sliders[i]), pot_min, pot_max, servo_min, servo_max);  // Map value to full range of servo
      servo_vals[i] = servo_val;
      Serial.print(" ");
      Serial.print(servo_val);
      Serial.print("    ");
      myservo.write(i+1, servo_val);
      }
    Serial.println();
    
    //myFile.println();
    //myFile.close();

    if(digitalRead(button) == HIGH){   // button released
      digitalWrite(button_LED, LOW);
      Serial.println("stop");
      if(button_status){
        Serial.println("closing file");
        //delay(1000);
        myFile.close();
        button_status = LOW;
      }
     }
      
  
    else{                              // button pressed
      if(SD_present){digitalWrite(button_LED, HIGH);}
      Serial.println("record");
      if(!button_status){
        Serial.println("changing_states");
        button_status = HIGH;
        for(int i=0; i<100; i++){
          Serial.println("file_" + String(i) + ".txt");
          if(SD.exists("file_" + String(i) + ".txt")){
            Serial.println(" exists");
          }
          else{
            filename = "file_" + String(i) + ".txt";
            Serial.println("new_file_name");
            myFile = SD.open(filename, FILE_WRITE);
            myFile.println();
            myFile.println("Time, slider_1, servo_1, slider_2, servo_2, slider_3, servo_3");
            //myFile.close();
            //delay(1000);
            break;  
          }
          delay(1000);
        }
       }
      unsigned long Time = millis();
      myFile.print(Time);
      myFile.print(" , ");
      for(int i=0; i<n_sliders; i++){
        myFile.print(slider_vals[i]);
        myFile.print(" , ");
        myFile.print(servo_vals[i]);
        myFile.print(" , ");
      }
      myFile.println();
     }
   }
   Serial.println(button_status);

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
