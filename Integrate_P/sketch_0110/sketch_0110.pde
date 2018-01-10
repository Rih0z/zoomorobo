import processing.serial.*;
Serial port;
float angle,anglex,anglez;
int ax = 0, ay = 0, az = 0;
int mx = 0, my = 0, mz = 0;
int zoneNumber, mode, sonic, motor_L, motor_R;
int green, red, blue;
int direc = 0;
int CX=250, CY=250;
int count;
int colorcheck;
//riho
int colorCheck_G = 3; //何色を認識したかこの値が変化したらカウントを増やす
int colorCount = 0;
int colorBef = 3; 
float omega;

void setup() {
   size(1024,768,P3D);
   zoneNumber=3;
   background(255);
   angle = 0;    //60度を設定
   anglex=0;
   anglez=0;
   count=0;
   //port = new Serial(this, "/dev/cu.usbserial-AE017KPV", 9600 ); // シリアルポート名は各自の環境に合わせて適宜指定
  // port = new Serial(this, "/dev/ttyUSB0",9600);
   //port.clear();
     //DrawCommonInfo();
}

void line3D(float x0, float y0,float z0,float x1,float y1,float z1) {
  float X0 = CX+y0-0.5*x0, Y0 = CY + 1.7320508*x0/2-z0;
  float X1 = CX+y1-0.5*x1, Y1 = CY + 1.7320508*x1/2-z1;
  line(X0,Y0,X1,Y1);
}

void drawVec(float x, float y, float z) {
  stroke(128);
  line3D(0,0,0,250,0,0);  line3D(0,0,0,0,250,0);  line3D(0,0,0,0,0,250);
  //stroke(0);
  line3D(0,0,0,x,y,0);
  line3D(x,y,0,x,y,z);
  line3D(0,0,0,x,0,0);
  line3D(x,0,0,x,y,0);
  line3D(x,y,0,0,y,0);
  line3D(0,y,0,0,0,0);
  stroke(255,0,0);  line3D(0,0,0,x,y,z);
  fill(0);  text(x,CX-80,490);  text(y,CX,490);  text(z,CX+80,490);
  
}


void draw() {
      stroke(255,255,255);
  fill(255);
  //colorCount
  rect(0, 0, width, height );
  fill(0);
  stroke(0,0,0);
  DrawCommonInfo();
  DrawZoneNumber();
  DrawZoneTask();
  
  
  if ( count == 120 ) {
    count = 0;
  // background(255);
  }
}

