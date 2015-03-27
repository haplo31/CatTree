#include <Herkulex.h>



//fonction d'écriture d'un type int en mémoire EEPROM
void sauverInt(int adresse, int val)
{  
    //découpage de la variable val qui contient la valeur à sauvegarder en mémoire
    unsigned char faible = val & 0x00FF; //récupère les 8 bits de droite (poids faible) -> 0010 1100
    //calcul : 1101 0111 0010 1100 & 0000 0000 1111 1111 = 0010 1100
 
    unsigned char fort = (val >> 8) & 0x00FF;  //décale puis récupère les 8 bits de gauche (poids fort) -> 1101 0111
    //calcul : 1101 0111 0010 1100 >> 8 = 0000 0000 1101 0111 puis le même & qu’avant
 
    //puis on enregistre les deux variables obtenues en mémoire
    EEPROM.write(adresse, fort) ; //on écrit les bits de poids fort en premier
    EEPROM.write(adresse+1, faible) ; //puis on écrit les bits de poids faible à la case suivante
}
//lecture de la variable de type int enregistrée précédemment par la fonction que l'on a créée

int lireInt(int adresse)
{
    int val = 0 ; //variable de type int, vide, qui va contenir le résultat de la lecture

    unsigned char fort = EEPROM.read(adresse);     //récupère les 8 bits de gauche (poids fort) -> 1101 0111
    unsigned char faible = EEPROM.read(adresse+1); //récupère les 8 bits de droite (poids faible) -> 0010 1100

    //assemblage des deux variable précédentes
    val = fort ;         // val vaut alors 0000 0000 1101 0111
    val = val << 8 ;     // val vaut maintenant 1101 0111 0000 0000 (décalage)
    val = val | faible ; // utilisation du masque
    // calcul : 1101 0111 0000 0000 | 0010 1100 = 1101 0111 0010 1100

    return val ; //on n’oublie pas de retourner la valeur lue !
}









int Motor1=252;
int Motor2=253;
int mvtPin=2;
int motionState=0;
int valMvtPin=0;
int laserPin=4;
long timer=0;
long beginActivity;
int randMotor1,randMotor2,randTime,randExercice,refMotor1,refMotor2;
int setMinMax,minMotor1,minMotor2,maxMotor1,maxMotor2;
int diffMotor1,diffMotor2;

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
  

  minMotor1=lireInt(10);
  minMotor2=lireInt(20);
  maxMotor1=lireInt(30);
  maxMotor2=lireInt(40);
	int precMotor1=minMotor1;
	int precMotor2=minMotor2
  
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
  
  
  Herkulex.setLed(Motor2,LED_GREEN2); //set the led 
  Herkulex.setLed(Motor1,LED_GREEN2); //set the led
  
  //Herkulex.end();
}
void loop(){
	if (setMinMax==0){
	  Herkulex.moveAllAngle(Motor1, 0, 2);
	  Herkulex.moveAllAngle(Motor2, 0, 2); 	
	  Herkulex.actionAll(1000);
	  Herkulex.setLed(Motor1,LED_PINK);
	  Herkulex.setLed(Motor2,LED_PINK);
	  delay(7000);
	  Herkulex.getAngle(Motor1)=minMotor1;
	  Herkulex.getAngle(Motor2)=minMotor2;
	  Herkulex.setLed(Motor1,LED_RED);
	  Herkulex.setLed(Motor2,LED_RED);
	  delay(1000);
	  Herkulex.moveAllAngle(Motor1, 0, 2);
	  Herkulex.moveAllAngle(Motor2, 0, 2); 	
	  Herkulex.actionAll(1000);
	  Herkulex.setLed(Motor1,LED_PINK);
	  Herkulex.setLed(Motor2,LED_PINK);
	  delay(7000);
	  Herkulex.getAngle(Motor1)=maxMotor1;
	  Herkulex.getAngle(Motor2)=maxMotor2; 
	  sauverInt(10,minMotor1);
	  sauverInt(20,minMotor2);
	  sauverInt(30,maxMotor1);
	  sauverInt(40,maxMotor2);
	  setMinMax=0; 
	}
	else{
		if(motionState==0){
			valMvtPin=digitalRead(mvtPin);
			if (valMvtPin==1){
				motionState=1;
				digitalWrite(laserPin,HIGH);
				beginActivity=millis();
			}
			else{
				delay(3000);
			}
		}
		else{
			if(millis()-beginActivity<30000){
		      	if(millis() - timer > 700)
			  	{
				    randMotor1 = int(random(minMotor1,maxMotor1));
				    randMotor2 = int(random(minMotor2,maxMotor2));
				    if(diffMotor1>diffMotor2){
				    	while (randMotor1<precMotor1){
				    		randMotor1 = int(random(minMotor1,maxMotor1));	
				    	}
				    }
				    else{
				    	while (randMotor2<precMotor2){
				    		randMotor2 = int(random(minMotor2,maxMotor2));	
				    	}				    	
				    }
				    randTime=int(random(500,1500));
				    Serial.print(randMotor1);
				    Herkulex.moveAllAngle(Motor1, randMotor1, 2);
				    Herkulex.moveAllAngle(Motor2, randMotor2, 2); 
				    Herkulex.actionAll(randTime);
				    timer=millis();
				    delay(randTime);
			  	}
			}
			else{
				motionState=0;
				digitalWrite(laserPin,LOW);
			}
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

