//モータW　制御ポート
// #define PORT_MOTOR1_W 18
// #define PORT_MOTOR2_W 5
// #define PORT_MOTOR3_W 15
// #define PORT_MOTOR4_W 2

#define PORT_MOTOR1_W 26
#define PORT_MOTOR2_W 27
#define PORT_MOTOR3_W 14
#define PORT_MOTOR4_W 12

//モータL　制御ポート
// #define PORT_MOTOR1_F 19
// #define PORT_MOTOR2_F 23
// #define PORT_MOTOR3_F 12
// #define PORT_MOTOR4_F 14

#define PORT_MOTOR1_F 19
#define PORT_MOTOR2_F 18
#define PORT_MOTOR3_F 5
#define PORT_MOTOR4_F 17

//LED制御ポート
// #define PORT_LED_W 16
// #define PORT_LED_F 4

#define PORT_LED_W 2
#define PORT_LED_F 23

//スイッチ制御ポート(コントロールボードSW203)
#define PORT_SW 0

//ブザー制御ポート
#define PORT_BUZZER 25

//モーター open / close
#define MOTOR_OPEN false
#define MOTOR_CLOSE true

//モータ駆動量
#define MOTOR_RANGE 125

//オープン時間の長さ
//※オープン時間 = DEFAULT_LENGTH - モータ駆動時間
#define DEFAULT_LENGTH 3

//LED ON/OFF
// #define LED_ON 0
// #define LED_OFF 1

#define LED_ON 1
#define LED_OFF 0

//接続Wi-Fi
#define SSID "barnet24" 
#define PASS "t8diw56s7sxie"

//デバイスホスト名
//ホスト名 = HOST_NAME + .airport
#define HOST_NAME "petoasis" // http://petoasis.airport/
