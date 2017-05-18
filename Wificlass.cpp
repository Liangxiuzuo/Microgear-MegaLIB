#include "Wificlass.h"
WiFiClass WiFi;

WiFiClass::WiFiClass(){
}


void WiFiClass::begin(const char *ssid, const char *password){
	Serialmanual::clearserial();
  Serial1.write("AT+CW=\"");
	Serial1.write(ssid);
	Serial1.write("\",\"");
	Serial1.write(password);
  Serial1.write("\"\r");
  Serialmanual::waitserial(1000);
  Serialmanual::clearserial();
}

bool WiFiClass::status(){
  Serialmanual::clearserial();
  Serial1.write("AT+CW?\r");

  if(Serialmanual::waitserial(1000)){
    Serialmanual::readserial(buffer);
    // Serial.print("Data is:");
    // Serial.print(buffer);
    sscanf(buffer,"%s %s",state,ip);
    if(strcmp(state,"CONNECTED") == 0){
      return true;
    }else{
      return false;    
    }
  }else{
    return false;
  }
}

char* WiFiClass::getip(){
	return ip;
}

// int WiFiClass::get_element(int buf_no){
//   switch (buf_no){
//     case 0: return microgear_buf->elements;
//     case 1: return client1_buf->elements;
//     case 2: return client2_buf->elements;
//   }
// }

// void WiFiClass::add_element(int buf_no, char *data){
//   switch (buf_no){
//     case 0: microgear_buf->add(microgear_buf,&data); break; 
//     case 1: client1_buf->add(client1_buf,&data); break; 
//     case 2: client2_buf->add(client2_buf,&data); break; 
//   }
// }

// void WiFiClass::pull_element(int buf_no, int c){
//    switch (buf_no){
//     case 0:  microgear_buf->pull(microgear_buf,c); break;
//     case 1:  client1_buf->pull(client1_buf,c); break;
//     case 2:  client2_buf->pull(client2_buf,c); break;
//   }
// }

// void WiFiClass::peek_element(int buf_no, char *c){
//   switch (buf_no){
//     case 0: microgear_buf->peek(microgear_buf,c); break;
//     case 1: client1_buf->peek(client1_buf,c); break;
//     case 2: client2_buf->peek(client2_buf,c); break;
//   }
// }





