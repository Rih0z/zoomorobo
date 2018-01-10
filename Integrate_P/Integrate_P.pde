import processing.serial.*;
Serial port; 
int zoneNumber, mode, sonic, motor_L, motor_R, accel_X, accel_Y, accel_Z, geomag_X, geomag_Y, geomag_Z;
int CX=250, CY=250;
int green, red, blue;
int high, low;
int i ; 
float direction_G;

int count = 0;
float ac_xyz = 0;
float V = 0;
float dx = 0;
int[] sensors = new int[5];
int[] sensors_p = new int[5];

float X_1=0,Y_1=0;
float POINT_F = 0;

int traceflag = 0;
int traceflag2 = 0;
int drawcount = 0;//何個目の図形をトレースしているか

float x_1 = 0, y_1 = 0;//トレースの初期座標 
float x_2 = 0, y_2 = 0;//map関数で調整

//riho
int colorCheck_G = 3; //何色を認識したかこの値が変化したらカウントを増やす
int colorCount = 0;
int colorBef = 3; 
float avex ; //傾き 
void setup() {
  size(1024, 768);
  background(255);
  println(Serial.list());
//  port = new Serial(this, "/dev/ttyUSB0", 9600 );
  // String arduinoPort = Serial.list()[1];
  // port = new Serial(this, arduinoPort, 9600 );
  //port = new Serial(this, "/dev/cu.usbserial-A90174UN", 9600 ); // シリアルポート名は各自の環境に合わせて適宜指定
}

void draw() {

 
  DrawCommonInfo();//共通情報の描画
  DrawZoneTask();//各ゾーンで描画や処理が必要な時
  DrawZoneNumber();//ゾーンナンバーを視覚化
  /*
  noStroke();  fill(255);  rect(0,0,1000,500);
   // Draw Acceleration vector
   CX = 250;
   drawVec(accel_X/100,accel_Y/100,accel_Z/100);
   // Draw Magnetic flux vector
   CX = 750;
   drawVec(geomag_X,geomag_Y,geomag_Z);
   // Draw Heading direction
   CX = 600;
   float scale = 0.5;
   line(CX-scale*geomag_Y,100+scale*geomag_X,CX+scale*geomag_Y,100-scale*geomag_X);
   line(CX+scale*geomag_Y,100-scale*geomag_X,CX+0.6*scale*geomag_Y+0.2*scale*geomag_X,100-0.6*scale*geomag_X+0.2*scale*geomag_Y);
   line(CX+scale*geomag_Y,100-scale*geomag_X,CX+0.6*scale*geomag_Y-0.2*scale*geomag_X,100-0.6*scale*geomag_X-0.2*scale*geomag_Y);
   */
  noFill();
}

void DrawCommonInfo() {
  //区画
  fill(255);
  stroke(200, 200, 200);
  line(0, height*0.3, width*0.65, height*0.3);
  line(width*0.65, 0, width*0.65, height);
  stroke(255);
  fill(255);
  rect(0, height-50, 300, 50);
  textSize(25);
  fill(0);

  text("zoneNumber= ", width*0.40, height*0.1);
  text("mode= ", width*0.49, height*0.15);
  textSize(20);
  text("Direction =", width*0.65, height*0.05);
  text("RGB= ", width*0.65, height*0.1);
  text("(geomag_X,geomag_Y,geomag_Z)= ", width*0.65, height*0.2);
  text("(accel_X,accel_Y,accel_Z)= ", width*0.65, height*0.3);
  text("motorspeed(L,R)=", width*0.65, height*0.4);
  
  textSize(25);
  
  //reload
  fill(255);
  rect(width*0.5+100, height*0.05, 50, 100);
  rect(width*0.65+140, height*0.01, 200, 50);
  rect(width*0.65+80, height*0.06, 250, 40);
  rect(width*0.65+10, height*0.21, 400, 40);
  rect(width*0.65+10, height*0.31, 400, 40);
  rect(width*0.65+10, height*0.41, 400, 40);
  fill(0);
  
  
  text((int)zoneNumber, width*0.50 + 100, height*0.1);
  text((int)mode, width*0.50 + 100, height*0.15);

  text(direction_G, width*0.65 +140, height*0.05);
  text("("+red+",", width*0.65 +80, height*0.1);
  text(green+",", width*0.65 +135, height*0.1);
  text(blue+")", width*0.65 +190, height*0.1);
  fill(red, green, blue);
  rect(width*0.65, height*0.1 + 5, 300, 50);
  fill(0);
  text("("+geomag_X+",", width*0.65, height*0.25);
  text(geomag_Y+",", width*0.65 +120, height*0.25);
  text(geomag_Z+")", width*0.65 +240, height*0.25);
  text("("+accel_X+",", width*0.65, height*0.35);
  text(accel_Y+",", width*0.65 +120, height*0.35);
  text(accel_Z+")", width*0.65 +240, height*0.35);
  text("("+motor_L+",", width*0.65, height*0.45);
  text(motor_R+")", width*0.65 +120, height*0.45);
  
  
}

