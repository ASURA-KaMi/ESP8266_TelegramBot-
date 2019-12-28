/*********SmartHouseTelegrammMessageBot*********
**********Use with ESP8266 2.3.0 lib************/
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "ESP8266TelegramBOT.h"
#include <Wire.h>
#include <Thinary_AHT10.h>

AHT10Class AHT10;

// Initialize Wifi connection to the router
const char* ssid = "";              // your network SSID (name)
const char* password = "";     // your network key



// Initialize Telegram BOT

#define BOTtoken "**:**"  //token of your BOT
#define BOTname "REM_wet_temp" //name of your BOT
#define BOTusername "REM_wet_bot" //username of your BOT

TelegramBOT bot(BOTtoken, BOTname, BOTusername);

int Bot_mtbs = 500; //mean time between scan messages
long Bot_lasttime;  //last time messages' scan has been done
int Last_msg = 0;   //id of last message
int gasValue = 0;   //variable for gas sensor
int gasCheck;       //time of last update from gas sensor



/********************************************
 * EchoMessages - function to Echo messages *
 ********************************************/
void Bot_ExecMessages(int Past, int Now) {
  if (Past != Now) {
    if (bot.message[1][5] == "/ayaya") {
      bot.sendMessage(bot.message[1][4], "https://youtu.be/D0q0QeQbw9U", "");
    }
    if (bot.message[1][5] == "/wet") {
      bot.sendMessage(bot.message[1][4], String("")+"Humidity(%RH):\t\t"+AHT10.GetHumidity()+"%", "");
    }
    if (bot.message[1][5] == "/temp") {
      bot.sendMessage(bot.message[1][4], String("")+"Temperature(℃):\t"+AHT10.GetTemperature()+"℃", "");
    }
    if (bot.message[1][5] == "/ledon") {
      digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
      bot.sendMessage(bot.message[1][4], "LED take's the master power ON", "");
    }
    if (bot.message[1][5] == "/ledoff") {
      digitalWrite(13, LOW);    // turn the LED off (LOW is the voltage level)
      bot.sendMessage(bot.message[1][4], "LED take's the master power OFF", "");
    }
    if (bot.message[1][5] == "/rgs") {
      String gasValueMsg = "Gas Value is: " + String(gasValue);
      bot.sendMessage(bot.message[1][4], gasValueMsg, "");
    }
    if (bot.message[1][5] == "/start") {
      bot.sendMessage(bot.message[1][4], "Ass we can", "");
      bot.sendMessage(bot.message[1][4], "/rgs : get Gas analyze", "");
      bot.sendMessage(bot.message[1][4], "/temp : get Temperature", "");
      bot.sendMessage(bot.message[1][4], "/wet : get Wetness", "");
    }
  }
}

void setup() {
  Serial.begin(921600);
  delay(3000);
  Wire.begin();
  if(AHT10.begin(eAHT10Address_Low))
    Serial.println("Init AHT10 Sucess.");
  else
    Serial.println("Init AHT10 Failure.");
    
  // attempt to connect to Wifi network:
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  bot.begin();      // launch Bot functionalities
  pinMode(13, OUTPUT);
  pinMode(A0, INPUT);
}
void loop() {
  if (millis() > gasCheck + 1000){
     gasValue = analogRead(A0);
     Serial.println(gasValue);
     String gasValueMsg = "Gas Value is: " + String(gasValue);
     Serial.println(gasValueMsg);
     gasCheck = millis();
  }
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    bot.getUpdates(bot.message[0][1]);   // launch API GetUpdates up to xxx message
    if (gasValue >= 200){
      String gasValueMsg = "WARNING!!!! Gas Value is: " + String(gasValue);
      bot.sendMessage("chatid", gasValueMsg, ""); //type the chatid for emergency messages
    }
    Bot_ExecMessages(Last_msg, bot.message[0][1].toInt());   // reply to message with Echo
    Serial.println(bot.message[0][1]);
    Last_msg = bot.message[0][1].toInt();
    Bot_lasttime = millis();
  }
}
