#include <Herkulex.h>
#include <EEPROM.h>


//Function for writing value in EEPROM
void saveInt(int adress, int val)
{  
    unsigned char weak = val & 0x00FF;
    unsigned char strong = (val >> 8) & 0x00FF; 
    EEPROM.write(adress, strong) ;
    EEPROM.write(adress+1, weak) ;
}
//Function for reading value from EEPROM
int readInt(int adress)
{
    int val = 0 ;
    unsigned char strong = EEPROM.read(adress);
    unsigned char weak = EEPROM.read(adress+1);
    val = strong ;
    val = val << 8 ;
    val = val | weak ;

    return val ;
}
int Motor1=252; // id motor HerkuleX 1
int Motor2=253; // id motor HerkuleX 2
int mvtPin=2;   // id pin motion sensor
int XMaxValue=61; // Maximum angle of X Servo
int YMaxValue=61; // Maximum angle of Y Servo
int motionState=0;
int valMvtPin=0;
int laserPin=4; // id pin laser
long timer=0;
long beginActivity;
int randMotor1,randMotor2,randTime,randExercice,refMotor1,refMotor2;
int setMinMax,minMotor1,minMotor2,maxMotor1,maxMotor2;
int diffMotor1,diffMotor2;
int precMotor1;
int precMotor2;
int setState;
void setup()  
{
  pinMode(laserPin,OUTPUT);
  digitalWrite(laserPin,HIGH); // Activating laser
  pinMode(mvtPin, INPUT);
  Serial.begin(115200);    // Open serial communications
  digitalWrite(laserPin,LOW); // Desactivating laser
  Herkulex.beginSerial1(115200); // Open serial port 1 
  Herkulex.reboot(Motor1); // Reboot first motor
  Herkulex.reboot(Motor2); // Reboot second motor
  delay(500); 
  Herkulex.initialize(); // Initialize motors
  
  // read status
  Serial.println("");
  Serial.print("Status1:"); 
  Serial.println(Herkulex.stat(Motor1));
  Serial.print("Status2:"); 
  Serial.println(Herkulex.stat(Motor2));
   
  minMotor1=readInt(10);
  minMotor2=readInt(20);
  maxMotor1=readInt(30);
  maxMotor2=readInt(40);
  precMotor1=minMotor1;
  precMotor2=minMotor2;
  
  delay(10);
   
  // Move servos to Home
  Serial.println("");
  Herkulex.moveAllAngle(Motor1, 0, 2);
  Herkulex.moveAllAngle(Motor2, 0, 2); 
  Herkulex.actionAll(2000);
  delay(2100);
  
  attachInterrupt(2, setLimits, RISING); // Set a routine who calls setLimits function when pressing the set button
  //End of initialization
  Herkulex.setLed(Motor2,LED_GREEN2);
  Herkulex.setLed(Motor1,LED_GREEN2);
}
void setLimits(){
       // noInterrupts();
	//Move servos to Home
	torqueOFF(Motor1);
	torqueOFF(Motor2);
	Herkulex.moveAllAngle(Motor1, 0, 2);
	Herkulex.moveAllAngle(Motor2, 0, 2); 	
	Herkulex.actionAll(1000);

	//Set servos led to pink and wait 7seconds
	Herkulex.setLed(Motor1,LED_PINK);
	Herkulex.setLed(Motor2,LED_PINK);
	delay(7000);

	//Get minimum angle values for servos
	Herkulex.setLed(Motor1,LED_RED);
	Herkulex.setLed(Motor2,LED_RED);
	delay(1000);
	minMotor1 = Herkulex.getAngle(Motor1);
	minMotor2 = Herkulex.getAngle(Motor2);
	Herkulex.setLed(Motor1,LED_GREEN2);
	Herkulex.setLed(Motor2,LED_GREEN2);
	delay(1000);

	//Move servos to Home again
	Herkulex.moveAllAngle(Motor1, 0, 2);
	Herkulex.moveAllAngle(Motor2, 0, 2); 	
	Herkulex.actionAll(1000);

	//Set servos led to pink and wait 7seconds
	Herkulex.setLed(Motor1,LED_PINK);
	Herkulex.setLed(Motor2,LED_PINK);
	delay(7000);

	//Get maximum angle values for servos
	Herkulex.setLed(Motor1,LED_RED);
	Herkulex.setLed(Motor2,LED_RED);
	delay(1000);	
	maxMotor1 = Herkulex.getAngle(Motor1);
	maxMotor2 = Herkulex.getAngle(Motor2);

	//Save values in EEPROM 
	saveInt(10,minMotor1);
	saveInt(20,minMotor2);
	saveInt(30,maxMotor1);
	saveInt(40,maxMotor2);
  //interrupts();
  setState=0;
	motionState=0;
	torqueON(Motor1);
	torqueON(Motor2); // for exit in standby mode	
};
void loop(){
	setLimits();
	/*
  Serial.println("loop");
  valueButton=digitalRead(21);
  if(valueButton==1){
  	setLimits();
  	setState=1;
  }
  else{
  	if(setState==0){
	    if(motionState==0){
				valMvtPin=digitalRead(mvtPin);
				if (valMvtPin==1){
					motionState=1;
					digitalWrite(laserPin,HIGH);
					beginActivity=millis();
					randExercice = int(random(1,3)); // Random choose of the exercice
				}
				else{
					delay(500);
				}
			}
			else{
				if(millis()-beginActivity<30000){
			    switch (randExercice) {
						// First Exercice
						case 1:
							if(millis()- timer > 700)
							{
								randMotor1 = int(random(minMotor1,maxMotor1));
						    randMotor2 = int(random(minMotor2,maxMotor2));
								randTime=int(random(500,1500));
								Serial.print(randMotor1);
								Herkulex.moveAllAngle(Motor1, randMotor1, 2);
								Herkulex.moveAllAngle(Motor2, randMotor2, 2); 
								Herkulex.actionAll(randTime);
								timer=millis();
								delay(randTime);
							}
						break;

						// Second Exercice
						case 2:
							if(millis() - timer > 5000){
								randMotor1 = int(random(minMotor1,maxMotor1));
						    randMotor2 = int(random(minMotor2,maxMotor2));
								refMotor1 = randMotor1;
								refMotor2 = randMotor2;
								randTime=int(random(500,1500));
								Herkulex.moveAllAngle(Motor1, randMotor1, 2);
								Herkulex.moveAllAngle(Motor2, randMotor2, 2); 
								Herkulex.actionAll(randTime);
								timer=millis();
								delay(randTime);
							}
							else{
							  // Shaking
								randMotor1 = refMotor1+int(random(-5,5));
								randMotor2 = refMotor2+int(random(-5,5));
								randTime=int(random(500,1500));
								Herkulex.moveAllAngle(Motor1, randMotor1, 2);
								Herkulex.moveAllAngle(Motor2, randMotor2, 2); 
								Herkulex.actionAll(40);
								delay(40);				  		
							}
						break;
					}
				}
				else{
					motionState=0;
					digitalWrite(laserPin,LOW);
				}
			} 
  	}

  }*/
}

