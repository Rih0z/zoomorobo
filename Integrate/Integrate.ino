#include <Wire.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <LSM303.h>


ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
LSM303 compass;

#define SPEED 170 				//Zumoのモータに与える回転力の基準値 

float red_G, green_G, blue_G; 			//カラーセンサーで読み取ったRGB値（0-255）
int zoneNumber_G; 				//ゾーン番号を表す状態変数
int mode_G; 					//各ゾーンでのモードを表す状態変数
unsigned long timeInit_G, timeNow_G; 		//スタート時間，経過時間
int motorR_G, motorL_G;				//左右のZumoのモータに与える回転力

float direction_G;				//Zumoが向いている角度

//大西
const int buzzerPin =  3;			//ブザーは3番ピン
const int ledPin    = 13;			//LEDは13番ピン
const int trig = 2;				//Trigピンを2番ピンに
const int echo = 4;				//Echoピンを4番ピンに
int distance_G;					//対象物との距離(cm)
int dirmin_G;
int dir_G;
//riho
int colorCheck_G = 3 ;
float avex = 0;
int identifyColor( int red, int green, int blue );
void turnRight( int speed );
void goStraight( int speed );
int steadyState( unsigned long period );
float turnTo(float dir);
void zoneToZone(void);
void zone_linetrace(void);
void zone_curling(void);
static int ChackC(void) ;
static int CheckB(void);
static int Mode_C = 2;
static int countr;
static int mode_Gs;
void write2byte(int x);
//nakazi
int identifyColor2( int red, int green, int blue, int value );
static float V_L = 0;   //左タイヤの速さ
static float V_R = 0;   //右タイヤの速さ
static float V_C = 0;   //中心の速さ
static float V_x = 0;   //中心の速さのx方向成分
static float V_x1 = 0;  //x方向成分の1つ先
static float V_y = 0;   //中心の速さのy方向成分
static float V_y1 = 0;  //y方向成分の1つ先
static float dt = 0.1; //0.1秒ごとに現在位置を更新
static float X = 0;     //過去のX座標
static float X_1 = 0;   //現在のX座標
static float Y = 0;     //過去のY座標
static float Y_1 = 0;   //現在のY座標
static int TurnC;
static int POINT_F = 0;
float diff;

