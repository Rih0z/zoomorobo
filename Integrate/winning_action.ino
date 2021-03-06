void winning_action(){
	static int done;
	static int count;
	static int d;
	static int d_add = 30;

	switch(mode_G){
		//セットアップ
		case 0:
			count = 0;
			//便宜上設定
			mode_G = 1;
			break;
			
		//少し直進
		case 1:
			goStraight(200);

			done = steadyState(500);
			if(done == 1){
				mode_G = 2;
			}
			break;
			
		//8の字回転(右回転)
		case 2:
			//数値は実践結果にて決定
			motorR_G = 100;
			motorL_G = 400;
			
			done = steadyState(1600);
			if(done == 1){
				mode_G = 3;
			}
			break;
			
		//8の字回転(左回転)
		case 3:
			//数値は実践結果にて決定
			motorR_G = 400;
			motorL_G = 100;
			
			done = steadyState(1700);
			if(done == 1){
				count++;
				//Serial.println(count);
				//8の字回転を一定回数行ったら次のアクションへ
				if(count >= 3){
					mode_G = 4;
					//カウンターリセット
					count = 0;
				}
				else{
					mode_G = 2;
				}
			}
			break;

		//*****その場で高速回転*****//
		//右回転
		case 4:
			turnRight(500);

			done = steadyState(1500);
			if(done == 1){
				mode_G = 5;
			}
			break;

		//左回転
		case 5:
			turnRight(-500);

			done = steadyState(1500);
			if(done == 1){
				mode_G = 6;
			}
			break;

		//*****ジグザグ移動*****//
		//直進
		case 6:
			goStraight(500);
			
			done = steadyState(150);
			if(done == 1){
				//後退ジグザグ移動モードへ
				if(count == 5){
					count = 0;
					mode_G = 9;
				}

				//右回転へ
				else if((count % 2) == 0){
					d = direction_G + d_add;
					mode_G = 7;
				}
				//左回転へ
				else{
					d = direction_G + (360 - d_add);
					mode_G = 8;
				}

				//数値補正
				if(d >= 360){
					d -= 360;
				}
			}
			break;
			
		//右回転
		case 7:
			turnRight(230);
			if(turnToDirection(d) == 1){
				count++;
				mode_G = 6;
			}
			break;
			
		//左回転
		case 8:
			turnRight(-230);
			if(turnToDirection(d) == 1){
				count++;
				mode_G = 6;
			}
			break;
			
		//後退
		case 9:
			goStraight(-500);
			done = steadyState(150);
			if(done == 1){
				//次のアクションへ
				if(count == 5){
					mode_G = 12;
				}

				//右回転へ
				else if((count % 2) == 1){
					d = direction_G + d_add;
					mode_G = 10;
				}
				//左回転へ
				else{
					d = direction_G + (360 - d_add);
					mode_G = 11;
				}

				//数値補正
				if(d >= 360){
					d -= 360;
				}
			}
			break;
			
		//右回転
		case 10:
			turnRight(230);
			if(turnToDirection(d) == 1){
				count++;
				mode_G = 9;
			}
			break;
			
		//左回転
		case 11:
			turnRight(-230);
			if(turnToDirection(d) == 1){
				count++;
				mode_G = 9;
			}
			break;
			
		//
		case 12:
			//ド
			tone(buzzerPin,262,200);
			
			done = steadyState(200);
			if(done == 1){
				mode_G = 13;
			}
			break;

		//
		case 13:
			//レ
			tone(buzzerPin,294,200);
			
			done = steadyState(200);
			if(done == 1){
				mode_G = 14;
			}
			break;
		
		//
		case 14:
			//ミ
			tone(buzzerPin,330,200);
			
			done = steadyState(200);
			if(done == 1){
				mode_G = 15;
			}
			break;

		//
		case 15:
			break;

		//
		case 16:
			break;
		
		//その他
		default:
			break;
	}
}
