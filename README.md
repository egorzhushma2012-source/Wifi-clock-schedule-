# Wifi-clock-schedule-
As part of this project, the program code for Wemos D1 mini has been developed using the Adafruit_GFX and Adafruit_SSD1306 libraries. The display shows information about the current time and the time until the next event.
## Configurate
1. Install code for you display.
2. replace **SSID** and **PASSWORD** in this line
```
// настройка и инициализация wifi
WiFi.begin("SSID", "PASSWORD");
```
with your wifi SSID and Password.
## Customization
*you can:*

- change NTP host (add code to void setup)
```
NTP.setHost("ntp1.stratum2.ru");
```
- change time zone change +3 to your timezone in 
```
NTP.begin(3);
```
