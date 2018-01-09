//カーリングゾーン
//作成者：大西涼衣
unsigned long interval;				//Echoのパルス幅（μs）
/*
static int distance;				//距離（cm）
static int dis_obj[2];				//対象物との距離を格納
static float dir_obj[2];			//対象物の方向を格納
*/

static int dis_min1,dis_min2;			//最も近い2つの対象物との距離
static float dir_min1,dir_min2;			//最も近い2つの対象物の方向

void zone_curling(){
	int done;
	static float diff;
	static float d;				//方向を一時的に格納
	static int count;			//対象物をいくつ運搬しているか格納
	static int mode_pv;			//黒検知モードかた元のモードに戻る時に使用
	static int cor = 25;			//超音波センサによって得た角度の補正値
	
	//ゾーンの出口とサイドの方向(適宜変更する必要)
	static int exit_d = 100;
	static int side_d = 20;
	
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
	
	static unsigned long tp = timeNow_G;
	
	//規定時間が経過したら次のゾーンに向かう
	if(timeNow_G - tp > 50){
		//zoneNumber_G = 0;
		//mode_G = 0;
	}
	else{
		tp = timeNow_G;
	}

	switch(mode_G){
		//セットアップ
		case 0:
			count = 0;
			diff = 0;
			dis_min1 = 99;
			//dis_min2 = 99;
			//mode_G = 1;
			mode_G = 30;
			break;
		
		//*****ゾーンの中心に移動*****//
		//ゾーン突入後少し前進
		case 30:
			done = steadyState(500);
			goStraight(150);
			
			if(done == 1){
				mode_G = 31;
			}
			
			break;
		
		//ゾーンのサイドを向くまで回転
		case 31:
			turnRight(-150);
			
			if(turnToDirection(side_d) == 1){
				mode_G = 32;
			}
			break;
			
		//黒を検知するまで直進
		case 32:
			goStraight(150);
			if(identifyColor(0,0,0)){
				mode_G = 33;
			}
			break;
			
		//ゾーンの中心に到着するまで後退
		case 33:
			//現状適当な値
			done = steadyState(1800);
			
			goStraight(-150);
			
			if(done == 1){
				mode_G = 34;
			}
			break;
		
		//*****円の中心まで移動*****//
		//ゾーンの出口方向（円の中心）に向くまで回転
		case 34:
			turnRight(150);
			
			if(turnToDirection(exit_d) == 1){
				mode_G = 1;
			}
			break;
		
		//ゾーンの中心(赤を検知する)まで直進
		case 1:
			goStraight(150);
			
			//黒を検知
			/*if(identifyColor(0,0,0)){
				mode_pv = mode_G;
				//mode_G = 99;
			}*/
			
			//時間による移動制御
			//done = steadyState(1500);
			/*if(done == 1){
				mode_G = 2;
			}*/
			
			//赤を検知(念の為時間経過によるモード移行も追加)
			if(identifyColor(169,36,16) || done == 1){
				mode_G = 35;
			}
			
			break;
		
		//少しだけ前進(赤を検知した時点ではまだ円の中心には少し足りないため)
		case 35:
			goStraight(150);
			
			done = steadyState(500);
			if(done == 1){
				mode_G = 2;
			}
			break;
		
		//*****対象物の探索*****//
		//一定時間の回転によって対象物の探索し，方向を格納
		case 2:
			turnRight(150);
			
			//一定時間回転
			done = steadyState(4500);
			if(done == 1){
				mode_G = 3;
				goStraight(0);
			}
			
			//最も近い対象物との距離,方向を格納
			if(5 < distance_G && distance_G < dis_min1 && distance_G < 25 && distance_G != 0){
				dis_min1 = distance_G;
				dir_min1 = direction_G + cor;
				
				if(dir_min1 >= 360){
					dir_min1 -= 360;
				}
			}
			//2番目に近い対象物
			/*else if((dis_min1 + 5) < distance_G && distance_G < dis_min2 && distance_G != 0){
				dis_min2 = distance_G;
				dir_min2 = direction_G + 10;
			}*/
			
			break;
			
		//対象物の方向に回転
		case 3:
			//diff = turnTo(dir_min1);
			turnRight(150);
			//対象物の方向に向いたら次のモードへ
			//if(abs(diff) <= 15){
			if(turnToDirection(dir_min1) == 1){
				//diff = 0;
				mode_G = 4;
			}
	  		break;
		
		//対象物に向かって直進
		case 4:
			goStraight(150);
			//対象物との距離が十分に近づいたら次のモードへ
			if(distance_G <= 7 && distance_G != 0){
				//真後ろに振り返るときの方向を格納
				if(count == 0){
					d = direction_G + 180;
				}
				//dir_G = d;
				if(d >= 360){
					d -= 360;
				}
				
				mode_G = 5;
			}
			
			//進行方向調整のためある程度近づいたら再度回転し進行方向を決定
			/*else if(distance_G <= 15 && distance_G != 0){
				mode_G = 2;
			}*/
			
			//黒を検知したら失敗→すこし後退し探索をやり直す
			if(identifyColor(0,0,0)){
				mode_G = 60;
			} 
			break;
		
		//再度探索を行うためすこしだけ後退(対象物の近くで探索を行うことで精度アップ)
		case 60:
			goStraight(-150);
			done = steadyState(1000);
			if(done == 1){
				mode_G = 2;
			}
			break;
		
		//*****対象物の運搬*****//
		//対象物をつかんだまま円の中心に向かうように回転
		case 5:
			turnRight(130);
			
			//円の中心に向いたら次のモードへ
			if(turnToDirection(d)){
				mode_G = 6;
			}
			break;
		
		//円の中心まで直進
		case 6:
			goStraight(150);
			//赤を検知
			if(identifyColor(169,36,16)){
				mode_G = 7;
				count++;
			}
			break;
		
		//対象物を離すために後退
		case 7:
			goStraight(-150);
			done = steadyState(1200);
			
			if(done == 1){
				mode_G = 8;
			}
			break;
			
		//ライントレース移行のため青を検知するまで前進
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
	  		
	  		done = steadyState(2000);
	  		
	  		//2つ目の対象物を運搬済みかつ出口方向を向いている場合脱出モードに移行
	  		if(count == 2 && done == 1){
	  			mode_G = 40;
	  		}
	  		
	  		//2つ目の対象物の探索に移行
	  		else if(count != 2 && done == 1){
				//mode_G = 20;		
				mode_G = 41;		//一定期間ラインとレースによる探索
				//mode_G = 50;		//リアルタイム探索
			}
			break;
		
		//出口を向くまでライントレース
		case 40:
			lightNow = (red_G + green_G + blue_G)/3.0;
			
			speedDiff = map(lightNow,lightMin,lightMax,-Kp*speed,Kp*speed);
			
	  		motorL_G = speed - speedDiff;
	  		motorR_G = speed + speedDiff;
	  		
	  		//出口を向いたら次のモードへ
	  		if(turnToDirection(exit_d) == 1){
	  			mode_G = 11;
	  		}
	  		
			break;
			
		//2つ目の対象物を探索
		case 41:
			lightNow = (red_G + green_G + blue_G)/3.0;
			
			speedDiff = map(lightNow,lightMin,lightMax,-Kp*speed,Kp*speed);
			
	  		motorL_G = speed - speedDiff;
	  		motorR_G = speed + speedDiff;
		
			//2つ目の対象物を探索
			if(3 < distance_G && distance_G < 25 && distance_G != 0){
				dis_min1 = distance_G;
				dir_min1 = direction_G + cor;
				
				if(dir_min1 >= 360){
					dir_min1 -= 360;
				}
			}
			
			//一定時間探索
			done = steadyState(13000);
			if(done == 1){
				mode_G = 42;
			}
			break;
		
		//次に対象物の方向を向いた時に直進
		case 42:
			lightNow = (red_G + green_G + blue_G)/3.0;
			
			speedDiff = map(lightNow,lightMin,lightMax,-Kp*speed,Kp*speed);
			
	  		motorL_G = speed - speedDiff;
	  		motorR_G = speed + speedDiff;
	  		
	  		if(turnToDirection(dir_min1) == 1){
	  			mode_G = 43;
	  		}
			break;
		
		//リアルタイム探索
		case 50:
			lightNow = (red_G + green_G + blue_G)/3.0;
			
			speedDiff = map(lightNow,lightMin,lightMax,-Kp*speed,Kp*speed);
			
	  		motorL_G = speed - speedDiff;
	  		motorR_G = speed + speedDiff;
	  		
	  		if(3 < distance_G && distance_G < 50 && distance_G != 0){
	  			mode_G = 43;
	  		}
		
			break;
			
		//2つ目の対象物まで直進
		case 43:
			goStraight(150);
			
			//対象物との距離が十分に近づいたら次のモードへ
			if(distance_G <= 6 && distance_G != 0){
				mode_G = 44;
				
				//振り返る方向を格納
				d = direction_G + 180;
				
				if(d >= 360){
					d -= 360;
				}
			}
			break;
		
		//振り返るまで回転
		case 44:
			turnRight(130);
			
			if(turnToDirection(d) == 1){
				mode_G = 45;
			}
			break;
			
		//青を検知するまで前進
		case 45:
			goStraight(150);
			
			//青を検知で次のモードへ
			if(identifyColor(25,38,107)){
				mode_G = 46;
			}
			break;
			
		//とりあえず停止
		case 46:
			goStraight(0);
			break;
		
		/*//右回転
		case 20:
			turnRight(150);
			
			//対象物発見
			if(distance_G < 30 && distance_G < dis_min1 && distance_G != 0){
					dis_min1 = distance_G;
					dir_min1 = direction_G + 10;
				
					mode_G = 10;
			}
			
			done = steadyState(500);
			if(done == 1){
				//mode_G = 21;
				mode_G = 9;
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
			
			done = steadyState(500);
			if(done == 1){
				mode_G = 9;
			}*/
		
		//2つ目の対象物の方向を向く
		case 10:
			turnRight(150);
			//対象物の方向に向いたら次のモードへ
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
