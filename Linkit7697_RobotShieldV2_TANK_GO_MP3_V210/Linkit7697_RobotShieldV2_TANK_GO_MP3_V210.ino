// 最後編輯 2018-4-02 by Mason
// Add tank_name = "TANK"+ tank_address.substring(0,2) +tank_address.substring(3,5); 

// 最後編輯 2018-3-12 by ShinWei Chiou

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
#include <LBLE.h>

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
LRemoteButton musicbutton9;
LRemoteButton volmaxbutton;
LRemoteButton volminbutton;
LRemoteButton mutebutton;

// Buzzer
const int BuzzerPin = 14;

// Servo
const int ServoPin = 5;
const int TurretTurnCenter = 90;
const int TurretTurnMax = 160;
const int TurretTurnMin = 10;
int Turret_Turn_Value = TurretTurnCenter;

// IR Battle System
const int IR_Button_Pin = 6;
const int IR_Receiver_Pin = 9;
const int IR_LED_Pin1 = 11;
const int IR_LED_Pin2 = 15;
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
    digitalWrite(BuzzerPin, HIGH);
    delayMicroseconds(i);
    digitalWrite(BuzzerPin, LOW);
    delayMicroseconds(i);
  }
}


/*------------------------------------------------------------*/
void Move_Forward()
{
  digitalWrite(12, LOW);
  digitalWrite(10, HIGH);
  digitalWrite(17, LOW);
  digitalWrite(13, HIGH);
}

void Move_Backward()
{
  digitalWrite(12, HIGH);
  digitalWrite(10, LOW);
  digitalWrite(17, HIGH);
  digitalWrite(13, LOW);
}

void Move_TurnRight()
{
  digitalWrite(12, LOW);
  digitalWrite(10, HIGH);
  digitalWrite(17, HIGH);
  digitalWrite(13, LOW);
}

void Move_TurnLeft()
{
  digitalWrite(12, HIGH);
  digitalWrite(10, LOW);
  digitalWrite(17, LOW);
  digitalWrite(13, HIGH);
}