int read2byte(Serial p) {
  int x = p.read();  x <<= 8;   x |=p.read();
  if (x>32757) x -= 65536;
  return x;
}
void moterMerter(){
  float moter;
  float omegam;
  moter = (motor_L + motor_R)/2;
  if(moter < 0 )
    moter += 200;
  if(moter > 200)
    moter -= 200;
  moter = map(moter, 0, 200, 0, 360);
  stroke(0);
  fill(0);
  text("50", 835, height-260);
  text("150", 835, height-260+185);
  text("0", 840-92, height-180);
  text("100", 840+92, height-180);
  fill(255);
  stroke(0, 0, 0);
  ellipse(850, height-180, 150, 150);
  omegam = moter/180.0 * PI;//送られてきた値をラジアンに変換

 float y2 = -sin(omegam);//モータの角度を計算
 float x2 = -cos(omegam);//モータの角度を計算

stroke(0);
line( sin(0)*70+850, -cos(3.141592)*70+height-180 , sin(0)*70+850, -cos(0)*70+height-180);//モータの角度を描画
line( 0*70+850+75, 0*70+height-180 , 0*70+850-75, 0*70+height-180);//モータの角度を描画
stroke(255, 0, 0);
line(850, height-180, x2*70+850, y2*70+height-180);//モータの角度を描画
stroke(0);
fill(0);
text("speedMeter",770,height-300);
}
void Drawhillclime(){
  

  
  noStroke();  fill(255);  //rect(0,0,1024,768); 
  textSize(30);
  fill(0);
  
  textSize(30);
  text("N", 490, height-460);
  text("S", 490, height-460+185);
  text("W", 490-92, height-380);
  text("E", 490+92, height-380);

  fill(255);
  stroke(0, 0, 0);
  ellipse(500, height-380, 150, 150);
  omega = direc/180.0 * PI;//送られてきた値をラジアンに変換
float x = sin(omega);//モータの角度を計算
float y = -cos(omega);//モータの角度を計算

stroke(0);
line( sin(0)*70+500, -cos(3.141592)*70+height-380 , sin(0)*70+500, -cos(0)*70+height-380);//モータの角度を描画
line( 0*70+575, 0*70+height-380 , 0*70+500-75, 0*70+height-380);//モータの角度を描画
stroke(255, 0, 0);
line(500, height-380, x*70+500, y*70+height-380);//モータの角度を描画

  // Draw Acceleration vector
 /* CX = 250;
  drawVec(ax/100,ay/100,az/100);
  // Draw Magnetic flux vector
  CX = 750;
  drawVec(mx,my,mz);
  // Draw Heading direction
  CX = 600;
  float scale = 0.5;*/
 /* line(CX-scale*my,100+scale*mx,CX+scale*my,100-scale*mx);
  line(CX+scale*my,100-scale*mx,CX+0.6*scale*my+0.2*scale*mx,100-0.6*scale*mx+0.2*scale*my);
  line(CX+scale*my,100-scale*mx,CX+0.6*scale*my-0.2*scale*mx,100-0.6*scale*mx-0.2*scale*my);*/
  CalcDisp();
  DrawZone3();
  stroke(0,0,0);/*
  if(colorcheck==0){
    fill(255,255,255);
  }else if(colorcheck==1){
    fill(255,0,0);
  }else{
    fill(0,0,255);
  }*/
  
//  rect(100, 550, 200, 180);
  translate(width*1/4, height/2);    //立体の中心を画面中央に移動
  angle = 360-direc;    //角度を1ずつ足していく
  anglex = map(ax/100,-170,170,-90,90);
  anglez = map(ay/100,-170,170,90,-90);
  rotateY(radians(angle));
  rotateX(radians(anglex));
   rotateZ(radians(anglez));
   stroke(0,0,0);
 //fill(0x00FFFFFF);
   fill(red,green, blue);
  box(150, 50, 200);    //150 x 150 x 150pxの立方体を描
  translate(100, 25,0);    //立体の中心を画面中央に移動
  fill(150,150,150);
  box(50, 50, 180);    //150 x 150 x 150pxの立方体を描
  translate(-200, 0,0);    //立体の中心を画面中央に移動
  fill(150,150,150);
  box(50, 50, 180);    //150 x 150 x 150pxの立方体を描
  //pillar(150,50 ,50);
  
}
void CalcDisp(){
  /*count++;
  if(count%10 != 0){
    ac_xyz += sqrt(accel_X*accel_X + accel_Y*accel_Y + accel_Z*accel_Z);
  }else{
    ac_xyz = ac_xyz/10;
    V = ac_xyz/6 + V;
    dx = V/6 + ac_xyz/2*36;
  }
*/
  if (colorCheck_G != colorBef){
    colorCount++;
    colorBef= colorCheck_G;
  }
 // print("ac_xyz = ");print(ac_xyz);print(" V = ");print(V);print(" dx = ");println(dx);
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
    case 3:
      //red
      stroke(255,0,0);
      fill(255,0,0);
      break;
  }
  ellipse(width * 0.22, height * 0.85 ,120,120);
  fill(0);
  stroke(0,0,0);
  text("checked", width*0.17, height*0.9);
  text("colorCount ", width*0.35, height*0.9);
  text((int)colorCount, width*0.50 + 40, height*0.9);
}
void DrawZoneTask() {
  //ゾーンの描画(下半分のやつ)
  /*if(zoneNumber != 4){
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
  }*/
  //各ゾーンタスクで必要な描画、処理
  switch(zoneNumber) {
  case 1://linetrace
    
    break;
  case 2://carling
    break;
  case 3://hillclimbing
    Drawhillclime();

    break;
  case 4://winning
    break;
  default:
    break;
  }
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

  text(direc, width*0.65 +140, height*0.05);
  text("("+red+",", width*0.65 +80, height*0.1);
  text(green+",", width*0.65 +135, height*0.1);
  text(blue+")", width*0.65 +190, height*0.1);
  fill(red, green, blue);
  rect(width*0.65, height*0.1 + 5, 300, 50);
  fill(0);
  text("("+mx+",", width*0.65, height*0.25);
  text(my+",", width*0.65 +120, height*0.25);
  text(mz+")", width*0.65 +240, height*0.25);
  text("("+ax+",", width*0.65, height*0.35);
  text(ay+",", width*0.65 +120, height*0.35);
  text(az+")", width*0.65 +240, height*0.35);
  text("("+motor_L+",", width*0.65, height*0.45);
  text(motor_R+")", width*0.65 +120, height*0.45);
  
  moterMerter();
  
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

void serialEvent(Serial p) {
  if (p.available() >=23 ) {
    if (p.read() == 'H') {
      ax = read2byte(p);      ay = read2byte(p);      az = read2byte(p);
      mx = read2byte(p);      my = read2byte(p);      mz = read2byte(p);
      direc = read2byte(p);
      //colorcheck=p.read();
      colorCheck_G  = p.read();
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
