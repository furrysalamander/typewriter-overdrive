#include <Stepper.h>
#include "symbol_logic.h"

const int stepsPerRevolution = 24;
const int revolutions = 10;
const int speed = 600;

const int CHAR_WIDTH = 10;
const int STEPS_PER_LINE = 36;

// initialize the stepper library on pins 8 through 11:
Stepper symbol_wheel(stepsPerRevolution, 2, 3, 4, 5);
Stepper line_feed(stepsPerRevolution, 6, 7, 8, 9);
Stepper x_carriage(stepsPerRevolution, 10, 11, 12, 13);

const uint8_t x_limit = A7;
const uint8_t solenoid_one = A0;
const uint8_t solenoid_two = A1;
const uint8_t motor_forward = A2;
const uint8_t motor_back = A3;

char symbol_wheel_pos = -1;
uint32_t x_pos = -1;

void spin_motor() {
  digitalWrite(motor_forward, HIGH);
  delay(75);
  digitalWrite(motor_forward, LOW);
}

void home_x() {
  x_carriage.step(10);
  for (uint8_t i = 0; i < 20; i++) {
    analogRead(x_limit);
  }
  while (analogRead(x_limit) < 1000) {
    //Serial.print(analogRead(x_limit));
    //Serial.print('\n');
    x_carriage.step(-1);
  }
}

void go_to_symbol(char symbol) {
  uint8_t new_pos = symbol_table[(uint8_t)symbol];
  //Serial.print("Symbol Code: "); Serial.println((uint8_t)symbol);
  //Serial.print("Current Position: "); Serial.println((uint8_t)symbol_wheel_pos);
  //Serial.print("New Position: "); Serial.println((uint8_t)new_pos);
  if (new_pos == 0xFF) {
    return;
  }
  int steps_back = -((symbol_wheel_pos - new_pos) % 97) * 2;
  int steps = ((new_pos - symbol_wheel_pos) % 97) * 2;
  if (abs(steps) < abs(steps_back)) {
    symbol_wheel.step(steps);
  }
  else {
    symbol_wheel.step(steps_back);
  }
  symbol_wheel_pos = new_pos;
  //Serial.print("Steps to get to new position: "); Serial.println(steps);
}

void advance_symbol_wheel() {
  symbol_wheel.step(2);
  symbol_wheel_pos++;
}

void home_symbol_wheel() {
  home_x();
  symbol_wheel.step(-stepsPerRevolution*revolutions);
  advance_symbol_wheel();
  symbol_wheel_pos = 0;
}

void set_x(uint32_t position) {
  x_carriage.step(position - x_pos);
  x_pos = position;
}

void advance_x() {
  x_carriage.step(CHAR_WIDTH);
  x_pos += CHAR_WIDTH;
}

void decrement_x() {
  if (x_pos < CHAR_WIDTH) {
    return;
  }
  x_carriage.step(-CHAR_WIDTH);
  x_pos -= CHAR_WIDTH;
}

void write_character() {
  digitalWrite(motor_forward, HIGH);
  digitalWrite(solenoid_one, HIGH);
  delay(50);
  digitalWrite(solenoid_one, LOW);
  delay(50);
  digitalWrite(motor_forward, LOW);
}

// Not working yet
void erase_character() {
  digitalWrite(motor_forward, HIGH);
  digitalWrite(solenoid_one, HIGH);
  digitalWrite(solenoid_two, HIGH);
  delay(100);
  digitalWrite(motor_forward, LOW);
  digitalWrite(solenoid_two, LOW);
  digitalWrite(solenoid_one, LOW);
}

void new_line() {
  line_feed.step(-STEPS_PER_LINE);
}

void backwards_line() {
  line_feed.step(STEPS_PER_LINE);
}