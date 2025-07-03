#include <EEPROM.h>

void EEP_Init(EEPROMClass& eep, int);
// void EEP_Write(String val);
void EEP_Write(EEPROMClass& eep, int write_address, String val);
bool EEP_Write2(EEPROMClass& eep, int write_address, String val);
// String EEP_Read(char end_char, int end_num);
String EEP_Read(EEPROMClass& eep, int read_address, char end_char, int end_num);
