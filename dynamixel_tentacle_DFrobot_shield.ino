/*
 * 
Potentiometer sliders to control the position of 3 Dynamixel A-12 
digital servo motors Latching push button is used to control 
potentiometer value and servo position, as time series, to an SD card. 

*/

#include <SPI.h>
#include <SD.h>
File myFile;

#include <RTClib.h>
RTC_DS3231 rtc;

#include <ServoCds55.h>
ServoCds55 myservo;

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

const int chipSelect = 4;          // change this to match your SD shield or module; Uno=4, Mega=53

int servoNum = 1;
char inputCommand ;                // a string to hold incoming data
boolean inputComplete = false;

const int sliders[] = {A1, A2, A3};// slider potentiometers to control motors
const int n_sliders = 3;           // number of sliders
const int LEDs[] = {5, 6, 7};      // LEDs on sliders
const int n_LEDs = 3;              // number of LEDs
const int button_LED = 8;          // LED on data record button
const int button = 9;              // data record button

int slider_vals [3];               // buffer to hold slider  values
int servo_vals [3];                // buffer to hold servo positions 

// Set max ad min values for mapping slider potentiometer to servo position 
const int pot_max = 1023;          // max potentiometer value
const int pot_min = 0;             // min potentiometer value           
const int servo_max = 300;         // servo max position (max range = [0, 300])
const int servo_min = 0;           // servo min position

bool button_status = LOW;          // default data record button position
bool SD_present = HIGH;            // default SD card status

String filename = "file_0.txt";




void setup () {
  Serial.begin (115200);
  myservo.begin ();
  pinMode(button_LED, OUTPUT);       
  pinMode(button, INPUT_PULLUP);    

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
 
  // SETUP RTC MODULE
  Serial.print("Initializing SD card...");
  if (!rtc.begin()) {
    Serial.println(F("Couldn't find RTC"));
    delay(500);
  }

  // When time needs to be set on a new device, or after a power loss
  // Set the RTC to the date & time this sketch was compiled
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
  // SETUP SD CARD
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect) or !card.init(SPI_HALF_SPEED, chipSelect) or !volume.init(card)) {
    
    // Identify problem
    if (!card.init(SPI_HALF_SPEED, chipSelect)) {
      Serial.println("card not detected!");
      delay(500);
    }

    if (!volume.init(card)) {
      Serial.println("FAT 16/32 partition not detected!");
      delay(500);
    }
    
    // Flash slider LEDs 5 times to show SD card initialization failed 
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
  }

  else{Serial.println("initialization done.");}
}

void loop () {
  delay(10000);                   // allow 10 seconds for servos to initialise
  myservo.setVelocity(200);       // set velocity to 100(range:0-300) in Servo mode
  //myservo.SetID(2,1);//ID:1   newID:2
//  myservo.write(1, 150);          // set servos ID 1-3 to centre/neutral position 
//  myservo.write(2, 150);          
//  myservo.write(3, 150);          
  Serial.println("servos ready");
  for(int i=0; i<n_LEDs; i++){    // turn all slider LEDs on
    digitalWrite(LEDs[i], HIGH);
  } 


  while(1){
    
    // Map value of each slider to corresponding servo
    for(int i=0; i<n_sliders; i++){
      Serial.print(analogRead(sliders[i]));
      slider_vals[i] = analogRead(sliders[i]);
      int servo_val = map(analogRead(sliders[i]), pot_min, pot_max, servo_min, servo_max);  
      servo_vals[i] = servo_val;
      Serial.print(" ");
      Serial.print(servo_val);
      Serial.print("    ");
      myservo.write(i+1, servo_val);
      }
    Serial.println();

    // If record button is released 
    if(digitalRead(button) == HIGH){   // button released (internal pullup)
      digitalWrite(button_LED, LOW);   // turn the record button LED off
      Serial.println("stop");
      if(button_status){               // if newly released
        Serial.println("closing file");// stop recording 
        myFile.close();
        button_status = LOW;
      }
     }
      
  
    // If button is pressed 
    else{                              
      if(SD_present){digitalWrite(button_LED, HIGH);}   // turn the record button on 
      Serial.println("record");
      if(!button_status){                               // if newly pressed
        button_status = HIGH;
        for(int i=0; i<100; i++){                       // check for exting files of format "file_x.txt"
          Serial.println("file_" + String(i) + ".txt");
          if(SD.exists("file_" + String(i) + ".txt")){
            Serial.println(" exists");
          }
          else{
            filename = "file_" + String(i) + ".txt";   // when an unused file name is found
            Serial.println("new_file_name"); 
            myFile = SD.open(filename, FILE_WRITE);    // open the file
            myFile.println();
            // write column headings
            myFile.println("Date, Time, ms_from_start, slider_1, servo_1, slider_2, servo_2, slider_3, servo_3");
            break;  
          }
          //delay(1000);
        }
       }
      
      DateTime now = rtc.now();

      myFile.print(now.year(), DEC);     // write date to SD card
      myFile.print("-");
      myFile.print(now.month(), DEC);
      myFile.print("-");
      myFile.print(now.day(), DEC);
      myFile.print(" , ");


      myFile.print(now.hour(), DEC);     // write time to SD card
      myFile.print("-");
      myFile.print(now.minute(), DEC);
      myFile.print("-");
      myFile.print(now.second(), DEC);
      myFile.print(" , ");
      
                         
      unsigned long Time = millis();     // write ms since start of recording to SD card
      myFile.print(Time);                
      myFile.print(" , ");

      
      for(int i=0; i<n_sliders; i++){
        myFile.print(slider_vals[i]);    // write slider vals to SD card
        myFile.print(" , ");
        myFile.print(servo_vals[i]);     // write servo vals to SD card
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
