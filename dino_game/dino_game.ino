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

byte sun[8] = { 
  B00000,
  B10101,
  B01110,
  B11111,
  B01110,
  B10101,
  B00000,
  B00000,
};

int pushButton = 5;
int cactusPosition = 15;
int buttonState = 0;
int totalPoints = 0;

int updatedCactusPosition() {
  cactusPosition = cactusPosition < 0 ? 15 : --cactusPosition;
  return cactusPosition;
}

int updatedButtonState() {
  buttonState = digitalRead(pushButton);
  return buttonState;
}

void setup()
{
  pinMode(pushButton, INPUT); // push button setup
  lcd.init(); // initialize the lcd 
  lcd.backlight(); // switch backlight on
  // create lcd characters
  lcd.createChar(0, dino);
  lcd.createChar(1, cactus);
  lcd.createChar(2, sun);
  lcd.clear();
  lcd.print("Press to start");
  while(digitalRead(pushButton)) {}
  lcd.clear();
}


void loop()
{
  lcd.clear();
  lcd.setCursor(4,updatedButtonState());
  lcd.write(0);
  lcd.setCursor(updatedCactusPosition(), 1);
  lcd.write(1);
  lcd.setCursor(0, 0);
  lcd.write(2);
  lcd.setCursor(7, 0);
  lcd.print("Score:");
  delay(500);
}