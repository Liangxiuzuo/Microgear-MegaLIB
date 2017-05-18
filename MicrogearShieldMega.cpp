#include "MicrogearShieldMega.h"

Serialmanual bufsyn;

//Constructor of Netpie microgear
Microgear::Microgear(){
   Serial1.begin(9600);
}

  void Microgear::init(char *appid, char *key, char *secret ,char *alias, char *token, char *tokensecret){
        Serial1.write("AT+MGCF=\"");
        Serial1.write(appid);
        Serial1.write("\",\"");
        Serial1.write(key);
        Serial1.write("\",\"");
        Serial1.write(secret);
        Serial1.write("\",\"");
        Serial1.write(alias);
        Serial1.write("\",\"");
        Serial1.write(token);
        Serial1.write("\",\"");
        Serial1.write(tokensecret);
        Serial1.write("\"\r");
  } 

  bool Microgear::status(){
  	Serialmanual::clearserial();
  	Serial1.write("AT+MGCN?\r");
  	char msg[30];
  	if(Serialmanual::waitserial(2000)) Serialmanual::readserial(msg);
	if(strncmp(msg,"OK\r\n",11) == 0)return 1;
	return 0;
  }

  void Microgear::connect(){
      Serial1.write("AT+MGCN\r");
  }

  bool Microgear::disconnect(){
      Serial1.write("AT+MGDC\r");
  }

  void Microgear::setalias(char *alias){
      Serial1.write("AT+MGSA=\"");
      Serial1.write(alias);
      Serial1.write("\"\r");
  }

  void Microgear::publish(char *topic,char *data){
      Serial1.write("AT+MGP=\"");
      Serial1.write(topic);
      Serial1.write("\",\"");
      Serial1.write(data);
      Serial1.write("\"\r");
  }

  void Microgear::chat(char *appid,char *data){
      Serial1.write("AT+MGC=\"");
      Serial1.write(appid);
      Serial1.write("\",\"");
      Serial1.write(data);
      Serial1.write("\"\r");
  }

  void Microgear::subscribe(char *topic){
      Serial1.write("AT+MGS=\"");
      Serial1.write(topic);
      Serial1.write("\"\r");
  }

  void Microgear::unsubscribe(char *topic){
      Serial1.write("AT+MGUS=\"");
      Serial1.write(topic);
      Serial1.write("\"\r");
  }

  void Microgear::loop(){
    bufsyn.transmission();
  }

  void Microgear::echomode(int mode){
    Serial1.write("ATE=");
    Serial1.write(mode);
    Serial1.write("\r");
    Serialmanual::clearserial();
  }

  void Microgear::pushmode(int mode){
    Serial1.write("ATP=");
    Serial1.write(mode);
    Serial1.write("\r");
    Serialmanual::clearserial();
  }

  int Microgear::available(){
    return (Serialmanual::get_element(0));  
  }

  void Microgear::read(MgStruct *mg){
      Serialmanual::pull_element(0, mg);
  }

  void Microgear::writefeed(char *appid, char *feeddata){
  	// AT+MGWF=0,"MicrogearShield","{Temp:5}"\r
	Serial1.write("AT+MGWF=0,\"");
  	Serial1.write(appid);
  	Serial1.write("\",\"");
  	Serial1.write(feeddata);
  	Serial1.write("\"\r");

  	Serial.write("AT+MGWF=0,\"");
  	Serial.write(appid);
  	Serial.write("\",\"");
  	Serial.write(feeddata);
  	Serial.write("\"\r\n");
  	Serialmanual::clearserial();
  }
//AT+MGWF=1,"MicrogearShield","{Temp:12}","fA78nqOtzasvAS1xg4MzlnfMn1FKHdUj"
  void Microgear::writefeed(char *appid, char *feeddata, char *apikey){
  	Serial1.write("AT+MGWF=1,\"");
  	Serial1.write(appid);
  	Serial1.write("\",\"");
  	Serial1.write(feeddata);
  	Serial1.write("\",\"");
  	Serial1.write(apikey);
  	Serial1.write("\"\r");

  	// Serial.write("AT+MGWF=1,\"");
  	// Serial.write(appid);
  	// Serial.write("\",\"");
  	// Serial.write(feeddata);
  	// Serial.write("\",\"");
  	// Serial.write(apikey);
  	// Serial.write("\"\r");
  	Serialmanual::clearserial();
}

int Microgear::countindex = 0;