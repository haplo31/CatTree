#include <Herkulex.h>
int Motor1=252;
int Motor2=253;
int mvtPin=2;
int motionState=0;
int valMvtPin=0;
int laserPin=4;
long timer=0;
long beginActivity;
int randMotor1,randMotor2,randTime,randExercice,refMotor1,refMotor2;
void setup()  
{
  pinMode(laserPin,OUTPUT);
  digitalWrite(laserPin,HIGH);
  pinMode(mvtPin, INPUT);
  delay(2000);  //a delay to have time for serial monitor opening
  Serial.begin(115200);    // Open serial communications
  Serial.println("Begin");
  digitalWrite(laserPin,LOW);
  Herkulex.beginSerial1(115200); //open serial port 1 
  Herkulex.reboot(Motor1); //reboot first motor
  Herkulex.reboot(Motor2); //reboot second motor
  delay(500); 
  Herkulex.initialize(); //initialize motors
  
  // read status
  Serial.println("");
  Serial.print("Status1:"); 
  Serial.println(Herkulex.stat(Motor1));
  Serial.print("Status2:"); 
  Serial.println(Herkulex.stat(Motor2));
  

   

  
  delay(10);
   
   // move all to angle
  Serial.println("");
  Serial.println("Move All to Angle");
  Herkulex.moveAllAngle(Motor1, 0, 2);
  Herkulex.moveAllAngle(Motor2, 0, 2); 
  Herkulex.actionAll(2000);
  delay(2100);
  Serial.print("Get Angle1:"); 
  Serial.println(Herkulex.getAngle(Motor1));
  Serial.print("Get Angle2:"); 
  Serial.println(Herkulex.getAngle(Motor2));
  
  
  Herkulex.setLed(Motor2,LED_PINK); //set the led 
  Herkulex.setLed(Motor1,LED_GREEN2); //set the led
  
  //Herkulex.end();
}
void loop(){
	if(motionState==0){
		valMvtPin=digitalRead(mvtPin);
		if (valMvtPin==1){
			motionState=1;
			digitalWrite(laserPin,HIGH);
			beginActivity=millis();
			randExercice = int(random(1,3));
		}
		else{
			delay(3000);
		}
	}
	else{
		if(millis()-beginActivity<30000){
			switch (randExercice) {
			    case 1:
			      	if(millis() - timer > 700)
				  	{
					    randMotor1 = int(random(61));
					    randMotor2 = int(random(61));
					    randTime=int(random(500,1500));
					    Serial.print(randMotor1);
					    Herkulex.moveAllAngle(Motor1, randMotor1, 2);
					    Herkulex.moveAllAngle(Motor2, randMotor2, 2); 
					    Herkulex.actionAll(randTime);
					    timer=millis();
					    delay(randTime);
				  	}
			      break;
			    case 2:
			      	if(millis() - timer > 5000)
				  	{
					    randMotor1 = int(random(61));
					    randMotor2 = int(random(61));
					    refMotor1= randMotor1;
					    refMotor2= randMotor2;
					    randTime=int(random(500,1500));
					    Serial.print(randMotor1);
					    Herkulex.moveAllAngle(Motor1, randMotor1, 2);
					    Herkulex.moveAllAngle(Motor2, randMotor2, 2); 
					    Herkulex.actionAll(randTime);
					    timer=millis();
					    delay(randTime);
				  	}
				  	else{
					    randMotor1 = refMotor1+int(random(-5,5));
					    randMotor2 = refMotor2+int(random(-5,5));
					    randTime=int(random(500,1500));
					    Serial.print(randMotor1);
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
  /*valMvtPin=digitalRead(mvtPin);
  if (valMvtPin==HIGH){
    if(motionState==LOW){
      motionState=HIGH;
      Serial.println("Movement detected");
    }
  }
  else{
    if (motionState==HIGH){
      motionState=LOW;
      Serial.println("Movement stopped");
    }
  }*/
 
}

