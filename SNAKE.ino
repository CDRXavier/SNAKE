/*SNAKE
  Copyright (C) 2018 CDR_Xavier

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <Arduboy.h>
int8_t snakeLength = 3;
int8_t SnakeBody[256][2];
bool released = true;
char initials[3];
uint8_t state;
int8_t foodX;
int8_t foodY;
int8_t Direction, lastDirection;
bool flash = true;
uint16_t score, tmpScore;
// DO NOT CHANGE THE DATATYPE, it will screw up HSE!
bool buttons = true;
PROGMEM const uint8_t Logo[] =
{
0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x3f, 0xff, 0xfc, 0xf0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0x70, 0x38, 0x1c, 0x0e, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0f, 0x3f, 0xff, 0xfc, 0xf0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0x70, 0x38, 0x1c, 0x0e, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0f, 0xbf, 0xff, 0xfc, 0xf0, 0xf8, 0x1c, 0x0e, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x3c, 0x3c, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0x70, 0x38, 0x1c, 0x0e, 0x07, 0x03, 0x01, 0x03, 0x0f, 0x3f, 0xff, 0xfc, 0xf0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xcf, 0xcf, 0xcf, 0xcf, 0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0x70, 0x38, 0x1c, 0x0e, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0f, 0x3f, 0xff, 0xfc, 0xf0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf3, 0xf3, 0xf3, 0xf3, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0xf3, 0xf3, 0xf3, 0xf3, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0xf3, 0xf3, 0xf3, 0xf3, 0x00, 0x00, 0xf3, 0xf3, 0xf3, 0xf3, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0xf3, 0xf3, 0xf3, 0xf3, 0x00, 0x00, 0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 
0x00, 0x00, 0x0c, 0x0e, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0f, 0x3f, 0x3f, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x00, 0x00, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x00, 0x00, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x00, 0x00, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 
};


Arduboy arduboy;
void Snake() {
  if ((arduboy.pressed(UP_BUTTON) || arduboy.pressed(DOWN_BUTTON) || arduboy.pressed(LEFT_BUTTON) || arduboy.pressed(RIGHT_BUTTON)) && buttons) {
    buttons = false;
    if (arduboy.pressed(UP_BUTTON)) Direction = 0;
    if (arduboy.pressed(DOWN_BUTTON)) Direction = 2;
    if (arduboy.pressed(LEFT_BUTTON)) Direction = 1;
    if (arduboy.pressed(RIGHT_BUTTON)) Direction = 3;
    if (abs(lastDirection - Direction) == 2) Direction = lastDirection;
    lastDirection = Direction;
  }
  if (arduboy.everyXFrames(6)) {
    buttons = true;
    for (short i = 0; i < snakeLength - 1; i ++) {
      SnakeBody[i][0] = SnakeBody[i + 1][0];
      SnakeBody[i][1] = SnakeBody[i + 1][1];
    }
    switch (Direction) {
      case 0:
        SnakeBody[snakeLength - 1][1] = SnakeBody[snakeLength - 1][1] - 4; break;
      case 2:
        SnakeBody[snakeLength - 1][1] = SnakeBody[snakeLength - 1][1] + 4; break;
      case 1:
        SnakeBody[snakeLength - 1][0] = SnakeBody[snakeLength - 1][0] - 4; break;
      case 3:
        SnakeBody[snakeLength - 1][0] = SnakeBody[snakeLength - 1][0] + 4; break;
    }
    arduboy.tunes.tone(200, 10);
    Check();
  }
  for (short i = 0; i < snakeLength; i ++) arduboy.fillRect(SnakeBody[i][0], SnakeBody[i][1], 4, 4, 1);
}
void createFood() {
  do {
    foodX = 4 * random(1, 31);
    foodY = 4 * random(1, 15);
  } while (checkFood(foodX, foodY));
}
bool checkFood(byte x, byte y) {
  for (short i = 0; i < snakeLength; i ++) {
    if (x == SnakeBody[i][0] && y == SnakeBody[i][1]) return true;
  }
  return false;
}


void Check() {
  for (short i = 0; i < snakeLength - 1;  i ++) {
    if (SnakeBody[snakeLength - 1][0] == SnakeBody[i][0] && SnakeBody[snakeLength - 1][1] == SnakeBody[i][1]) {
      delay(500);
      arduboy.clear();
      arduboy.setCursor(10, 30);
      arduboy.print("GAME OVER");
      arduboy.display();
      delay(2500);
      state = 2;
    }
  }
}
void enterInitials() {
  int8_t index = 0;
  initials[0] = ' ';
  initials[1] = ' ';
  initials[2] = ' ';
  while (index < 3) {
    arduboy.clear();
    arduboy.setCursor(16, 0);
    arduboy.print("HIGH SCORE");
    arduboy.setCursor(88, 0);
    arduboy.print(score);
    arduboy.setCursor(56, 20);
    arduboy.print(initials[0]);
    arduboy.setCursor(64, 20);
    arduboy.print(initials[1]);
    arduboy.setCursor(72, 20);
    arduboy.print(initials[2]);
    for (int8_t i = 0; i < 3; i++) {
      arduboy.drawLine(55 + (i * 8), 27, 56 + (i * 8) + 6, 27, 1);
    }
    arduboy.drawLine(56 + (index * 8), 28, 56 + (index * 8) + 6, 28, 1);
    delay(150);
    if (arduboy.pressed(LEFT_BUTTON) && released) {
      released = false;
      arduboy.tunes.tone(1046, 200);
      index--;
      if (index < 0) index = 0;
    }

    if (arduboy.pressed(RIGHT_BUTTON) && released) {
      released = false;
      index++;
      if (index > 2) index = 2;
      arduboy.tunes.tone(1046, 200);
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
    if (arduboy.pressed(UP_BUTTON)) {
      initials[index]--;
      arduboy.tunes.tone(523, 150);
      if (initials[index] == ' ') initials[index] = '?';
      if (initials[index] == '/') initials[index] = 'Z';
      if (initials[index] == 31)  initials[index] = '/';
      if (initials[index] == '@') initials[index] = ' ';
    }
    if (arduboy.pressed(A_BUTTON)) {
      if (index >= 2) {
        index = index + 1;
        arduboy.tunes.tone(1046, 200);
      }
    }
    if (arduboy.notPressed(LEFT_BUTTON) && arduboy.notPressed(RIGHT_BUTTON))
      released = true;
    arduboy.display();
  }
}
void resetGame() {
  Direction = lastDirection = 3;
  for (byte i = 0; i < snakeLength; i ++) {
    SnakeBody[i][0] = 0;
    SnakeBody[i][1] = 0;
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
  state = 0;
}
void food() {
  if (SnakeBody[snakeLength - 1][0] == foodX && SnakeBody[snakeLength - 1][1] == foodY) {
    arduboy.tunes.tone(523, 100);
    snakeLength += 1;
    score = score + 4;
    if (snakeLength >= 256) {
      score = score + 64;
      delay(3000);
      state = 2;
    }
    createFood();
    SnakeBody[snakeLength - 1][0] = SnakeBody[snakeLength - 2][0];
    SnakeBody[snakeLength - 1][1] = SnakeBody[snakeLength - 2][1];
  }
  arduboy.drawRect(foodX + 1, foodY, 2, 4, 1);
  arduboy.fillRect(foodX, foodY + 1, 4, 2, 1);
}

void setup() {
  arduboy.begin();
  arduboy.display();
  delay(500);
  arduboy.clear();
  arduboy.drawBitmap(8, 8, Logo, 112, 48, 1);
  arduboy.display();
  delay(2500);
  arduboy.setFrameRate(30);
  arduboy.initRandomSeed();
  if (arduboy.audio.enabled()) arduboy.audio.on();
  else arduboy.audio.off();
  resetGame();
}
void loop() {
  if (state == 3)arduboy.setFrameRate(10);
  else arduboy.setFrameRate(30);
  if (!(arduboy.nextFrame())) return;
  arduboy.clear();
  switch (state) {
    case 0: {
        arduboy.setCursor(20, 15);
        arduboy.setTextSize(3);
        arduboy.print(F("SNAKE"));
        arduboy.setTextSize(2);
        arduboy.setCursor(31, 53);
        arduboy.setTextSize(1);
        if (arduboy.everyXFrames(15)) flash = !flash;
        if (flash) arduboy.print(F("PRESS FIRE!"));
        if (arduboy.pressed(B_BUTTON) && released) state = 1;
        if (arduboy.pressed(A_BUTTON) && released) {
          released = false;
          state = 3;
        }
        if ((arduboy.pressed(UP_BUTTON) || arduboy.pressed(DOWN_BUTTON)) && released) {
          released = false;
          if (arduboy.audio.enabled()) arduboy.audio.off();
          else arduboy.audio.on();
          arduboy.audio.saveOnOff();
        }
      }
      break;
    case 1:
      if ((SnakeBody[snakeLength - 1][0] >= 0) && (SnakeBody[snakeLength - 1][0] <= WIDTH) && (SnakeBody[snakeLength - 1][1] >= 0) && (SnakeBody[snakeLength - 1][1] <= HEIGHT)) {
        arduboy.setCursor(100, 50);
        arduboy.setTextSize(1);
        arduboy.print(score);
        Snake();
        food();
        arduboy.drawRect(0, 0, WIDTH, HEIGHT, 1);
      } else {
        delay(3000);
        state = 2;
      }
      if (arduboy.pressed(A_BUTTON) && released) {
        released = false;
        state = 4;
      }
      break;
    case 2:
      {
        char tmpInitials[3];
        // Each block of EEPROM has 7 high scores, and each high score entry
        // is 5 byte long:  3 char for initials and a short for score.
        // High score processing
        for (int8_t i = 0; i < 7; i++) {
          EEPROM.get(100 + (5 * i), tmpScore);
          if (tmpScore == 0xFFFF) tmpScore = 0;
          if (score > tmpScore) {
            enterInitials();
            for (byte j = i; j < 7; j++) {
              EEPROM.get(100 + (5 * j), tmpScore);
              if (tmpScore == 0xFFFF) tmpScore = 0;

              tmpInitials[0] = (char)EEPROM.read(100 + (5 * j) + 2);
              tmpInitials[1] = (char)EEPROM.read(100 + (5 * j) + 3);
              tmpInitials[2] = (char)EEPROM.read(100 + (5 * j) + 4);

              // write score and initials to current slot
              EEPROM.put(100 + (5 * j), score);
              EEPROM.write(100 + (5 * j) + 2, initials[0]);
              EEPROM.write(100 + (5 * j) + 3, initials[1]);
              EEPROM.write(100 + (5 * j) + 4, initials[2]);

              // tmpScore and tmpInitials now hold what we want to
              //write in the next slot.
              score = tmpScore;
              initials[0] = tmpInitials[0];
              initials[1] = tmpInitials[1];
              initials[2] = tmpInitials[2];
            }
            //reset the initials
            initials[0] = ' ';
            initials[1] = ' ';
            initials[2] = ' ';
            break;
          }
        }
      }
      resetGame();
      break;
    case 3:
      // Each block of EEPROM has 7 high scores, and each high score entry
      // is 5 int8_ts long:  3 int8_ts for initials and one int16_t for score
      arduboy.setCursor(32, 0);
      arduboy.print("HIGH SCORES");

      for (int i = 0; i < 7; i++) {
        EEPROM.get(100 + (5 * i), score);
        if (score == 0xFFFF) score = 0;
        initials[0] = (char)EEPROM.read(100 + (5 * i) + 2);
        initials[1] = (char)EEPROM.read(100 + (5 * i) + 3);
        initials[2] = (char)EEPROM.read(100 + (5 * i) + 4);

        arduboy.setCursor(30, 9 + (i * 8));
        arduboy.setTextSize(1);
        arduboy.print(i + 1);
        arduboy.print(F(" "));
        arduboy.print(initials[0]);
        arduboy.print(initials[1]);
        arduboy.print(initials[2]);
        arduboy.print(F(" "));
        arduboy.print(score);

        score = 0;
      }
      if (arduboy.pressed(LEFT_BUTTON)) state = 5;
      if (arduboy.pressed(A_BUTTON) && released) {
        released = false;
        state = 0;
      }
      break;
    case 4:
      arduboy.setCursor(32, 30);
      arduboy.setTextSize(2);
      arduboy.print(F("PAUSE"));
      if (arduboy.pressed(B_BUTTON) && released) {
        released = false;
        state = 1;
      }
      if (arduboy.pressed(A_BUTTON) && released) {
        released = false;
        resetGame();
      }
      if ((arduboy.pressed(UP_BUTTON) || arduboy.pressed(DOWN_BUTTON)) && released) {
        released = false;
        if (arduboy.audio.enabled()) arduboy.audio.off();
        else arduboy.audio.on();
        arduboy.audio.saveOnOff();
      }
      break;
    case 5:
      arduboy.setCursor(14, 32);
      arduboy.setTextSize(1);
      arduboy.print(F("EREASE HIGHSCORE?"));
      arduboy.setCursor(14, 40);
      arduboy.print(F("START = RIGHT + B"));
      if (arduboy.pressed(RIGHT_BUTTON + B_BUTTON)) {
        released = false;
        for (int i = 0; i < 35; i ++) {
          arduboy.clear();
          arduboy.setCursor(36, HEIGHT / 2);
          arduboy.print(F("WORKING..."));
          arduboy.display();
          EEPROM.update(i + 100, 0xff);
        }
        delay(500);
        arduboy.setCursor(32, HEIGHT / 2);
        arduboy.clear();
        arduboy.print(F("EREASE DONE"));
        arduboy.display();
        delay(1000);
        state = 0;
        break;
      }
  }
  if (arduboy.audio.enabled()) {
    arduboy.drawRect(124, 61, 2, 2, 1);
    arduboy.drawLine(125, 59, 125, 62, 1);
    arduboy.drawPixel(126, 59, 1);
  }
  arduboy.display();
  if (arduboy.notPressed(UP_BUTTON | DOWN_BUTTON | A_BUTTON | B_BUTTON))
    released = true;
}