static int traceflag = 0;//ライントレースしているかどうか
void setup()
{
  Serial.begin(9600);
  Wire.begin();

  button.waitForButton();
  setupColorSensor(); 			//カラーセンサーのsetup
  //calibrationColorSensorWhite(); 	//カラーセンサーのキャリブレーション
  //button.waitForButton();
  //calibrationColorSensorBlack(); 	//カラーセンサーのキャリブレーション
  calibrationColorSensorManual(); 	//カラーセンサーのキャリブレーション（手動設定）

  setupCompass(); 			//地磁気センサーのsetup
  calibrationCompass(); 			//地磁気センサーのキャリブレーション
  //CalibrationCompassManual(); 		//地磁気センサーのキャリブレーション（手動設定）

  zoneNumber_G = 0;
  mode_G = 2; 				//スタート時はzoneToZoneの途中から
  button.waitForButton();
  timeInit_G = millis();

  //実験のために措置的に設定
  //zoneNumber_G = 2;
  //mode_G = 0;

  //大西
  pinMode(ledPin, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
}

void loop() {
  readRGB(); 				//カラーセンサでRGB値を取得(0-255)
  timeNow_G = millis() - timeInit_G; 	//経過時間
  static unsigned long timePrev_Z = 0;
  motors.setSpeeds(motorL_G, motorR_G); 	//左右モーターへの回転力入力
  direction_G = averageHeadingLP();	//Zumoが向いている角度を格
  sendData(); 				//データ送信

  //Zumo buttonが押されていればtrue，そうでなければ false
  if (button.isPressed()) {
    zoneNumber_G = 0; 		//zoneToZone に移行
    mode_G = 0;
    delay(100);
  }

  //実験のために措置的に設定
  //zoneNumber_G = 2;

  /*static unsigned long tp = timeNow_G;

    //規定時間が経過したら次のゾーンに向かう
    if(zoneNumber_G != 0){
  	if(timeNow_G - tp > 9999999999999999999999){
  		zoneNumber_G = 0;
  		mode_G = 0;
  	}
    }
    else{
  	tp = timeNow_G;
    }*/

  switch (zoneNumber_G) {
    case 0:
     timePrev_Z = timeNow_G;
      zoneToZone(); 		//zone to zone (start to zone)
      break;
    case 1:
      if (timeNow_G - timePrev_Z < 50000){
        zone_linetrace();
      }
      if (timeNow_G - timePrev_Z > 50000) {
        diff = turnTo(110);
        motorL_G = SPEED + diff;
        motorR_G = SPEED - diff;
        if ( diff < 10 ) { 
        zoneNumber_G = 0;
        mode_G = 0;
        }
      }
      break;
    case 2:
      if (timeNow_G - timePrev_Z < 50000){
      zone_curling(); 	//zone 2
      }
      if (timeNow_G - timePrev_Z > 50000) {
        diff = turnTo(110);
        motorL_G = SPEED + diff;
        motorR_G = SPEED - diff;
        if ( diff < 10 ) { 
        zoneNumber_G = 0;
        mode_G = 0;
        }
      }
      break;
    case 3:
      if (timeNow_G - timePrev_Z < 50000){
      zone3beta();
      }
      if (timeNow_G - timePrev_Z > 50000) {
        diff = turnTo(193);
        motorL_G = SPEED + diff;
        motorR_G = SPEED - diff;
        if ( diff < 10 ) { 
        zoneNumber_G = 0;
        mode_G = 0;
        }
      }
      break;
      //zone_curling();		//zone 3
      break;
    case 4:
      //zone_linetrace(); 	//winning action
      break;
    default:
      break;
  }
}

//通信方式2
void sendData()
{
  static unsigned long timePrev = 0;
  static int inByte = 0;
  static float mx, my, mz;
  static float V_L = 0.0;   //左タイヤの速さ
  static float V_R = 0.0;   //右タイヤの速さ
  static float V_C = 0.0;   //中心の速さ
  static float V_x = 0.0;   //中心の速さのx方向成分
  static float V_x1 = 0.0;  //x方向成分の1つ先
  static float V_y = 0.0;   //中心の速さのy方向成分
  static float V_y1 = 0.0;  //y方向成分の1つ先
  static float dt = 0.05; //0.1秒ごとに現在位置を更新
  static float X = 0.0;     //過去のX座標
  static float X_1 = 0.0;   //現在のX座標
  static float Y = 0.0;     //過去のY座標
  static float Y_1 = 0.0;   //現在のY座標
  
  
  // 50msごとにデータ送信（通信方式２），500msデータ送信要求なし-->データ送信．
  if ( inByte == 0 || timeNow_G - timePrev > 500 || (Serial.available() > 0 && timeNow_G - timePrev > 50)) { // 50msごとにデータ送信
    inByte = Serial.read();
    inByte = 1;
    // Serial.print(direction_G);

    dt = ((float)timeNow_G - (float)timePrev) / 1000.0;
    //Serial.println(direction_G);
    V_L = (float)motorL_G;
    V_R = (float)motorR_G;
    V_C = (V_L + V_R) / 2.0; //中心の速度

    V_x = V_x1; //過去のx成分、y成分
    V_y = V_y1;

    V_x1 = V_C * cos( ((float)direction_G * 3.14) / 180.0);
    V_y1 = V_C * sin( ((float)direction_G * 3.14) / 180.0);

    X = X_1; //通過後の地点
    Y = Y_1; //

    X_1 = X + (0.5 * (V_x1 + V_x)) * dt;  //現在のX座標
    Y_1 = Y + (0.5 * (V_y1 + V_y)) * dt;  //現在のY座標

 
    Serial.write('H');
    write2byte(compass.a.x);  write2byte(compass.a.y);  write2byte(compass.a.z);
    mx = compass.m.x;  my = compass.m.y;  mz = compass.m.z;
    mx = map(mx,compass.m_min.x,compass.m_max.x,-100,100);
  my = map(my,compass.m_min.y,compass.m_max.y,-100,100);
  mz = map(mz,compass.m_min.z,compass.m_max.z,-100,100);  
    write2byte((int)mx);  write2byte((int)my);  write2byte((int)mz); write2byte((int)direction_G);
    Serial.write((int)colorCheck_G);
   // Serial.write(zoneNumber_G);
   // Serial.write(mode_G);
   /* Serial.write((int)(100*direction_G) >> 8); // 100 倍して整数化(小数点 2 位まで送信)
    Serial.write((int)(100*direction_G) & 255); // 整数化した時の値が-32768∼32767 の範囲*/

    Serial.write((int)red_G );
    Serial.write((int)green_G );
    Serial.write((int)blue_G );

    Serial.write(motorR_G );
    Serial.write(motorL_G );

    Serial.write(zoneNumber_G);

    Serial.write(mode_G);

    timePrev = timeNow_G;
  }
}
void write2byte(int x) {
  Serial.write(x >> 8);
  Serial.write(x & 255);
}
