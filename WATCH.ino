#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

String val;
int displayhour;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RTC_DS3231 rtc;
char daysOfTheWeek [7][4] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};

void setup() {
  
 
  Serial.begin(9600);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 initialization failed"));
    while (1);
  }

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("RTC not found!");
    display.display();
    while (1);
  }

  // If RTC isn't running, set it to compile time
  if (!rtc.begin()) {
    Serial.println("RTC not running, setting the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  
  display.display();
  delay(2000);


  display.clearDisplay();
}

void loop() {
  float temp  = rtc.getTemperature();
  float tempf = (temp * 1.8) +32;
 
  DateTime now = rtc.now();

  

  // Date line
  display.setTextSize(1.5);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.print(daysOfTheWeek[now.dayOfTheWeek()]);
  display.print(" <<< ");
  display.print(now.month());
  display.print('/');
  display.print(now.day());
  display.print('/');
  display.println(now.year());

  if(now.hour()==0){
    displayhour = 12;
    val = "AM";

  }
  else if(now.hour()<12){
    displayhour = now.hour();
    val = "AM";
  }
  else if (now.hour()==12){
    displayhour = 12;
    val = "PM";
  }
  else if(now.hour()>12){
    displayhour = now.hour() -12;
    val = "PM";
  }
  // Time line
  display.setTextSize(2);
  display.setCursor(12, 20);
  if (displayhour< 10) display.print('0');
  display.print(displayhour);
  display.print(':');
  if (now.minute() < 10) display.print('0');
  display.print(now.minute());
  display.print(':');
  
  if (now.second() <10) display.print('0');
  display.print(now.second());
  display.print("");
  display.setTextSize(1.5);
  display.print(val);
  
  display.setCursor(45,36);
  
  display.print(tempf);
  display.print("' F");
  display.setCursor(10,50);
  if (val == "AM"){
    display.print("Good Morning Gabby");
  }
    
  if(val == "PM"  && displayhour <= 5){
    display.print("Good Afternoon Gabby");

  }
  if(val == "PM" && displayhour >=6)  {
    display.print("Good Evening Gabby");
  }

  display.display();
  

  delay(1000);
  display.clearDisplay();
}
