#include "ESP8266TelegramBOT.h"


TelegramBOT::TelegramBOT(String token, String name, String username)	{ //Class constructor
  token=token;
  name=name;
  username=username;
}


void TelegramBOT::begin(void)	{
  message[0][0]="0";   // number of received messages
  message[1][0]="";    
  message[0][1]="0";  // code of last read Message
}


  
/**************************************************************************************************
 * function to achieve connection to api.telegram.org and send command to telegram                *
 * (Argument to pass: URL to address to Telegram)                                                 *
 **************************************************************************************************/
String TelegramBOT::connectToTelegram(String command)  {
  String mess="";
  long now;
  bool avail;
  // Connect with api.telegram.org       
  IPAddress server(149,154,167,220);
  if (client.connect(server, 443)) {  
    //Serial.println(".... connected to server");
    String a="";
    char c;
    int ch_count=0;
    client.println("GET /"+command);
    now=millis();
    avail=false;
    while (millis()-now<1500) {  
      while (client.available()) {
        char c = client.read();
        //Serial.write(c);
        if (ch_count<700)  {
          mess=mess+c;
          ch_count++;
        }
        avail=true;
      }
      if (avail) {
        //Serial.println();
        //Serial.println(mess);
        //Serial.println();
      break;
      }
    }
  }
  return mess;
}




/***************************************************************
 * GetUpdates - function to receive all messages from telegram *
 * (Argument to pass: the last+1 message to read)             *
 ***************************************************************/
void TelegramBOT::getUpdates(String offset)  {
  Serial.println("GET Update Messages ");
  String command="bot"+_token+"/getUpdates?offset="+offset;
  String mess=connectToTelegram(command);       //recieve reply from telegram.org
  // parsing of reply from Telegram into separate received messages
  int i=0;                //messages received counter
  if (mess!="") {
    Serial.print("Sent Update request messages up to : ");
    Serial.println(offset);
    String a="";
    int ch_count=0;
    String c;
    for (int n=1; n<mess.length()+1; n++) {   //Search for each message start
      ch_count ++;
      c =  mess.substring(n-1,n);
      //Serial.print(c);
      a=a+c;
      if (ch_count>8) {
        if (a.substring(ch_count-9)=="update_id")  {
          if (i>1) break;
          message[i][0]=a.substring(0, ch_count-11);
          a=a.substring(ch_count-11);
          i ++;
          ch_count=11;
        }
      }
    }
    if (i==1)  {
    message[i][0]=a.substring(0, ch_count);   //Assign of parsed message into message matrix if only 1 message)
    }
    if (i>1)  i=i-1;
  }
  //check result of parsing process
  if (mess=="") {     
    Serial.println("failed to update");
    return;
  }   
  if (i==0) {
    Serial.println("no new messages");
    Serial.println();
    message[0][0]="0";
  }
  else {
    message[0][0]=String(i);   //returns how many messages are in the array
    //Serial.println();        	
    for (int b=1; b<i+1; b++)  {
      Serial.println(message[b][0]);
    }
  }
}





/***********************************************************************
 * SendMessage - function to send message to telegram                  *
 * (Arguments to pass: chat_id, text to transmit and markup(optional)) *
 ***********************************************************************/
void TelegramBOT::sendMessage(String chat_id, String text, String reply_markup)  {
  bool sent=false;
  // Serial.println("SEND Message ");
  long sttime=millis();
  if (text!="") {
    while (millis()<sttime+8000) {    // loop for a while to send the message
      String command="bot"+_token+"/sendMessage?chat_id="+chat_id+"&text="+text+"&reply_markup="+reply_markup;
      String mess=connectToTelegram(command);
      //Serial.println(mess);
      int messageLenght=mess.length();
      for (int m=5; m<messageLenght+1; m++)  {
        if (mess.substring(m-10,m)=="{\"ok\":true")  {  //Chek if message has been properly sent
          sent=true;
          break;
        }
      }
      if (sent==true)   {
        //  Serial.print("Message delivred: \"");
        //  Serial.print(text);
        //  Serial.println("\"");
        //  Serial.println();
        break;
      }
      
      delay(1000);
      //	Serial.println("Retry");
    }
  }
  // if (sent==false) Serial.println("Message not delivered");
}