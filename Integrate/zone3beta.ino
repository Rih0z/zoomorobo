//
// Zone 3 Beta http://r-dimension.xsrv.jp/classes_j/1_interactive3d/ PROCESSING
//
#define LEVEL 500//登頂した時の判定
#define LEVEL2 730//下山した時の判定
#define SROPE 4500
static int redF = 0 ;
static int blueF = 0 ;
const float direction_exitbeta[3] = { //3
  279, 28, 279
};
//avex = 0;

float turnTo(float dir) {
  float heading, diff;

  heading = direction_G;
  if (heading > 180) heading -= 360;
  heading -= dir;
  if (heading > 180) heading -= 360;
  if (heading < -180) heading += 360;
  diff = -heading * 5;          // P-制御
  if (diff > 200) diff = 200;   // 飽和
  if (diff < -200) diff = -200; // 飽和
  return diff;
}
void zone3beta()
{
  static int pointF2 = 0 ;
  static int eFlag = 0 ;
  static int blackF = 0 ;
  static int countTurn = 0;
  static int dirFlag;
  float direction_target;
  int done;
  float speed0, diff;
  static int down_flag = 0 ;
  static int rideF ;
  avex = 0.9 * avex + 0.1 * compass.a.x;
  static int firstF = 0 ;
  static int downF = 0 ;
  static int countave;

  static float ave_avex;

  static int dirNow;
  static int directionIn;

  static int pointF  = 0;
  const float direction_3[3] = { // �e�]�[��(1-3)�̒E�o�����̊p�x�i�e���Őݒ�j
    192, 28, 279//1番目 右にまわる ・・90度足したら入場側，90度引いたら入場と反対側
  };
  static int turnFlag = 0 ;
  Serial.println("avex");
  Serial.println(avex);
  if (firstF == 0)
  {
    firstF = 1;
    // mode_Gs = mode_G
    mode_G = 0 ;
  }
  if (blackF)
    CheckB();


  switch ( mode_G ) {
    case 0: // setupはここ
      countTurn = 0;
      mode_G = 1;
      pointF = 0;
      blackF = 1;
      break;
    case 1: // 山探し(ここでは直進)
      if (avex > SROPE ) { // 登り始めたら
        blackF = 0;
        // rideF = 1;
        mode_G = 111;
        diff = 0 ;
        directionIn = direction_G;
        //motors.setSpeeds(0, 0);
        //delay(199999);
      }
      speed0 = 200;
      // diff = -0.02 * compass.a.y; // P-制御
      if (steadyState(1000))
        pointF = 1;
      break;
    case 111:
      //if (rideF)
      //  {
      done = steadyState(100);
      if ( done == 1 ) {
        mode_G = 112;
        rideF = 0 ;
        diff = 0 ;
      }
      speed0 = -130;
      diff = 0;
      //   }

      break;
    case 112:
      done = steadyState(100);
      speed0 = 80;
      diff = 80;
      if (downF)
        diff = -diff;
      if (done)
      {
        mode_G = 2;
        if (downF)
        {
          downF = 0 ;
          mode_G = 4;
        }
      }
      break;
    case 2:
      if (redF) {
        redF = 0 ;
        blueF = 0 ;
        mode_G = 23;
      } else {
        redF = 0 ;
        blueF = 0 ;
      }
      diff = -0.02 * abs(compass.a.x) + 0.01 * abs(compass.a.y); //斜めにぐるぐる回る処理x - y + で左回り コースによって使い分ける
      //diff = diff * 1.2;
      speed0 = 150;
      done = steadyState(9500);//傾斜の計測できるといい
      if ( done == 1 )
        mode_G = 23;
      break;
    /*
      case 22 :
      speed0 = 150;
      diff = 0.01 * abs(compass.a.x) - 0.02 * abs(compass.a.y);
      done = steadyState(7500);
      if ( done == 1 )
      mode_G = 23;
      break;
    */
    case 23: // 山登り中
      if ((avex < LEVEL) && (avex > -LEVEL)) { // 登頂したら頂上の中心部まで進む
        // done = steadyState(150);        // 200ms は要調整
        speed0 = 0;
        diff = 0;
        mode_G = 15 ; //本来は3 混んでいるので色のないとろろでじっけんできるように
        blueF = 1;//onaziku
      }
      if (done == 1) {  // 頂上の中心部まで来たら停止
        //rideF = 0 ;
        speed0 = 0;
        diff = 0;
        mode_G = 15 ; //本来は3 混んでいるので色のないとろろでじっけんできるように
      } else {
        speed0 = 150;
        diff = -0.02 * compass.a.y; // P-制御
      }
      break;
    case 3 :
      speed0 = 130;
      downF = 1;
      if (avex > SROPE )
        mode_G = 111;
      break;
    case 4: // 500ms停止
      speed0 = 130;
      diff = 0.02 * abs(compass.a.x) - 0.01 * abs(compass.a.y); //斜めにぐるぐる下る
      diff = diff * 1.2;
      diff = - diff;
      if ((avex < LEVEL) && (avex > -LEVEL)) { // 下山したら頂上の中心部まで進む
        done = steadyState(200);        // 200ms は要調整
        mode_G = 15;
      }
      break;
    //下る いい感じに斜めに
    case 10 ://青を認識した直後
      speed0 = 130;
      diff = 0.02 * abs(compass.a.y) - 0.02 * abs(compass.a.x); //45度なら0に近くなる
      if (diff == 0) {//下る方向を決めたら 傾きの差が0なら
        speed0 = 150;
        diff = 0;
        mode_G = 11;
      }
      break;
    case 11 ://下るとことを探す
      //diff = 0.02 * abs(compass.a.y) - 0.02 * abs(compass.a.x); //45度なら0に近くなる まっすぐおりてみる
      speed0 = 150;
      if (avex > SROPE) { // 下り始めたら
        mode_G = 12;
      }

      break;
    case 12 :
      if ((avex < LEVEL2) && (avex > -LEVEL2)) { // 下山たら頂上の中心部まで進む
        done = steadyState(150);        // 200ms は要調整
        down_flag = 0 ;
        blackF = 1 ;
        speed0 = 0;
        diff = 0;
        mode_G = 15;//青の下山した時
      }
      if (done == 1) {  // 頂上の中心部まで来たら停止
      } else {
        speed0 = 150;
      }
      break;
    case 13:
      diff = 40 ;
      speed0 = 150 ;
      //      done = (targetDirection( direction_3[1] + 45));
      if ((avex < LEVEL) && (avex > -LEVEL)) { // 登り始めたら
        // rideF = 1;
        mode_G = 2;
        diff = 0 ;
        //motors.setSpeeds(0, 0);
        //delay(199999);
      }
      if (done == 1)
        mode_G = 1;
      break;
    case 14 ://赤を検知した直後の処理
      speed0 = 130 ;
      diff = 0.02 * compass.a.y;
      //  diff = -0.02 * abs(compass.a.x) + 0.02 * abs(compass.a.y); //45度なら0に近くなる
      // diff = -diff ;
      diff = diff * 1.5;
      if ((avex < LEVEL2) && (avex > -LEVEL2)) { // 下山たら方向転換
        mode_G = 15;
      }
      break;
    case 15 :
      speed0 = 0 ;
      diff = 0 ;
      // if (dirFlag == 0)
      // {
      switch (pointF)
      {
        case 0 :
          direction_target = directionIn + 45;
          break;
        case 1://山が奥の場合
          direction_target = directionIn + 135;
          break;
      }
      if (pointF2)
        direction_target = directionIn + 180;
      //if (direction_target > 361  )
      //direction_target -= 360 ;
      // dirFlag = 1;asffas
      //}
      done = steadyState(1000);
      if (done != 1)
      {
        diff =  turnTo(direction_target);
        if (abs(diff) <= 50)
          diff = 0 ;
      } else {
        switch (countTurn)
        {
          case 0 :
            if (blueF) {
              mode_G = 10;
              countTurn = 1;
            } else {
              mode_G = 0 ;
              pointF2 = 1 ;
            }
            break;
          case 1 :
            mode_G = 0;
            pointF2 = 1;
            break;
        }
      }
      break;
    case 100:
      eFlag = 1 ;
      //speed0 = 0 ;
      // diff = 0 ;
      // mode_G = mode_Gs;
      break;
    case 101:
      eFlag = 0 ;
      mode_G = mode_Gs ;
      break;
    default:
      break;
  }

  if (redF == 0 && blueF == 0 )
    colorCheck_G =ChackC();//青0 赤1
  if (eFlag == 0)
  {
    motorL_G = speed0 + diff;
    motorR_G = speed0 - diff;
  }
}

