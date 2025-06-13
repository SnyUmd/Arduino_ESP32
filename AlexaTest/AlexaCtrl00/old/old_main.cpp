#include "main.h"


//**********************************************************************
void setup() {
  Serial.begin(115200);
  initPort();
  LedFlash(2, 500);
  WifiSetup();
  SetupFauxmo();

}

//**********************************************************************
void loop() {
  fauxmo.handle();
  delay(10);
}

//**********************************************************************
//**********************************************************************
//**********************************************************************
void initPort()
{
  pinMode(LED_INTERNAL, OUTPUT);
  pinMode(SW_INTERNAL, INPUT);

  digitalWrite(LED_INTERNAL, LED_OFF);
}

//**********************************************************************
void WifiSetup()
{
  WiFi.mode(WIFI_MODE_NULL);
  Serial.printf("connecting to %s¥n", ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }

  Serial.println();
  Serial.print("WiFi connected: ");
  Serial.println(WiFi.localIP());
  LedFlash(5, 100);
}

//**********************************************************************
void SetupFauxmo(){
  fauxmo.createServer(true);
  // fauxmo.setPort(80);  //※
  fauxmo.enable(true);

  fauxmo.addDevice(id_device);

  fauxmo.onSetState([](unsigned char device_id, const char *device_name, bool state, unsigned char value) {
      Serial.printf("Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);

      if (strcmp(device_name, id_device) == 0) {
        digitalWrite(LED_INTERNAL, state ? HIGH : LOW);
      }
    });

    /*
  // You have to call enable(true) once you have a WiFi connection
    // You can enable or disable the library at any moment
    // Disabling it will prevent the devices from being discovered and switched
    fauxmo.createServer(true);
    // fauxmo.setPort(80);  //※
    fauxmo.enable(true);

    // Add virtual devices
    fauxmo.addDevice("照明");

    // fauxmoESP 2.0.0 has changed the callback signature to add the device_id,
    // this way it's easier to match devices to action without having to compare strings.
    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
        Serial.printf("[MAIN] Device #%d (%s) state: %s\n", device_id, device_name, state ? "ON" : "OFF");
        digitalWrite(LED_INTERNAL, state);
    });

    // Callback to retrieve current state (for GetBinaryState queries)
    // fauxmo.onGetState([](unsigned char device_id, const char * device_name, unsigned char value) {
    //     return digitalRead(LED_INTERNAL);
    // });
    */
  }

//**********************************************************************
void LedFlash(int flash_num, int interval){
  digitalWrite(LED_INTERNAL, LED_OFF);
  for(int i = 0; i < flash_num; i++){
    delay(interval);
    digitalWrite(LED_INTERNAL, LED_ON);
    delay(interval);
    digitalWrite(LED_INTERNAL, LED_OFF);
  }
}
