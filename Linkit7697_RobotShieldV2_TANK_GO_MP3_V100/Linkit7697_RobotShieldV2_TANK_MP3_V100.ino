// 最後編輯 2018-2-14 by ShinWei Chiou

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
LRemoteButton musicbuttonA;
LRemoteButton musicbuttonB;
LRemoteButton musicbuttonC;
LRemoteButton musicbuttonD;
LRemoteButton volumebutton;
LRemoteButton mutebutton;

// Buzzer
const int BuzzerPin = 14;

// Servo
const int ServoPin = 5;
int iTurretSlider = 90;

// IR Battle System
const int IR_Button_Pin = 6;
const int IR_Receiver_Pin = 9;
const int IR_LED_Pin1 = 11;
const int IR_LED_Pin2 = 15;
const int Damage_Value = 25;
int IR_Value = 0;
int Tank_HP_Value = 100;
int Tank_AM_Value = 5;

// Music
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
  TurretServo.write(iTurretSlider);
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

  // Setup the Remote Control's UI canvas
  LRemote.setName("V2-TANK");
  LRemote.setOrientation(RC_PORTRAIT);
  LRemote.setGrid(6, 12);

  // Add a text Title label
  Titlelabel.setText("Infrared Battle Tank V1.0");
  Titlelabel.setPos(0, 0);
  Titlelabel.setSize(6, 1);
  Titlelabel.setColor(RC_GREY);
  LRemote.addControl(Titlelabel);

  // Add a text HP label
  HPlabelT.setText("+HP");
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
  AMlabelT.setText("AMMO");
  AMlabelT.setPos(4, 1);
  AMlabelT.setSize(2, 1);
  AMlabelT.setColor(RC_GREY);
  LRemote.addControl(AMlabelT);

  AMlabelS.setText("5");
  AMlabelS.setPos(4, 2);
  AMlabelS.setSize(2, 1);
  AMlabelS.setColor(RC_GREY);
  LRemote.addControl(AMlabelS);

  // Add a Forward button
  forwardbutton.setText("Forward");
  forwardbutton.setPos(2, 1);
  forwardbutton.setSize(2, 2);
  forwardbutton.setColor(RC_BLUE);
  LRemote.addControl(forwardbutton);

  // Add a Backward button
  backwardbutton.setText("Backward");
  backwardbutton.setPos(2, 3);
  backwardbutton.setSize(2, 2);
  backwardbutton.setColor(RC_BLUE);
  LRemote.addControl(backwardbutton);

  // Add a TurnLeft button
  turnleftbutton.setText("TurnLeft");
  turnleftbutton.setPos(0, 3);
  turnleftbutton.setSize(2, 2);
  turnleftbutton.setColor(RC_BLUE);
  LRemote.addControl(turnleftbutton);

  // Add a TurnRight button
  turnrightbutton.setText("TurnRight");
  turnrightbutton.setPos(4, 3);
  turnrightbutton.setSize(2, 2);
  turnrightbutton.setColor(RC_BLUE);
  LRemote.addControl(turnrightbutton);

  // Add a Fire button
  firebutton.setText("Fire");
  firebutton.setPos(0, 7);
  firebutton.setSize(6, 2);
  firebutton.setColor(RC_PINK);
  LRemote.addControl(firebutton);

  // Add a Repairs button
  repairsbutton.setText("Repairs");
  repairsbutton.setPos(0, 9);
  repairsbutton.setSize(3, 2);
  repairsbutton.setColor(RC_GREEN);
  LRemote.addControl(repairsbutton);

  // Add a Reload button
  reloadbutton.setText("Reload");
  reloadbutton.setPos(3, 9);
  reloadbutton.setSize(3, 2);
  reloadbutton.setColor(RC_GREEN);
  LRemote.addControl(reloadbutton);

  // Add a Turret slider
  turretslider.setText("Turret Slider (10 ~ 160)");
  turretslider.setPos(0, 5);
  turretslider.setSize(6, 2);
  turretslider.setColor(RC_ORANGE);
  turretslider.setValueRange(10, 160, 90);
  LRemote.addControl(turretslider);

  // Add a Music button
  musicbuttonA.setText("♫1");
  musicbuttonA.setPos(0, 11);
  musicbuttonA.setSize(1, 1);
  musicbuttonA.setColor(RC_GREY);
  LRemote.addControl(musicbuttonA);

  musicbuttonB.setText("♫2");
  musicbuttonB.setPos(1, 11);
  musicbuttonB.setSize(1, 1);
  musicbuttonB.setColor(RC_GREY);
  LRemote.addControl(musicbuttonB);

  musicbuttonC.setText("♫3");
  musicbuttonC.setPos(2, 11);
  musicbuttonC.setSize(1, 1);
  musicbuttonC.setColor(RC_GREY);
  LRemote.addControl(musicbuttonC);

  musicbuttonD.setText("♫4");
  musicbuttonD.setPos(3, 11);
  musicbuttonD.setSize(1, 1);
  musicbuttonD.setColor(RC_GREY);
  LRemote.addControl(musicbuttonD);

  mutebutton.setText("MUTE");
  mutebutton.setPos(4, 11);
  mutebutton.setSize(1, 1);
  mutebutton.setColor(RC_GREY);
  LRemote.addControl(mutebutton);

  volumebutton.setText("VOL");
  volumebutton.setPos(5, 11);
  volumebutton.setSize(1, 1);
  volumebutton.setColor(RC_GREY);
  LRemote.addControl(volumebutton);

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


    // Turret
    if (turretslider.isValueChanged())
    {
      Titlelabel.updateText("Tank Go!");

      iTurretSlider = 180 - turretslider.getValue();
      TurretServo.write(iTurretSlider);
    }


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


    // Reload
    if (reloadbutton.isValueChanged())
    {
      if (reloadbutton.getValue() == 1)
      {
        if ( Tank_AM_Value < 5)
        {
          mp3_set_volume (Volume_Value);
          delay(10);
          mp3_play (3);

          Tank_AM_Value++;

          AMlabelS.updateText(String(Tank_AM_Value, 10));

          Titlelabel.updateText("Reloading !");

          delay (500);
          mp3_stop ();
        }
        else
        {
          Titlelabel.updateText("Full Ammo !");
        }

        Serial.println("Reload Tank_AT_Value");
      }
    }

  }


  // IR Test Button
  if (digitalRead(IR_Button_Pin) == HIGH)
  {
    IR_Send_Code_1();

    Ray_Gun_Sound();

    IR_Send_Code_2();

    Serial.println("IR Send Test Code");
  }


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


  // Play Music A
  if (musicbuttonA.isValueChanged())
  {
    if (musicbuttonA.getValue() == 1)
    {
      Titlelabel.updateText("Katyusha");

      mp3_set_volume (Volume_Value);
      delay(10);
      mp3_play (10);
    }
  }

  // Play Music B
  if (musicbuttonB.isValueChanged())
  {
    if (musicbuttonB.getValue() == 1)
    {
      Titlelabel.updateText("Panzerlied");

      mp3_set_volume (Volume_Value);
      delay(10);
      mp3_play (11);
    }
  }

  // Play Music C
  if (musicbuttonC.isValueChanged())
  {
    if (musicbuttonC.getValue() == 1)
    {
      Titlelabel.updateText("Marching");

      mp3_set_volume (Volume_Value);
      delay(10);
      mp3_play (12);
    }
  }

  // Play Music D
  if (musicbuttonD.isValueChanged())
  {
    if (musicbuttonD.getValue() == 1)
    {
      Titlelabel.updateText("Engine Start !");

      mp3_set_volume (Volume_Value);
      delay(10);
      mp3_play (13);
    }
  }

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
  if (volumebutton.isValueChanged())
  {
    if (volumebutton.getValue() == 1)
    {
      if (Volume_Value < 30)
      {
        Volume_Value = Volume_Value + 5;
      }
      else
      {
        Volume_Value = 5;
      }

      Titlelabel.updateText("Volume = " + String(Volume_Value, 10));

      mp3_set_volume (Volume_Value);
      delay(10);
    }
  }


}


