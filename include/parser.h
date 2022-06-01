// Thanks to Antonin RAFFIN https://medium.com/@araffin/simple-and-robust-computer-arduino-serial-communication-f91b95596788
// code copied from https://github.com/araffin/cpp-arduino-serial/tree/2075fd8f203483af9c82caffba4bdbc84024ea63

#include<Arduino.h>
#include<stdint.h>
#include<stddef.h>
#include "BluetoothSerial.h"



void update_motors_orders();

enum Command {
  HANDSHAKE = 101,
  ALREADY_CONNECTED = 105,
  ERROR = 108,
  RECEIVED = 100,
  BYE = 102,
  LED = 106,
  MOTOR= 107,
  FLOAT_TEST = 109,
};

typedef enum Command command;

void stop();





Command read_order();


float readFloat();

void write_float(float num);

void wait_for_bytes(int num_bytes, unsigned long timeout);


void read_signed_bytes(int8_t* buffer, size_t n);


int8_t read_i8();

int16_t read_i16();

int32_t read_i32();

void write_order(enum Command command);

void write_i8(int8_t num);


void write_i16(int16_t num);


void write_i32(int32_t num);


void get_messages_from_serial();

void sendPacket(int8_t size, Command command, int8_t data);

void sendHeader(int8_t size, Command command);