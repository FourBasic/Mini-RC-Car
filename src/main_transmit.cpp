#include <SPI.h>
//#include <nRF24L01.h>
#include <RF24.h>

#define iX A2
#define iY A1
#define iRT A0
#define iLT A3

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";
byte tx[3]; /*0 - fwd speed*/ /*1 - rev speed*//*2 -  steering angle*/
int rawX, rawY, rawLT, rawRT;
const int xMin=77, xMid=79, xMax=285;//Left(min)
const int yMin=84, yMid=196, yMax=304;//fwd(min)
const int LTMin=0, LTMid=50, LTMax=100;
const int RTMin=0, RTMid =50, RTMax=100;
long xSF_1000=865, ySF_1000=1000, LTSF_1000=1000, RTSF_1000=1000;
long calc;
int test;


void setup() {
	Serial.begin(9600);
	radio.begin();
	radio.openWritingPipe(address);
	radio.setPALevel(RF24_PA_MIN);
	radio.stopListening();

	pinMode(iX, INPUT);
	pinMode(iY, INPUT);
	pinMode(iRT, INPUT);
	pinMode(iLT, INPUT);

	Serial.println("TX");
	delay(500);
}

void loop() {

  //delay(100);
  
	rawX = analogRead(iX); rawY = analogRead(iY);  rawLT = analogRead(iLT); rawRT = analogRead(iRT);
  
  ////////////////////////Steering////////////////////////
	calc = rawX;
	if (calc > 496) { // LEFT
		calc = ((calc-496)/8); 
		calc = 90+calc;
	} else if (calc <= 496) { //RIGHT	
		calc = ((496-calc)/8); 
		calc = 90-calc;
	} 
	if (calc > 140) {
		calc = 140;
	}
	if (calc < 40) {
		calc = 40;
	}
 
	tx[2] = calc;
	///////////////////////////////////////////////////////
	
	
	////////////////////////Throttle////////////////////////
	calc = rawY;
	if (calc > 540) { //Go FWD
		calc = (calc - 480) / 3; //Scale
		if (calc > 255) { calc = 255; } //Limits
		if (calc < 10 ) { calc = 0; }
		tx[0] = calc;
		tx[1] = 0;
	} else if (calc == 0){ //Go REV
		tx[0] = 80;
		tx[1] = 255;
		//Serial.println("Reversing");
	} else {
    tx[0] = 0;
    tx[1] = 0;
	}
  ///////////////////////////////////////////////////////

	
	//const char text[] = "Hello World";
	radio.write(&tx, sizeof(tx));
	delay(1);
	//Serial.print(rawX); Serial.print(" ---Raw---X--- "); Serial.println(tx[2]); 
	Serial.print(rawY); Serial.print(" ---Raw---Y--- "); Serial.println(tx[0]); 
/*
	Serial.print("x"); Serial.print(rawX); 
	Serial.print(" - y"); Serial.print(rawY); 
	Serial.print("- LT"); Serial.print(rawLT); 
	Serial.print(" - RT"); Serial.println(rawRT); 
*/
}
