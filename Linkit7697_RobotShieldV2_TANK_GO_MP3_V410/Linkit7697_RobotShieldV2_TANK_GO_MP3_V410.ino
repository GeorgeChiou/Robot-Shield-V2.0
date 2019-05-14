// 最後編輯 2019-5-08 by ShinWei Chiou
// 移除 搖桿 提供比賽
// 中文化 提供比賽

// 最後編輯 2019-5-06 by ShinWei Chiou
// 移除 修復按鈕 提供比賽

// 最後編輯 2018-4-18 by ShinWei Chiou
// 搖桿控制 LRemoteJoyStick

// 最後編輯 2018-4-15 by ShinWei Chiou
// 橫向控制 Change to Use RC_LANDSCAPE

// 最後編輯 2018-4-02 by Mason
// Add tank_name = "TANK"+ tank_address.substring(0,2) +tank_address.substring(3,5);

// 最後編輯 2018-3-27 by ShinWei Chiou

// DFPlayer mini mp3 module.
// github as default source provider
// https://github.com/DFRobot/DFPlayer-Mini-mp3

// For Linkit-7697 + Robot Shield V2.1
// Left Motor : P10 P12，Right Motor : P13 P17
// Servo : P5
// Buzzer : P14
// IR Test Button : P6
// IR Receiver : P9
// IR LED : P11 , P15
// DFplayer TX : P4


#include <Servo.h>
#include <LRemote.h>
#include <SoftwareSerial.h>

#define Right_Wheel_A   12
#define Right_Wheel_B   10
#define Left_Wheel_A    17
#define Left_Wheel_B    13

#define Buzzer_Pin      14

#define Servo_Pin       5

#define IR_LED_Pin1     11
#define IR_LED_Pin2     15
#define IR_Button_Pin   6
#define IR_Receiver_Pin 9

// DFplayer
#include <DFPlayer_Mini_Mp3.h>

// On LinkIt 7697, the RX pin must be one of the EINT pins.
// There are no limitations on TX pin.
SoftwareSerial mp3Serial(2, 4); // RX, TX

// Label
LRemoteLabel Titlelabel;
LRemoteLabel HPlabelT;
LRemoteLabel HPlabelS;
LRemoteLabel AMlabelT;
LRemoteLabel AMlabelS;

// JoyStick
LRemoteJoyStick joystickDirection;

// Button
LRemoteButton forwardbutton;
LRemoteButton backwardbutton;
LRemoteButton turnleftbutton;
LRemoteButton turnrightbutton;
LRemoteButton firebutton;
LRemoteButton reloadbutton;
LRemoteButton repairsbutton;
LRemoteSlider turretslider;
LRemoteButton turretcenterbutton;
LRemoteButton musicbutton1;
LRemoteButton musicbutton2;
LRemoteButton musicbutton3;
LRemoteButton musicbutton4;
LRemoteButton musicbutton5;
LRemoteButton musicbutton6;
LRemoteButton musicbutton7;
LRemoteButton musicbutton8;
LRemoteButton volmaxbutton;
LRemoteButton volminbutton;
LRemoteButton mutebutton;

// Servo
const int TurretTurnCenter = 90;
const int TurretTurnMax = 160;
const int TurretTurnMin = 10;
int Turret_Turn_Value = TurretTurnCenter;

// IR Battle System
const int Damage_Value = 25;
const int Tank_AM_MaxValue = 5;
int Tank_HP_Value = 100;
int Tank_AM_Value = Tank_AM_MaxValue;
int IR_Value = 0;

// Music
const int VolumeMax = 30;
const int VolumeMin = 5;
int Volume_Value = 15;

// Create Servo object
Servo TurretServo;


/*------------------------------------------------------------*/
// IR Send Code 38kHz
void IR_Send_Code_1()
{
  for (int i16t = 0; i16t < 16; i16t++)
  {
    for (int i37k = 0; i37k < 125; i37k++)
    {
      digitalWrite(IR_LED_Pin1, HIGH);
      delayMicroseconds(12);
      digitalWrite(IR_LED_Pin1, LOW);
      delayMicroseconds(12);
    }
    delay(2);
  }
}

