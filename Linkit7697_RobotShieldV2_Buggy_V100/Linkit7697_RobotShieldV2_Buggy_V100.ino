// 最後編輯 2018-11-30 by ShinWei Chiou
// 初版

// Jason Workshop
// Website: http://jasonworkshop.com
// FB page: http://fb.com/jasonworkshop

// For Linkit-7697 + Robot Shield V2.1
// Steering Servo : P15
// MotorA : P10 , P12
// Buzzer : P14

#include <Servo.h>
#include <EEPROM.h>
#include <LRemote.h>

// Label
LRemoteLabel TitleLabel;

// Button
LRemoteButton BuzzerButton;

// JoyStick
LRemoteJoyStick Joystick_M;
LRemoteJoyStick Joystick_S;

// Slider
LRemoteSlider Servo_Trim_Slider;

// create Servo object to control a servo
#define Servo_Pin  15
const int TurnCenter = 90;
const int TurnMax = 150;
const int TurnMin = 30;
int Running_Servo;
int Running_Servo_Trim;
int Running_Servo_Center;
Servo SteeringServo;

// Motor
#define MotorA_P   12
#define MotorA_N   10

// Buzzer
#define BuzzerPin  14
int Tone_counter = 0;
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


/*------------------------------------------------------------*/
// Beep Sound
void Beep(int note, int duration)
{
  // Play tone on buzzerPin
  tone(BuzzerPin, note, duration);

  // Play different depending on value of 'Tone_counter'
  if (Tone_counter % 2 == 0)
  {
    delay(duration);
  }
  else
  {
    delay(duration);
  }

  // Stop tone on buzzerPin
  noTone(BuzzerPin);

  // Buzzer
  pinMode(BuzzerPin, OUTPUT);

  delay(50);
}


/*------------------------------------------------------------*/
void MotorDirection(int x, int y)
{
  const int y_termP = 10;
  const int y_termN = -10;

  // Forward
  if (y > y_termP )
  {
    analogWrite(MotorA_P, 0);
    analogWrite(MotorA_N, map(y, 10, 100, 50, 255));
  }

  // Backward
  else if (y < y_termN)
  {
    analogWrite(MotorA_P, map(y * -1, 10, 100, 50, 255));
    analogWrite(MotorA_N, 0);
  }

  // Break
  else
  {
    analogWrite(MotorA_P, 255);
    analogWrite(MotorA_N, 255);
    delay(100);
    digitalWrite(MotorA_P, LOW);
    digitalWrite(MotorA_N, LOW);
  }
}


/*------------------------------------------------------------*/
void SteerDirection(int x, int y)
{
  const int x_termP = 30;
  const int x_termN = -30;
  int icab = 0;

  // Turn Right
  if (x > x_termP )
  {
    icab = map(x, 30, 100, Running_Servo_Center, TurnMin);

    SteeringServo.write(icab);

    //Serial.print("Right icab = ");
    //Serial.println(icab);
  }

  // Turn Left
  else if (x < x_termN)
  {
    icab = map(x * -1, 30, 100, Running_Servo_Center, TurnMax);

    SteeringServo.write(icab);

    //Serial.print("Left icab = ");
    //Serial.println(icab);
  }

  // Center
  else
  {
    SteeringServo.write(Running_Servo_Center);
  }
}


