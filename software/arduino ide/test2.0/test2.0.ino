//头文件
#include <MPU6050_tockn.h>
#include <Wire.h>
#include <BleCombo.h>
#include <math.h>
#include <string.h>
#include <U8g2lib.h>
#include "BluetoothSerial.h"
#include "FS.h"
#include "SPIFFS.h"
#include <SimpleKalmanFilter.h>
#include <Arduino.h>

//储存手势信息
//顺时针
float clockwise_yaw[32] = {0,-0.201,-0.394,-0.571,-0.725,-0.849,-0.938,-0.988,-0.999,-0.968,-0.898,-0.791,-0.651,-0.485,-0.299,-0.101,0.101,0.299,0.485,0.651,0.791,0.898,0.968,0.999,0.988,0.938,0.849,0.725,0.571,0.394,0.201,0};
float clockwise_agy[32] = {-1,-0.980,-0.919,-0.821,-0.689,-0.529,-0.347,-0.151,0.051,0.251,0.440,0.612,0.759,0.874,0.954,0.995,0.995,0.954,0.874,0.759,0.612,0.440,0.251,0.051,-0.151,-0.347,-0.529,-0.689,-0.821,-0.919,-0.980,-1};
//逆时针
float anticlock_yaw[32] = {0,0.201,0.394,0.571,0.725,0.849,0.938,0.988,0.999,0.968,0.898,0.791,0.651,0.485,0.299,0.101,-0.101,-0.299,-0.485,-0.651,-0.791,-0.898,-0.968,-0.999,-0.988,-0.938,-0.849,-0.725,-0.571,-0.394,-0.201,0};
float anticlock_agy[32] = {-1,-0.980,-0.919,-0.821,-0.689,-0.529,-0.347,-0.151,0.051,0.251,0.440,0.612,0.759,0.874,0.954,0.995,0.995,0.954,0.874,0.759,0.612,0.440,0.251,0.051,-0.151,-0.347,-0.529,-0.689,-0.821,-0.919,-0.980,-1};
//对勾
float tick_yaw[32] = {69.530,69.211,68.891,68.563,68.234,67.980,67.738,67.241,66.680,65.877,64.987,63.796,62.456,60.695,58.653,56.134,53.196,50.158,47.003,43.918,40.937,38.303,36.363,34.686,33.733,32.927,32.715,32.571,32.881,33.202,33.676,34.150};
float tick_agy[32] = {-5.720,-5.338,-4.955,-4.081,-3.171,-0.857,1.674,4.746,7.955,10.879,13.700,15.888,17.761,18.777,19.222,18.965,18.095,16.655,14.564,12.253,9.611,6.895,4.031,1.367,-0.742,-2.745,-4.323,-5.828,-6.864,-7.853,-8.202,-8.550};

float right_arrow_yaw[12]={-7.46,-14.61,-22.13,-28.88,-31.76,-30.22,-26.53,-22.57,-16.77,-11.55,-7.72,-5.45};
float left_arrow_yaw[16]={0};
//自定义动态手势
float gesture_1_yaw[32]={0};
float gesture_1_agy[32]={0};
float gesture_1_agx[32]={0};
int gesture_1_thumb[32]={0};
int gesture_1_second[32]={0};
int gesture_1_third[32]={0};
int gesture_1_fourth[32]={0};
int gesture_1_fifth[32]={0};
//自定义静态手势
float gesture_2_thumb;
float gesture_2_second;
float gesture_2_third;
float gesture_2_fourth;
float gesture_2_fifth;

//多线程头文件
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
//宏定义
#define Min(a,b) (a<b?a:b)
//声明u8g2和mpu
U8G2_SH1107_128X80_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE,22,21);
TwoWire myWire(1);
MPU6050 mpu6050(myWire);
BluetoothSerial SerialBT;
//卡尔曼滤波器
SimpleKalmanFilter simpleKalmanFilter1(0.5, 0.5, 0.007);
SimpleKalmanFilter simpleKalmanFilter2(0.5, 0.5, 0.007);
SimpleKalmanFilter simpleKalmanFilter3(0.5, 0.5, 0.007);
SimpleKalmanFilter simpleKalmanFilter4(0.5, 0.5, 0.007);
SimpleKalmanFilter simpleKalmanFilter5(0.5, 0.5, 0.007);
String device_name = "ESP32-BT-Slave-YYC";

