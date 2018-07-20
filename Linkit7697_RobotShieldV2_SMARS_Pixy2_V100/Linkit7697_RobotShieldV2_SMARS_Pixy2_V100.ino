// 最後編輯 2018-7-20 by ShinWei Chiou

// For Linkit-7697 + Robot Shield V2.1
// Left Motor : P10 P12
// Right Motor : P13 P17
// Buzzer : P14
// https://docs.labs.mediatek.com/resource/linkit7697-arduino/en/kits-and-shields/robot-shield

// Pixy2 module.
// SCL : P8
// SDA : P9
// https://github.com/charmedlabs/pixy2/raw/master/releases/arduino/arduino_pixy2-1.0.2.zip

// Pixy module.
// http://www.cmucam.org/attachments/download/1157/arduino_pixy-0.1.7.zip

// DFPlayer mini mp3 module.
// TX : P4
// https://github.com/DFRobot/DFPlayer-Mini-mp3


#include <Wire.h>
#include <LRemote.h>
#include <SoftwareSerial.h>

#include <PixyI2C.h>

#define Right_Wheel_A   12
#define Right_Wheel_B   10
#define Left_Wheel_A    17
#define Left_Wheel_B    13
#define Buzzer_Pin      14

// DFplayer
#include <DFPlayer_Mini_Mp3.h>

// On LinkIt 7697, the RX pin must be one of the EINT pins.
// There are no limitations on TX pin.
SoftwareSerial mp3Serial(2, 4); // RX, TX

// Button
LRemoteLabel label;
LRemoteButton forwardbutton;
LRemoteButton backwardbutton;
LRemoteButton turnleftbutton;
LRemoteButton turnrightbutton;
LRemoteButton buzzerbutton;
LRemoteSwitch pixyswitchButton;
LRemoteButton musicbutton1;
LRemoteButton musicbutton2;
LRemoteButton musicbutton3;
LRemoteButton volmaxbutton;
LRemoteButton volminbutton;
LRemoteButton mutebutton;

// Music
const int VolumeMax = 30;
const int VolumeMin = 5;
int Volume_Value = 15;

// Pixy CMUcam5 object
PixyI2C pixy;
int Pixy_OnOff = 0;
int signature = 0;
int xblocks = 0;
int yblocks = 0;
unsigned int width = 0;
unsigned int height = 0;
unsigned int area = 0;
unsigned int newarea = 0;
int Xmin = 70;  //left
int Xmax = 200; //right
int maxArea = 0;
int minArea = 0;
static int isignature = 0;


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

void Pixy_scan()
{
  uint16_t blocks;
  blocks = pixy.getBlocks();
  signature = pixy.blocks[isignature].signature;
  xblocks = pixy.blocks[isignature].x;
  width = pixy.blocks[isignature].width;
  height = pixy.blocks[isignature].height;
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

  // Buzzer PIN Set
  pinMode(Buzzer_Pin, OUTPUT);

  // Initialize Pixy2
  pixy.init();

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
  tank_name = "PIXY" + tank_address.substring(0, 2) + tank_address.substring(3, 5);

  // Setup the Remote Control's Name
  LRemote.setName(tank_name);

  // Setup the Remote Control's UI canvas
  LRemote.setOrientation(RC_PORTRAIT);
  LRemote.setGrid(3, 6);

  // Add a Forward button
  forwardbutton.setText("Forward");
  forwardbutton.setPos(1, 0);
  forwardbutton.setSize(1, 1);
  forwardbutton.setColor(RC_BLUE);
  LRemote.addControl(forwardbutton);

  // Add a Backward button
  backwardbutton.setText("Backward");
  backwardbutton.setPos(1, 2);
  backwardbutton.setSize(1, 1);
  backwardbutton.setColor(RC_BLUE);
  LRemote.addControl(backwardbutton);

  // Add a TurnLeft button
  turnleftbutton.setText("TurnLeft");
  turnleftbutton.setPos(0, 1);
  turnleftbutton.setSize(1, 1);
  turnleftbutton.setColor(RC_BLUE);
  LRemote.addControl(turnleftbutton);

  // Add a TurnRight button
  turnrightbutton.setText("TurnRight");
  turnrightbutton.setPos(2, 1);
  turnrightbutton.setSize(1, 1);
  turnrightbutton.setColor(RC_BLUE);
  LRemote.addControl(turnrightbutton);

  // Add a Buzzer button
  buzzerbutton.setText("Buzzer");
  buzzerbutton.setPos(1, 1);
  buzzerbutton.setSize(1, 1);
  buzzerbutton.setColor(RC_YELLOW);
  LRemote.addControl(buzzerbutton);

  // Add an Pixy on/off switch
  pixyswitchButton.setText("Pixy");
  pixyswitchButton.setPos(1, 3);
  pixyswitchButton.setSize(1, 1);
  pixyswitchButton.setColor(RC_PINK);
  LRemote.addControl(pixyswitchButton);

  // Add a Music button
  musicbutton1.setText("♫1");
  musicbutton1.setPos(0, 4);
  musicbutton1.setSize(1, 1);
  musicbutton1.setColor(RC_GREY);
  LRemote.addControl(musicbutton1);

  musicbutton2.setText("♫2");
  musicbutton2.setPos(1, 4);
  musicbutton2.setSize(1, 1);
  musicbutton2.setColor(RC_GREY);
  LRemote.addControl(musicbutton2);

  musicbutton3.setText("♫3");
  musicbutton3.setPos(2, 4);
  musicbutton3.setSize(1, 1);
  musicbutton3.setColor(RC_GREY);
  LRemote.addControl(musicbutton3);

  volmaxbutton.setText("VOL+");
  volmaxbutton.setPos(0, 5);
  volmaxbutton.setSize(1, 1);
  volmaxbutton.setColor(RC_GREY);
  LRemote.addControl(volmaxbutton);

  volminbutton.setText("VOL-");
  volminbutton.setPos(1, 5);
  volminbutton.setSize(1, 1);
  volminbutton.setColor(RC_GREY);
  LRemote.addControl(volminbutton);

  mutebutton.setText("MUTE");
  mutebutton.setPos(2, 5);
  mutebutton.setSize(1, 1);
  mutebutton.setColor(RC_GREY);
  LRemote.addControl(mutebutton);

  // Start broadcasting our remote contoller
  LRemote.begin();
  Serial.println("LRemote begin ...");
}


