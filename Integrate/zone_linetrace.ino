
// P

void zone_linetrace()
{
  float lightNow;
  static float lightMin = 21; // 各自で設定
  static float lightMax = 255;// 各自で設定
  static float speed = 100; // パラメーター
  float speedDiff;
  float Kp = 3.0; // パラメーター
  int done, done1;
  static int redf = 0, bluef = 0, greenf = 0, pinkf = 0;
  static float a = direction_G;
  switch ( mode_G ) {
    case 0:
      mode_G = 1;
      break;
    case 1:
      motorL_G = SPEED;
      motorR_G = SPEED;
      if ( identifyColor2( 193, 41, 21, 30) && redf == 0) { // 赤を検知
        mode_G = 2;
        redf = 1;
      }
      if ( identifyColor2( 24, 37, 102, 30) && bluef == 0) { // 青を検知
        mode_G = 2;
        bluef = 1;
      }
      if ( identifyColor2( 78, 159, 78, 30) && greenf == 0) { // 緑を検知
        mode_G = 2;
        greenf = 1;
      }
      if ( identifyColor2( 238, 125, 155, 30) && pinkf == 0) { // ピンクを検知
        mode_G = 7;
        pinkf = 1;
      }
      if ( identifyColor2( 0, 0, 0 , 30)) { //黒を検知
        mode_G = 6;
      }
      break;
      if ( identifyColor2( 235, 216, 60, 30 ) && redf == 1 && bluef == 1 && greenf == 1 && pinkf == 1) { // 黄色を検知
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
      /*if ( lightNow < (lightMin + lightMax) / 2.0 ) // 左回転
        speedDiff = -Kp * speed;
        else // 右回転
        speedDiff = Kp * speed;*/
      speedDiff = map (lightNow, lightMin, lightMax, -Kp * speed, Kp * speed);
      motorL_G = speed + speedDiff;
      motorR_G = speed - speedDiff;
      done = steadyState(8000); //8秒経過したら
      if ( done == 1 )
        mode_G = 1;
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
    case 6:
      turnRight( -SPEED );
      done = steadyState(500); //1000ms回転
      if (done == 1) {
        mode_G = 1;
      }
      break;
    case 7:
      turnRight( -SPEED );
      if ( identifyColor( 255, 255, 255 ) ) { // 白を検知
        mode_G = 8;
      }
      break;
    case 8:
      Kp = 4.5;
      speed = 70;
      lightNow = (red_G + green_G + blue_G ) / 3.0;
      /*  if ( lightNow < (lightMin + lightMax) / 2.0 ) // 左回転
        speedDiff = -Kp * speed;
        else // 右回転
        speedDiff = Kp * speed;*/
      speedDiff = map (lightNow, (238.0+125.0+155.0)/3.0, lightMax, -Kp * speed, Kp * speed);
      motorL_G = speed + speedDiff;
      motorR_G = speed - speedDiff;
      done = steadyState(8000); //8秒経過したら
      if ( done == 1 ){
        mode_G = 1;
        Kp = 3.0;
        speed = 100;
      }
      break;
    default:
      break;
  }
}