//一些声明
unsigned long ab=0,lasta=0,minus=0;
unsigned long abc=0,lastabc=0;
float lastagx=0,lastagy=0,lastyaw=0;
int thumb=0, second=0,third=0,fourth=0,fifth=0;
float estimated1=0, estimated2=0,estimated3=0,estimated4=0,estimated5=0;
float a[32]={0},b[32]={0},c[32]={0},d[32]={0},e[32]={0};
float AGY[32]={0}, YAW[32]={0}, AGX[32]={0};
float averageAGY=0, averageYAW=0, avergeAGX=0;
//unbend手势
int clockwise=7,anticlock=8,tick=11;
//bend345手势
int clockwise_bend345=2,anticlock_bend345=3,tick_bend345=17;
//bend45手势
int clockwise_bend45=20,anticlock_bend45=21,tick_bend45=1;
//自定义手势
int gesture_1=0;
int record_jing_count=0,record_dong_count=0;
int flag1=0,flag2=0,flag3=0,flag4=0;
int flagDelay1=0,flagDelay2=0,flagDelay3=0,flagDelay4=0,flagDelay5=0;     
int changed=1;                         
int dong_record_begin=0;       
int record_done=0;                                           
//读取上位机串口信息
const int maxLength = 50; // 设置最大字符串长度
char receivedString[maxLength]; // 用于存储接收到的字符串
int stringIndex = 0; // 当前字符在字符串中的索引
//三层状态机
enum State{KeyMouse,Record,VR};
State state=KeyMouse;
//模式
enum Motion{unbend,bend2,bend3,bend4,bend5,bend45,bend345,bend2345,none,before,record_jing,record_dong,after};
Motion motion=none;
//动态手势状态
enum Dong_state{not_dong,start_record_dong};
Dong_state dong_state=not_dong;
//修改&删除手势函数并储存在文件中
void reviseFunction(int i,int j)
{
  switch(i)
  {
    case 1:
      clockwise=j;
    break;
    case 2:
      anticlock=j;
    break;
    case 3:
      tick=j;
    break;
  }
}

//计算相似度(相关系数)
float waveSimilarity(const float* wave1, const float* wave2, int size)
 {  // 计算均值
    float mean_wave1 = 0.0;
    float mean_wave2 = 0.0;
    for (size_t i = 0; i < size; ++i) {
      mean_wave1 += wave1[i];
      mean_wave2 += wave2[i]; }
    mean_wave1 /=size;
    mean_wave2 /=size; 
    float numerator = 0.0;// 计算相关系数的分子和分母
    float denominator_wave1 = 0.0;
    float denominator_wave2 = 0.0;
    for (size_t i = 0; i < size; ++i) {
      float deviation_wave1 = wave1[i] - mean_wave1;
      float deviation_wave2 = wave2[i] - mean_wave2;
      numerator += deviation_wave1 * deviation_wave2;
      denominator_wave1 += deviation_wave1 * deviation_wave1;
      denominator_wave2 += deviation_wave2 * deviation_wave2;
    }  // 计算相关系数
    float correlation_coefficient = numerator / (sqrt(denominator_wave1) * sqrt(denominator_wave2));
    return correlation_coefficient;
}

//时间拉伸算法(实现在时间尺度上波形的拉伸或压缩)
void elasticTimeStretch(const float* waveform, int originalLength, float* stretchedWaveform, int stretchedLength) {
  for (int i = 0; i < stretchedLength; i++) {
    float position = static_cast<float>(i) * (originalLength - 1) / (stretchedLength - 1);
    int prevPosition = static_cast<int>(position);
    int nextPosition = prevPosition + 1;
    if (nextPosition >= originalLength) {
      stretchedWaveform[i] = waveform[prevPosition];
    }
    else {
      double fraction = position - prevPosition;
      stretchedWaveform[i] = waveform[prevPosition] + fraction * 
      (waveform[nextPosition] - waveform[prevPosition]);
    }
  }
}