/*------------------------------------------------------------*/
void loop() {

  // Process the incoming BLE write request
  LRemote.process();


  // Motor
  if (forwardbutton.isValueChanged()) {
    if (forwardbutton.getValue() == 1) {
      Move_Forward();
    } else {
      Motor_Break();
    }
  }

  if (backwardbutton.isValueChanged()) {
    if (backwardbutton.getValue() == 1) {
      Move_Backward();
    } else {
      Motor_Break();
    }
  }

  if (turnleftbutton.isValueChanged()) {
    if (turnleftbutton.getValue() == 1) {
      Move_TurnLeft();
    } else {
      Motor_Break();
    }
  }

  if (turnrightbutton.isValueChanged()) {
    if (turnrightbutton.getValue() == 1) {
      Move_TurnRight();
    } else {
      Motor_Break();
    }
  }


  // Buzzer
  if (buzzerbutton.isValueChanged()) {
    if (buzzerbutton.getValue() == 1) {
      tone(Buzzer_Pin, 523, 500);
      delay(100);
    } else {
      noTone(Buzzer_Pin);
    }
  }


  /*----------------------------------*/
  // Mute Music
  if (mutebutton.isValueChanged())
  {
    if (mutebutton.getValue() == 1)
    {
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

        mp3_set_volume (Volume_Value);
        delay(10);
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

        mp3_set_volume (Volume_Value);
        delay(10);
      }
    }
  }

  /*----------------------------------*/
  // Play Music 1
  if (musicbutton1.isValueChanged())
  {
    if (musicbutton1.getValue() == 1)
    {
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
      mp3_set_volume (Volume_Value);
      delay(10);
      mp3_play (12);
    }
  }

  // Pixy ON/OFF
  if (pixyswitchButton.isValueChanged())
  {
    Pixy_OnOff = pixyswitchButton.getValue();
    Motor_Break();
  }

  while (millis() < 5000)
  {
    Pixy_scan();
    area = width * height;
    maxArea = area + 1000;
    minArea = area - 1000;
  }

  // Pixy Auto Demo
  if (Pixy_OnOff == 1)
  {
    Pixy_scan();

    if (signature == 1)
    {
      newarea = width * height;

      if (xblocks < Xmin)
      {
        Move_TurnLeft();
      }
      else if (xblocks > Xmax)
      {
        Move_TurnRight();
      }
      else if (newarea < minArea)
      {
        Move_Forward();
      }
      else if (newarea > maxArea)
      {
        Move_Backward();
      }

      else
      {
        Motor_Break();
      }
    }
    else
    {
      Motor_Break();
    }
  }


}
