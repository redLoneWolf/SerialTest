#include <Arduino.h>

#include <parameters.h>
#include <parser.h>


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


BluetoothSerial SerialBT;
boolean connected = false;
char PREAMBLE = '$';

uint8_t serialBufferRX[64];
uint8_t count = 0;
// BluetoothSerial SerialBT;

void setup()
{

  pinMode(LED_BUILTIN, OUTPUT);
  // Serial.begin(9600);
    SerialBT.begin("ESP32_Robojax"); //Bluetooth device name

  
}

void led()
{
  int8_t state = read_i8();

 // write_i8(state);
  if (state == 1)
  {
    digitalWrite(LED_BUILTIN,LOW);
    connected = false;
  }
  else
  {
    connected = true;
    digitalWrite(LED_BUILTIN,HIGH);
  }

    sendPacket(4, LED, 1);
  // write_i8(102);
  // write_i8(103);
  // write_i8(104);
}

void sendHeader(int8_t size, Command command)
{
  write_i8((byte)PREAMBLE); // $

  write_i8(size); // size

  write_i8((byte)command); // command
}

void sendPacket(int8_t size, Command command, int8_t data)
{
  sendHeader(size, command);

  int t = 0;
  while (t <= size)
  {
    serialBufferRX[t] = t;
    write_i8(t);
    t++;
  }

  // switch (size)
  // {
  // case 1:
  //   write_i8(data);
  //   break;

  // case 2:
  //   write_i16(data);
  //   break;

  // case 4:
  //   write_i32(data);
  //   break;

  // default:
  //   break;
  // }
  // command
}


unsigned long previousMillis = 0;
const long interval = 500;

void checkCommand(Command command);

void checkConnected()
{
  if (connected)
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void loop()
{

  checkConnected();

  if (SerialBT.available() > 0)
  {
    if ((byte)read_i8() == (byte)PREAMBLE)
    {
      int8_t size = read_i8();
      if (size > 0)
      {
        Command command = (Command)read_i8();
        checkCommand(command);
      }
    }else
    {
      /* code */
    }
    
  }
}

void checkHandshake()
{
  if (connected)
  {
    // write_i8(ALREADY_CONNECTED);
    while (count <= 3)
    {
      serialBufferRX[count] = 125;
      count++;
    }
    sendPacket(count, ALREADY_CONNECTED, 1);

    // write_float(12.3);
    
  }
  else
  {
    connected = true;
    // write_i8(HANDSHAKE);

    while (count <= 3)
    {
      serialBufferRX[count] = 125;
      count++;
    }
    
    sendPacket(count, HANDSHAKE, 1);
    // write_float(12.2);
  }
}

void checkCommand(Command command)
{

  switch (command)
  {
  case HANDSHAKE:
    checkHandshake();
    break;

  case LED:
    led();
    break;

  default:
    write_i8(ERROR);
    break;
  }
}








void wait_for_bytes(int num_bytes, unsigned long timeout)
{
  unsigned long startTime = millis();
  //Wait for incoming bytes or exit if timeout
  while ((SerialBT.available() < num_bytes) && (millis() - startTime < timeout))
  {
  }
}

Command read_order()
{
  return (Command)read_i32();
}

// NOTE : SerialBT.readBytes is SLOW
// this one is much faster, but has no timeout
void read_signed_bytes(int8_t *buffer, size_t n)
{
  size_t i = 0;
  int c;
  while (i < n)
  {
    c = SerialBT.read();
    if (c < 0)
      break;
    *buffer++ = (int8_t)c; // buffer[i] = (int8_t)c;
    i++;
  }
}

int8_t read_i8()
{
  wait_for_bytes(1, 100); // Wait for 1 byte with a timeout of 100 ms
  return (int8_t)SerialBT.read();
}

int16_t read_i16()
{
  int8_t buffer[2];
  wait_for_bytes(2, 100); // Wait for 2 bytes with a timeout of 100 ms
  read_signed_bytes(buffer, 2);
  return (((int16_t)buffer[0]) & 0xff) | (((int16_t)buffer[1]) << 8 & 0xff00);
}

int32_t read_i32()
{
  int8_t buffer[4];
  wait_for_bytes(4, 200); // Wait for 4 bytes with a timeout of 200 ms
  read_signed_bytes(buffer, 4);
  return (((int32_t)buffer[0]) & 0xff) | (((int32_t)buffer[1]) << 8 & 0xff00) | (((int32_t)buffer[2]) << 16 & 0xff0000) | (((int32_t)buffer[3]) << 24 & 0xff000000);
}

void write_order(enum Command command)
{
//   uint8_t *Order = (uint8_t *)&command;
  write_i32(command);
}

void write_i8(int8_t num)
{
  SerialBT.write(num);
}

float readFloat(){

    int8_t buffer[4];
    for (int i = 0; i <4; i++)
    {
      buffer[i] = read_i8();
    }
    

    float x = *(float *)&buffer;
    return x;
}

void write_float(float num)
{

  SerialBT.write((byte*)&num, sizeof(float));

}
void write_i16(int16_t num)
{
  int8_t buffer[2] = {(int8_t)(num & 0xff), (int8_t)(num >> 8)};
  SerialBT.write((uint8_t *)&buffer, 2 * sizeof(int8_t));
}

void write_i32(int32_t num)
{
  int8_t buffer[4] = {(int8_t)(num & 0xff), (int8_t)(num >> 8 & 0xff), (int8_t)(num >> 16 & 0xff), (int8_t)(num >> 24 & 0xff)};
  SerialBT.write((uint8_t *)&buffer, 4 * sizeof(int8_t));
}