//键鼠功能执行函数
void function(int i)
{
  switch(i)
  {
    case 1:
    Keyboard.write(KEY_MEDIA_PLAY_PAUSE);
    break;
    case 2:
    Keyboard.write(KEY_RIGHT_ARROW);
    break;
    case 3:
    Keyboard.write(KEY_LEFT_ARROW);
    break;
    case 4:
    Keyboard.write(KEY_UP_ARROW);
    break;
    case 5:
    Keyboard.write(KEY_DOWN_ARROW);
    break;
    case 6:
    Keyboard.write(KEY_MEDIA_MUTE);
    break;
    case 7:
    Keyboard.write(KEY_MEDIA_VOLUME_UP);
    break;
    case 8:
    Keyboard.write(KEY_MEDIA_VOLUME_DOWN);
    break;
    case 9:
    Keyboard.write(KEY_MEDIA_NEXT_TRACK);
    break;
    case 10:
    Keyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
    break;
    case 11:
    Keyboard.write(KEY_MEDIA_CALCULATOR);
    break;
    case 12:
    Keyboard.write(KEY_MEDIA_EMAIL_READER);
    break;
    case 13:
    Keyboard.write(KEY_MEDIA_WWW_SEARCH);
    break;
    case 14://撤回
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.write(90);
    Keyboard.releaseAll();
    break;
    case 15://全选
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.write(65);
    Keyboard.releaseAll();
    break;
    case 16://保存
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.write(83);
    Keyboard.releaseAll();
    break;
    case 17://截图
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.write(82);
    Keyboard.releaseAll();
    break;
    case 18://复制
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.write(67);
    Keyboard.releaseAll();
    break;
    case 19://粘贴
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.write(86);
    Keyboard.releaseAll();
    break;
    case 20://ppt播放
    Keyboard.press(KEY_F5);
    Keyboard.releaseAll();
    break;
    case 21://ESC键
    Keyboard.press(KEY_ESC);
    Keyboard.releaseAll();
    break;
  }
}

//计算相似度
float calcualte_similarity(float* Agy,float* Yaw, int& flag,int functionnum )
{

}

//检查相似性函数
void checkSimilarity(float* Agy,float* Yaw,int& flag,int functionnum,float threshold)
{
  int gesturecircle1=0;
  for(int i=24;i<33&&!gesturecircle1&&!flag; i+=2)
    {
        float stretchedagy11[32]={0},stretchedyaw11[32]={0};
        elasticTimeStretch(AGY,i,stretchedagy11,32);
        elasticTimeStretch(YAW,i,stretchedyaw11,32);
        float sim1=waveSimilarity(stretchedyaw11,Yaw,32);
        float sim2=waveSimilarity(stretchedagy11,Agy,32);
          if(sim2>threshold&&sim1>threshold)
          {
            gesturecircle1=1;
            flag=30;
          }
      if(gesturecircle1)
      {
        Serial.println("Similar!");
        //Keyboard.write(KEY_MEDIA_VOLUME_DOWN);
        function(functionnum);
      }
    }
    if(flag)
    {flag--;}
}

//oled绘制函数
void draw(int state,int motion)
{
      u8g2.firstPage();
    do {   
      u8g2.setFont(u8g2_font_unifont_t_chinese1); 
      if(motion==8)
      {
         u8g2.setCursor(18,35);
         u8g2.print("陀螺仪校准中");
         u8g2.setCursor(34,55);
         u8g2.print("请勿移动");
      }
      else
      {
      u8g2.setCursor(2,15);
      u8g2.print("当前模式：");
      u8g2.setCursor(34,35);
      if(state==0)
      {
      u8g2.print("键鼠模式"); 
      u8g2.setCursor(2,55);
      u8g2.print("当前状态：");
        if(motion==0)
        {
          u8g2.setCursor(34,75);
          u8g2.print("鼠标移动"); 
        }
        else if(motion==1)
        {
          u8g2.setCursor(34,75);
          u8g2.print("左键点击"); 
        }
          else if(motion==2)
        {
          u8g2.setCursor(34,75);
          u8g2.print("右键点击"); 
        }
          else if(motion==3)
          {
          u8g2.setCursor(34,75);
          u8g2.print("滚轮点击"); 
          }
          else if(motion==4)
          {
          u8g2.setCursor(34,75);
          u8g2.print("小指弯曲"); 
          }
          else if(motion==5)
          {
          u8g2.setCursor(34,75);
          u8g2.print("二指状态"); 
          }
          else if(motion==6)
          {
          u8g2.setCursor(34,75);
          u8g2.print("食指状态"); 
          }
          else if(motion==7)
          {
          u8g2.setCursor(34,75);
          u8g2.print("握拳状态"); 
          }
      }
      else if(state==1)
      {
        u8g2.print("录制模式"); 
        u8g2.setCursor(2,55);
        u8g2.print("当前状态：");
          if(motion==9)
          {
            u8g2.setCursor(34,75);
            u8g2.print("录制准备"); 
          }
          else if(motion==10||motion==11)
          {
            u8g2.setCursor(34,75);
            u8g2.print("录制中"); 
          }
          else if(motion==12)
          {
            u8g2.setCursor(34,75);
            u8g2.print("录制结束"); 
          }
        }
        else if(state==2)
        {
          u8g2.print("动捕模式"); 
        }
      }
      u8g2.drawFrame(0,0,u8g2.getDisplayWidth(),u8g2.getDisplayHeight() );
    } while (u8g2.nextPage());
}

