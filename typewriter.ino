#include "movement.h"

const int SERIAL_BUFFER_SIZE = 512;
char serial_buffer[SERIAL_BUFFER_SIZE];

void demo_symbol_wheel() {
  home_symbol_wheel();
  for (int j = 0; j < 8; j++) {
  for (int i = 0; i < 64; i++) {
    write_character();
    advance_symbol_wheel();
    advance_x();
  }
  home_x();
  line_feed.step(-36);
  }
}

void print_char(char input) {
  switch(input) {
    case ' ':
      advance_x();
      break;
    case '\n':
      new_line();
      set_x(0);
      break;
    case '\r':
      new_line();
      set_x(0);
      break;
    case '\t':
      advance_x();
      advance_x();
      advance_x();
      advance_x();
      break;
    case 8: // Backspace
    case 127:
      decrement_x();
      break;
    case 27:
      backwards_line();
    default:
      go_to_symbol(input);
      write_character();
      advance_x();
      break;
  }
}

void print_string(const char* input) {
  //const char* message = "This is a demonstration.";
  for (int i = 0; input[i] != 0; i++) {
    print_char(input[i]);
  }
  //new_line();
  //Serial.print("Printed: ");
  //Serial.println(message);
}

void print_from_serial() {
  while(true) {
    while (!Serial.available());
    uint16_t serial_buffer_index = 0;
    while (Serial.available() && serial_buffer_index < SERIAL_BUFFER_SIZE) {
      serial_buffer[serial_buffer_index] = Serial.read();
      Serial.write(serial_buffer[serial_buffer_index]);
      serial_buffer_index++;
    }
    for (uint16_t i = 0; i < serial_buffer_index; i++) {
      print_char(serial_buffer[i]);
    }
  }
}

void setup() {
  // set the speed at 60 rpm:
  // 800 for X carriage
  // 
  Serial.begin(9600);
  build_symbol_table();
  pinMode(x_limit, INPUT);
  pinMode(solenoid_one, OUTPUT);
  pinMode(solenoid_two, OUTPUT);
  pinMode(motor_forward, OUTPUT);
  pinMode(motor_back, OUTPUT);
  
  symbol_wheel.setSpeed(speed + 100);
  x_carriage.setSpeed(speed);
  line_feed.setSpeed(speed);
  // initialize the serial port:
  home_symbol_wheel();
  spin_motor();
  print_from_serial();
}

void loop() {
  while (!Serial.available());
  switch (Serial.read()) {
  case 'a':
    x_carriage.step(-1);
    break;
  case 'd':
    x_carriage.step(1);
    break;
  case 'w':
    line_feed.step(-1);
    break;
  case 's':
    line_feed.step(1);
    break;
  case 'q':
    symbol_wheel.step(-1);
    break;
  case 'e':
    symbol_wheel.step(1);
    break;
  case 'h':
    home_symbol_wheel();
    break;
  case 'p':
    write_character();
    break;
  case 'b':
    erase_character();
    break;
  case 'o':
    spin_motor();
    break;
  case ';':
    demo_symbol_wheel();
    break;
  //case 'u':
  //  just_solenoid_two();
  //  break;
  }
}
