#include "Arduino_Ini_Setup.h"


//*************************************************************************************************
bool mArduino_Serial_Set(HardwareSerial& HS_serial)
{
	HS_serial.begin(115200);
	if (!HS_serial)
		return false;
	else
		return true;
}

//*************************************************************************************************
void mSetPort_Out(int i_port_num, bool bl_high)
{
	pinMode(i_port_num, OUTPUT);
	if (bl_high)
		digitalWrite(i_port_num, 1);
	else
		digitalWrite(i_port_num, 0);
}

//*************************************************************************************************
void mSetPort_Imput(int i_port_num, bool bl_pull, bool bl_high)
{
	if (bl_pull)
	{
		if (bl_high)
			pinMode(i_port_num, INPUT_PULLUP);
		else
			pinMode(i_port_num, INPUT_PULLDOWN);
	}
	else
		pinMode(i_port_num, INPUT);
}

//*************************************************************************************************
void mErrLoop(int i_port_num, int i_delay_time)
{
	while (1)
	{
		digitalWrite(i_port_num, !digitalRead(i_port_num));
		delay(i_delay_time);
	}
}


//**************************************************************************************************************************
//**************************************************************************************************************************
//**************************************************************************************************************************
//**************************************************************************************************************************
//ESP32
//**************************************************************************************************************************
//**************************************************************************************************************************
//**************************************************************************************************************************
//**************************************************************************************************************************

bool mESP32_BLSerial_Set(BluetoothSerial& BS_Serial, String strBT_Name, HardwareSerial HS_serial)
{
	if (!BS_Serial.begin(strBT_Name))
	{
		HS_serial.println("Err BT");
		return false;
	}
	else
		return true;

	//BS_Serial.begin(strBT_Name);
	//return true;
}

//*************************************************************************************************
int mESP32_BLSerial_Read(BluetoothSerial& BT_Serial)
{
	if (0 < BT_Serial.available())
		return BT_Serial.read();
	else
		return -1;
}





//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//Wi-Fi
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
bool mESP32_WiFi_Set
(
	
	WiFiClass& wifi_class,
	WiFiServer& wifi_server,
	HardwareSerial& hs_serial,
	char ch_ssid[],
	char ch_pass[],
	int i_signal_portNum,
	bool bl_serial_message
)
{
	/*
	int iCnt = 0;
	String strMessage = "";

	wifi_class.begin(ch_ssid, ch_pass);

	while (wifi_class.status() != WL_CONNECTED)
	{
		if (bl_serial_message)
			hs_serial.print(".");
		delay(100);
		digitalWrite(i_signal_portNum, !digitalRead(i_signal_portNum));
		iCnt++;

		if (iCnt > 20)
		{
			wifi_class.disconnect();
			//wifi_class.begin(ch_ssid, ch_pass);
			//iCnt = 0;
			return false;
		}
	}
	digitalWrite(i_signal_portNum, 1);

	wifi_server.begin();

	if (bl_serial_message)
	{
		hs_serial.println(" connected");
		hs_serial.print("HTTP Server: http://");
		hs_serial.print(wifi_class.localIP());
		hs_serial.println("/");
	}

	return true;
	*/

	int iCnt = 0;
	String strMessage = "";

	wifi_class.begin(ch_ssid, ch_pass);

	while (wifi_class.status() != WL_CONNECTED)
	{
		if (bl_serial_message)
			hs_serial.print(".");
		delay(100);
		digitalWrite(i_signal_portNum, !digitalRead(i_signal_portNum));
		iCnt++;

		if (iCnt > 20)
		{
			wifi_class.disconnect();
			//wifi_class.begin(ch_ssid, ch_pass);
			//iCnt = 0;
			return false;
		}
	}
	digitalWrite(i_signal_portNum, 1);

	wifi_server.begin();

	if (bl_serial_message)
	{
		String strTest;
		strTest = "HTTP Server: http://"; 
		strTest += wifi_class.localIP();
		strTest += "/";
		hs_serial.println(" connected");
		delay(1000);
		hs_serial.println(strTest);
		delay(1000);
		//hs_serial.println(wifi_class.localIP());
		//delay(1000);
		//hs_serial.println("/");
	}

	return true;
}


//*************************************************************************************************
//現在時刻の取得
//WiFiに接続されている事前提
//初回読込時は、初期値が読み込まれてしまう。
//始めは、下記のように時間を空けて２回読むことで、現在時刻となる。