//多线程执行oled绘制函数
void u8g2_task(void *pvParameters) {
  while (1) {
    draw((int)(state),(int)(motion));
    vTaskDelay(pdMS_TO_TICKS(50)); // 延迟一秒
  }
}

//文件读取
void readFile()
{

}

//文件写入
void writeFile()
{

}

bool checkunbend(int *a)
{
  float sum=0;
  float average=0;
   for(int i=0;i<8;i++)
   {
     sum+=a[i];
   }
   average=sum/8;
   if(average>2200)
   {return true;}
   else 
   return false;
}

bool checktotalbend(int *a)
{
  float sum=0;
  float average=0;
   for(int i=0;i<8;i++)
   {
     sum+=a[i];
   }
   average=sum/8;
   if(average<2000)
   {return true;}
   else 
   return false;
}

void setup() {
  Serial.begin(115200);
  //
  u8g2.begin();
  u8g2.setDisplayRotation(U8G2_R1);
  u8g2.setFlipMode(1);
  u8g2.enableUTF8Print(); 
  draw((int)(state),(int)(motion));
  //
  Keyboard.begin();
  Mouse.begin();
  //
  myWire.begin(32,33);
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  motion=unbend;
  //开启多线程
  xTaskCreatePinnedToCore(u8g2_task, "u8g2_task", 4096, NULL, 1, NULL, 1);
}
void loop()
{
  if(changed)
  {
    if(Keyboard.isConnected())
    {
      Mouse.end();
      Keyboard.end();
      SerialBT.begin(device_name);
      Serial.println("BT start!");
      changed=0;
    }
  }
  // if (SerialBT.available()) {
  //   Serial.write(SerialBT.read());
  // }
  //读取串口数据
  if (SerialBT.available() > 0)
  {
    char receivedChar = SerialBT.read(); // 读取一个字节数据
    if (receivedChar == '\n' || stringIndex >= maxLength - 1) {
      receivedString[stringIndex] = '\0'; // 在字符串末尾添加空字符，构成C风格字符串
      if(strcmp(receivedString,"jing")==0||strcmp(receivedString,"dong")==0)
      {
        state=Record;
        motion=before;
      }
      if(strcmp(receivedString,"dongstart")==0)
      {
        motion=record_dong;
      }
      if(strcmp(receivedString,"KeyMouse")==0)
      {
        state=KeyMouse;
        motion=unbend;
      }
      if(strcmp(receivedString,"016")==0)
      {
        clockwise=7;
      }
      else if(strcmp(receivedString,"017")==0)
      {
        clockwise=8;
      }
      else if(strcmp(receivedString,"018")==0)
      {
        clockwise=9;
      }
      else if(strcmp(receivedString,"019")==0)
      {
        clockwise=10;
      }
      else if(strcmp(receivedString,"0110")==0)
      {
        clockwise=11;
      }
      else if(strcmp(receivedString,"0111")==0)
      {
        clockwise=11;
      }
      if(strcmp(receivedString,"gesture112")==0)
      {
        gesture_1=13;
      }
      else if(strcmp(receivedString,"gesture116")==0)
      {
        gesture_1=17;
      }
      stringIndex = 0;
    } else {
      receivedString[stringIndex] = receivedChar;
      stringIndex++;
    }
  }  
  //程序主体
  if(Keyboard.isConnected())
  {
  //获取五指信号
  thumb=2000-analogRead(37);
  second=analogRead(38)-110;
  third=analogRead(35);
  fourth=analogRead(34);
  fifth=(int)(1.35*analogRead(12));
  estimated1 = simpleKalmanFilter1.updateEstimate(thumb);
  estimated2 = simpleKalmanFilter2.updateEstimate(second);
  estimated3 = simpleKalmanFilter3.updateEstimate(third);
  estimated4 = simpleKalmanFilter4.updateEstimate(fourth);
  estimated5 = simpleKalmanFilter5.updateEstimate(fifth);
  //获取mpu三轴信号
  mpu6050.update();
  float agx = mpu6050.getAngleX();
  float agy = mpu6050.getAngleY();
  float yaw = mpu6050.getAngleZ();
  //移动鼠标
  Mouse.move((int)(11*lastyaw-11*yaw),(int)(8*agy-8*lastagy));
  for (int i=1;i<32;i++)
  {
    AGY[i-1]=AGY[i];
    YAW[i-1]=YAW[i];
  }
  AGY[31]=agy;YAW[31]=yaw;
  //Serial.printf("%f %f %f %f %f\n",estimated1,estimated2,estimated3,estimated4,estimated5);
   // Serial.print(agx);      //输出x轴数据
   // Serial.print("yaw:");
   // Serial.print(yaw);
   // Serial.print("  agy:");      //输出y轴数据
   // Serial.println(agy);     
   //食指顺时针画圈
if(state==KeyMouse)
{ 
  if(motion==bend45)
  { //二指顺时针转圈
    checkSimilarity(clockwise_agy,clockwise_yaw,flagDelay1,clockwise_bend45,0.97);
    //二指逆时针转圈
    checkSimilarity(anticlock_agy,anticlock_yaw,flagDelay1,anticlock_bend45,0.97);
    //二指打勾
    //checkSimilarity(tick_agy,tick_yaw,flagDelay1,tick_bend45,0.97);
  }
  else if(motion==bend345)
  { //食指顺时针转圈
    checkSimilarity(clockwise_agy,clockwise_yaw,flagDelay2,clockwise_bend345,0.97);
    //食指逆时针转圈
    checkSimilarity(anticlock_agy,anticlock_yaw,flagDelay2,anticlock_bend345,0.97);
    //食指打勾
    checkSimilarity(tick_agy,tick_yaw,flagDelay2,tick_bend345,0.97);
  }
  else
  {
    checkSimilarity(clockwise_agy,clockwise_yaw,flagDelay3,clockwise,0.97);
    //二指逆时针转圈
    checkSimilarity(anticlock_agy,anticlock_yaw,flagDelay3,anticlock,0.97);

    //checkSimilarity(tick_agy,tick_yaw,flagDelay3,tick,0.97);
  }
  if(record_done==1)
  {
    checkSimilarity(gesture_1_agy,gesture_1_yaw,flagDelay4,gesture_1,0.97);
  }
}   //...
    //  float sim1=waveSimilarity(YAW,clockwise_yaw,32);
    //  float sim2=waveSimilarity(AGY,clockwise_agy,32);
    //  if(sim1*sim2>0.92)
    //  {
    //    Serial.print("wow!");
    //  }
    // Serial.print("sim1:");
    // Serial.print(sim1);
    // Serial.print("  sim2:");
    // Serial.println(sim2);
  //自定义静止手势1
    // float sim3=100*waveSimilarity(YAW,right_arrow_yaw,12);
    // float sim2=waveSimilarity(AGY,tick_agy,16);
    //  if(sim3>96&&(YAW[0]-YAW[6])>15)
    //  {
    //    //Serial.print("right_arrow!");
    //  }
    // Serial.print("sim1:");
    // Serial.println(sim1);
    // Serial.print("  sim2:");
    // Serial.println(sim2);
  //自定义动态手势1
  for (int i=1;i<32;i++)
  {
    a[i-1]=a[i]; b[i-1]=b[i]; c[i-1]=c[i]; d[i-1]=d[i]; e[i-1]=e[i];
  }
  a[31]=estimated1;b[31]=estimated2;c[31]=estimated3;d[31]=estimated4;e[31]=estimated5;
          // float sim1=waveSimilarity(YAW,right_arrow_yaw,16);
          // float sim2=waveSimilarity(YAW,left_arrow_yaw,16);
          // if(sim1>0.96&&agx>70)
          // {
          //   Keyboard.press(KEY_RIGHT_ARROW);
          //   Keyboard.release(KEY_RIGHT_ARROW);
          // }
          // if(sim2>0.96&&agx>70)
          // {
          //   Keyboard.press(KEY_LEFT_ARROW);
          //   Keyboard.release(KEY_LEFT_ARROW);
          // }
   switch(state)
   {
    case KeyMouse:
      switch(motion)
      {
        case unbend:
          if(estimated2<2000&&estimated3<2000&&estimated4<2000&&estimated5<1800)
          {
            motion=bend2345;
            flag4=0;
          }
          else if(estimated2<2000)
          {
            motion=bend2;
            flag1=0;
          }
          else if(estimated3<2000)
          {
            motion=bend3;
            flag2=0;
          }
          else if(estimated4<2000)
          {
            motion=bend4;
            flag3=0;
          }
          else if(estimated5<1800)
          {
            motion=bend5;
          }
        break;

        case bend2:
          flag1++;
          if(flag1<7&&estimated2>2100)
          {
            motion=unbend;
            Mouse.click(MOUSE_LEFT);
          }
          if(flag1>=7)
          {
            Mouse.press(MOUSE_LEFT);
            if(estimated2>2100)
            {
              motion=unbend;
              Mouse.release(MOUSE_LEFT);
            }
          }
        break;

        case bend3:
          flag2++;
          if(flag2<7&&estimated3>2100)
          {
            motion=unbend;
            Mouse.click(MOUSE_RIGHT);
          }
          if(flag2>=7)
          {
            Mouse.press(MOUSE_RIGHT);
            if(estimated3>2100)
            {
              motion=unbend;
              Mouse.release(MOUSE_RIGHT);
            }
          }
          if(estimated4<2000&&estimated5<1800)
          {
            motion=bend345;
          }
        break;

        case bend4:
          flag3++;
          if(flag3<7&&estimated4>2100)
          {
            motion=unbend;
            Mouse.click(MOUSE_MIDDLE);
          }
          if(flag3>=7)
          {
            Mouse.press(MOUSE_MIDDLE);
            if(estimated4>2100)
            {
              motion=unbend;
              Mouse.release(MOUSE_MIDDLE);
            }
          }
          if(estimated5<1800)
          {
            motion=bend45;
          }
          else if(estimated3<2000&&estimated5<1800)
          {
            motion=bend345;
          }
        //Mouse.move((int)(12*lastyaw-12*yaw),(int)(10*agy-10*lastagy));
        break;

        case bend5:
          if(estimated5>1900)
          {
            Keyboard.press(KEY_UP_ARROW);
            Keyboard.release(KEY_UP_ARROW);
            motion=unbend;
          }
          if(estimated4<2000)
          {
            motion=bend45;
          }
          if(estimated3<2000&&estimated4<2000)
          {
            motion=bend345;
          }
        break;

        case bend45:
          if(estimated4>2100||estimated5>1900)
          {
            motion=unbend;
          }
          if(estimated3<2000)
          {
            motion=bend345;
          }
        break;

        case bend345:
          if(estimated3>2100||estimated4>2100||estimated5>1900)
          {
            motion=unbend;
          }
        break;

        case bend2345:
          if(estimated2>2100||estimated3>2100||estimated4>2100||estimated5>1900)
          {
            motion=unbend;
          }
        break;   
      }
    break;

    case Record:
      if(motion==record_dong)
      {
        switch(dong_state)
        {
          case not_dong:
            if(second>2100||third>2100||fourth>2100)
            {
              dong_state=start_record_dong;
            }
          break;
          case start_record_dong:
            for (int i=1;i<16;i++)
            {
              gesture_1_yaw[i-1]=gesture_1_yaw[i];
              gesture_1_agy[i-1]=gesture_1_agy[i];
            }
            gesture_1_yaw[31]=yaw;
            gesture_1_agy[31]=agy;
            if(second<2000&&third<2000&&fourth<2000)
            {
              dong_state=not_dong;
              motion=unbend;
              state=KeyMouse;
              record_done=1;
            }
          break;
        }
      }
    break;

    case VR:
      SerialBT.printf("Data %f %f %f %f %f %f %f %f\n",(agx-lastagx),(agy-lastagy),(yaw-lastyaw),estimated1,estimated2,estimated3,estimated4,estimated5);
    break;
   }
    lastagy=agy;
    lastyaw=yaw;
    lastagx=agx;
  }
  delay(15);
}