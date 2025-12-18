//библиотеки для дисплэя
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
//библиотеки для NTP
#include <Arduino.h>
#include <GyverNTP.h>
//наименование дислэя
Adafruit_SH1107 display = Adafruit_SH1107(128, 128, &Wire, -1);
//расписание (всё в минутах)
int needtime[]={510, 550, 560, 600, 620, 660, 680, 720, 740, 780, 795, 835, 845, 885,1440};
void setup() {
  //инициализация дислэя
  if (!display.begin(0x3C, true)) {
    Serial.println(F("SH1107 initialization failed!"));
    for (;;); // Зависание при ошибке
  }
  
  //загрузка (exemple)
  display.display();// Показать загрузочный экран
  delay(2000);
  display.clearDisplay();  
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println(F("Hello SH1107!"));
  display.println(F("Adafruit_SH110X"));
  display.display(); // Обновить экран  
  //инициилизация Serial
  Serial.begin(9600);
  // настройка и инициализация wifi
  WiFi.begin("SSID", "PASSWORD");
  while (WiFi.status() != WL_CONNECTED) delay(100);
  display.clearDisplay(); 
  display.setCursor(0, 0);
  display.println("Connected");
  display.display();
  // обработчик ошибок NTP
  NTP.onError([]() {
    display.clearDisplay(); 
    display.setCursor(0, 0);
    display.println(NTP.readError());
    display.println("online: ");
    Serial.println(NTP.online());
    display.display();
  });
  //запуск NTP
  NTP.begin(3);

}

void loop() {
  // loop
  
  Datime dt = NTP;  // или Datime dt(NTP)
  dt.year;
  dt.second;
  dt.hour;
  dt.weekDay;
  dt.yearDay;
  bool istime=true;
  int i=0;
  while (istime){
    if (dt.hour*60+dt.minute<needtime[i]){
      istime=false;
    }
    i++;
  }
  if (NTP.tick()) {
    Datime dt = NTP;  // или Datime dt(NTP)
    dt.year;
    dt.second;
    dt.hour;
    dt.weekDay;
    dt.yearDay;   
    // вывод даты и времени строкой
    display.clearDisplay(); 
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.println(NTP.toString());// NTP.timeToString(), NTP.dateToString()
    display.setTextSize(3);
    display.setCursor(0, 64);
    display.println(String((needtime[i-1]-dt.hour*60-dt.minute)/60));
    display.setCursor(20, 64);
    display.println("h");
    display.setCursor(40, 64);
    display.println(String((needtime[i-1]-dt.hour*60-dt.minute)%60));
    display.setCursor(80, 64);
    display.println("m");
    display.setTextSize(1);
    display.setCursor(0, 92);
    display.println("to next scheduled event");
    display.display();  

  }
}
