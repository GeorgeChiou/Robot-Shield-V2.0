// 最後編輯 2018-3-07 by ShinWei Chiou

#include <Servo.h>

const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;

const int buzzerPin = 14;  // buzzer Pin
const int button_Pin = 6;

int counter = 0;

Servo myservo2;  // create servo object to control a servo
Servo myservo3;  // create servo object to control a servo
Servo myservo4;  // create servo object to control a servo
Servo myservo5;  // create servo object to control a servo
Servo myservo7;  // create servo object to control a servo
Servo myservo8;  // create servo object to control a servo
Servo myservo9;  // create servo object to control a servo
Servo myservo11;  // create servo object to control a servo
Servo myservo15;  // create servo object to control a servo
Servo myservo16;  // create servo object to control a servo


void beep(int note, int duration)
{
  //Play tone on buzzerPin
  tone(buzzerPin, note, duration);

  //Play different LED depending on value of 'counter'
  if (counter % 2 == 0)
  {
    delay(duration);
  } else
  {
    delay(duration);
  }

  //Stop tone on buzzerPin
  noTone(buzzerPin);

  delay(50);

  //Increment counter
  counter++;
}

void firstSection()
{
  beep(a, 500);
  beep(a, 500);
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);

  delay(500);

  beep(eH, 500);
  beep(eH, 500);
  beep(eH, 500);
  beep(fH, 350);
  beep(cH, 150);
  beep(gS, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);

  delay(500);
}


void secondSection()
{
  beep(aH, 500);
  beep(a, 300);
  beep(a, 150);
  beep(aH, 500);
  beep(gSH, 325);
  beep(gH, 175);
  beep(fSH, 125);
  beep(fH, 125);
  beep(fSH, 250);

  delay(325);

  beep(aS, 250);
  beep(dSH, 500);
  beep(dH, 325);
  beep(cSH, 175);
  beep(cH, 125);
  beep(b, 125);
  beep(cH, 250);

  delay(350);
}


void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  delay(100);

  // Button Read
  pinMode(button_Pin, INPUT);

  // Analog Read
  pinMode(buzzerPin, OUTPUT);

  // Motor PIN Set
  pinMode(10, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(17, OUTPUT);

  // Servo PIN Set
  myservo2.attach(2);  // attaches the servo on pin 2 to the servo object
  myservo3.attach(3);  // attaches the servo on pin 3 to the servo object
  myservo4.attach(4);  // attaches the servo on pin 4 to the servo object
  myservo5.attach(5);  // attaches the servo on pin 5 to the servo object
  myservo7.attach(7);  // attaches the servo on pin 7 to the servo object
  myservo8.attach(8);  // attaches the servo on pin 8 to the servo object
  myservo9.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo11.attach(11);  // attaches the servo on pin 10 to the servo object
  myservo15.attach(15);  // attaches the servo on pin 10 to the servo object
  myservo16.attach(16);  // attaches the servo on pin 10 to the servo object
}


void loop()
{
    //Play tone on buzzerPin
  tone(buzzerPin, 261, 125);
  delay(500);

  //Stop tone on buzzerPin
  noTone(buzzerPin);
  delay(500);


  // Servo Test 60
  Serial.println("PIN 2 Servo Test = 60");
  myservo2.write(60);
  Serial.println("PIN 3 Servo Test = 60");
  myservo3.write(60);
  Serial.println("PIN 4 Servo Test = 60");
  myservo4.write(60);
  Serial.println("PIN 5 Servo Test = 60");
  myservo5.write(60);
  Serial.println("PIN 7 Servo Test = 60");
  myservo7.write(60);
  Serial.println("PIN 8 Servo Test = 60");
  myservo8.write(60);
  Serial.println("PIN 9 Servo Test = 60");
  myservo9.write(60);
  Serial.println("PIN 11 Servo Test = 60");
  myservo11.write(60);
  Serial.println("PIN 15 Servo Test = 60");
  myservo15.write(60);
  Serial.println("PIN 16 Servo Test = 60");
  myservo16.write(60);
  delay(500);                       // wait for a second

  // Servo Test 120
  Serial.println("PIN 2 Servo Test = 120");
  myservo2.write(120);
  Serial.println("PIN 3 Servo Test = 120");
  myservo3.write(120);
  Serial.println("PIN 4 Servo Test = 120");
  myservo4.write(120);
  Serial.println("PIN 5 Servo Test = 120");
  myservo5.write(120);
  Serial.println("PIN 7 Servo Test = 120");
  myservo7.write(120);
  Serial.println("PIN 8 Servo Test = 120");
  myservo8.write(120);
  Serial.println("PIN 9 Servo Test = 120");
  myservo9.write(120);
  Serial.println("PIN 11 Servo Test = 120");
  myservo11.write(120);
  Serial.println("PIN 15 Servo Test = 120");
  myservo15.write(120);
  Serial.println("PIN 16 Servo Test = 120");
  myservo16.write(120);
  delay(500);                       // wait for a second

  // Servo Test 90
  Serial.println("PIN 2 Servo Test = 90");
  myservo2.write(90);
  Serial.println("PIN 3 Servo Test = 90");
  myservo3.write(90);
  Serial.println("PIN 4 Servo Test = 90");
  myservo4.write(90);
  Serial.println("PIN 5 Servo Test = 90");
  myservo5.write(90);
  Serial.println("PIN 7 Servo Test = 90");
  myservo7.write(90);
  Serial.println("PIN 8 Servo Test = 90");
  myservo8.write(90);
  Serial.println("PIN 9 Servo Test = 90");
  myservo9.write(90);
  Serial.println("PIN 11 Servo Test = 90");
  myservo11.write(90);
  Serial.println("PIN 15 Servo Test = 90");
  myservo15.write(90);
  Serial.println("PIN 16 Servo Test = 90");
  myservo16.write(90);
  delay(500);                       // wait for a second


  // Motor Test
  Serial.println("Motor Test : P11 Lo , P12 Hi , P13 Lo , P17 Hi");
  digitalWrite(10, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);
  digitalWrite(17, HIGH);
  delay(500);                       // wait for a second

  Serial.println("Motor Test : P11 Hi , P12 Lo , P13 Hi , P17 Lo");
  digitalWrite(12, LOW);
  digitalWrite(10, HIGH);
  digitalWrite(17, LOW);
  digitalWrite(13, HIGH);
  delay(500);                       // wait for a second

  Serial.println("Motor Test : STOP");
  digitalWrite(12, LOW);
  digitalWrite(10, LOW);
  digitalWrite(17, LOW);
  digitalWrite(13, LOW);


  //Play tone on buzzerPin
  if (digitalRead(button_Pin) == HIGH)
  {
    Serial.println("Play firstSection on buzzerPin");
    firstSection();

    //Serial.println("Play secondSection on buzzerPin");
    //secondSection();
  }
}

