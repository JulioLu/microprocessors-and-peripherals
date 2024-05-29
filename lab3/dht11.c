#include "dht11.h"


void startSignal(){
	//setting the mode of the pin that corresponds to the DHT11 pin2 to output
	gpio_set_mode(PC_1, Output);
	gpio_set(PC_1, 0);
	delay_ms(18);
	gpio_set(PC_1, 1);
	delay_us(40);	
	gpio_set_mode(PC_1, Input);
	delay_us(160);
	while(gpio_get(PC_1));
}


uint8_t dataTransmission(){
	
	delay_us(20); // start to transmit 1 bit data 
	
	while(!gpio_get(PC_1));
	
	delay_us(29);
	if(!gpio_get(PC_1)) return 0;
	else {
		delay_us(41);


		return 1;
	}

}



float* getData(){

	startSignal();

	int i,j; 
	uint8_t data[5] = {0,0,0,0,0};
	//data[4]=1;
	static float measurments[2] = {0,0};
	//data[0]=0;
	for(i=0,j=0; i < 40; i++){
		data[j] = (uint8_t)(data[j]<<1) | dataTransmission();
		if((i+1)%8==0){j++;}
	}	
	if(checksum(data)){
		measurments[0] = data[0] + data[1]/(float)10;// Humudity
		measurments[1] = data[2] + data[3]/(float)10;//Temperature
		return measurments;
		
	}
	else{
		return NULL;
	}
}

bool checksum(uint8_t* data){
	int i;
	uint8_t checksum = 0;
	for(i=0; i<4;i++){
		checksum += data[i];
	}
	if(checksum == data[4]){
		return true;
	}
	else{
		return false;
	}
}