void DrawZoneTask() {
  //ゾーンの描画(下半分のやつ)
  if(zoneNumber != 4){
    strokeWeight(10);
    stroke(0);
    fill(0);
    //1024,768
    line(width*0.6,height*0.35,width*0.6,height*0.95);
    line(width*0.6-20,height*0.35,width*0.6-20,height*0.95);
    line(width*0.1,height*0.35,width*0.6,height*0.35);
    line(width*0.1,height*0.95,width*0.6,height*0.95);
    line(width*0.1,height*0.35,width*0.1,height*0.95);
    stroke(0,0,0);
    line(width*0.6-10,height*0.35,width*0.6-10,height*0.95);
    stroke(256,0,0);
    line(width*0.1+10,height*0.35,width*0.1+10,height*0.95);
    stroke(0,0,0);
  }
  //各ゾーンタスクで必要な描画、処理 デバッグ用に3で固定
  //zoneNumber = 3;
  switch(zoneNumber) {
  case 1://linetrace
    if(drawcount==1){
      x_2 = map(X_1, 0, 800, width*0.1, width*0.35);
      y_2 = map(Y_1, 0, 900, height*0.35, height*0.65);
      stroke(0,0,255);
      point(x_2, y_2);
    }
    break;
  case 2://carling
    break;
  case 3://hillclimbing
    CalcDisp();
    DrawZone3();

    break;
  case 4://winning
    break;
  default:
    break;
  }
}

void DrawZoneNumber(){
 
  strokeWeight(2);
  stroke(0);
  fill(0);
  line(width*0.1, height*0.28, width*0.5, height*0.28);
  line(width*0.1, height*0.1, width*0.3, height*0.1);
  line(width*0.1, height*0.01, width*0.1, height*0.28);
  line(width*0.3, height*0.01, width*0.3, height*0.28);
  line(width*0.3, height*0.18, width*0.5, height*0.18);
  line(width*0.4, height*0.18, width*0.4, height*0.28);
  line(width*0.5, height*0.18, width*0.5, height*0.28);
  line(width*0.1, height*0.01, width*0.3, height*0.01);
  switch(zoneNumber){
    case 1:
      DrawStar(10,20,width*0.45,height*0.23);
    break;
    case 2:
      DrawStar(10,20,width*0.35,height*0.23);
    break;
    case 3:
      DrawStar(10,20,width*0.2,height*0.23);
    break;
    case 4:
      DrawStar(10,20,width*0.2,height*0.05);
    break;
    default:
      DrawStar(10,20,width*0.55,height*0.23);
    break;
  }
}

void DrawStar(int R,int R_out,float C_x,float C_y){
  //R中心から頂点までの距離（半径）,R_out中心から棘までの距離（半径）,C_x中心座標x,C_y中心座標y
  int vertex_num = 10;//頂点数*2
  int R_in = R_out/2;//中心から谷までの距離（半径）
  //星
  noFill();
  strokeWeight(2);
  stroke(200, 0, 0);

  pushMatrix();
  translate(C_x, C_y);
  rotate(radians(-90));
  beginShape();
  for (int i = 0; i < vertex_num; i++) {
    if (i%2 == 0) {
      R = R_out;
    } else {
      R = R_in;
    }
    vertex(R*cos(radians(360*i/vertex_num)), R*sin(radians(360*i/vertex_num)));
  }
  endShape(CLOSE);
  popMatrix();
}

