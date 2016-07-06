#include <Arduino.h>
#include <EEPROM.h>

#include "_config.h"
#include "_types.h"

#include "lineReader.h"

int values[16];
int SENSOR_MIN;
int SENSOR_MAX;

int MUX_INS[] = {
  PIN_MUX_S0,
  PIN_MUX_S1,
  PIN_MUX_S2,
  PIN_MUX_S3
};

void LineReader::init(){
  pinMode(PIN_MUX_AN, INPUT);
  pinMode(PIN_MUX_S0, OUTPUT);
  pinMode(PIN_MUX_S1, OUTPUT);
  pinMode(PIN_MUX_S2, OUTPUT);
  pinMode(PIN_MUX_S3, OUTPUT);
  SENSOR_MIN = EEPROM.read(EEPROM_ADR_SENSOR_MIN);
  SENSOR_MAX = EEPROM.read(EEPROM_ADR_SENSOR_MAX);

}

void LineReader::readValues(){
	for(uint8_t i = 8; i < 16; i++){
		for(int n = 0; n < 4; n++){
			digitalWrite(MUX_INS[n], i & (0x1 << n));
		}
		values[i] = analogRead(PIN_MUX_AN);
	}
}

int LineReader::getValue(int index){
	return values[index];
}

float getPosition(){
  float weighted_sum = 0;
  float sum = 0;
  for(int i = 0; i < 9; i++){
    float value = map(getValue(i), SENSOR_MIN, SENSOR_MAX, 0, 1);
    weighted_sum = weighted_sum + value  * (i + 1);
    sum = sum + value;
  }
  float position = ((weighted_sum / sum) - 4.5) / 3.5;
  return position;
}