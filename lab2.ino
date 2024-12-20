#include <Arduino.h> 
#include <U8g2lib.h>
#include <DHT.h>
#include <RTClib.h>

#define PHOTORESICTOR_PIN A0
#define VIN 5 // V power voltage
#define R 10000 //ohm resistance value
 
U8G2_ST7565_ERC12864_1_4W_SW_SPI u8g2 (U8G2_R0, /* scl=*/ 13 , /* si=*/ 11 , /* cs=*/ 10 , /* rs=*/ 9 , /* rse=*/ 8);
DHT dht(4, DHT11);
RTC_DS3231 rtc;
 
void setup() {
  Serial.begin(9600);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the time!");
    // Set the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  } else {
    Serial.println("RTC is running with correct time.");
  }

  u8g2.begin();
  u8g2.setContrast(10);
  u8g2.enableUTF8Print();

  pinMode(PHOTORESICTOR_PIN, INPUT_PULLUP);
}
 
void loop() {
  u8g2.firstPage();

  do {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    u8g2.setFont(u8g2_font_cu12_t_cyrillic);

    u8g2.setCursor(0, 12);
    u8g2.print("H: ");
    u8g2.print(h);
    u8g2.print(" %");

    u8g2.setCursor(0, 28);
    u8g2.print("T: ");
    u8g2.print(t);
    u8g2.print(" C");

    u8g2.setCursor(0, 44);
    u8g2.print("L: ");
    u8g2.print(
      sensorRawToPhys(
        analogRead(PHOTORESICTOR_PIN)
      )
    );
    u8g2.print(" lum");
    

    DateTime now = rtc.now();

    char buffer[20];
    sprintf(
      buffer,
      "%02d.%02d.%02d %02d:%02d:%02d",
      now.day(),
      now.month(),
      now.year() % 100,
      now.hour(),
      now.minute(),
      now.second()
    );

    u8g2.setCursor(0, 60);
    u8g2.print(buffer);

  } while(u8g2.nextPage());

  delay(1000);
 }

// Conversion rule
int sensorRawToPhys(int raw) {
  float Vout = float(raw) * (VIN / float(1023));// Conversion analog to voltage
  float RLDR = (R * (VIN - Vout)) / Vout; // Conversion voltage to resistance
  int phys = 500 / (RLDR / 1000); // Conversion resitance to lumen

  return phys;
}