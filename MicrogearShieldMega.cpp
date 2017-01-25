#include "RingBuf.h"
#include "Arduino.h"	
#include "Print.h"
#include "Client.h"
#include "IPAddress.h"

#include "MicrogearShieldMega.h"

#define buffersize 50

WiFiClass WiFi;



	WiFiClient::WiFiClient(){
		time = millis();
		Serial1.begin(57600);
		if(countindex <3){
			index = ++countindex;	
		}
		client1 = RingBuf_new(1, buffersize);
		client2 = RingBuf_new(1, buffersize);
		time = millis();
	}	

  int WiFiClient::connect(IPAddress ip, uint16_t port){}
  int WiFiClient::connect(const char *host, uint16_t port){
  	//MGS+CONNECTCLIENT1	
  	Serial1.print("MGS+CONNECTCLIENT");
  	Serial1.print(index);
  	Serial1.print(" ");
  	Serial1.print(host);
  	Serial1.print(",");
  	Serial1.println(port);


  	// Serial.print("MGS+CONNECTCLIENT");
  	// Serial.print(index);
  	// Serial.print(" ");
  	// Serial.print(host);
  	// Serial.print(",");
  	// Serial.println(port);

  	Serialmanual::readserial(myserialdata);
  	if(strcmp(myserialdata,"true\r\n") == 0)return 1;
  	return 0;
  }

  size_t WiFiClient::write(uint8_t b){
  	  return write(&b, 1);
  }
  size_t WiFiClient::write(const uint8_t *buf, size_t size){
  	//MGS+PRINT1 GET /search?q=arduino HTTP/1.0

  	Serial1.print("MGS+PRINT");
  	Serial1.print(index);
  	Serial1.print(" ");

    char* buf_buf = buf;

    for(int i=0; i< size;i++){
  
      Serial1.print(buf_buf[i]);
    }
  	Serial1.println("");

  	delay(1000);
  	return size;
  }


 void WiFiClient::transmission(){
 	if((millis() - time) > 200){
 		time = millis();
 		sendrequest();	
 		getdatafrommyserial();
 	} 	
 }

 void WiFiClient::sendrequest(){
 	Serial1.print("MGS+READ");
	Serial1.print(index);
	// Serial.print("Sending data to esp8266: ");
	// Serial.print("MGS+READ");
	// Serial.print(index);
	Serial1.print(" ");
	// Serial.print(" ");
	if((index == 1) && (client1->elements < (buffersize-10))){
		// Serial.println("Request to server1");
		Serial1.println(buffersize - client1->elements);
		delay(100);	
		// Serial.println(buffersize - client1->elements);	
	}
	else if((index == 2) && (client2->elements < (buffersize-10))){
		// Serial.println("Request to server2");
		Serial1.println(buffersize - client2->elements);
		delay(100);
		// Serial.println(buffersize - client2->elements);
	}
 }

 void WiFiClient::getdatafrommyserial(){
 	if(Serial1.available()){
 		byte header = Serial1.read();
 		int client_num = (header & B11000000);
 		int payload_size = (header & B00111111);
 		// Serial.print("Received packet from: ");
 		if(client_num == 64){
 		// Serial.println("client1");
 		// Serial.print("Size: ");
 		// Serial.println(payload_size);
 		// Serial.print("Client1 size before add: ");
 		// Serial.println(client1->elements);
 			for(int count =0; count<payload_size; count++){
 				char a = Serial1.read();
 				//Serial.println("Add to client1");
 				client1->add(client1,&a);
 			}
 		// Serial.print("Client1 size after add: ");
 		// Serial.println(client1->elements);
 		// Serial.println();
 		}else if(client_num == 128){ 			
 		// Serial.println("client2");
 		// Serial.print("Size: ");
 		// Serial.println(payload_size);
 		// Serial.print("Client2 size before add: ");
 		// Serial.println(client2->elements);
 			for(int count = 0;count < payload_size;count++){
 				char b = Serial1.read();
 				
 				client2->add(client2,&b);
 			}
 		// Serial.print("Client2 size after add: ");
 		// Serial.println(client2->elements);
 		}
 	}
 }

  int WiFiClient::available(){
  	transmission();
  	if(index == 1){
  		return client1->elements;
  	}
  	if(index == 2){
  		return client2->elements;
  	}
  }

  int WiFiClient::read(){
    transmission();
  	int c;
  	if(index == 1){
  		client1->pull(client1,&c);
  	}
  	if(index == 2){
  		client2->pull(client2,&c);
  	}
  	delay(10);
  	return c;
  }
  int WiFiClient::read(uint8_t *buf, size_t size){
    transmission();
  	if(index == 1){
  		client1->pull(client1,buf);
  	}
  	if(index == 2){
  		client2->pull(client2,buf);
  	}
  	delay(10);
  	return size;
  }

  int WiFiClient::peek(){}
  void WiFiClient::flush(){}
  void WiFiClient::stop(){
  	//MGS+STOP2
  	Serial1.print("MGS+STOP");
  	Serial1.println(index);
  }
  uint8_t WiFiClient::connected(){
  	//MGS+STAT1
  	Serial1.print("MGS+STAT");
  	Serial1.print(index);
  	Serialmanual::readserial(myserialdata);
  	if(strcmp(myserialdata,"true\r\n") == 0)return 1;
  	return 0;
  }	
	WiFiClient::operator bool(){}



void Serialmanual::readserial(char *buf){
	delay(500);
	
	if(Serial1.available()){
		int count =0;
		while(Serial1.available()){
			char d = Serial1.read();
			*(buf+count) = d;
			count++;
			delay(1);
		}
	}
}

void WiFiClass::begin(const char *ssid, const char *password){
	//MGS+SETUPWIFI AP0001,Password@9
	Serial1.print("MGS+SETUPWIFI ");
	Serial1.print(ssid);
	Serial1.print(",");
	Serial1.println(password);
	delay(500);
}

bool WiFiClass::status(){
//MGS+CHECKSTATUS
	Serial1.println("MGS+CHECKSTATUS");
	Serialmanual::readserial(buffer);
	sscanf(buffer,"%s %s",state,ip);
	if(strcmp(state,"true") == 0)return true;
	return false;
}

char* WiFiClass::localIP(){
	return ip;
}


// char Wifi::stat = NULL;
int WiFiClient::countindex = 0;