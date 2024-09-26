#include <Arduino.h>
#include "LiquidCrystal_I2C.h"
#include "button.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
button btn1(2);
int l_hit_count = 0;
int r_hit_count = 0;
int goal_hits = 50;
bool pause = false;
bool hit_switch = false;
bool goal_time_start = false;
unsigned long total_time = 0;
unsigned long goal_time = 0;
unsigned long tmp_time = 0;
unsigned long hit_tmp_time = 0;

String timeConvert(unsigned long t) {
  String res, h, m, s;
  int hh = 0, mm = 0, ss = 0;
  ss = t % 60;
  mm = t / 60;
  if (mm >= 60) {
    hh = mm / 60;
    mm = mm % 60;
  }

  h = hh < 10 ? "0" + String(hh) : String(hh);
  m = mm < 10 ? "0" + String(mm) : String(mm);
  s = ss < 10 ? "0" + String(ss) : String(ss);
  res = m + ":" + s;
  return res;
}

String scoreConvert(String prefix, int s) {
  String res = prefix;
  if (s < 10) {
    res = res + "00" + String(s);
  } else if (s < 100) {
    res = res + "0" + String(s);
  } else {
    res = res + String(s);
  }
  return res;
}

void clearLine(LiquidCrystal_I2C screen, int line) {
  screen.setCursor(0, line);
  for (int n = 0; n < 16; n++) {
    screen.print(" ");
  }
}

void printUp(LiquidCrystal_I2C screen, int l_score, int r_score) {
  clearLine(screen, 0);
  screen.setCursor(1, 0);
  screen.print(scoreConvert("L:", l_score)); // L:000
  screen.setCursor(9, 0);
  screen.print(scoreConvert("R:", r_hit_count)); //R:000
}

void printDown(LiquidCrystal_I2C screen, unsigned long t_time, unsigned long h_time) {
  clearLine(screen, 1);
  screen.setCursor(1, 1);
  screen.print(timeConvert(t_time)); //00:00
  screen.setCursor(9, 1);
  screen.print(timeConvert(h_time)); //00:00
} 

void printMessage(LiquidCrystal_I2C screen, String upMessage, String downMessage) {
  screen.clear();
  screen.setCursor(0, 0);
  screen.print(upMessage);
  screen.setCursor(0, 1);
  screen.print(downMessage);
}

void setup() {
  lcd.init();
  lcd.backlight();
  printUp(lcd, 0, 0);
}

void loop() {
  
  if (btn1.isClick()) {
    if (!pause) {
      printMessage(lcd, 
        "-    PAUSE     -",
        "----------------");
    } else {
      printUp(lcd, l_hit_count, r_hit_count);
      printDown(lcd, total_time, goal_time);
    }
    pause = !pause;
  }

  if (!pause) {

    int sence = digitalRead(8);

    if ((sence == 1) && ((millis() - hit_tmp_time) > 500)) {
      
      if (!goal_time_start) goal_time = 0;
      goal_time_start = true;
      hit_tmp_time = millis();
      hit_switch ? r_hit_count++ : l_hit_count++;
      
      bool is_goal_achieve = hit_switch ? r_hit_count % goal_hits == 0 : l_hit_count % goal_hits == 0;

      printUp(lcd, l_hit_count, r_hit_count);

      if (is_goal_achieve) {
        hit_switch = !hit_switch;
        goal_time_start = false;
        tone(4, 600, 600);
      }
    }

    if (millis() - tmp_time >= 1000) {
      tmp_time = millis();
      total_time++;
      if (goal_time_start) goal_time++;

      printDown(lcd, total_time, goal_time);
    }

    
  }
}