/*------------------------------------------------------------*/
void setup()
{
  // Initialize serial communications at 9600 bps:
  Serial.begin(9600);

  // Buzzer
  pinMode(BuzzerPin, OUTPUT);

  // Motor PIN Set
  pinMode(MotorA_P, OUTPUT);
  pinMode(MotorA_N, OUTPUT);

  // Read EEPROM Trim
  Running_Servo_Trim = (int8_t)EEPROM.read(0);
  if (Running_Servo_Trim > 180 || Running_Servo_Trim < 0)
  {
    Running_Servo_Trim = 90;

    EEPROM.write(0, Running_Servo_Trim);
    delay(10);
  }

  // Servo PIN Set
  SteeringServo.attach(Servo_Pin);
  delay(100);

  // Initialize Servo
  Running_Servo_Center = Running_Servo_Trim;
  SteeringServo.write(Running_Servo_Center);


  // Initialize BLE subsystem & get BLE address
  LBLE.begin();
  while (!LBLE.ready()) {
    delay(100);
  }

  Serial.print("Device Address = [");
  LBLEAddress ble_address;
  String buggy_address;

  ble_address = LBLE.getDeviceAddress();
  buggy_address = ble_address.toString();
  Serial.print(buggy_address);
  Serial.println("]");

  String buggy_name;
  buggy_name = "CAR" + buggy_address.substring(0, 2) + buggy_address.substring(3, 5);

  // Setup the Remote Control's Name
  LRemote.setName(buggy_name);

  // Setup the Remote Control's UI canvas
  LRemote.setOrientation(RC_LANDSCAPE);
  LRemote.setGrid(16, 9);

  // Add a text Title Label
  TitleLabel.setText("Linkit-7697 RobotShield Buggy V1.0");
  TitleLabel.setPos(0, 0);
  TitleLabel.setSize(16, 1);
  TitleLabel.setColor(RC_GREY);
  LRemote.addControl(TitleLabel);

  // Add a Buzzer button
  BuzzerButton.setText("((BEEP))");
  BuzzerButton.setPos(7, 1);
  BuzzerButton.setSize(2, 2);
  BuzzerButton.setColor(RC_PINK);
  LRemote.addControl(BuzzerButton);

  // Add Joystick Direction
  Joystick_M.setPos(0, 1);
  Joystick_M.setSize(7, 7);
  Joystick_M.setColor(RC_BLUE);
  LRemote.addControl(Joystick_M);

  // Add Joystick Direction
  Joystick_S.setPos(9, 1);
  Joystick_S.setSize(7, 7);
  Joystick_S.setColor(RC_ORANGE);
  LRemote.addControl(Joystick_S);

  // Add a Trim Slider
  //Servo_Trim_Slider.setText("Servo Trim Slider");
  Servo_Trim_Slider.setPos(6, 7);
  Servo_Trim_Slider.setSize(4, 2);
  Servo_Trim_Slider.setColor(RC_ORANGE);
  Servo_Trim_Slider.setValueRange(TurnMin, TurnMax, Running_Servo_Center);
  LRemote.addControl(Servo_Trim_Slider);

  // Start broadcasting our remote contoller
  LRemote.begin();
  Serial.println("LRemote begin ...");
}


/*------------------------------------------------------------*/
void loop()
{
  // BLE central device, e.g. an mobile app
  if (!LRemote.connected())
  {
    Serial.println("Waiting for connection ...");
    delay(10);
  }
  else
  {
    delay(10);
  }

  // Process the incoming BLE write request
  LRemote.process();


  /*----------------------------------*/
  // Motor
  if (Joystick_M.isValueChanged())
  {
    auto dir = Joystick_M.getValue();

    //Serial.print("Joystick_M ");
    //Serial.println(dir);

    MotorDirection(dir.x, dir.y);
  }

  // Steer
  if (Joystick_S.isValueChanged())
  {
    auto dir = Joystick_S.getValue();

    //Serial.print("Joystick_S ");
    //Serial.println(dir);

    SteerDirection(dir.x, dir.y);
  }


  /*----------------------------------*/
  // Trim Slider
  if (Servo_Trim_Slider.isValueChanged())
  {
    Running_Servo_Trim = Servo_Trim_Slider.getValue();

    Running_Servo_Center = Running_Servo_Trim;

    EEPROM.write(0, Running_Servo_Center);
    delay(10);

    SteeringServo.write(Running_Servo_Center);

    //Serial.print("Running_Servo_Center ");
    //Serial.println(Running_Servo_Center);
  }


  /*----------------------------------*/
  // Buzzer Button
  if (BuzzerButton.isValueChanged())
  {
    if (BuzzerButton.getValue() == 1)
    {
      Beep(fH, 150);
    }
  }

}
