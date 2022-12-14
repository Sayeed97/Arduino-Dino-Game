//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

byte dino[8] = { //dino
  B01110,
  B10111,
  B11110,
  B11111,
  B11000,
  B11111,
  B11010,
  B10010,
};

byte cactus[8] = { //cactus
  B00101,
  B10101,
  B10101,
  B10101,
  B10110,
  B01100,
  B00100,
  B00100,
};

byte cross[8] = { //cactus
  B10001,
  B01010,
  B01010,
  B01100,
  B01100,
  B01010,
  B0100,
  B00100,
};

int pushButton = 5;
int cactusPosition = 15;

int updatedCactusPosition() {
  cactusPosition = cactusPosition < 0 ? 15 : --cactusPosition;
  return cactusPosition;
}

void setup()
{
  pinMode(pushButton, INPUT); // push button setup
  lcd.init(); // initialize the lcd 
  lcd.backlight(); // switch backlight on
  // create lcd characters
  lcd.createChar(0, dino);
  lcd.createChar(1, cactus);
}


void loop()
{
  lcd.setCursor(3,digitalRead(pushButton));
  lcd.write(0);
  lcd.setCursor(updatedCactusPosition(), 1);
  lcd.write(1);
  delay(500);
  lcd.clear();
}