
#define trigPin1 2
#define echoPin1 3
#define trigPin2 4
#define echoPin2 5
#define trigPin3 6
#define echoPin3 7

int dir1PinA = 12;
int dir2PinA = 11;
int speedPinA = 13; // Needs to be a PWM pin to be able to control motor speed

// Motor 2
int dir1PinB = 10;
int dir2PinB = 9;
int speedPinB = 8; // Needs to be a PWM pin to be able to control motor speed



long duration, distance, RightSensor, BackSensor, FrontSensor, LeftSensor;

void setup()
{
  Serial.begin (9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);



  pinMode(dir1PinA, OUTPUT);
  pinMode(dir2PinA, OUTPUT);
  pinMode(speedPinA, OUTPUT);
  pinMode(dir1PinB, OUTPUT);
  pinMode(dir2PinB, OUTPUT);
  pinMode(speedPinB, OUTPUT);
}

void loop() {


  SonarSensor(trigPin1, echoPin1);
  RightSensor = distance;
  SonarSensor(trigPin2, echoPin2);
  LeftSensor = distance;
  SonarSensor(trigPin3, echoPin3);
  FrontSensor = distance;

  Serial.print(LeftSensor);
  Serial.print(" - ");
  Serial.print(FrontSensor);
  Serial.print(" - ");
  Serial.println(RightSensor);

  if (FrontSensor > 30) {
    // 30 represents units from wall
    //left motor forward
    analogWrite(speedPinA, 155);//Sets speed variable via PWM
    digitalWrite(dir1PinA, LOW);
    digitalWrite(dir2PinA, HIGH);

    //right motor forward
    analogWrite(speedPinB, 155);
    digitalWrite(dir1PinB, LOW);
    digitalWrite(dir2PinB, HIGH);
  }
  else if (FrontSensor <= 60) {
    analogWrite(speedPinA, 0);//Sets speed variable via PWM
    digitalWrite(dir1PinA, LOW);
    digitalWrite(dir2PinA, HIGH);

    analogWrite(speedPinB, 0);
    digitalWrite(dir1PinB, HIGH);
    digitalWrite(dir2PinB, LOW);
    if (LeftSensor > RightSensor) {
      //only one motor runs here to turn
      analogWrite(speedPinA, 155);//Sets speed variable via PWM
      digitalWrite(dir1PinA, LOW);
      digitalWrite(dir2PinA, HIGH);

      if (FrontSensor > LeftSensor) {
        analogWrite(speedPinA, 0);//Sets speed variable via PWM
        digitalWrite(dir1PinA, LOW);
        digitalWrite(dir2PinA, HIGH);

        analogWrite(speedPinB, 0);
        digitalWrite(dir1PinB, HIGH);
        digitalWrite(dir2PinB, LOW);
      }
    }
    else {      
      analogWrite(speedPinB, 155);//Sets speed variable via PWM
      digitalWrite(dir1PinB, HIGH);
      digitalWrite(dir2PinB, LOW);      
      if (FrontSensor > RightSensor) {
        analogWrite(speedPinA, 0);//Sets speed variable via PWM
        digitalWrite(dir1PinA, LOW);
        digitalWrite(dir2PinA, HIGH);

        analogWrite(speedPinB, 0);
        digitalWrite(dir1PinB, LOW);
        digitalWrite(dir2PinB, HIGH);
      }
    }
  }
}
void SonarSensor(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2000);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10000);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;

}
