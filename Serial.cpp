#include "Serial.h"




Serialmanual::Serialmanual(){
	microgear_buf = RingBuf_new(sizeof(MgStruct),mgbuffersize);
    client1_buf = RingBuf_new(1, clientbuffersize);
    client2_buf = RingBuf_new(1, clientbuffersize);
    current_time =0;
    past_time = 0;
}

int Serialmanual::waitserial(int time){
	unsigned long timeout = millis();
	while(true){
		if(Serial1.available()){
			// Serial.println("True");
			return 1;
		}else if(millis() - timeout > time){
			// Serial.println("Timeout");
			return 0;
		}
	}
}

void Serialmanual::readserial(char *buf){
	unsigned long timeout = millis();
		int count =0;
		// Serial.print("Data inside readserial: ");
		while(Serial1.available()){
            delay(10);
			char d = Serial1.read();
			// Serial.print(d);
            buf[count++] = d;
            buf[count] = '\0';
			// *(buf + count) = d;
			// count++;
		}
}

void Serialmanual::clearserial(){
	unsigned long timeout = millis();
		while(Serial1.available()){
			Serial1.read();
		}
}


void Serialmanual::transmission(){
	// current_time = millis();
 //  	if(current_time - past_time > ROUTINE_SYN_TIME){
	// 	past_time = current_time;
		// Serial.println("inside transmission");
		sendrequest();
		// if(waitserial(150)) getdatafrommyserial();
  	// } 
}

void Serialmanual::sendrequest(){

    for(int count =1; count < 4; count ++){
	current_time = millis();
        if((count == 1) && (get_element(1) < (clientbuffersize-10))){
   			clearserial();
            Serial1.print("AT+CRTA");
            Serial1.print(count);
            Serial1.write("=");
            // int requesting_byte;
            // if(clientbuffersize - get_element(1) > 64){
            // 	requesting_byte = 80;
            // } else{
            // 	requesting_byte = clientbuffersize - get_element(1);
            // }
            // Serial1.print(requesting_byte);
            Serial1.print(clientbuffersize - get_element(1));
            Serial1.write("\r");
            if(waitserial(500)) getdatafrommyserial();

        }
        else if((count == 2) && (get_element(2)< (clientbuffersize-10))){
        	clearserial();
            Serial1.print("AT+CRTA");
            Serial1.print(count);
            Serial1.print("=");
			// int requesting_byte;
   //          if(clientbuffersize - get_element(2)> 64){
   //          	requesting_byte = 63;
   //          } else{
   //          	requesting_byte = clientbuffersize - get_element(2);
   //          }
   //          Serial1.print(requesting_byte);
            Serial1.print(clientbuffersize - get_element(2));
            Serial1.write("\r");
            if(waitserial(500)) getdatafrommyserial();

         //    Serial1.print(clientbuffersize - get_element(2));
            // Serial.print("AT+CRTA");
            // Serial.print(count);
            // Serial.print("=");
            // Serial.print(clientbuffersize - get_element(2));
            // Serial.write("\r");

         //    if(waitserial(500)) getdatafrommyserial();

          
        // }else if((count == 3) && (get_element(0)< mgbuffersize) && (current_time - past_time > ROUTINE_SYN_TIME)){
	 	}else if((count == 3) && (get_element(0)< mgbuffersize)){
            // Serial.print("MG buffer size: ");
            //     Serial.println(get_element(0));
        	// past_time = current_time;
        	// clearserial();
      		// delay(100);
            Serial1.write("AT+MGPTA\r");
            // Serial.println("AT+MGPTA\r");
         //    // Serial.println("loop3 getdata");
            if(waitserial(500)) getdatafrommyserial();

            // Serial.print("Print \"AT+MGPTA\" to ESP8266\n\r");
        }
    }
}

void Serialmanual::getdatafrommyserial(){

        int header = (int)Serial1.read();
        int nego_key = (header & B11110000);
        int sender = (header & B00001111);
        // delay(1);		
        // Serial.print("\nHeader: ");
        // Serial.println(header);
  // 	 	Serial.print("Received data from client number: ");
		// Serial.println(sender);
  //       Serial.print("Nego key: ");
  //       Serial.println(nego_key);
    
		if(nego_key == 160){
            delay(10);
			int payload_size = Serial1.read();
			// Serial.print("Received data from client number: ");
			// Serial.println(sender);

			// Serial.print("Size: ");
   //      	Serial.println(payload_size);
			// Serial.print("Payload: ");

	        if(sender == 1){
	            for(int count =0; count<payload_size; count++){
	                delay(1);
	                char a = Serial1.read(); 
	                // Serial.print(a);
	                add_element(1, &a);    
	            }


	        }else if(sender == 2){    
	            for(int count = 0;count < payload_size;count++){
                    delay(1);
	                char b = Serial1.read();
	                // Serial.print(b); 
	                add_element(2, &b); 
	                // client2->add(client2,&b);   
	            }
	             // Serial.println();
	          //   Serial.print("Client2 buffer: ");
	          // Serial.println(get_element(2));



	        }else if(sender == 3){
	        	char mg_buf_inside[80];
	        	int count=0;
	        	MgStruct mm;
	        	while(count<payload_size){
        		 	delay(1);
        		 	mg_buf_inside[count++] = (char)Serial1.read();
	                mg_buf_inside[count] ='\0';    
	        	}	
				sscanf(mg_buf_inside, "%[^:]:%s", &mm.topic, &mm.msg);
				// Serial.print("Topic: ");
				// Serial.println(mm.topic);
				// Serial.print("Payload: ");
				// Serial.println(mm.msg);
	            add_element(0, &mm);
                
	        }	 
	    }else if(nego_key == 176){
            // Serial.print("No data detect");
	    	clearserial();
	    }


}



int Serialmanual::get_element(int buf_no){
  switch (buf_no){
    case 0: return microgear_buf->elements;
    case 1: return client1_buf->elements;
    case 2: return client2_buf->elements;
  }
}

void Serialmanual::add_element(int buf_no, void *data){
  switch (buf_no){
    case 0: microgear_buf->add(microgear_buf,(MgStruct*)data); break; 
    case 1: client1_buf->add(client1_buf,(char*)data); break; 
    case 2: client2_buf->add(client2_buf,(char*)data); break; 
  }
}

void Serialmanual::pull_element(int buf_no, void* c){
   switch (buf_no){
    case 0:  microgear_buf->pull(microgear_buf,c); break;
    case 1:  client1_buf->pull(client1_buf,c); break;
    case 2:  client2_buf->pull(client2_buf,c); break;
  }
}

void Serialmanual::peek_element(int buf_no, char *c){
  switch (buf_no){
    case 0: microgear_buf->peek(microgear_buf,c); break;
    case 1: client1_buf->peek(client1_buf,c); break;
    case 2: client2_buf->peek(client2_buf,c); break;
  }
}