void IR_Send_Code_2()
{
  for (int i16t = 0; i16t < 16; i16t++)
  {
    for (int i37k = 0; i37k < 125; i37k++)
    {
      digitalWrite(IR_LED_Pin2, HIGH);
      delayMicroseconds(12);
      digitalWrite(IR_LED_Pin2, LOW);
      delayMicroseconds(12);
    }
    delay(2);
  }
}


/*------------------------------------------------------------*/
// Ray Gun Sound
void Ray_Gun_Sound()
{
  for (int i = 0; i < 300; i++)
  {
    digitalWrite(Buzzer_Pin, HIGH);
    delayMicroseconds(i);
    digitalWrite(Buzzer_Pin, LOW);
    delayMicroseconds(i);
  }
}


/*------------------------------------------------------------*/
void Move_Forward()
{
  digitalWrite(Right_Wheel_A, LOW);
  digitalWrite(Right_Wheel_B, HIGH);
  digitalWrite(Left_Wheel_A, LOW);
  digitalWrite(Left_Wheel_B, HIGH);
}

void Move_Backward()
{
  digitalWrite(Right_Wheel_A, HIGH);
  digitalWrite(Right_Wheel_B, LOW);
  digitalWrite(Left_Wheel_A, HIGH);
  digitalWrite(Left_Wheel_B, LOW);
}

void Move_TurnRight()
{
  digitalWrite(Right_Wheel_A, LOW);
  digitalWrite(Right_Wheel_B, HIGH);
  digitalWrite(Left_Wheel_A, HIGH);
  digitalWrite(Left_Wheel_B, LOW);
}

void Move_TurnLeft()
{
  digitalWrite(Right_Wheel_A, HIGH);
  digitalWrite(Right_Wheel_B, LOW);
  digitalWrite(Left_Wheel_A, LOW);
  digitalWrite(Left_Wheel_B, HIGH);
}

void Motor_Break()
{
  digitalWrite(Right_Wheel_A, HIGH);
  digitalWrite(Right_Wheel_B, HIGH);
  digitalWrite(Left_Wheel_A, HIGH);
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(Right_Wheel_A, LOW);
  digitalWrite(Right_Wheel_B, LOW);
  digitalWrite(Left_Wheel_A, LOW);
  digitalWrite(Left_Wheel_B, LOW);
}

void Move_Fire()
{
  digitalWrite(Right_Wheel_A, HIGH);
  digitalWrite(Right_Wheel_B, LOW);
  digitalWrite(Left_Wheel_A, HIGH);
  digitalWrite(13, LOW);
  delay(50);
  digitalWrite(Right_Wheel_A, LOW);
  digitalWrite(Right_Wheel_B, HIGH);
  digitalWrite(Left_Wheel_A, LOW);
  digitalWrite(13, HIGH);
  delay(50);
  digitalWrite(Right_Wheel_A, HIGH);
  digitalWrite(Right_Wheel_B, HIGH);
  digitalWrite(Left_Wheel_A, HIGH);
  digitalWrite(Left_Wheel_B, HIGH);
}

void Move_Damage()
{
  digitalWrite(Right_Wheel_A, HIGH);
  digitalWrite(Right_Wheel_B, LOW);
  digitalWrite(Left_Wheel_A, HIGH);
  digitalWrite(Left_Wheel_B, LOW);
  delay(100);
  digitalWrite(Right_Wheel_A, LOW);
  digitalWrite(Right_Wheel_B, HIGH);
  digitalWrite(Left_Wheel_A, LOW);
  digitalWrite(Left_Wheel_B, HIGH);
  delay(100);
  digitalWrite(Right_Wheel_A, HIGH);
  digitalWrite(Right_Wheel_B, HIGH);
  digitalWrite(Left_Wheel_A, HIGH);
  digitalWrite(Left_Wheel_B, HIGH);
  delay(50);
  digitalWrite(Right_Wheel_A, LOW);
  digitalWrite(Right_Wheel_B, LOW);
  digitalWrite(Left_Wheel_A, LOW);
  digitalWrite(Left_Wheel_B, LOW);

  TurretServo.write(60);
  delay(300);
  TurretServo.write(120);
  delay(300);
  TurretServo.write(Turret_Turn_Value);
  delay(300);
}


