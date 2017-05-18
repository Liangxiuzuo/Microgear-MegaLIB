/***********************************************************************
*
*	This file suppose the purposes of setup microgear
*		<1>Support functions in Microgear Task
*			<1.1>Set config key secret alias token token_secret {Type II}
*			<1.2>Connect from Netpie {Type I}
*			<1.3>Disconnect from Netpie {Type I}		
*			<1.3>Set alias name	{Type II}
*			<1.4>Publish data {Type II}
*			<1.5>Subscribe topic {Type II}
*			<1.6>Unscribe topic {Type II}
*			<1.7>loop
*			<1.8>Config echo mode {Type II}
*			<1.9>Config push mode {Type II}
*
*************************************************************************/

#ifndef MicrogearShieldUno_h
	#define MicrogearShieldUno_h

	#include "WiFiclass.h"
	#include "RingBuf.h"
	#include "Wificlient.h"
	#include "Serial.h"

	#include "Arduino.h"
	#include "Print.h"	
	#include "Client.h"
	#include "IPAddress.h"
	
	using namespace std;
	
	extern WiFiClass WiFi;
	


class Microgear{

public:
   Microgear();
  
   void init(char *appid, char *key, char *secret ,char *alias, char *token, char *tokensecret);		//Type II
   void connect();	
   bool status();																					//Type I
   bool disconnect();																					//Type I			
   void setalias(char *alias);																			//Type II	
   void publish(char *topic,char *data);																//Type II
   void chat(char *appid,char *data);																	//Type II
   void subscribe(char *topic);																			//Type I			
   void unsubscribe(char *topic);																		//Type II
   static void loop();
   void echomode(int mode);
   void pushmode(int mode);
   void writefeed(char *appid, char *feeddata);
   void writefeed(char *appid, char *feeddata, char *apikey);

   int available();
   void read(MgStruct *mg);

private:
	  struct RingBuf* microgear_buf;
	  static int countindex;
	  int index;
	  char myserialdata[30];
	  char ip[16];
	  char state;
	  unsigned long time;
	  uint8_t getFirstSocket();
};

#endif