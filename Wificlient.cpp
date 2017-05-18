#include "Wificlient.h"

WiFiClient::WiFiClient(){
		if(countindex <3){
			index = countindex++;	
		}
}	


int WiFiClient::connect(IPAddress ip, uint16_t port){}
int WiFiClient::connect(const char *host, uint16_t port){
	// Serial.print("Inside wificlient connect");
	Serialmanual::clearserial();
  	Serial1.write("AT+CCS");
  	Serial1.print(index);
  	Serial1.write("=\"");
  	Serial1.write(host);
  	Serial1.write("\",");
  	Serial1.print(port);
    Serial1.write("\r");

   //  Serial.write("AT+CCS");
  	// Serial.print(index);
  	// Serial.write("=\"");
  	// Serial.write(host);
  	// Serial.write("\",");
  	// Serial.print(port);
   //  Serial.write("\r");
    
  	if(Serialmanual::waitserial(2000)) Serialmanual::readserial(myserialdata);
    // Serial.print("Data from connect is: ");
    // Serial.println(myserialdata);
  	if(strncmp(myserialdata,"OK\r\n",11) == 0){
  		// Serial.println("client 1 connect successfully");
  		return 1;
  	}
  	return 0;
}





size_t WiFiClient::write(uint8_t b){
	Serialmanual::clearserial();
  	return write(&b, strlen((char*)b));
}

size_t WiFiClient::write(const uint8_t* buf, size_t size){
	Serialmanual::clearserial();
  	Serial1.print("AT+CP");
  	Serial1.print(index);
  	Serial1.print("=\"");
    for(int count =0; count < size-1 ;count++){
      Serial1.print((char)*(buf+count));
    }
    Serial1.write("\"\r");

    // Serial.print("AT+CP");
    // Serial.print(index);
    // Serial.print("=\"");
    // for(int count =0; count < 10 ;count++){
    //   Serial.print((char)*(buf+count));
    // }
    // Serial.write("\"\r");

    if(Serialmanual::waitserial(2000)) Serialmanual::readserial(myserialdata);
    if(strncmp(myserialdata,"OK\r\n",9) == 0)return size;
    return 0;
}


   
// }
  
  //Return Number of available data in buffer 
int WiFiClient::available(){
  	if(index == 1){
      // Serial.print("Clinet1 buffer: ");
      // Serial.println(Serialmanual::get_element(1));
  	   	return (Serialmanual::get_element(1));
  	}
  	if(index == 2){
      // Serial.print("Clinet2 buffer: ");
      // Serial.println(Serialmanual::get_element(2));
  		  return (Serialmanual::get_element(2));
  	}
}

int WiFiClient::read(){
  	int c;
  	if(index == 1){
  		  Serialmanual::pull_element(1, &c);
        //client1->pull(client1,&c);
  	}
  	if(index == 2){
      Serialmanual::pull_element(2, &c);
  		  //client2->pull(client2,&c);
  	}
  	return c;
}


int WiFiClient::read(uint8_t *buf, size_t size){
    int count,buf1,buf2;
  	for(count = 0 ; count<size ; count++){
  		if(index == 1){
         Serialmanual::pull_element(1, &buf1);
    			//client1->pull(client1,&buf1);
          buf[count] = buf1;
  		}
  		if(index == 2){
        Serialmanual::pull_element(2, &buf2);
         // client2->pull(client2,&buf2);
          buf[count] = buf2;
  		}	
  	}
  	return count;
}

int WiFiClient::peek(){
  	int c;
  	if(index == 1){
      Serialmanual::peek_element(1, c);
  		  //client1->peek(client1,c);
  	}
  	if(index == 2){
        Serialmanual::peek_element(2, c);
  		  //client2->peek(client2,c);
  	}
  	return c;
}

void WiFiClient::flush(){}
void WiFiClient::stop(){
  	Serial1.write("AT+CD");
  	Serial1.print(index);
    Serial1.write("\r");
}
  
uint8_t WiFiClient::connected(){
	Serialmanual::clearserial();
 	char msg[30];
  	Serial1.write("AT+CCS");
  	Serial1.print(index);
    Serial1.write("?\r");
  	if(Serialmanual::waitserial(2000)) Serialmanual::readserial(msg);
	if(strncmp(msg,"OK\r\n",11) == 0)return 1;
	return 0;
}



WiFiClient::operator bool(){}
int WiFiClient::countindex = 1;