void CalcDisp(){
  count++;
  if(count%10 != 0){
    ac_xyz += sqrt(accel_X*accel_X + accel_Y*accel_Y + accel_Z*accel_Z);
  }else{
    ac_xyz = ac_xyz/10;
    V = ac_xyz/6 + V;
    dx = V/6 + ac_xyz/2*36;
  }

  if (colorCheck_G != colorBef){
    colorCount++;
    colorBef= colorCheck_G;
  }
  print("ac_xyz = ");print(ac_xyz);print(" V = ");print(V);print(" dx = ");println(dx);
  print("colorCount");println(colorCount);print("colorCheck_G");println(colorCheck_G);

}
void DrawZone3(){
/*
    //1024,768
    line(width*0.6,height*0.35,width*0.6,height*0.95);
    line(width*0.6-20,height*0.35,width*0.6-20,height*0.95);
    line(width*0.1,height*0.35,width*0.6,height*0.35);
    line(width*0.1,height*0.95,width*0.6,height*0.95);
    line(width*0.1,height*0.35,width*0.1,height*0.95);
    stroke(0,0,0);
  fill(255);
  rect(width*0.5+100, height*0.05, 50, 100);
  rect(width*0.65+140, height*0.01, 200, 50);
  rect(width*0.65+80, height*0.06, 250, 40);
  rect(width*0.65+10, height*0.21, 400, 40);
  rect(width*0.65+10, height*0.31, 400, 40);
  rect(width*0.65+10, height*0.41, 400, 40);
  fill(0);
 */
 //reove
  stroke(255,255,255);
  fill(255);
  //colorCount
  rect(width*0.5+40, height*0.87, 25, 30);
  fill(0);
  stroke(0,0,0);
  //colorCheck_Gの初期値は3
  switch(colorCheck_G)
  {
    case 0 : 
      //bluetext("blue", width*0.22, height*0.9);
      stroke(0,0,255);
      fill(0,0,255);
      break;
    case 1:
      //red
      stroke(255,0,0);
      fill(255,0,0);
      break;
  }
  ellipse(width * 0.22, height * 0.85 ,120,120);
  fill(0);
  stroke(0,0,0);
  text("checked", width*0.17, height*0.9);
  text("colorCount ", width*0.40, height*0.9);
  text((int)colorCount, width*0.50 + 40, height*0.9);
}

void line3D(float x0, float y0, float z0, float x1, float y1, float z1) {
  float X0 = CX+y0-0.5*x0, Y0 = CY + 1.7320508*x0/2-z0;
  float X1 = CX+y1-0.5*x1, Y1 = CY + 1.7320508*x1/2-z1;
  line(X0, Y0, X1, Y1);
}

void drawVec(float x, float y, float z) {
  stroke(128);
  line3D(0, 0, 0, 250, 0, 0);  
  line3D(0, 0, 0, 0, 250, 0);  
  line3D(0, 0, 0, 0, 0, 250);
  stroke(0);
  line3D(0, 0, 0, x, y, 0);
  line3D(x, y, 0, x, y, z);
  line3D(0, 0, 0, x, 0, 0);
  line3D(x, 0, 0, x, y, 0);
  line3D(x, y, 0, 0, y, 0);
  line3D(0, y, 0, 0, 0, 0);
  stroke(255, 0, 0);  
  line3D(0, 0, 0, x, y, z);
  fill(0);  
  text(x, CX-80, 490);  
  text(y, CX, 490);  
  text(z, CX+80, 490);
}
int read2byte(Serial p) {
  int x = p.read();  x <<= 8;   x |=p.read();
  if (x>32757) x -= 65536;
  return x;
}
void serialEvent(Serial p) {
  if (p.available() >=23 ) {
    if (p.read() == 'H') {
      ax = read2byte(p);      ay = read2byte(p);      az = read2byte(p);
      mx = read2byte(p);      my = read2byte(p);      mz = read2byte(p);
      direc = read2byte(p);
      colorcheck=p.read();
      //RGB
      red =  p.read();
      green = p.read();
      blue =  p.read();
      motor_R = p.read();
      motor_L = p.read();
      zoneNumber = p.read();
      mode =  p.read();
       p.clear(); // 念のためクリア
      p.write("A");
      ++count;
      println(count);
    }
  }
}
