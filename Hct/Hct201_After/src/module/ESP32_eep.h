
void EEP_Init(EEPROMClass& eep);
// void EEP_Write(String val);
void EEP_Write(EEPROMClass& eep, int write_address, String val);
// String EEP_Read(char end_char, int end_num);
String EEP_Read(EEPROMClass& eep, int read_address, char end_char, int end_num);