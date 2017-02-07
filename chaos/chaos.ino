
#include <SPI.h>
#include <SD.h>
#include "Adafruit_GFX.h"
#include "Waveshare_HX8347D.h"

#include "global.h"


File miro_file;
Waveshare_HX8347D tft = Waveshare_HX8347D();


int mouse_speed_value = 5;


void load_miro_file()
{
  int i;
  uint8_t ch;
  
  
  Serial.print("Initializing SD card...");

  if (!SD.begin(5)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");


  
  i = 0;
  miro_file = SD.open("0th.maz");  
  if (miro_file) 
  {
    Serial.println("0th.maz");
    
    // read from the file until there's nothing else in it:
    while (miro_file.available()) {
      //Serial.write(miro_file.read());
      ch = miro_file.read();
      mirohide[i] = ch;
      mirohide[i] |= 0xF0 ;      
      i++;
    }
    // close the file:
    miro_file.close();
    Serial.println("load ok");
  }  
  
}

void frame_update()
{
  static uint8_t update = 0;

  if(update > mouse_speed_value)
  { 
    tft.drawFrame();
    update = 0;
  }  
  update++;
}

uint32_t line_color;

void setcolor(int color)
{
  line_color = color;
}

void setlinestyle(int a, int b, int c)
{
  
}

void line(int x0, int y0, int x1, int y1)
{
  tft.drawLine(x0, y0, x1, y1, line_color);
}

void outtextxy(int x, int y, char *p_str)
{
  tft.setCursor(x, y);
  tft.print(p_str);
}

void putpixel(int x, int y, int color)
{
  tft.drawPixel(x, y, color);
}

void body(double *xx1,double *yy1)
{
    setcolor(WHITE);
    line(ceil(xx1[5]),ceil(yy1[5]),ceil(xx1[6]),ceil(yy1[6]));
    line(ceil(xx1[5]),ceil(yy1[5]),ceil(xx1[7]),ceil(yy1[7]));
    line(ceil(xx1[6]),ceil(yy1[6]),ceil(xx1[8]),ceil(yy1[8]));
    line(ceil(xx1[7]),ceil(yy1[7]),ceil(xx1[8]),ceil(yy1[8]));
    putpixel(ceil(xx1[9]),ceil(yy1[9]),WHITE);
    putpixel(ceil(xx1[10]),ceil(yy1[10]),WHITE);

  frame_update();
}

void body_e(double *xx2,double *yy2)
{
  
  
    setcolor(BLACK);
    setlinestyle(0,0,3);
    line(ceil(xx2[5]),ceil(yy2[5]),ceil(xx2[6]),ceil(yy2[6]));
    line(ceil(xx2[5]),ceil(yy2[5]),ceil(xx2[7]),ceil(yy2[7]));
    line(ceil(xx2[6]),ceil(yy2[6]),ceil(xx2[8]),ceil(yy2[8]));
    line(ceil(xx2[7]),ceil(yy2[7]),ceil(xx2[8]),ceil(yy2[8]));
    putpixel(ceil(xx2[9]),ceil(yy2[9]),BLACK);
    putpixel(ceil(xx2[10]),ceil(yy2[10]),BLACK);

    setlinestyle(0,0,1);
    //tft.drawFrame();
}

void body2(double x,double y,double *xx3,double *yy3 )
{
    setcolor(YELLOW);
    line(ceil(x+xx3[5]),ceil(y+yy3[5]),ceil(x+xx3[6]),ceil(y+yy3[6]));
    line(ceil(x+xx3[5]),ceil(y+yy3[5]),ceil(x+xx3[7]),ceil(y+yy3[7]));
    line(ceil(x+xx3[6]),ceil(y+yy3[6]),ceil(x+xx3[8]),ceil(y+yy3[8]));
    line(ceil(x+xx3[7]),ceil(y+yy3[7]),ceil(x+xx3[8]),ceil(y+yy3[8]));
    putpixel(ceil(x+xx3[9]),ceil(y+yy3[9]),WHITE);
    putpixel(ceil(x+xx3[10]),ceil(y+yy3[10]),WHITE);
    setcolor(WHITE);
    frame_update();
}


void body_e2(double x,double y,double *xx4,double *yy4)
{
    setcolor(BLACK);
    setlinestyle(0,0,3);
    line(ceil(x+xx4[5]),ceil(y+yy4[5]),ceil(x+xx4[6]),ceil(y+yy4[6]));
    line(ceil(x+xx4[5]),ceil(y+yy4[5]),ceil(x+xx4[7]),ceil(y+yy4[7]));
    line(ceil(x+xx4[6]),ceil(y+yy4[6]),ceil(x+xx4[8]),ceil(y+yy4[8]));
    line(ceil(x+xx4[7]),ceil(y+yy4[7]),ceil(x+xx4[8]),ceil(y+yy4[8]));
    putpixel(ceil(x+xx4[9]),ceil(y+yy4[9]),BLACK);
    putpixel(ceil(x+xx4[10]),ceil(y+yy4[10]),BLACK);
    setlinestyle(0,0,1);
    setcolor(WHITE);
    //tft.drawFrame();
}

void bar2(int x,int y,int x2,int y2,int color)
{
    //setfillstyle(1,color);
    tft.drawLine(x,y,x2,y2, color);
    
    frame_update();
}

void setup() {
  uint32_t t_time;
  
  Serial.begin(9600);
  while(!Serial);

  tft.begin();
  load_miro_file();

  tft.begin();
  tft.setRotation(1);    
  tft.fillScreen(HX8347D_BLACK);


  //init_run();
  //body(x_dim,y_dim);

  tft.setTextColor(HX8347D_WHITE);
  tft.setTextSize(1);

  
  mouse_speed_value = 5;
  first_run();

  //deka_run();
  mouse_speed_value = 15;
  second_run();
    
  Serial.println("init");
}


void loop(void) {
}