void Motor_Break()
{
  digitalWrite(12, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(17, HIGH);
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(12, LOW);
  digitalWrite(10, LOW);
  digitalWrite(17, LOW);
  digitalWrite(13, LOW);
}

void Move_Fire()
{
  digitalWrite(12, HIGH);
  digitalWrite(10, LOW);
  digitalWrite(17, HIGH);
  digitalWrite(13, LOW);
  delay(50);
  digitalWrite(12, LOW);
  digitalWrite(10, HIGH);
  digitalWrite(17, LOW);
  digitalWrite(13, HIGH);
  delay(50);
  digitalWrite(12, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(17, HIGH);
  digitalWrite(13, HIGH);
}

void Move_Damage()
{
  digitalWrite(12, HIGH);
  digitalWrite(10, LOW);
  digitalWrite(17, HIGH);
  digitalWrite(13, LOW);
  delay(100);
  digitalWrite(12, LOW);
  digitalWrite(10, HIGH);
  digitalWrite(17, LOW);
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(12, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(17, HIGH);
  digitalWrite(13, HIGH);
  delay(50);
  digitalWrite(12, LOW);
  digitalWrite(10, LOW);
  digitalWrite(17, LOW);
  digitalWrite(13, LOW);

  TurretServo.write(60);
  delay(300);
  TurretServo.write(120);
  delay(300);
  TurretServo.write(Turret_Turn_Value);
  delay(300);
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
  TurretServo.attach(ServoPin);
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
  tank_name = "TANK"+ tank_address.substring(0,2) +tank_address.substring(3,5); 

  // Setup the Remote Control's UI canvas
  LRemote.setName(tank_name);

  LRemote.setOrientation(RC_PORTRAIT);
  LRemote.setGrid(9, 16);

  // Add a text Title label
  Titlelabel.setText("Infrared Battle Tank V2.1");
  Titlelabel.setPos(0, 0);
  Titlelabel.setSize(9, 1);
  Titlelabel.setColor(RC_GREY);
  LRemote.addControl(Titlelabel);

  // Add a text HP label
  HPlabelT.setText("+HP");
  HPlabelT.setPos(0, 1);
  HPlabelT.setSize(2, 2);
  HPlabelT.setColor(RC_GREY);
  LRemote.addControl(HPlabelT);

  HPlabelS.setText("100%");
  HPlabelS.setPos(2, 1);
  HPlabelS.setSize(2, 2);
  HPlabelS.setColor(RC_GREY);
  LRemote.addControl(HPlabelS);

  // Add a text AMMO label
  AMlabelT.setText("AMMO");
  AMlabelT.setPos(5, 1);
  AMlabelT.setSize(2, 2);
  AMlabelT.setColor(RC_GREY);
  LRemote.addControl(AMlabelT);

  AMlabelS.setText(String(Tank_AM_Value, 10));
  AMlabelS.setPos(7, 1);
  AMlabelS.setSize(2, 2);
  AMlabelS.setColor(RC_GREY);
  LRemote.addControl(AMlabelS);

  // Add a Forward button
  forwardbutton.setText("↑");
  forwardbutton.setPos(2, 3);
  forwardbutton.setSize(2, 3);
  forwardbutton.setColor(RC_BLUE);
  LRemote.addControl(forwardbutton);

  // Add a Backward button
  backwardbutton.setText("↓");
  backwardbutton.setPos(2, 7);
  backwardbutton.setSize(2, 3);
  backwardbutton.setColor(RC_BLUE);
  LRemote.addControl(backwardbutton);

  // Add a TurnLeft button
  turnleftbutton.setText("←");
  turnleftbutton.setPos(0, 5);
  turnleftbutton.setSize(2, 3);
  turnleftbutton.setColor(RC_BLUE);
  LRemote.addControl(turnleftbutton);

  // Add a TurnRight button
  turnrightbutton.setText("→");
  turnrightbutton.setPos(4, 5);
  turnrightbutton.setSize(2, 3);
  turnrightbutton.setColor(RC_BLUE);
  LRemote.addControl(turnrightbutton);

  // Add a Reload button
  reloadbutton.setText("RELOAD");
  reloadbutton.setPos(6, 3);
  reloadbutton.setSize(3, 2);
  reloadbutton.setColor(RC_GREEN);
  LRemote.addControl(reloadbutton);

  // Add a Fire button
  firebutton.setText("FIRE");
  firebutton.setPos(6, 5);
  firebutton.setSize(3, 3);
  firebutton.setColor(RC_PINK);
  LRemote.addControl(firebutton);

  // Add a Repairs button
  repairsbutton.setText("REPAIRS");
  repairsbutton.setPos(6, 8);
  repairsbutton.setSize(3, 2);
  repairsbutton.setColor(RC_GREEN);
  LRemote.addControl(repairsbutton);

  // Add a Turret center button
  turretcenterbutton.setText("⌂");
  turretcenterbutton.setPos(2, 6);
  turretcenterbutton.setSize(2, 1);
  turretcenterbutton.setColor(RC_ORANGE);
  LRemote.addControl(turretcenterbutton);

  // Add a Turret slider
  turretslider.setText("Turret Slider");
  turretslider.setPos(0, 10);
  turretslider.setSize(9, 3);
  turretslider.setColor(RC_ORANGE);
  turretslider.setValueRange(TurretTurnMin, TurretTurnMax, TurretTurnCenter);
  LRemote.addControl(turretslider);

  // Add a Music button
  musicbutton1.setText("♫1");
  musicbutton1.setPos(0, 13);
  musicbutton1.setSize(2, 1);
  musicbutton1.setColor(RC_GREY);
  LRemote.addControl(musicbutton1);

  musicbutton2.setText("♫2");
  musicbutton2.setPos(2, 13);
  musicbutton2.setSize(2, 1);
  musicbutton2.setColor(RC_GREY);
  LRemote.addControl(musicbutton2);

  musicbutton3.setText("♫3");
  musicbutton3.setPos(4, 13);
  musicbutton3.setSize(2, 1);
  musicbutton3.setColor(RC_GREY);
  LRemote.addControl(musicbutton3);

  musicbutton4.setText("♫4");
  musicbutton4.setPos(0, 14);
  musicbutton4.setSize(2, 1);
  musicbutton4.setColor(RC_GREY);
  LRemote.addControl(musicbutton4);

  musicbutton5.setText("♫5");
  musicbutton5.setPos(2, 14);
  musicbutton5.setSize(2, 1);
  musicbutton5.setColor(RC_GREY);
  LRemote.addControl(musicbutton5);

  musicbutton6.setText("♫6");
  musicbutton6.setPos(4, 14);
  musicbutton6.setSize(2, 1);
  musicbutton6.setColor(RC_GREY);
  LRemote.addControl(musicbutton6);

  musicbutton7.setText("♫7");
  musicbutton7.setPos(0, 15);
  musicbutton7.setSize(2, 1);
  musicbutton7.setColor(RC_GREY);
  LRemote.addControl(musicbutton7);

  musicbutton8.setText("♫8");
  musicbutton8.setPos(2, 15);
  musicbutton8.setSize(2, 1);
  musicbutton8.setColor(RC_GREY);
  LRemote.addControl(musicbutton8);

  musicbutton9.setText("♫9");
  musicbutton9.setPos(4, 15);
  musicbutton9.setSize(2, 1);
  musicbutton9.setColor(RC_GREY);
  LRemote.addControl(musicbutton9);

  volmaxbutton.setText("VOL+");
  volmaxbutton.setPos(6, 13);
  volmaxbutton.setSize(3, 1);
  volmaxbutton.setColor(RC_GREY);
  LRemote.addControl(volmaxbutton);

  volminbutton.setText("VOL-");
  volminbutton.setPos(6, 14);
  volminbutton.setSize(3, 1);
  volminbutton.setColor(RC_GREY);
  LRemote.addControl(volminbutton);

  mutebutton.setText("MUTE");
  mutebutton.setPos(6, 15);
  mutebutton.setSize(3, 1);
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
    //delay(500);
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
    if (forwardbutton.isValueChanged())
    {
      Titlelabel.updateText("Tank Go!");

      if (forwardbutton.getValue() == 1) {
        Move_Forward();
      } else {
        Motor_Break();
      }
    }

    if (backwardbutton.isValueChanged())
    {
      Titlelabel.updateText("Tank Go!");

      if (backwardbutton.getValue() == 1) {
        Move_Backward();
      } else {
        Motor_Break();
      }
    }

    if (turnleftbutton.isValueChanged())
    {
      Titlelabel.updateText("Tank Go!");

      if (turnleftbutton.getValue() == 1) {
        Move_TurnLeft();
      } else {
        Motor_Break();
      }
    }

    if (turnrightbutton.isValueChanged())
    {
      Titlelabel.updateText("Tank Go!");

      if (turnrightbutton.getValue() == 1) {
        Move_TurnRight();
      } else {
        Motor_Break();
      }
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

      Titlelabel.updateText("Damage !");

      if ( Tank_HP_Value >= 1)
      {
        Tank_HP_Value = Tank_HP_Value - Damage_Value;
      }

      if ( Tank_HP_Value <= Damage_Value)
      {
        Titlelabel.updateText("!!! Repairs !!!");
      }

      HPlabelS.updateText(String(Tank_HP_Value, 10) + "%");

      delay (100);
      Move_Damage();

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

  // Play Music 9
  if (musicbutton9.isValueChanged())
  {
    if (musicbutton9.getValue() == 1)
    {
      Titlelabel.updateText("No File !");

      mp3_set_volume (Volume_Value);
      delay(10);
      mp3_play (18);
    }
  }

}


