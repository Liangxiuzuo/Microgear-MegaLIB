/***********************************************************************
*
*	This file suppose the contain API for communication between ESP8266 and 
*   Arduino && Syncronization between both devices
*		<1>Support functions in Serial Task
*			<1.1>Read serial
*			<1.2>clearserial
*			<1.3>Syncronization buffer (ESP8266 && Arduino Uno) 		
*			<1.3>get element  	
*			<1.4>add element
*			<1.5>pull element 
*			<1.6>peek element
*			
*
*************************************************************************/

#ifndef Serial_h
	#define Serial_h

	#include "Arduino.h"
	#include "RingBuf.h"

	#define clientbuffersize 127
	#define mgbuffersize 5
	#define MAXTIMEOUT 500
	#define ROUTINE_SYN_TIME 1

	
	static struct RingBuf* microgear_buf;
    static struct RingBuf* client1_buf;
    static struct RingBuf* client2_buf;

	class Serialmanual{
		public:

			Serialmanual();

		  	static void readserial(char *buf);
		  	static void clearserial();

		  		
		  static int waitserial(int time);
		  //Syncronization between buffer
		  void transmission ();
		  void sendrequest();
		  void getdatafrommyserial(); 

	       static int get_element(int buf_no);
		   static void add_element(int buf_no, void *data);
		   static void pull_element(int buf_no , void* c);
		   static void peek_element(int  buf_no, char *c);

		private:
			unsigned long current_time,past_time;
	};
#endif