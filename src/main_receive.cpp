#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#define qMTR_MODE 2
#define qMTR_SLEEP 4
#define qMTR_FWD 5
#define qMTR_REV 6
#define qSRVO 3

RF24 radio(7, 8); // CE, CSN
Servo steering; 

const byte address[6] = "00001";
byte rx[3]; /*0 - fwd speed*/ /*1 - rev speed*//*2 -  steering angle*/
long steeringTO;
int receiveTO;


void setup() {
	Serial.begin(9600);
	radio.begin();
	radio.openReadingPipe(0, address);
	radio.setPALevel(RF24_PA_MIN);
	radio.startListening();

	pinMode(qMTR_MODE, OUTPUT);
	pinMode(qMTR_SLEEP, OUTPUT);
	pinMode(qMTR_FWD, OUTPUT);
	pinMode(qMTR_REV, OUTPUT);
	steering.attach(qSRVO);

	digitalWrite(qMTR_MODE, HIGH);
	//analogWrite(qSRVO, 0);
	steering.write(90);
	delay(500);
	digitalWrite(qMTR_SLEEP, HIGH);

	Serial.println("RX");
	delay(500);
}

void loop() {

	if (radio.available()) {
		receiveTO = 0;
		//char text[32] = "";
		radio.read(&rx, sizeof(rx));
		analogWrite(qMTR_FWD, rx[0]);
		analogWrite(qMTR_REV, rx[1]);
		//analogWrite(qMTR_FWD, 40);
		//analogWrite(qMTR_REV, 100);
		//map(value, fromLow, fromHigh, toLow, toHigh)
		rx[2] = map(rx[2],0,180,180,0);
		//rx[2] -= 7;
		steering.write(rx[2]);

	if (rx[2]  > 80 and rx[2] < 100) {
		steeringTO += 1;
	} else {
		steeringTO = 0;
	}
	if (steeringTO > 100) {
		//Serial.println("Detached");
		steering.detach();
	} else {
		steering.attach(qSRVO);
	}
		
		Serial.print("fwd"); Serial.print(rx[0]);
		Serial.print(" - rev"); Serial.print(rx[1]);
		Serial.print(" - steer"); Serial.println(rx[2]);
		
	}	else {
		receiveTO += 1;
		if (receiveTO > 100) {
			steering.write(90);			
			analogWrite(qMTR_FWD, 0);
			analogWrite(qMTR_REV, 0);
			delay(500);
			steering.detach();
		}
	}
	//Serial.println(steeringTO);

/*
	delay(5000);
	steering.attach(qSRVO);
	steering.write(90);


	for (int i=90; i>=50; i-=5) {//CENTER-RIGHT
		steering.write(i);
		Serial.println(i);
		delay(20);
	}
	steering.write(51);
	delay(1000);
	for (int i=50; i<=100; i+=5) {//RIGHT-CENTER
		steering.write(i);
		Serial.println(i);
		delay(20);
	}
	delay(100);
	steering.write(90);
	delay(1000);


	for (int i=90; i<=130; i+=5) {//CENTER-LEFT
		steering.write(i);
		Serial.println(i);
		delay(20);
	}
	delay(1000);
	steering.write(129);
	delay(1000);
	for (int i=130; i>=80; i-=5) {//LEFT-CENTER
		steering.write(i);
		Serial.println(i);
		delay(20);
	}
	delay(500);
	steering.write(90);
	steering.detach();

	delay(10000);
*/
/*
	analogWrite(qMTR_REV, 0);
	delay(30000);
	analogWrite(qMTR_FWD, 50);
	steering.write(90);
	delay(5000);
	analogWrite(qMTR_FWD, 0);
	steering.write(90);
	delay(15000);
	analogWrite(qMTR_FWD, 50);
	steering.write(130);
	delay(5000);
	analogWrite(qMTR_FWD, 0);
	steering.write(90);
	delay(15000);
	analogWrite(qMTR_FWD, 150);
	steering.write(90);
	delay(5000);
	analogWrite(qMTR_FWD, 0);
	steering.write(90);
	delay(15000);
	analogWrite(qMTR_FWD, 255);
	steering.write(90);
	delay(3000);
	analogWrite(qMTR_FWD, 0);
	steering.write(90);
*/

}
