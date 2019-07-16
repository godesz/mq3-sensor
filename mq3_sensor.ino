/*  Version 0.01
 *  Just testing the github
 *  comments are missing
 *  final alc level is not valid, but something 
 *  Need more calibration
 */

#include <U8g2lib.h>
#include <FastLED.h>
#include <SPI.h>

#define NoD 100
#define NoReject 20 //always Noreject*2+1<NoD

CRGB leds[1];

int16_t alc_value = 0;
int16_t basic_value = 25;
int16_t max_sensorvalue = 800;

uint16_t alc_data[NoD];

U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);

float get_NoD_m()
{
  int16_t i,j, swap;
  float result = 0;
  //alc_data
  for(i=0;i<NoD;i++) alc_data[i]=analogRead(A0);

  for(i=0;i<NoD-1;i++)
    for(j=i+1;j<NoD;j++)
      {
        if (alc_data[i]>alc_data[j]) {
          swap = alc_data[i];
          alc_data[i] = alc_data[j];
          alc_data[j] = swap;
          }
      }
    for(i=NoReject;i<NoD-NoReject;i++)
    result += alc_data[i];
  Serial.print("tomb:");
  Serial.println(result);
  result/=NoD-(2*NoReject);
    return result;
}


void u8g2_prepare() {
  // OLED setup
  u8g2.setFont(u8g2_font_logisoso42_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}

void u8g2_printnumber(int alc_val) {
  float level = ((alc_val - basic_value) / (max_sensorvalue - basic_value)) * 3.5;   // Calculating from A0 to g/L
  u8g2.setCursor( 1, 5);
  if (level > 0.00) {
    u8g2.print(level, 2);
    u8g2.setFont(u8g2_font_helvB18_tf);
    u8g2.drawStr(105, 25, "%");
  }
  else u8g2.drawStr(1, 5, "0.00%");
  
}
void u8g2_print_a0(int alc_val) {
  u8g2.setFont(u8g2_font_crox3h_tf);
  //float level = (alc_val-25.0)/400;
  float level = (get_NoD_m()-25.0)/400;
  u8g2.setCursor( 1, 5);
  u8g2.print(alc_val);
  u8g2.setCursor( 1, 35);
  if (level>0.001) u8g2.print(level,2);
  else u8g2.print("No %");
  
}


void setup() {
  u8g2.begin();   // Init of OLED (128x64)
  u8g2_prepare();  
  
  Serial.begin(9600);
  delay( 3000 );    // Starting delay
  FastLED.addLeds<WS2812, 5, GRB>(leds, 1).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( 16 );
  leds[0] = CRGB(0, 0, 255); FastLED.show();  delay(500);
  leds[0] = CRGB(0, 255, 0); FastLED.show();  delay(500);
  leds[0] = CRGB(255, 0, 0); FastLED.show();  delay(500);
  //get base A0 datas
  for(int i=0;i<NoD;i++) alc_data[i]=analogRead(A0);
}

void loop() {
  alc_value = analogRead(A0);
  //Serial.print("A0: "); Serial.println(alc_value); //debug for values
  u8g2.clearBuffer();
  u8g2_printnumber(alc_value);
  //u8g2_print_a0(alc_value);
  u8g2.sendBuffer();
  leds[0] = CRGB(random(0, 255), random(0, 255), random(0, 255));  // 1 led indicator
  FastLED.show();
  delay(100);
}
