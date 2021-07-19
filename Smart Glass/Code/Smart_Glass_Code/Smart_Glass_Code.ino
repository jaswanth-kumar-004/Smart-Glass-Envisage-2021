#include <Adafruit_Sensor.h>
#include<SoftwareSerial.h>
#include <dht.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire,
OLED_RESET);
dht DHT;
#define DHT11_PIN 7
SoftwareSerial mySerial(0, 1); // RX, TX
int page = 1;
const int buttonPin = 12;
boolean buttonState = LOW;
const int PIRPin = 13;
float humidity;
float temperature;
void setup()
{
pinMode(buttonPin, INPUT);
pinMode(PIRPin, INPUT);
mySerial.begin(9600);
if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
{
mySerial.println(F("SSD1306 allocation failed"));
for(;;); // Don't proceed, loop forever
}
}
void loop()
{
int chk = DHT.read11(DHT11_PIN);
if(debounceButton(buttonState) == HIGH && buttonState == LOW)
{
if(page == 3)
page = 1;
else
page++;
buttonState = HIGH;
} else if(debounceButton(buttonState) == LOW && buttonState == HIGH)
{
buttonState = LOW;
}
while (mySerial.available()>0)
{
display.clearDisplay();
display.setTextSize(1.5);
display.setTextColor(SSD1306_WHITE); // Draw white text
display.setCursor(0, 0);
display.cp437(true);
String currentTime = mySerial.readStringUntil('|');
String currentDate = mySerial.readStringUntil('|');
String caller = mySerial.readStringUntil('|');
String batteryPercentageText = "";
int batteryPercentage = 0;
int decimalMultiplier;
while(1)
{
char a = mySerial.read();
if(a == '|')
break;
batteryPercentageText = batteryPercentageText + a;
batteryPercentage = 10 * batteryPercentage + a - '0';
}
String stepsCount = mySerial.readStringUntil('|');
if(caller != "")
{
display.setTextSize(1);
display.setCursor(0, 20);
display.print(caller);
display.print(" ");
display.setCursor(0, 35);
display.setTextSize(2);
display.print("CALLING...");
display.setTextSize(1);
}
else if(digitalRead(PIRPin) == HIGH)
{
display.setCursor(0, 35);
display.setTextSize(1);
display.println("WARNING!!");
display.println("Move Away");
}
else
{
if(page == 1)
{
display.setCursor(0, 0);
display.setTextSize(1);
display.print(currentTime);
display.print(" ");
display.print(currentDate);
drawBatteryPercentage(110, 30, batteryPercentage);
} if(page == 2)
{
display.setCursor(30, 10);
display.setTextSize(1);
display.print("NO. OF STEPS");
display.setCursor(60,30);
display.print(stepsCount);
} if(page == 3)
{
display.setCursor(20, 10);
display.setTextSize(1);
display.print("TEMP");
display.setCursor(17,20);
display.print(DHT.temperature);
display.setCursor(60, 10);
display.print("HUMIDITY");
display.setCursor(67, 20);
display.print(DHT.humidity);
delay(100);
}
}
}
display.display();
delay(100);
}
void drawBatteryPercentage(int x, int y, int batteryPercentage)
{
int batteryChargeBars =0;
batteryChargeBars = batteryPercentage/4;
display.drawRect(0 + x, 2 + y, 14, 27, SSD1306_WHITE);
display.drawRect(3 + x, 0 + y, 8, 3, SSD1306_WHITE);
display.drawLine(4 + x, 2 + y, 9 + x, 2 + y, SSD1306_BLACK);
for(int i=1; i <= batteryChargeBars; i++)
{
if(i==1 || i==25)
{
display.drawLine(2 + x, 27 - i + y, 11 + x, 27 - i + y, SSD1306_WHITE);
} else
{
display.drawLine(2 + x, 27 - i + y, 11 + x, 27 - i + y, SSD1306_WHITE);
}
}
}

boolean debounceButton(boolean state)
{
boolean stateNow = digitalRead(buttonPin);
if(state!=stateNow)
{
delay(10);
stateNow = digitalRead(buttonPin);
} return stateNow;
}