//delay(2500);
//strStartTime = mGetTime_jst();
//delay(2500);
//strStartTime = mGetTime_jst();
//*************************************************************************************************
String mGetTime_jst()
{
	time_t t;
	struct tm* tm;
	String strReturn = "";
	String strBuf;
	int iBuf;
	static const char* wd[7] = { "Sun","Mon","Tue","Wed","Thr","Fri","Sat" };

	configTime(3600 * 9, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");

	t = time(NULL);
	tm = localtime(&t);

	strReturn += tm->tm_year + 1900;
	strReturn += "/";
	strReturn += tm->tm_mon + 1;
	strReturn += "/";
	strReturn += tm->tm_mday;
	strReturn += "/";
	strReturn += wd[tm->tm_wday];
	strReturn += " - ";

	if (tm->tm_hour < 10)
		strReturn += "0";
	strReturn += tm->tm_hour;
	strReturn += ":";
	if (tm->tm_min < 10)
		strReturn += "0";
	strReturn += tm->tm_min;
	strReturn += ":";
	if (tm->tm_sec < 10)
		strReturn += "0";
	strReturn += tm->tm_sec;

	return strReturn;

}




//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//Multi task
//xTaskCreatePinnedToCore(Task_2nd, "Task2nd", 4096, NULL, 1, NULL, 1);
//
//BaseType_t xTaskCreatePinnedToCore(
//TaskFunction_t pxTaskCode,         	//< タスクの関数ポインタ
//const char* const pcName,         	//< タスク名
//const uint16_t usStackDepth,       	//< タスクのスタックサイズ
//void* const pvParameters,         	//< タスク関数に渡す引数
//UBaseType_t uxPriority,            	//< タスクの優先度
//TaskHandle_t* const pxCreatedTask,	//< タスクのハンドル
//const BaseType_t xCoreID           	//< コア番号(0 or 1)
//);

//マルチタスクについて-----------------------------------
//xTaskCreatePinnedToCore
//(
//	loop2, //タスク名
//	"loop2", //"タスク名"自由
//	4096//スタックメモリサイズ
//	NULL, //NULL
//	1, //タスク優先順位
//	NULL,// タスクハンドルポインタ NULL
//	1//Core ID
//);
//void loop2(void* pvParameters)	//別タスクで動かすメソッド
//{
//}
//*************************************************************************************************
void mTask_Set(TaskFunction_t task_2nd, const char* name, uint32_t i32t_StackDepth, void* pvParameters, UBaseType_t uxPriority,
	TaskHandle_t* pvCreatedTask, BaseType_t xCoreID)
{
	//xTaskCreatePinnedToCore(task_2nd, name, 4096, NULL, 1, NULL, 1);
	xTaskCreatePinnedToCore(task_2nd, name, i32t_StackDepth, pvParameters, uxPriority, pvCreatedTask, xCoreID);
}

//*************************************************************************************************
//void mTask_2nd(void* pvParameters)
void mTask_2nd(void* arg)
{
	//mPWM_Out_iniSet(0, A19, 10000, 8, 50);
	//delay(500);
	//mPWM_Out_iniSet(0, A19, 1, 8, 0);
	//delay(500);

}


//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
float mGetTemperature_BuiltIn()
{
	return temperatureRead();
}


//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//PWM out
//IO32=A4, IO33=A5, IO26=A19, IO25=A18, IO27=A17, IO14=A16
//IO12=A15, IO13=A14, IO15=A13, IO4=A10, IO2=A12, ??=A11
void mPWM_Out_iniSet(int i_ch_no, int i_port, int i_freq, int i_resolution, int i_percent)
{
	ledcSetup(i_ch_no, i_freq, i_resolution);
	ledcAttachPin(i_port, i_ch_no);

	float flBuf1 = (float)i_percent / 100;
	float flBuf2 = powf(16, (float)i_resolution / 4);

	//ledcWrite(i_ch_no, (int)((float)(i_percent / 100) * (16 ^ (i_resolution / 4))));
	ledcWrite(i_ch_no, (uint32_t)(flBuf1 * flBuf2));
}

//*************************************************************************************************
void mPWM_Out(int i_ch_no, int i_resolution, int i_percent)
{
	float flBuf1 = (float)i_percent / 100;
	float flBuf2 = powf(16, (float)i_resolution / 4);

	//ledcWrite(i_ch_no, (int)((float)(i_percent / 100) * (16 ^ (i_resolution / 4))));
	ledcWrite(i_ch_no, (uint32_t)(flBuf1 * flBuf2));
}




//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
void mBeep(int i_port_no_r, int i_port_no_l, bool bl_r, bool bl_l, int i_beep_time, int i_vol)
{
	for (int i = 0; i <= i_beep_time * 2; i++)
	{
		if (bl_r)
			dacWrite(i_port_no_r, 0);
		if (bl_l)
			dacWrite(i_port_no_l, 0);
		delayMicroseconds(i_beep_time);

		if (bl_r)
			dacWrite(i_port_no_r, i_vol);
		if (bl_l)
			dacWrite(i_port_no_l, i_vol);
		delayMicroseconds(i_beep_time);
	}

	if (bl_r)
		dacWrite(i_port_no_r, 0);
	if (bl_l)
		dacWrite(i_port_no_l, 0);
}