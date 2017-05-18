/***********************************************************************
*
*	This file use to support functions with in Wifi Task
*		<1>Support functions in Wifi Task
*			<1.1>Connect to ACCESS POINT {Type II}	
*			<1.2>Get status of Wifi Connection {Type I}
*			<1.3>Get IP 
*				
*	
*
*************************************************************************/


#ifndef WiFiClass_h
	#define WiFiClass_h

	#include "RingBuf.h"
	#include "Serial.h"

	// #include "SoftwareSerial.h"
	#include "Arduino.h"
	
	
	// extern SoftwareSerial mySerial;

	class WiFiClass{
		public:
		   WiFiClass();
		   void begin(const char *ssid, const char *password);			
		   bool status();												
		   char* getip();												

		   // static int get_element(int buf_no);
		   // static void add_element(int buf_no, char *data);
		   // static void pull_element(int buf_no , int c);
		   // static void peek_element(int  buf_no, char *c);
		private:
		   char buffer[150];
		   char state[20];
		   char ip[20];
	};
	extern WiFiClass WiFi;
#endif