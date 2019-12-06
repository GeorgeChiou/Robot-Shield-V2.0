// 最後編輯 2019-12-5 by ShinWei Chiou

// For Linkit-7697 + Robot Shield V2.1
// MAX7219 CLK : P11 , CS : P16 , DIN : P15
// Servo : P5
// Buzzer : P14
// IR Receiver : P9
// IR Test Button : P6

// DFPlayer mini mp3 module.
// github as default source provider
// https://github.com/DFRobot/DFPlayer-Mini-mp3
// DFplayer RX : P2 , TX : P4


#include <Servo.h>
#include <LRemote.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

// Buzzer
#define Buzzer_Pin 14

// Servo
#define Servo_Pin 5
const int TurretTurnCenter = 90;
const int TurretTurnMax = 160;
const int TurretTurnMin = 10;
Servo TurretServo;

// IR Battle System
#define IR_Button_Pin    6
#define IR_Receiver_Pin  9

// DFplayer
// On LinkIt 7697, the RX pin must be one of the EINT pins.
// There are no limitations on TX pin.
SoftwareSerial mp3Serial(2, 4); // RX, TX
unsigned int Volume_Value = 25;

// Label
LRemoteLabel Titlelabel;

// Button
LRemoteButton repairsbutton;

// MAX7219 8x8 Dot Matrices
#define MAX7219_CLK_Pin  11
#define MAX7219_CS_Pin   16
#define MAX7219_DIN_Pin  15

// MAX7219 address map
#define MAX7219_DECODE_REG      (0x09)
#define MAX7219_INTENSITY_REG   (0x0A)
#define MAX7219_SCANLIMIT_REG   (0x0B)
#define MAX7219_SHUTDOWN_REG    (0X0C)
#define MAX7219_DISPLAYTEST_REG (0x0F)
#define MAX7219_DIGIT_REG(pos)  ((pos) + 1)
#define MAX7219_COLUMN_REG(pos) MAX7219_DIGIT_REG(pos)

// shutdown mode
#define MAX7219_OFF             (0x0)
#define MAX7219_ON              (0x1)

// number of columns of the display matrx
#define NUM_OF_COLUMNS  (8)

// for each character bitmap, it consumes bytes
#define BYTE_PER_MAP    (8)

unsigned int char_index = 0;

// matrix pattern
const byte char_pattern[] =
{
  0x00, 0x3E, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x3E, // 0
  0x00, 0x3F, 0x0C, 0x0C, 0x0C, 0x0C, 0x0E, 0x0C, // 1
  0x00, 0x3F, 0x33, 0x06, 0x1C, 0x30, 0x33, 0x1E, // 2
  0x00, 0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, // 3
  0x00, 0x78, 0x30, 0x7F, 0x33, 0x36, 0x3C, 0x38, // 4

  0x00, 0x1E, 0x33, 0x30, 0x30, 0x1F, 0x03, 0x3F, // 5
  0x00, 0x1E, 0x33, 0x33, 0x1F, 0x03, 0x06, 0x1C, // 6
  0x00, 0x0C, 0x0C, 0x0C, 0x18, 0x30, 0x33, 0x3F, // 7
  0x00, 0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, // 8
  0x00, 0x0E, 0x18, 0x30, 0x3E, 0x33, 0x33, 0x1E, // 9

  //  0x00, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x1E, 0x0C, // A
  //  0x00, 0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, // B
  //  0x00, 0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, // C
  //  0x00, 0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, // D
  //  0x00, 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, // E
  //  0x00, 0x0F, 0x06, 0x16, 0x1E, 0x16, 0x46, 0x7F, // F

  0x00, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x63, // X
  0x00, 0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, // O

  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Full
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // space
};

#define DISPLAY_STR_LENGTH  (sizeof(char_pattern) / BYTE_PER_MAP)

void Set_MAX7219_register(byte address, byte value)
{
  digitalWrite(MAX7219_CS_Pin, LOW);
  shiftOut(MAX7219_DIN_Pin, MAX7219_CLK_Pin, MSBFIRST, address);
  shiftOut(MAX7219_DIN_Pin, MAX7219_CLK_Pin, MSBFIRST, value);
  digitalWrite(MAX7219_CS_Pin, HIGH);
}

void Clear_MAX7219_matrix()
{
  // clear the dot matrix
  for (int i = 0; i < NUM_OF_COLUMNS; i++)
  {
    Set_MAX7219_register(MAX7219_COLUMN_REG(i), B00000000);
  }
}

