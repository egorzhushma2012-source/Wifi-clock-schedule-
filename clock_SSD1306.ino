// Библиотеки для дисплея SSD1306
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Библиотеки для NTP
#include <Arduino.h>
#include <GyverNTP.h>

// Параметры дисплея
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Расписание (всё в минутах)
int needtime[] = {510, 550, 560, 600, 620, 660, 680, 720, 740, 780, 795, 835, 845, 885, 1440};

void setup() {
  // Инициализация дисплея
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 initialization failed!"));
    for (;;); // Зависание при ошибке
  }

  // Загрузка
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  // Инициализация Serial
  Serial.begin(9600);

  // Настройка и инициализация Wi‑Fi
  WiFi.begin("SSID", "PASSWORD");
  while (WiFi.status() != WL_CONNECTED) delay(100);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Connected");
  display.display();

  // Обработчик ошибок NTP
  NTP.onError([]() {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(NTP.readError());
    display.println("online: ");
    Serial.println(NTP.online());
    display.display();
  });

  // Запуск NTP
  NTP.begin(3);
}

void loop() {
  if (NTP.tick()) {
    Datime dt = NTP; 

    bool istime = true;
    int i = 0;

    while (istime) {
      if (dt.hour * 60 + dt.minute < needtime[i]) {
        istime = false;
      } else {
        i++;
      }
      // Защита от выхода за границы массива
      if (i >= sizeof(needtime) / sizeof(needtime[0])) {
        istime = false;
        i = sizeof(needtime) / sizeof(needtime[0]) - 1;
      }
    }

    // Вывод даты и времени строкой
    display.clearDisplay();

    // Верхняя строка: текущее время и дата
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println(NTP.toString()); // NTP.timeToString(), NTP.dateToString()

    // Нижняя часть: таймер до следующего события
    display.setTextSize(2);
    display.setCursor(0, 32);
    int minutesLeft = needtime[i] - (dt.hour * 60 + dt.minute);
    display.println(String(minutesLeft / 60)); // Часы
    display.setCursor(15, 32);
    display.println("h");
    display.setCursor(30, 32);
    display.println(String(minutesLeft % 60)); // Минуты
    display.setCursor(55, 32);
    display.println("m");
    display.setTextSize(1);
    display.setCursor(0, 47);
    display.println("to next scheduled event");
    display.display();
  }
}
