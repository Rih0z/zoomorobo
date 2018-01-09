
// P

void zone_linetrace()
{
  float lightNow;
  static float lightMin = 21; // 各自で設定
  static float lightMax = 255;// 各自で設定
  static float speed = 100; // パラメーター
  float speedDiff;
  float Kp = 2.0; // パラメーター
  int done = 0;
  static int flagR = 0,flagG = 0,flagB = 0,flagP = 0;
  static int preflagR = 0,preflagG = 0,preflagB = 0,preflagP = 0;
  static float a = direction_G;


  static float POINT_BLUE_X = 0;
  static float POINT_BLUE_Y = 0;
  static float direction_G2;//入射角度
  static float dir_max = direction_G+10;
  static float dir_min = direction_G-10;


  Serial.print(POINT_BLUE_X);
  Serial.print(":");
  Serial.println(POINT_BLUE_Y);


  
  switch ( mode_G ) {
    case 0:
      mode_G = 1;
    break;
    case 1:
    motorL_G = SPEED;
    motorR_G = SPEED;
    if ( identifyColor2( 193, 41, 21,50) && flagR==0) { // 赤を検知
        mode_G = 2;
        flagR=1;
    }
    if ( identifyColor2( 24, 37, 102,30) && flagB==0) { // 青を検知
        mode_G = 2;
        flagB=1;
        POINT_BLUE_X = X_1;
        POINT_BLUE_Y = Y_1;

        done = steadyState(3000);
        if ( done == 1 ) {

          POINT_F = 1;
          direction_G2 = direction_G;
          if(dir_min < 0)
            dir_min += 360;
          if(dir_max > 360)
            dir_max -= 360;
        }
       
        if (POINT_F == 1) {
          if (X_1 <= POINT_BLUE_X +10 && X_1 >= POINT_BLUE_X -10) {
            if (Y_1 <= POINT_BLUE_Y +10 && Y_1 >= POINT_BLUE_Y -10) {
               preflagB = 1;
               mode_G = 1;
               traceflag = 0;
               
            }
          }
        }


        
    }
    if ( identifyColor2( 78, 159, 78,30) && flagG==0) { // 緑を検知
        mode_G = 2;
        flagG=1;
    }
    if ( identifyColor2( 238, 125, 155,30) && flagP==0) { // ピンクを検知
        mode_G = 2;
        flagP=1;
    }
    if ( identifyColor( 0, 0, 0 )) { //黒を検知
        turnRight( -1000 );
        done = steadyState(700); //700ms回転
        //mode_G = 7;
    }
    if ( identifyColor2( 235, 216, 60, 50 ) && flagR==1 && flagB==1 && flagG==1 && flagP==1) { // 黄色を検知
        mode_G = 4;
    }
    break;
    case 2:
    turnRight( -SPEED );
    if ( identifyColor( 255, 255, 255 ) ) { // 白を検知
        mode_G = 3;
    }
    break;
    case 3:
      lightNow = (blue_G );
        speedDiff = map (lightNow, lightMin, lightMax, -Kp*speed, Kp*speed);
      motorL_G = speed + speedDiff;
      motorR_G = speed - speedDiff;
      done = steadyState(10000); //10秒経過したら
      if ( done == 1 )
      mode_G = 1;
      break;

    case 7:
      if ( identifyColor( 0, 0, 0 ) )  { //黒を検知
         if ( TurnC == 0 ) {
          turnRight( SPEED );
          done = steadyState(200); //200ms回転
          if ( done == 1 ) TurnC++;
         }
         else if ( TurnC == 1 ) {
          turnRight( -SPEED );
          done = steadyState(400); //400ms回転
          if ( done == 1 ) TurnC++;
         }
         else if ( TurnC == 2 ) {
          turnRight( SPEED );
          done = steadyState(600); //600ms回転
          if ( done == 1 ) TurnC++;
         }
         else if ( TurnC == 3 ) {
          turnRight( -SPEED );
          done = steadyState(800); //800ms回転
          if ( done == 1 ) TurnC++;
         }
         else if ( TurnC == 4 ) {
          turnRight( SPEED );
          done = steadyState(1000); //1000ms回転
          if ( done == 1 ) TurnC = 0;
         }
         
      }
      else if ( identifyColor( 255, 255, 255 ) )  { //白を検知
        mode_G = 1;
        TurnC = 0;
      }
      break;
      
    case 4:
    motorL_G = speed;
    motorR_G = speed;
    if ( identifyColor( 0, 0, 0 )) { //黒を検知
        mode_G = 5;
    }
    break;
    case 5:
    motorL_G = 0;
    motorR_G = 0;
    break;
    default:
      break;
  }
}