static int CheckB(void)
{
  switch (Mode_C)
  {
    case 2 :
      countr = 0;
      //goStraight( SPEED );
      if ( identifyColor( 0, 0, 0 ) ) { // �������m
        //minDistance = 9999999; // identifyZone()�֐��Ŏg���ϐ��̏�����
        Mode_C = 5;
        if (mode_G != 100)
          mode_Gs = mode_G;
        mode_G = 100;
      }
      break;
    case 5: // �������m����܂Œ��i
      if (countr == 0) {
        turnRight(SPEED * 1.5);
        if (steadyState((countr + 1) * 200)) {
          countr++;
          if (!identifyColor( 0, 0, 0 )) { // �������m
            Mode_C = 2;
            mode_G = 101;
          }
        }
      } else if (countr == 1) {
        turnRight(-SPEED * 1.5);
        if (steadyState((countr + 1) * 200)) {
          countr++;
          if (!identifyColor( 0, 0, 0 )) { // �������m
            Mode_C = 2;
            mode_G = 101;

          }
        }
      } else if (countr == 2) {
        turnRight(SPEED * 1.5);
        if (steadyState((countr + 1) * 200)) {
          countr++;
          if (!identifyColor( 0, 0, 0 )) { // �������m
            Mode_C = 2;
            mode_G = 101;

          }
        }
      } else if (countr == 3) {
        turnRight(-SPEED * 1.5);
        if (steadyState((countr + 1) * 200)) {
          countr++;
          if (!identifyColor( 0, 0, 0 )) { // �������m
            Mode_C = 2;
            mode_G = 101;

          }
        }
      } else if (countr == 4) {
        turnRight(SPEED * 1.5);
        if (steadyState((countr + 1) * 200)) {
          countr++;
          if (!identifyColor( 0, 0, 0 )) { // �������m
            Mode_C = 2;
            mode_G = 101;
          }
        }
      } else if (countr == 5) {
        turnRight(SPEED * 1.5);
        if (steadyState((countr + 1) * 200)) {
          countr++;
          if (!identifyColor( 0, 0, 0 )) { // �������m
            Mode_C = 2;
            mode_G = 101;
          }
        }
      } else if (countr == 6) {
        turnRight(SPEED * 1.5);
        if (steadyState((countr + 1) * 200)) {
          countr++;
          if (!identifyColor( 0, 0, 0 )) { // �������m
            Mode_C = 2;
            mode_G = 101;
          }
        }
      } else {
        countr = 0;
      }

      /*  turnRight(SPEED*2);
        if (!identifyColor( 255, 255, 255 )) { // �������m
          //minDistance = 9999999; // identifyZone()�֐��Ŏg���ϐ��̏�����
          mode_G = 2;
        }*/

      break;
  }
  return 0 ;
}
static int ChackC(void) {
  if ( identifyColor( 9, 35 , 80  ) ) //青を検知したら
  {
    tone(3, 262, 300);
    mode_G = 15;
    blueF = 1;
    return 0;

  }
  if ( identifyColor( 123, 31 , 24  ) ) //赤を検知したら
  {
    tone(3, 262, 300);
    mode_G = 14;
    redF = 1;
    return 1;
  }
}

