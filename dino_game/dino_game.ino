#include <LiquidCrystal_I2C.h>

// all the game macro definitions
#define DINO_CHARACTER 0
#define CACTUS_CHARACTER 1
#define SUN_CHARACTER 2
#define PUSH_BUTTON_PIN 5

// set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27,20,4); 

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

byte sun[8] = { //sun
  B00000,
  B10101,
  B01110,
  B11111,
  B01110,
  B10101,
  B00000,
  B00000,
};

// all the game states 
int cactusPosition = 15;
int buttonState = 0;
unsigned long buttonTimeout = 0;
int gameScore = 0;
char stringBuffer[10];

int updatedCactusPosition() {
  cactusPosition = cactusPosition < 0 ? 15 : --cactusPosition;
  return cactusPosition;
}

int updatedButtonState() {
  if(digitalRead(PUSH_BUTTON_PIN) == LOW && !buttonState) {
    Serial.println("Pressed!");
    buttonTimeout = millis();
    buttonState = true;
  } 
  if(buttonState && millis() - buttonTimeout >= 1500) {
    buttonState = false;
    Serial.println("Released!");
  }
  return !buttonState;
}

void checkIfGameOver() {
  if(cactusPosition == 4) {
    if(!buttonState) {
      lcd.clear();
      lcd.print("GAME OVER!!!");
      delay(1000);
      lcd.clear();
      lcd.print("Press to restart");
      // wait for the user to restart the game
      while(digitalRead(PUSH_BUTTON_PIN)) {}
      // reset all the game states prior to the actual restart
      cactusPosition = 15;
      buttonState = 1;
      gameScore = 0;
      sprintf(stringBuffer, "Score:%d", gameScore);
    } else {
        sprintf(stringBuffer, "Score:%d", ++gameScore);
    }
  } else {
    sprintf(stringBuffer, "Score:%d", gameScore);
  }
}

void setup()
{
  // push button setup
  pinMode(PUSH_BUTTON_PIN, INPUT);
  // lcd initialization
  lcd.init(); 
  lcd.backlight();
  // create custom game characters
  lcd.createChar(DINO_CHARACTER, dino);
  lcd.createChar(CACTUS_CHARACTER, cactus);
  lcd.createChar(SUN_CHARACTER, sun);
  lcd.clear();
  // start the game once the user is ready
  lcd.print("Press to start");
  while(digitalRead(PUSH_BUTTON_PIN)) {}
  lcd.clear();
}


void loop()
{
  checkIfGameOver();
  // update all the lcd with all updated game states
  lcd.clear();
  lcd.setCursor(4,updatedButtonState());
  lcd.write(DINO_CHARACTER);
  lcd.setCursor(updatedCactusPosition(), 1);
  lcd.write(CACTUS_CHARACTER);
  lcd.setCursor(0, 0);
  lcd.write(SUN_CHARACTER);
  // update the game score board
  lcd.setCursor(7, 0);
  lcd.print(stringBuffer);
  delay(300);
}