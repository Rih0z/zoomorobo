//カーリングゾーン
//作成者：大西涼衣

void zone_curling_a(){
	int done;
	static float diff;
	static float d;				//方向を一時的に格納
	static int count;			//対象物をいくつ運搬しているか格納
	static int mode_pv;			//黒検知モードかた元のモードに戻る時に使用
	
	//ライントレース
	float lightNow;
	static float lightMin = 0;	//各自で設定
	static float lightMax = 240;	//各自で設定
	float speedDiff;
	static float speed = 100; 	//パラメーター
	float Kp = 3.0; 		//パラメーター

	//対象物との距離を測定
	digitalWrite(trig,HIGH);		//10μsのパルスを超音波センサのTrigピンに出力
	delayMicroseconds(10);
	digitalWrite(trig,LOW);
	
	//interval = pulseIn(echo,HIGH,23068);	//Echo信号がHIGHである時間(μs)を計測
	interval = pulseIn(echo,HIGH,5767);
	distance_G = 340 * interval / 10000 /2;	//距離(cm)に変換
	//Serial.println(distance);		//距離をシリアルモニタに表示

	switch(mode_G){
		//セットアップ
		case 0:
			count = 0;
			diff = 0;
			dis_min1 = 9999;
			dis_min2 = 9999;
			mode_G = 1;
			break;
			
		//ゾーンの中心まで直進(要改善)
		case 1:
			goStraight(150);
			
			//黒を検知
			if (identifyColor(0,0,0)){
				mode_pv = mode_G;
				//mode_G = 99;
			}
			
			done = steadyState(1500);
			if(done == 1){
				goStraight(0);
				mode_G = 2;
			}
			break;
			
		//対象物の探索(円の中心付近で回転)
		case 2:
			turnRight(150);
			
			//最も近い対象物との距離,方向を格納
			if(distance_G <= dis_min1 && distance_G != 0){
				dis_min1 = distance_G;
				dir_min1 = direction_G + 10;
			}
			//2番目に近い対象物
			else if(dis_min1 + 5 < distance_G && distance_G <= dis_min2 && distance_G != 0){
				dis_min2 = distance_G;
				dir_min2 = direction_G + 10;
			}
			
			//4秒間回転し探索
			done = steadyState(4000);
			if(done == 1){
				mode_G = 3;
			}
			break;
			
		//対象物の方向に回転
		case 3:
			diff = turnTo(dir_min1);
			//対象物の方向に向いたら次のモードへ
			if(abs(diff) <= 15){
				diff = 0;
				mode_G = 4;
			}
	  		break;
		
		//対象物に向かって直進
		case 4:
			goStraight(150);
			//対象物との距離が十分に近づいたら次のモードへ
			if(distance_G <= 6 && distance_G != 0){
				//真後ろに振り返るときの方向を格納
				if(count == 0){
					d = direction_G + 180;
				}
				dir_G = d;
				if(d >= 360){
					d -= 360;
				}
				
				mode_G = 5;
			}
			break;
		
		//対象物をつかんだまま円の中心に向かうように回転
		case 5:
			goStraight(0);
			//diff = turnTo(d);
			turnRight(130);
			
			//円の中心に向いたら次のモードへ
			/*if (abs(diff) <= 10){*/
			if(turnToDirection(d)){
				/*diff = 0;*/
				mode_G = 6;
			}
			break;
		
		//直進
		case 6:
			goStraight(150);
			//赤を検知
			if(identifyColor(169,36,16)){
				mode_G = 7;
				count++;
			}
			break;
		
		//後退
		case 7:
			goStraight(-150);
			done = steadyState(1000);
			
			if(done == 1){
				mode_G = 8;
			}
			break;
			
		//青を検知するまで前進
		case 8:
			goStraight(150);
			//青を検知でライントレースへ
			if(identifyColor(25,38,107)){
				dis_min1 = 9999;
				dis_min2 = 9999;
				mode_G = 9;
			}
			break;
		
		//P制御により青線の端に沿ってライントレース
		case 9:
			lightNow = (red_G + green_G + blue_G)/3.0;
			
			speedDiff = map(lightNow,lightMin,lightMax,-Kp*speed,Kp*speed);
			
	  		motorL_G = speed - speedDiff;
	  		motorR_G = speed + speedDiff;
	  		
	  		//done = steadyState(3000);
	  		done = steadyState(2000);
	  		
	  		//2つ目の対象物を運搬済みの場合、出口方向を向いている時、次のモードに移行
	  		if(count == 2 && turnToDirection(90)){
	  			mode_G = 11;
	  		}
	  		
	  		//2つ目の対象物の探索
	  		else if(count != 2 && done == 1){
			  	//2つ目の対象物を探索
			  	/*if(distance_G < 100 && distance_G != 0){
					dis_min1 = distance_G;
					dir_min1 = direction_G + 10;
				
					mode_G = 10;
				}*/
				mode_G = 20;
			}
			break;
		
		//右回転
		case 20:
			turnRight(150);
			
			//対象物発見
			if(distance_G < 30 && distance_G < dis_min1 && distance_G != 0){
					dis_min1 = distance_G;
					dir_min1 = direction_G + 10;
				
					mode_G = 10;
			}
			
			done = steadyState(1000);
			if(done == 1){
				mode_G = 21;
			}
			
		//左回転
		case 21:
			turnRight(-150);
			
			//対象物発見
			if(distance_G < 30 && distance_G < dis_min1 && distance_G != 0){
					dis_min1 = distance_G;
					dir_min1 = direction_G + 10;
				
					mode_G = 10;
			}
			
			done = steadyState(1000);
			if(done == 1){
				mode_G = 9;
			}
		
		//2つ目の対象物の方向を向く
		case 10:
			//diff = turnTo(dir_min1);
			turnRight(150);
			//対象物の方向に向いたら次のモードへ
			//if(abs(diff) <= 30){
			if(turnToDirection(dir_min1)){
				diff = 0;
				
				//円の中心に向かう方向を格納
				d = direction_G + 270;
				if(d >= 360){
					d -= 360;
				}
				
				mode_G = 4;
			}
			break;
		
		//出口に向かって直進
		case 11:
			goStraight(150);
			zoneNumber_G = 0;
			mode_G = 2;
			break;
		
		//
		case 12:
			
			break;
		
		//
		case 13:
			
			break;
		
		//
		case 14:
			
			break;
		
		//
		case 15:
			
			break;
		
		//
		case 99:
			break;
		
		default:
	  		break;
  	}
  	
  	motorL_G += diff;
	motorR_G -= diff;
}
