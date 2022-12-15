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
unsigned long cactusPositionUpdateTime = 0;
bool isCactusUpdated = false;
int gameScore = 0;
char stringBuffer[10];

// the dino will stay in the air for 1 second
int updatedButtonState() {
  if(digitalRead(PUSH_BUTTON_PIN) == LOW && !buttonState) {
    Serial.println("Pressed!");
    buttonTimeout = millis();
    buttonState = true;
  } 
  if(buttonState && millis() - buttonTimeout >= 1000) {
    buttonState = false;
    Serial.println("Released!");
  }
  return !buttonState;
}

void updateCactusPosition() {
  cactusPosition = cactusPosition < 0 ? 15 : --cactusPosition;
  cactusPositionUpdateTime = millis();
  isCactusUpdated = true;
}

// a simple no operation function
void noop() {
  // arduino no operation: https://www.reddit.com/r/arduino/comments/3ynemw/is_there_a_nop_in_arduino/
  __asm__("nop\n\t");
}

// updates the cactus position after 400 ms
void checkForCactusPositionUpdate() {
  millis() - cactusPositionUpdateTime >= 100 ? updateCactusPosition() : noop();
}

void checkIfGameOver() {
  if(cactusPosition == 4 && isCactusUpdated) {
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
  // reset the cactus status state
  isCactusUpdated = !isCactusUpdated;
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
  // reset button state to avoid misreading when the game starts
  buttonState = !buttonState;
  lcd.clear();
  // some initial display settings for the game
  sprintf(stringBuffer, "Score:%d", gameScore);
  cactusPositionUpdateTime = millis();
}

void loop()
{
  checkIfGameOver();
  // update all the lcd with all updated game states
  lcd.clear();
  lcd.setCursor(4,updatedButtonState());
  lcd.write(DINO_CHARACTER);
  checkForCactusPositionUpdate();
  lcd.setCursor(cactusPosition, 1);
  lcd.write(CACTUS_CHARACTER);
  lcd.setCursor(0, 0);
  lcd.write(SUN_CHARACTER);
  // update the game score board
  lcd.setCursor(7, 0);
  lcd.print(stringBuffer);
  // this delay determines how fast the overall game refreshes 
  delay(300);
}