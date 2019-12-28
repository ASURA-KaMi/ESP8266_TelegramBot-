#ifndef ESP8266TelegramBOT_h
#define ESP8266TelegramBOT_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

class TelegramBOT
{
  public:
    TelegramBOT (String, String, String);
  	String message[3][6];  // amount of messages read per time  (update_id, name_id, name, lastname, chat_id, text)
  	void begin(void);
  	void analizeMessages(void);
  	void sendMessage(String chat_id, String text, String reply_markup);
  	void getUpdates(String offset);
	const char* fingerprint = "37:21:36:77:50:57:F3:C9:28:D0:F7:FA:4C:05:35:7F:60:C1:20:44";  //Telegram.org Certificate 

  private:
    String connectToTelegram(String command);
    String _token;
    String _name;
    String _username;
    WiFiClientSecure client;
};

#endif
