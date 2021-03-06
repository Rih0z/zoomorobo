//回転開始時の角度を格納する変数
int d;

//方針１
/*void zone_geomagnetic(){
	//停止
	motors.setSpeeds(0, 0);
	delay(500);
	//直進
	motors.setSpeeds(SPEED, SPEED);
	delay(500);
	//停止
	motors.setSpeeds(0, 0);
	delay(500);
	
	//右に90度回転した後に1秒間直進をzoneNumber_G*2回実行
	for(int i = 0; i < 2*zoneNumber_G; i++){
		d = direction_G;
		
		//右に90度回転
		while(!targetDirection(180)){
			turnRight(SPEED);
		}
		
		//直進
		motors.setSpeeds(SPEED, SPEED);
		delay(1000);
	}

	//button.waitForButton(); //Zumo bottunが押されるまで停止
	//zoneNumber_G = 0;
	//mode_G = 0;
}*/


//方針2
void zone_geomagnetic(){
	static int countTurn = 0;
	int done;

	switch (mode_G){
		case 0: //setupはここ
			countTurn = 0;
			mode_G = 1;
		case 1: //500ms停止
			motorL_G = 0;
			motorR_G = 0;
			done = steadyState(500);
			if (done == 1)
				mode_G = 2;
			break;
		case 2: //500ms直進
			motorL_G = SPEED;
			motorR_G = SPEED;
			done = steadyState(500);
			if (done == 1)
				mode_G = 3;
			break;
		case 3: //500ms停止
			motorL_G = 0;
			motorR_G = 0;
			done = steadyState(500);
			if (done == 1){
				mode_G = 4;
				d = direction_G;
			}
			break;
		case 4: //右に90度回転
			motorL_G = -SPEED;
			motorR_G = SPEED;
			done = targetDirection(d + 90);
			if (done == 1)
				mode_G = 5;
			break;
		case 5: //1秒間停止
			motorL_G = 0;
			motorR_G = 0;
			done = steadyState(1000);
			if (done == 1){
				countTurn++;
				if (countTurn == 2*zoneNumber_G) //条件満たすまでmode_G=4-5の繰り返し
					mode_G = 6;
				else
					mode_G = 4;
			}
			break;
		case 6: //停止
			motorL_G = 0;
			motorR_G = 0;
			countTurn = 0;
			break;
		default:
			break;
	}
}