/*------------------------------------------------------------*/
void SteerDirection(int x, int y) {

  const int x_termP = 50;
  const int x_termN = -50;
  const int y_termP = 10;
  const int y_termN = -10;

  if (y > y_termP )
  {
    if (x >= x_termP) {
      Move_TurnRight();
    }
    else if (x <= x_termN) {
      Move_TurnLeft();
    }
    else {
      Move_Forward();
    }
  }

  else if (y < y_termN)
  {
    if (x >= x_termP) {
      Move_TurnRight();
    }
    else if (x <= x_termN) {
      Move_TurnLeft();
    }
    else {
      Move_Backward();
    }
  }

  else
  {
    if (x >= x_termP) {
      Move_TurnRight();
    }
    else if (x <= x_termN) {
      Move_TurnLeft();
    }
    else {
      Motor_Break();
    }
  }

}


/*------------------------------------------------------------*/
void setup() {

  // Initialize serial communications at 9600 bps:
  Serial.begin(9600);

  // Set the data rate for the SoftwareSerial port
  mp3Serial.begin(9600);

  // Add for DFplayer
  mp3_set_serial (mp3Serial);  // set Serial for DFPlayer-mini mp3 module

  // Motor PIN Set
  pinMode(10, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(17, OUTPUT);

  // IR PIN Set
  pinMode(IR_Receiver_Pin, INPUT);
  pinMode(IR_Button_Pin, INPUT);
  pinMode(IR_LED_Pin1, OUTPUT);
  pinMode(IR_LED_Pin2, OUTPUT);

  // Initialize Servo
  TurretServo.attach(Servo_Pin);
  TurretServo.write(90);

  // Initialize BLE subsystem & get BLE address
  LBLE.begin();
  while (!LBLE.ready()) {
    delay(100);
  }

  Serial.print("Device Address = [");
  LBLEAddress ble_address;
  String tank_address;

  ble_address = LBLE.getDeviceAddress();
  tank_address = ble_address.toString();
  Serial.print(tank_address);
  Serial.println("]");

  String tank_name;
  tank_name = "TANK" + tank_address.substring(0, 2) + tank_address.substring(3, 5);

  // Setup the Remote Control's Name
  LRemote.setName(tank_name);

  // Setup the Remote Control's UI canvas
  LRemote.setOrientation(RC_LANDSCAPE);
  LRemote.setGrid(16, 9);

  // Add a text Title label
  Titlelabel.setText("Infrared Battle Tank V4.1");
  Titlelabel.setPos(2, 0);
  Titlelabel.setSize(14, 1);
  Titlelabel.setColor(RC_GREY);
  LRemote.addControl(Titlelabel);

  // Add a text HP label
  HPlabelT.setText("血量");
  HPlabelT.setPos(0, 1);
  HPlabelT.setSize(2, 1);
  HPlabelT.setColor(RC_GREY);
  LRemote.addControl(HPlabelT);

  HPlabelS.setText("100%");
  HPlabelS.setPos(0, 2);
  HPlabelS.setSize(2, 1);
  HPlabelS.setColor(RC_GREY);
  LRemote.addControl(HPlabelS);

  // Add a text AMMO label
  AMlabelT.setText("彈量");
  AMlabelT.setPos(5, 1);
  AMlabelT.setSize(2, 1);
  AMlabelT.setColor(RC_GREY);
  LRemote.addControl(AMlabelT);

  AMlabelS.setText(String(Tank_AM_Value, 10));
  AMlabelS.setPos(5, 2);
  AMlabelS.setSize(2, 1);
  AMlabelS.setColor(RC_GREY);
  LRemote.addControl(AMlabelS);

  // Add Joystick Direction
  //joystickDirection.setPos(0, 3);
  //joystickDirection.setSize(7, 6);
  //joystickDirection.setColor(RC_BLUE);
  //LRemote.addControl(joystickDirection);

  // Add a Forward button
  forwardbutton.setText("前進");
  forwardbutton.setPos(2, 3);
  forwardbutton.setSize(3, 3);
  forwardbutton.setColor(RC_BLUE);
  LRemote.addControl(forwardbutton);

  // Add a Backward button
  backwardbutton.setText("後退");
  backwardbutton.setPos(2, 6);
  backwardbutton.setSize(3, 3);
  backwardbutton.setColor(RC_BLUE);
  LRemote.addControl(backwardbutton);

  // Add a TurnLeft button
  turnleftbutton.setText("左轉");
  turnleftbutton.setPos(0, 4);
  turnleftbutton.setSize(2, 4);
  turnleftbutton.setColor(RC_BLUE);
  LRemote.addControl(turnleftbutton);

  // Add a TurnRight button
  turnrightbutton.setText("右轉");
  turnrightbutton.setPos(5, 4);
  turnrightbutton.setSize(2, 4);
  turnrightbutton.setColor(RC_BLUE);
  LRemote.addControl(turnrightbutton);

  // Add a Reload button
  reloadbutton.setText("塡彈");
  reloadbutton.setPos(13, 1);
  reloadbutton.setSize(3, 2);
  reloadbutton.setColor(RC_GREEN);
  LRemote.addControl(reloadbutton);

  // Add a Fire button
  firebutton.setText("發射");
  firebutton.setPos(13, 3);
  //firebutton.setSize(3, 4);
  firebutton.setSize(3, 6);
  firebutton.setColor(RC_PINK);
  LRemote.addControl(firebutton);

  // Add a Repairs button
  //repairsbutton.setText("REPAIRS");
  //repairsbutton.setPos(13, 7);
  //repairsbutton.setSize(3, 2);
  //repairsbutton.setColor(RC_GREEN);
  //LRemote.addControl(repairsbutton);

  // Add a Turret center button
  turretcenterbutton.setText("砲塔回正");
  turretcenterbutton.setPos(2, 1);
  turretcenterbutton.setSize(3, 2);
  turretcenterbutton.setColor(RC_ORANGE);
  LRemote.addControl(turretcenterbutton);

  // Add a Turret slider
  turretslider.setText("砲塔旋轉");
  turretslider.setPos(7, 6);
  turretslider.setSize(6, 3);
  turretslider.setColor(RC_ORANGE);
  turretslider.setValueRange(TurretTurnMin, TurretTurnMax, TurretTurnCenter);
  LRemote.addControl(turretslider);

  // Add a Music button
  musicbutton1.setText("♫1");
  musicbutton1.setPos(8, 1);
  musicbutton1.setSize(2, 1);
  musicbutton1.setColor(RC_GREY);
  LRemote.addControl(musicbutton1);

  musicbutton2.setText("♫2");
  musicbutton2.setPos(8, 2);
  musicbutton2.setSize(2, 1);
  musicbutton2.setColor(RC_GREY);
  LRemote.addControl(musicbutton2);

  musicbutton3.setText("♫3");
  musicbutton3.setPos(8, 3);
  musicbutton3.setSize(2, 1);
  musicbutton3.setColor(RC_GREY);
  LRemote.addControl(musicbutton3);

  musicbutton4.setText("♫4");
  musicbutton4.setPos(8, 4);
  musicbutton4.setSize(2, 1);
  musicbutton4.setColor(RC_GREY);
  LRemote.addControl(musicbutton4);

  musicbutton5.setText("♫5");
  musicbutton5.setPos(10, 1);
  musicbutton5.setSize(2, 1);
  musicbutton5.setColor(RC_GREY);
  LRemote.addControl(musicbutton5);

  musicbutton6.setText("♫6");
  musicbutton6.setPos(10, 2);
  musicbutton6.setSize(2, 1);
  musicbutton6.setColor(RC_GREY);
  LRemote.addControl(musicbutton6);

  musicbutton7.setText("♫7");
  musicbutton7.setPos(10, 3);
  musicbutton7.setSize(2, 1);
  musicbutton7.setColor(RC_GREY);
  LRemote.addControl(musicbutton7);

  musicbutton8.setText("♫8");
  musicbutton8.setPos(10, 4);
  musicbutton8.setSize(2, 1);
  musicbutton8.setColor(RC_GREY);
  LRemote.addControl(musicbutton8);

  volmaxbutton.setText("大聲");
  volmaxbutton.setPos(8, 5);
  volmaxbutton.setSize(2, 1);
  volmaxbutton.setColor(RC_GREY);
  LRemote.addControl(volmaxbutton);

  volminbutton.setText("小聲");
  volminbutton.setPos(10, 5);
  volminbutton.setSize(2, 1);
  volminbutton.setColor(RC_GREY);
  LRemote.addControl(volminbutton);

  mutebutton.setText("靜音");
  mutebutton.setPos(0, 0);
  mutebutton.setSize(2, 1);
  mutebutton.setColor(RC_GREY);
  LRemote.addControl(mutebutton);

  // Start broadcasting our remote contoller
  LRemote.begin();
  Serial.println("LRemote begin ...");
}


/*------------------------------------------------------------*/
void loop() {

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


  // Tank HP Limit
  if (Tank_HP_Value >= 1)
  {
    /*----------------------------------*/
    // Move
    //if (joystickDirection.isValueChanged())
    //{
    //  auto dir = joystickDirection.getValue();
    //
    //  Serial.println(dir);
    //
    //  SteerDirection(dir.x, dir.y);
    //}

    if (forwardbutton.getValue())
    {
      Move_Forward();
    }
    else if (backwardbutton.getValue())
    {
      Move_Backward();
    }
    else if (turnleftbutton.getValue())
    {
      Move_TurnLeft();
    }
    else if (turnrightbutton.getValue())
    {
      Move_TurnRight();
    }
    else
    {
      Motor_Break();
    }


    /*----------------------------------*/
    // Turret
    if (turretcenterbutton.isValueChanged())
    {
      if (turretcenterbutton.getValue() == 1)
      {
        Turret_Turn_Value = TurretTurnCenter;
        TurretServo.write(TurretTurnCenter);
        delay(100);
      }
    }

    if (turretslider.isValueChanged())
    {
      Titlelabel.updateText("Tank Go!");

      Turret_Turn_Value = 180 - turretslider.getValue();
      TurretServo.write(Turret_Turn_Value);
    }


    /*----------------------------------*/
    // Fire
    if (firebutton.isValueChanged())
    {
      if (firebutton.getValue() == 1)
      {
        if ( Tank_AM_Value >= 1)
        {
          mp3_set_volume (Volume_Value);
          delay(10);
          mp3_play (1);

          Titlelabel.updateText("!!! FIRE !!!");

          Tank_AM_Value--;

          IR_Send_Code_1();

          IR_Send_Code_2();

          Move_Fire();

          delay (700);
          mp3_stop ();
        }

        if ( Tank_AM_Value <= 0)
        {
          Titlelabel.updateText("!!! Reload !!!");
        }

        AMlabelS.updateText(String(Tank_AM_Value, 10));

        Serial.print("Tank_AT_Value = ");
        Serial.println(Tank_AM_Value);
      }
      else
      {
        Motor_Break();
      }
    }


    /*----------------------------------*/
    // Reload
    if (reloadbutton.isValueChanged())
    {
      if (reloadbutton.getValue() == 1)
      {
        if ( Tank_AM_Value < Tank_AM_MaxValue)
        {
          mp3_set_volume (Volume_Value);
          delay(10);
          mp3_play (3);

          Titlelabel.updateText("Please Wait ...");

          for (int iAM = Tank_AM_Value; iAM <= Tank_AM_MaxValue; iAM++)
          {
            Tank_AM_Value = iAM;

            AMlabelS.updateText(String(Tank_AM_Value, 10));

            delay(200);
          }

          Titlelabel.updateText("Completed !");

          Serial.println("Reload Tank_AT_Value");

          delay(100);
          mp3_stop ();
        }
        else
        {
          Titlelabel.updateText("Full Ammo !");
        }
      }
    }

  }

  /*----------------------------------*/
  // IR Test Button
  if (digitalRead(IR_Button_Pin) == HIGH)
  {
    IR_Send_Code_1();

    Ray_Gun_Sound();

    IR_Send_Code_2();

    Serial.println("IR Send Test Code");
  }


  /*----------------------------------*/
  // IR Battle System
  if (digitalRead(IR_Receiver_Pin) == LOW)
  {
    IR_Value = pulseIn(IR_Receiver_Pin, LOW);

    if ( IR_Value >= 2700 && IR_Value <= 3400 )
    {
      mp3_set_volume (Volume_Value);
      delay(10);
      mp3_play (2);
      delay (100);

      Titlelabel.updateText("Damage !");

      if ( Tank_HP_Value >= 1)
      {
        Tank_HP_Value = Tank_HP_Value - Damage_Value;
        Move_Damage();
      }

      if ( Tank_HP_Value <= Damage_Value)
      {
        Titlelabel.updateText("!!! Repairs !!!");
      }

      HPlabelS.updateText(String(Tank_HP_Value, 10) + "%");

      Serial.print("IR_Value = ");
      Serial.print(IR_Value);
      Serial.print(" , Tank_HP_Value = ");
      Serial.println(Tank_HP_Value);

      delay (100);
      mp3_stop ();
    }
  }


  /*----------------------------------*/
  // Repairs
  if (repairsbutton.isValueChanged())
  {
    if (repairsbutton.getValue() == 1)
    {
      if ( Tank_HP_Value < 100)
      {
        mp3_set_volume (Volume_Value);
        delay(10);
        mp3_play (4);

        Titlelabel.updateText("Please Wait ...");

        for (int iHP = Tank_HP_Value; iHP <= 100; iHP++)
        {
          Tank_HP_Value = iHP;

          HPlabelS.updateText(String(Tank_HP_Value, 10) + "%");

          delay(20);
        }

        Titlelabel.updateText("Completed !");

        Serial.println("Repair Tank_HP_Value");

        delay(100);
        mp3_stop ();
      }
      else
      {
        Titlelabel.updateText("No Damage !");
      }
    }
  }


  /*----------------------------------*/
  // Mute Music
  if (mutebutton.isValueChanged())
  {
    if (mutebutton.getValue() == 1)
    {
      Titlelabel.updateText("!!! Mute !!!");

      mp3_stop ();
    }
  }

  // Adjust Volume Music
  if (volmaxbutton.isValueChanged())
  {
    if (volmaxbutton.getValue() == 1)
    {
      if (Volume_Value < 30)
      {
        Volume_Value = Volume_Value + 5;

        Titlelabel.updateText("Volume = " + String(Volume_Value, 10));

        mp3_set_volume (Volume_Value);
        delay(10);
      }
      else
      {
        Titlelabel.updateText("Volume = Max");
      }
    }
  }

  if (volminbutton.isValueChanged())
  {
    if (volminbutton.getValue() == 1)
    {
      if (Volume_Value > 5)
      {
        Volume_Value = Volume_Value - 5;

        Titlelabel.updateText("Volume = " + String(Volume_Value, 10));

        mp3_set_volume (Volume_Value);
        delay(10);
      }
      else
      {
        Titlelabel.updateText("Volume = Min");
      }
    }
  }


  /*----------------------------------*/
  // Play Music 1
  if (musicbutton1.isValueChanged())
  {
    if (musicbutton1.getValue() == 1)
    {
      Titlelabel.updateText("Katyusha");

      mp3_set_volume (Volume_Value);
      delay(10);
      mp3_play (10);
    }
  }

  // Play Music 2
  if (musicbutton2.isValueChanged())
  {
    if (musicbutton2.getValue() == 1)
    {
      Titlelabel.updateText("Panzerlied");

      mp3_set_volume (Volume_Value);
      delay(10);
      mp3_play (11);
    }
  }

  // Play Music 3
  if (musicbutton3.isValueChanged())
  {
    if (musicbutton3.getValue() == 1)
    {
      Titlelabel.updateText("Marching");

      mp3_set_volume (Volume_Value);
      delay(10);
      mp3_play (12);
    }
  }

  // Play Music 4
  if (musicbutton4.isValueChanged())
  {
    if (musicbutton4.getValue() == 1)
    {
      Titlelabel.updateText("Engine Start !");

      mp3_set_volume (Volume_Value);
      delay(10);
      mp3_play (13);
    }
  }

  // Play Music 5
  if (musicbutton5.isValueChanged())
  {
    if (musicbutton5.getValue() == 1)
    {
      Titlelabel.updateText("No File !");

      mp3_set_volume (Volume_Value);
      delay(10);
      mp3_play (14);
    }
  }

  // Play Music 6
  if (musicbutton6.isValueChanged())
  {
    if (musicbutton6.getValue() == 1)
    {
      Titlelabel.updateText("No File !");

      mp3_set_volume (Volume_Value);
      delay(10);
      mp3_play (15);
    }
  }

  // Play Music 7
  if (musicbutton7.isValueChanged())
  {
    if (musicbutton7.getValue() == 1)
    {
      Titlelabel.updateText("No File !");

      mp3_set_volume (Volume_Value);
      delay(10);
      mp3_play (16);
    }
  }

  // Play Music 8
  if (musicbutton8.isValueChanged())
  {
    if (musicbutton8.getValue() == 1)
    {
      Titlelabel.updateText("No File !");

      mp3_set_volume (Volume_Value);
      delay(10);
      mp3_play (17);
    }
  }

}