void Initialize_MAX2719()
{
  // disable test mode.
  Set_MAX7219_register(MAX7219_DISPLAYTEST_REG, MAX7219_OFF);

  // set medium intensity.
  Set_MAX7219_register(MAX7219_INTENSITY_REG, 0x1);

  // turn off display.
  Set_MAX7219_register(MAX7219_SHUTDOWN_REG, MAX7219_OFF);

  // drive 8 digits.
  Set_MAX7219_register(MAX7219_SCANLIMIT_REG, 7);

  // no decode mode for all positions.
  Set_MAX7219_register(MAX7219_DECODE_REG, B00000000);

  // clear matrix display
  Clear_MAX7219_matrix();
}

void Display_MAX2719()
{
  // turn off display first
  Set_MAX7219_register(MAX7219_SHUTDOWN_REG, MAX7219_OFF);

  // display one bitmap
  for (int i = 0; i < BYTE_PER_MAP; i++)
  {
    // starting from column
    Set_MAX7219_register(MAX7219_COLUMN_REG(i), char_pattern[char_index * BYTE_PER_MAP + i]);
  }

  // turn on display
  Set_MAX7219_register(MAX7219_SHUTDOWN_REG, MAX7219_ON);
}


/*------------------------------------------------------------*/
void setup()
{
  // Initialize serial communications at 9600 bps:
  Serial.begin(9600);

  // Set the data rate for the SoftwareSerial port
  mp3Serial.begin(9600);

  // Add for DFplayer
  mp3_set_serial (mp3Serial);  // set Serial for DFPlayer-mini mp3 module
  delay(10);

  // MAX7219 PIN Set
  pinMode(MAX7219_CLK_Pin, OUTPUT);
  pinMode(MAX7219_CS_Pin, OUTPUT);
  pinMode(MAX7219_DIN_Pin, OUTPUT);

  // IR PIN Set
  pinMode(IR_Receiver_Pin, INPUT);
  pinMode(IR_Button_Pin, INPUT);

  // Initialize Servo
  TurretServo.attach(Servo_Pin);
  TurretServo.write(TurretTurnCenter);

  // Initialize MAX2719 states
  Initialize_MAX2719();
  Display_MAX2719();

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
  tank_name = "FLAG" + tank_address.substring(0, 2) + tank_address.substring(3, 5);

  // Setup the Remote Control's Name
  LRemote.setName(tank_name);

  // Setup the Remote Control's UI canvas
  LRemote.setOrientation(RC_PORTRAIT);
  LRemote.setGrid(3, 6);

  // Add a text Title label
  Titlelabel.setText("Infrared Battle Tower V1.0");
  Titlelabel.setPos(0, 0);
  Titlelabel.setSize(3, 1);
  Titlelabel.setColor(RC_GREY);
  LRemote.addControl(Titlelabel);

  // Add a Repairs button
  repairsbutton.setText("REPAIRS");
  repairsbutton.setPos(0, 1);
  repairsbutton.setSize(3, 2);
  repairsbutton.setColor(RC_GREEN);
  LRemote.addControl(repairsbutton);

  // Start broadcasting our remote contoller
  LRemote.begin();

  // Start MP3
  mp3_set_volume (Volume_Value);
  delay(10);
  mp3_play (8002);

  // Stop MP3
  delay(3000);
  mp3_stop ();
}


/*------------------------------------------------------------*/
void loop()
{
  /*----------------------------------*/
  // Process the incoming BLE write request
  LRemote.process();


  /*----------------------------------*/
  // IR Battle System
  if (digitalRead(IR_Receiver_Pin) == LOW)
  {
    int IR_Value = pulseIn(IR_Receiver_Pin, LOW);

    if ( IR_Value >= 2700 && IR_Value <= 3400 )
    {
      if (char_index <= 9)
      {
        char_index++;

        mp3_set_volume (Volume_Value);
        delay(10);
        mp3_play (2);
      }

      Display_MAX2719();

      if (char_index == 10)
      {
        Titlelabel.updateText("Defeated !");

        delay(1000);
        mp3_stop ();
        delay(10);

        mp3_set_volume (30);
        delay(10);
        mp3_play (1);

        TurretServo.write(TurretTurnMax);
      }

      delay(1000);
      mp3_stop ();
    }
  }


  /*----------------------------------*/
  // Repairs
  if (repairsbutton.isValueChanged())
  {
    if (repairsbutton.getValue() == 1)
    {
      // Start MP3
      mp3_set_volume (Volume_Value);
      delay(10);
      mp3_play (8002);

      char_index = 0;

      Titlelabel.updateText("Repairs !");

      TurretServo.write(TurretTurnCenter);

      Display_MAX2719();

      // Stop MP3
      delay(3000);
      mp3_stop ();

      Titlelabel.updateText("S T A R T");
    }
  }


  /*----------------------------------*/
  // Main Delay
  delay(10);
}
