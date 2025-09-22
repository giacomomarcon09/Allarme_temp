#include <Arduino.h>
  #include "Modbus.h"
  #include "ModbusSerial.h"


#define BAUD        115200 
#define ID             1   //slaveID
#define TXPIN         -1   //Disable DE/RE use UART TTL

ModbusSerial modbus;

//temp sensor
const unsigned int TEMP_SENSOR_PIN = A0;
const float SUPPLY_VOLTAGE = 5.0;
const int MAX_TEMP = 30;  //Allarm trigger

//buzzer
const unsigned int BUZZER_PIN = 2;
unsigned int frequency = 1000;  //Starting freq
unsigned int direction = 1;    //Freq going up 


void setup() {
  //buzzer
  pinMode(BUZZER_PIN, OUTPUT);

  //Config Modbus Serial (port, speed, rs485 tx pin)
  modbus.config(&Serial, BAUD, TXPIN);
  //Set the Slave ID
  modbus.setSlaveId(ID);
  //Add
  modbus.addCoil(0);
  modbus.addHreg(0);  
  modbus.addHreg(1);
}
 
void loop() {

  modbus.task();

  float temp = get_temperature();
  if(temp>MAX_TEMP){
    modbus.Coil(0,1);  //Coil(0) <- 1
  }

  if(modbus.Coil(0)){
    allarm();
  }

  uint16_t *ptr = (uint16_t*)&temp;
    modbus.Hreg(0, ptr[0]); // low word
    modbus.Hreg(1, ptr[1]); // high word
}

void allarm(){
  freqController();
  tone(BUZZER_PIN,frequency,20);
}

void freqController(){
  if(frequency<1000){
    direction = 1;
  }else if(frequency>2000){
    direction = -1;
  }
  frequency += direction*1;
}

float get_temperature() {
  int sensor_voltage = analogRead(TEMP_SENSOR_PIN); 
  float voltage = sensor_voltage * SUPPLY_VOLTAGE / 1024; 
  return (voltage * 1000 - 500) / 10;
}