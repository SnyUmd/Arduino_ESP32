// 参考：https://kohacraft.com/archives/202006201124.html


#include "main.hpp"

// const char *ssid = "um50-fx";
// const char *password = "n17e92@53S19n";

Espalexa espalexa;

//**********************************************************************
void initPort()
{
  pinMode(PORT_LED_DARK, OUTPUT);
  pinMode(PORT_LED_BRIGHT, OUTPUT);
  pinMode(PORT_SW_INTERNAL, INPUT);

  pinMode(PORT_MORTER_0,OUTPUT);
  pinMode(PORT_MORTER_1,OUTPUT);
  pinMode(PORT_MORTER_2,OUTPUT);
  pinMode(PORT_MORTER_3,OUTPUT);

  // digitalWrite(PORT_LED_DARK, LED_OFF_EX);
  // digitalWrite(PORT_LED_BRIGHT, LED_OFF_EX);

  digitalWrite(PORT_MORTER_0, LOW);
  digitalWrite(PORT_MORTER_1, LOW);
  digitalWrite(PORT_MORTER_2, LOW);
  digitalWrite(PORT_MORTER_3, LOW);

  // ledcSetup(PWM_CH1, PWM_FRQ , PWM_BIT_NUM) ; //チャンネル , 周波数 , bit数(分解能)
  // ledcAttachPin(PORT_LED_INTERNAL , PWM_CH1) ; //端子番号 , チャンネル
  // ledcWrite(PWM_CH1, 0); //チャンネル , Duty比

  ledcSetup(PWM_CH1, PWM_FRQ , PWM_BIT_NUM) ; //チャンネル , 周波数 , bit数(分解能)
  ledcAttachPin(PORT_LED_DARK , PWM_CH1) ; //端子番号 , チャンネル
  ledcWrite(PWM_CH1, 255); //チャンネル , Duty比

  ledcSetup(PWM_CH2, PWM_FRQ , PWM_BIT_NUM) ; //チャンネル , 周波数 , bit数(分解能)
  ledcAttachPin(PORT_LED_BRIGHT , PWM_CH2) ; //端子番号 , チャンネル
  ledcWrite(PWM_CH2, 255); //チャンネル , Duty比
}

//**********************************************************************
void WifiSetup()
{
  WiFi.mode(WIFI_STA);
  Serial.printf("connecting to %s¥n", deviceSts.ssid);
  WiFi.begin(deviceSts.ssid, deviceSts.pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }

  Serial.println();
  Serial.print("WiFi connected: ");
  Serial.println(WiFi.localIP());
}

//**********************************************************************
//our callback functions
void firstLightChanged(uint8_t brightness) {
    if(MotorSet.blMotorAction) return;
    // if(brightness <= 1) return;
    int next_power = (brightness > 1) ? brightness / 2 : 1;
    int lp = next_power - deviceSts.now_power;
    bool bl_dark = (lp < 0) ? true: false;

    delay(1000);

    //do what you need to do here
    // showLED( brightness );
    Serial.println("【Alexa】");
    Serial.print("now power : ");
    Serial.println(deviceSts.now_power);
    Serial.print("next power : ");
    Serial.println(next_power);
    Serial.print("dark : ");
    Serial.println(bl_dark);
    Serial.print("loop : ");
    Serial.println(lp);

    MotorSet.blMotorAction = true;
    MotorSet.loop = std::abs(lp);
    MotorSet.is_dark = bl_dark;
    
    Serial.println("【MotorSet】");
    Serial.print("blMotorAction : ");
    Serial.println(MotorSet.blMotorAction);
    Serial.print("loop : ");
    Serial.println(MotorSet.loop);
    Serial.print("is dark : ");
    Serial.println(MotorSet.is_dark);

    // for(int i = 0; i < std::abs(lp); i++){
    //   motorAction(bl_dark, MORTER_STEP);
    // }
    // MotorSet.blMotorAction = false;

    deviceSts.now_power = next_power;
    EEP_Write2(eep, eepAdd.eep_address_now_power, String(next_power));

}

//**********************************************************************
void LedFlash(int led_power, int flash_num, int interval, int led_port, int logic_led_off){
  int pwmch;
  switch (led_port)
  {
  case PORT_LED_DARK:
    pwmch = PWM_CH1;
    break;
  case PORT_LED_BRIGHT:
    pwmch = PWM_CH2;
    break;
  default:
    break;
  }
  digitalWrite(led_port, logic_led_off);
  for(int i = 0; i < flash_num; i++){
    delay(interval / 2);
    ledcWrite(pwmch, 128);
    delay(interval / 2);
    ledcWrite(pwmch, 255);
  }
}
//**********************************************************************
void eepConfig(){
  String flg = EEP_Read(eep, eepAdd.eep_address_flg, END_CHAR, eep.length());
  if(flg != "1"){
    EEP_Write2(eep, eepAdd.eep_address_ssid, SSID_0);
    EEP_Write2(eep, eepAdd.eep_address_pass, PASS_0);
    EEP_Write2(eep, eepAdd.eep_address_device_name, DEVICW_NAME);
    EEP_Write2(eep, eepAdd.eep_address_now_power, "10");
    EEP_Write2(eep, eepAdd.eep_address_flg, "1");
  }

  deviceSts.ssid = EEP_Read(eep, eepAdd.eep_address_ssid, END_CHAR, eep.length());
  deviceSts.pass = EEP_Read(eep, eepAdd.eep_address_pass, END_CHAR, eep.length());
  deviceSts.device_name = EEP_Read(eep, eepAdd.eep_address_device_name, END_CHAR, eep.length());
  deviceSts.now_power = EEP_Read(eep, eepAdd.eep_address_now_power, END_CHAR, eep.length()).toInt();

  
  Serial.println("【init】");
  Serial.print("ssid : ");
  Serial.println(deviceSts.ssid);
  Serial.print("device name : ");
  Serial.println(deviceSts.device_name);
  Serial.print("now power : ");
  Serial.println(deviceSts.now_power);

}

//**********************************************************************
void task0(void* arg){
  while(1){

    if(MotorSet.blMotorAction){
      
        for(int i = 0; i < MotorSet.loop; i++){
          motorAction(MotorSet.is_dark, MORTER_STEP);
        }

        MotorSet.blMotorAction = false;
    }
    delay(1);
  }
}

//**********************************************************************
//**********************************************************************
void setup() {
    Serial.begin(115200);
    EEP_Init(eep, EEP_SIZE);
    eepConfig();

    initPort();
    LedFlash(200, 5, 200, PORT_LED_DARK, LED_OFF_EX);

    WifiSetup();
    LedFlash(100, 5, 200, PORT_LED_DARK, LED_OFF_EX);

    espalexa.addDevice(deviceSts.device_name, firstLightChanged,0); //simplest definition, default state off
    espalexa.begin();
    LedFlash(0, 5, 200, PORT_LED_BRIGHT, LED_OFF_EX);

    xTaskCreatePinnedToCore(task0, "task0", 4096, NULL, 1, NULL, 1);

    // for(int i = 0; i < 10; i++)
    // {
      // delay(100);
      // motorAction(false, 10);
    // }

}

//**********************************************************************
//**********************************************************************
void loop() {
   espalexa.loop();

  //  if(MotorSet.blMotorAction){
  //     for(int i = 0; i < std::abs(MotorSet.loop); i++){
  //       motorAction(MotorSet.is_dark, MORTER_STEP);
  //     }
  //     MotorSet.blMotorAction = false;
  //  }

   delay(1);
}

