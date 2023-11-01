#include <EEPROM.h>

void EEP_Init(EEPROMClass& eep)
{
    eep.begin(100);
}

void EEP_Write(EEPROMClass& eep, int write_address, String val)
{
    int size = val.length();
    auto c = val.c_str();
    int cnt = 0;
    for(int i = 0; i < size; i++)
    {
        eep.put(i + write_address, c[i]);
        cnt++;
    }
    eep.put(cnt + write_address, '*');
    eep.commit();
}

String EEP_Read(EEPROMClass& eep, int read_address, char end_char, int end_num)
{
    String result_str = "";
    char c[end_num];
    char c_buf;
    int i = 0;
    while(1)
    {
        eep.get(i + read_address, c_buf);
        if(c_buf == end_char) break;
        result_str += c_buf;
        i++;
        if(i >= end_num) break;
    }
    return result_str;
}