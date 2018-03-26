#include <Arduboy.h>
Arduboy arduboy;
int8_t snakeLength = 3;
int8_t tick = 0;
int8_t hi,lo;
int8_t SnakeBody[196][2] = {{20,24},{24,24},{28,24}};
bool released = true;
int16_t initials[3], tmpInitials[3];
uint8_t state = 0;
int8_t foodX = 4 * random(1,31);
int8_t foodY = 4 * random(1,15);
int8_t Direction = 3;
int8_t lastDirection;
int16_t score, tmpScore;
bool buttons = false;
bool checkFood(byte x, byte y) {
  for (short i = 0;i < snakeLength; i ++) {
    if (x == SnakeBody[i][0] && y == SnakeBody[i][1]) return true;
  }
  return false;
}
void createFood() {
  do {
    foodX = 4 * random(1,31);
    foodY = 4 * random(1,15);
  } while(checkFood(foodX,foodY));
}
void Snake() {

  if (arduboy.pressed(UP_BUTTON)) Direction = 0;
  if (arduboy.pressed(DOWN_BUTTON)) Direction = 2;
  if (arduboy.pressed(LEFT_BUTTON)) Direction = 1;
  if (arduboy.pressed(RIGHT_BUTTON)) Direction = 3;
  if (abs(lastDirection - Direction) == 2) Direction = lastDirection;
  lastDirection = Direction;
  
  if (tick > 4) {
    tick = 0;
    for (short i = 0; i < snakeLength - 1; i ++) {
      SnakeBody[i][0] = SnakeBody[i + 1][0];
      SnakeBody[i][1] = SnakeBody[i + 1][1];
    }
    switch(Direction) {
      case 0:
      SnakeBody[snakeLength - 1][1] = SnakeBody[snakeLength - 1][1] - 4; break;
      case 2:
      SnakeBody[snakeLength - 1][1] = SnakeBody[snakeLength - 1][1] + 4; break;
      case 1:
      SnakeBody[snakeLength - 1][0] = SnakeBody[snakeLength - 1][0] - 4; break;
      case 3:
      SnakeBody[snakeLength - 1][0] = SnakeBody[snakeLength - 1][0] + 4; break;
    }
    arduboy.tunes.tone(200,10);
    for (short i = 0; i < snakeLength; i ++) arduboy.drawRect(SnakeBody[i][0],SnakeBody[i][1],4,4,1);
    Check();
  }
}
void Check() {
  for (short i = 0; i < snakeLength - 1;  i ++) {
    if(SnakeBody[snakeLength - 1][0] == SnakeBody[i][0] && SnakeBody[snakeLength - 1][1] == SnakeBody[i][1]) {
      delay(500);
      arduboy.setCursor(10,30);
      arduboy.print("GAME OVER");
      arduboy.display();
      delay(2500);
      state = 2;
      break;
    }
  }
}
void enterInitials() {
  byte index = 0;
  initials[0] = ' ';
  initials[1] = ' ';
  initials[2] = ' ';
  while (true) {
    arduboy.clear();
    arduboy.setCursor(16,0);
    arduboy.print("HIGH SCORE");
    arduboy.setCursor(88, 0);
    arduboy.print(score);
    arduboy.setCursor(56, 20);
    arduboy.print(initials[0]);
    arduboy.setCursor(64, 20);
    arduboy.print(initials[1]);
    arduboy.setCursor(72, 20);
    arduboy.print(initials[2]);
    for(byte i = 0; i < 3; i++) {
      arduboy.drawLine(56 + (i*8), 27, 56 + (i*8) + 6, 27, 1);
    }
    arduboy.drawLine(56, 28, 88, 28, 0);
    arduboy.drawLine(56 + (index*8), 28, 56 + (index*8) + 6, 28, 1);
    delay(150);

    if (arduboy.pressed(LEFT_BUTTON) && released) {
      released = false;
      arduboy.tunes.tone(523, 200);
      index--;
      if (index < 0) index = 0;
    }

    if (arduboy.pressed(RIGHT_BUTTON) && released) {
      released = false;
      index++;
      if (index > 2) index = 2;
      arduboy.tunes.tone(523, 200);
    }

    if (arduboy.pressed(DOWN_BUTTON)) {
      initials[index]++;
      arduboy.tunes.tone(523, 150);
      // A-Z 0-9 :-? !-/ ' '
      if (initials[index] == '0') initials[index] = ' ';
      if (initials[index] == '!') initials[index] = 'A';
      if (initials[index] == '[') initials[index] = '0';
      if (initials[index] == '@') initials[index] = '!';
    }
    if (arduboy.pressed(UP_BUTTON)){
      initials[index]--;
      arduboy.tunes.tone(523, 150);
      if (initials[index] == ' ') initials[index] = '?';
      if (initials[index] == '/') initials[index] = 'Z';
      if (initials[index] == 31)  initials[index] = '/';
      if (initials[index] == '@') initials[index] = ' ';
    }
    if (arduboy.pressed(A_BUTTON) && released) {
      released = false;
      if (index >= 2) {
        state = 0;
        arduboy.tunes.tone(1046, 200);
        return;
      }
    }
    if (arduboy.notPressed(LEFT_BUTTON) && arduboy.notPressed(RIGHT_BUTTON) && arduboy.notPressed(A_BUTTON)) released = true;
    arduboy.display();
  }
}
void resetGame() {
  Direction = 3;
  for(byte i = 0; i < snakeLength; i ++) {
    SnakeBody[i][0] = '\0';
    SnakeBody[i][1] = '\0';
  }
  snakeLength = 3;
  SnakeBody[0][0] = 20;
  SnakeBody[0][1] = 24;
  SnakeBody[1][0] = 24;
  SnakeBody[1][1] = 24;
  SnakeBody[2][0] = 28;
  SnakeBody[2][1] = 24;
  createFood();
  score = 0;
}
void food() {
  if (SnakeBody[snakeLength - 1][0] == foodX && SnakeBody[snakeLength - 1][1] == foodY) {
    arduboy.tunes.tone(523,100);
    snakeLength += 1;
    score  = score + 10 + 5 * (snakeLength / 25);
    if (snakeLength == 128) {
      score = score + 1023;
      delay(1000);
      state = 2;
    }
    createFood();
    SnakeBody[snakeLength - 1][0] = SnakeBody[snakeLength - 2][0];
    SnakeBody[snakeLength - 1][1] = SnakeBody[snakeLength - 2][1];
  }
  arduboy.fillRect(foodX + 1,foodY,2,4,1);
  arduboy.fillRect(foodX,foodY + 1,4,2,1);
}

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(30);
  arduboy.initRandomSeed();
  if (arduboy.audio.enabled()) arduboy.audio.on();
  else arduboy.audio.off();
  arduboy.tunes.tone(987, 160);
  delay(160);
  arduboy.tunes.tone(1318, 400);
  arduboy.setTextSize(2);
}
void loop() {
  if(!(arduboy.nextFrame())) return;
  arduboy.clear();
  switch (state) {
    case 0:
    arduboy.setCursor(20,15);
    arduboy.setTextSize(3);
    arduboy.print(F("SNAKE"));
    arduboy.setTextSize(2);
    arduboy.setCursor(31, 53);
    arduboy.setTextSize(1);
    arduboy.print(F("PRESS FIRE!"));
    arduboy.setTextSize(2);
    if (arduboy.pressed(B_BUTTON) && released) {
      released = false;
      state = 1;
    }
    if (arduboy.pressed(A_BUTTON) && released) {
      released = false;
      state = 4;
    }
    if ((arduboy.pressed(UP_BUTTON) || arduboy.pressed(DOWN_BUTTON)) && released) {
      released = false;
      if (arduboy.audio.enabled()) arduboy.audio.off();
      else arduboy.audio.on();
      arduboy.audio.saveOnOff();
    }
    break;
    case 1:
    if((SnakeBody[snakeLength - 1][0] >= 0) && (SnakeBody[snakeLength - 1][0] <= WIDTH) && (SnakeBody[snakeLength - 1][1] >= 0) && (SnakeBody[snakeLength - 1][1] <= HEIGHT)) {
      arduboy.setCursor(100,50);
      arduboy.setTextSize(1);
      arduboy.print(score);
      arduboy.setTextSize(2);
      Snake();
      food();
      arduboy.drawRect(0,0,WIDTH,HEIGHT,1);
    } else {
      delay(500);
      arduboy.clear();
      arduboy.setCursor(10,30);
      arduboy.print(F("GAME OVER"));
      arduboy.display();
      delay(2500);
      state = 2;
    }
    if (arduboy.pressed(A_BUTTON) && released) {
      released = false;
      state = 3;
    }
    break;
    case 2:
    // Each block of EEPROM has 7 high scores, and each high score entry
    // is 5 bytes long:  3 bytes for initials and two bytes for score.
    // High score processing
    for(byte i = 0; i < 7; i++) {
      hi = EEPROM.read(100 + (5*i));
      lo = EEPROM.read(100 + (5*i) + 1);
      if ((hi == 0xFF) && (lo == 0xFF)) {
        // The values are uninitialized, so treat this entry
        // as a score of 0.
        tmpScore = 0;
      } else tmpScore = (hi << 8) | lo;
      if (score > tmpScore) {
        arduboy.setTextSize(1);
        enterInitials();
        arduboy.setTextSize(2);
        for(byte j = i;j < 7;j++) {
          hi = EEPROM.read(100 + (5*j));
          lo = EEPROM.read(100 + (5*j) + 1);

          if ((hi == 0xFF) && (lo == 0xFF)) tmpScore = 0;
          else tmpScore = (hi << 8) | lo;

          tmpInitials[0] = (char)EEPROM.read(100 + (5*j) + 2);
          tmpInitials[1] = (char)EEPROM.read(100 + (5*j) + 3);
          tmpInitials[2] = (char)EEPROM.read(100 + (5*j) + 4);
  
          // write score and initials to current slot
          EEPROM.write(100 + (5*j), ((score >> 8) & 0xFF));
          EEPROM.write(100 + (5*j) + 1, (score & 0xFF));
          EEPROM.write(100 + (5*j) + 2, initials[0]);
          EEPROM.write(100 + (5*j) + 3, initials[1]);
          EEPROM.write(100 + (5*j) + 4, initials[2]);
  
          // tmpScore and tmpInitials now hold what we want to
          //write in the next slot.
          score = tmpScore;
          initials[0] = tmpInitials[0];
          initials[1] = tmpInitials[1];
          initials[2] = tmpInitials[2];
        }
        initials[0] = ' ';
        initials[1] = ' ';
        initials[2] = ' ';
      }
    }
    resetGame();
    resetGame();
    resetGame();
    state = 4;
    break;
    case 3:
    arduboy.setCursor(34, 28);
    arduboy.print(F("PAUSE"));
    if (arduboy.pressed(B_BUTTON) && released) {
      released = false;
      state = 1;
    }
    if (arduboy.pressed(UP_BUTTON) || arduboy.pressed(DOWN_BUTTON) && released) {
      released = false;
      if (arduboy.audio.enabled()) arduboy.audio.off();
      else arduboy.audio.on();
      arduboy.audio.saveOnOff();
    }
    if (arduboy.pressed(A_BUTTON) && released) {
      released = false;
      resetGame();
      state = 0;
    }
    break;
    case 4:
    // Each block of EEPROM has 7 high scores, and each high score entry
    // is 5 bytes long:  3 bytes for initials and two bytes for score
    arduboy.setCursor(32, 0);
    arduboy.setTextSize(1);
    arduboy.print(F("HIGH SCORES"));
    for(int i = 0; i < 7; i++) {
      arduboy.setCursor(24,10+(i*8));
      arduboy.print(i + 1);
      hi = EEPROM.read(100 + (5*i));
      lo = EEPROM.read(100 + (5*i) + 1);
    
      if ((hi == 0xFF) && (lo == 0xFF)) score = 0;
      else score = (hi << 8) | lo;
    
      initials[0] = (char)EEPROM.read(100 + (5*i) + 2);
      initials[1] = (char)EEPROM.read(100 + (5*i) + 3);
      initials[2] = (char)EEPROM.read(100 + (5*i) + 4);
    
      if (score > 0) {
        char text[16];
        sprintf(text, "%c%c%c %u", initials[0], initials[1], initials[2], score);
        arduboy.setCursor(48, 10 + (i*8));
        arduboy.print(text);
      }
      score = 0;
    }
    arduboy.setTextSize(2);
    if (arduboy.pressed(LEFT_BUTTON) && arduboy.pressed(A_BUTTON)) {
      released = false;
      state = 5;
    }
    if (arduboy.pressed(A_BUTTON) && released) {
      released = false;
      state = 0;
    }
    if (arduboy.pressed(B_BUTTON) && released) {
      released = false;
      state = 1;
    }
    if ((arduboy.pressed(UP_BUTTON) || arduboy.pressed(DOWN_BUTTON)) && released) {
      released = false;
      if (arduboy.audio.enabled()) arduboy.audio.off();
      else arduboy.audio.on();
      arduboy.audio.saveOnOff();
    }
    break;
    case 5:
    arduboy.setTextSize(1);
    arduboy.setCursor(14, 32);
    arduboy.print(F("EREASE HIGHSCORE?"));
    arduboy.setCursor(14, 40);
    arduboy.print(F("START = RIGHT + B"));
    if (arduboy.pressed(RIGHT_BUTTON + B_BUTTON)) {
      for (int i = 0; i < 70; i ++) {
        arduboy.clear();
        arduboy.setCursor(36, HEIGHT / 2);
        arduboy.print(F("WORKING..."));
        arduboy.display();
        EEPROM.update(i + 100,0xff);
      }
      delay(500);
      arduboy.setCursor(32, HEIGHT / 2);
      arduboy.clear();
      arduboy.print(F("EREASE DONE"));
      arduboy.display();
      delay(1000);
      state = 0;
      arduboy.setTextSize(2);
      break;
    }
    if (arduboy.pressed(A_BUTTON) && released) {
      released = false;
      state = 0;
    }
  }
  if (arduboy.notPressed(A_BUTTON | B_BUTTON | LEFT_BUTTON | RIGHT_BUTTON | UP_BUTTON | DOWN_BUTTON)) released = true;
  tick ++;
  if (arduboy.audio.enabled()) {
    arduboy.drawRect(124,61,2,2,1);
    arduboy.drawLine(125, 59, 125, 62,1);
    arduboy.drawPixel(126,59,1);
  }
  arduboy.display();
}
