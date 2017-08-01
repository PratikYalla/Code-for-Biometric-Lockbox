
#include <Keypad.h>
#include <FPS_GT511C3.h>
#include <SoftwareSerial.h>
#include <Password.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

FPS_GT511C3 fps(12, 13);

Password password = Password("1234");

int address = 0;

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {2, 3, 4, 5}; 
byte colPins[COLS] = {6, 7, 8}; 

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


bool State_0 = false;
bool State_1 = false;
bool State_2 = false;
bool State_3 = false;
char typed_password;

Servo myservo;  

int pos = 0;    

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);


void setup()
{
  Serial.begin(9600);
  delay(100);
  fps.Open();
  fps.SetLED(true);
  ///getPassword();
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {

  if (State_0 == false) {
    lcd.begin(16, 2);
    lcd.clear();
    lcd.print("Type Password!");
    delay(5000);
    for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
      // scroll one position left:
      lcd.scrollDisplayLeft();
      // wait a bit:
      delay(150);
    }
    int counter = 0;
    while (counter < 4) {
      char key = keypad.getKey();
      if (key) {
        password.append(key);
        lcd.begin(16, 2);
        lcd.clear();
        lcd.println(" you typed: ");
        lcd.println(key);
        for (int positionCounter = 0; positionCounter < 1; positionCounter++) {
          // scroll one position left:
          lcd.scrollDisplayLeft();
          // wait a bit:
          delay(150);
        }
        counter++;
      }
    }
    if (password.evaluate()) {
      lcd.begin(16, 2);
      lcd.clear();
      lcd.println("Correct Password");
      delay(1000);
      for (int positionCounter = 0; positionCounter < 0; positionCounter++) {
        // scroll one position left:
        lcd.scrollDisplayLeft();
        // wait a bit:
        delay(150);
      }
      State_0 = true;
    }
    else {
      lcd.begin(16, 2);
      lcd.clear();
      lcd.println(" Incorrect Password");
      delay(1000);
      for (int positionCounter = 0; positionCounter < 3; positionCounter++) {
        // scroll one position left:
        lcd.scrollDisplayLeft();
        // wait a bit:
        delay(150);
      }
      delay(1000);
      password.reset();
    }
  }


  while (State_0 && !State_1 ) {
    if (fps.IsPressFinger())
    {
      fps.CaptureFinger(false);
      int id = fps.Identify1_N();
      if (id < 200)
      {
        lcd.begin(16, 2);
        lcd.clear();
        lcd.print("Verified id:");
        lcd.println(id);
        delay(1000);
        State_1 = true;
      }
      else
      {
        lcd.begin(16, 2);
        lcd.clear();
        lcd.println("Finger not found");
      }
    }
    else
    {
      lcd.begin(16, 2);
      lcd.clear();
      lcd.println("Please press finger");
      delay(1000);
      for (int positionCounter = 0; positionCounter < 3; positionCounter++) {
        // scroll one position left:
        lcd.scrollDisplayLeft();
        // wait a bit:
        delay(150);
      }
      delay(1000);
      for (int positionCounter = 0; positionCounter < 3; positionCounter++) {
        // scroll one position right:
        lcd.scrollDisplayRight();
        // wait a bit:
        delay(150);
      }
    }
    delay(100);

  }
  if (State_1  && !State_2) {
    myservo.write(180);              
    delay(15);                      
    State_2 = true;

  }
  if (State_2 && !State_3) {
    lcd.begin(16, 2);
    lcd.clear();
    lcd.println("Type Password to Terminate Access or Enroll New Finger");
    delay(3000);
    for (int positionCounter = 0; positionCounter < 24; positionCounter++) {
      // scroll one position left:
      lcd.scrollDisplayLeft();
      // wait a bit:
      delay(350);
    }
    delay(1000);
    for (int positionCounter = 0; positionCounter < 24; positionCounter++) {
      // scroll one position right:
      lcd.scrollDisplayRight();
      // wait a bit:
      delay(150);
    }
    int counter = 0;
    while (counter < 1) {
      char key = keypad.getKey();
      if (key == '*') {
        lcd.begin(16, 2);
        lcd.clear();
        lcd.println(" you typed: ");
        lcd.println(key);
        delay(3000);
        lcd.begin(16, 2);
        lcd.clear();
        lcd.println(" Access Terminated");
        delay(3000);
        for (int positionCounter = 0; positionCounter < 2; positionCounter++) {
          // scroll one position left:
          lcd.scrollDisplayLeft();
          // wait a bit:
          delay(150);
        }
        delay(3000);
        counter++;
        myservo.write(90);              
        State_0 = false;
        State_1 = false;
        State_2 = false;


        delay(15);

      }
      else if (key == '#') {
        int enrollid = 0;
        bool usedid = true;
        while (usedid == true)
        {
          usedid = fps.CheckEnrolled(enrollid);
          if (usedid == true) enrollid++;
        }
        fps.EnrollStart(enrollid);

        // enroll
        lcd.begin(16, 2);
        lcd.clear();
        lcd.print("Press finger to Enroll #");
        lcd.println(enrollid);
        delay(1000);
        for (int positionCounter = 0; positionCounter < 9; positionCounter++) {
          // scroll one position left:
          lcd.scrollDisplayLeft();
          // wait a bit:
          delay(150);
        }
        delay(1000);
        while (fps.IsPressFinger() == false) delay(100);
        bool bret = fps.CaptureFinger(true);
        int iret = 0;
        if (bret != false)
        {
          lcd.begin(16, 2);
          lcd.clear();
          lcd.println("Remove finger");
          fps.Enroll1();
          while (fps.IsPressFinger() == true) delay(100);
          lcd.begin(16, 2);
          lcd.clear();
          lcd.println("Press same finger again");
          delay(1000);
          for (int positionCounter = 0; positionCounter < 7; positionCounter++) {
            // scroll one position left:
            lcd.scrollDisplayLeft();
            // wait a bit:
            delay(150);
          }
          while (fps.IsPressFinger() == false) delay(100);
          bret = fps.CaptureFinger(true);
          if (bret != false)
          {
            lcd.begin(16, 2);
            lcd.clear();
            lcd.println("Remove finger");
            fps.Enroll2();
            while (fps.IsPressFinger() == true) delay(100);
            lcd.begin(16, 2);
            lcd.clear();
            lcd.println("Press same finger yet again");
            delay(1000);
            for (int positionCounter = 0; positionCounter < 11; positionCounter++) {
              // scroll one position left:
              lcd.scrollDisplayLeft();
              // wait a bit:
              delay(150);
            }
            while (fps.IsPressFinger() == false) delay(100);
            bret = fps.CaptureFinger(true);
            if (bret != false)
            {
              lcd.begin(16, 2);
              lcd.clear();
              lcd.println("Remove finger");
              iret = fps.Enroll3();
              if (iret == 0)
              {
                lcd.begin(16, 2);
                lcd.clear();
                lcd.println("Enrolling Successfull");
                delay(1000);
                for (int positionCounter = 0; positionCounter < 5; positionCounter++) {
                  // scroll one position left:
                  lcd.scrollDisplayLeft();
                  // wait a bit:
                  delay(150);
                }
                delay(1000);
                State_0 = false;
                State_1 = false;
                State_2 = false;
                State_3 = false;
              }
              else
              {
                lcd.begin(16, 2);
                lcd.clear();
                lcd.print("Enrolling Failed with error code:");
                lcd.println(iret);
                delay(1000);
                for (int positionCounter = 0; positionCounter < 21; positionCounter++) {
                  // scroll one position left:
                  lcd.scrollDisplayLeft();
                  // wait a bit:
                  delay(150);
                }
                delay(1000);
              }
            }
          }
        }
      }
    }
  }
}






