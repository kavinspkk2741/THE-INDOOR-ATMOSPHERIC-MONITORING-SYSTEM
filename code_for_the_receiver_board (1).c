//IOT Project IAMS Reciver code
//Nate Hardy - 2021

const int redP = 2;          //Pin for the Red of the RGB LED
const int greenP = 3;       //Pin for the Green of the RGB LED
const int blueP = 4;        //Pin for the blue of the RGB LED

const int baseTemp = 70;    //Base temperture to compare measured temp to
const int baseHumid = 30;   //Base humidity to compare measured values to
const int goodCO2 = 1350;   //CO2 level considered 'safe'

double temp;                                            //calculated temperture value
int DHT11TempVar, DHT11HumidVar, TMP36temp, MQ2Var;     //Varables for the sensor values from the other Argon
int red, green, blue;                                   //Values for red, green, and blue to be sent to the LED

void DHT11temp(const char *event, const char *data) {
    DHT11TempVar = atof(data);  //Reading the cloud data from the DHT-11 temperture
}

void DHT11humid(const char *event, const char *data) {
    DHT11HumidVar = atof(data); //Reading the cloud data from the DHT-11 humdity
}

void TMP36(const char *event, const char *data) {
    TMP36temp = atof(data);     //Reading the cloud data from the TMP-36 
}

void MQ2(const char *event, const char *data) {
    MQ2Var = atof(data);        //Reading the cloud data from the MQ-2 sensor
}

void RGBWrite(int redV, int greenV, int blueV) {
    analogWrite(redP, redV);                            //function to write data to the RGB LED
    analogWrite(greenP, greenV);
    analogWrite(blueP, blueV);
}

void setup() {
    Serial.begin(9600);                                 //Serial setup and self-test
    Serial.println("Serial Test OK");
    
    pinMode(redP, OUTPUT);                              //Setup pin mode for the RGB LED 
    pinMode(greenP, OUTPUT);
    pinMode(blueP, OUTPUT);
    
    red = 100;                                          //Initilize the values for the different colors
    green = 100;
    blue = 100;
    
    Particle.subscribe("DHT-11 in degF", DHT11temp);    //Subscribe to the data outputed from the other Argon
	Particle.subscribe("DHT-11 in %h", DHT11humid);
	
	Particle.subscribe("TMP36 in degF", TMP36);
   
    Particle.subscribe("MQ-2 Data", MQ2);
    
}

void loop() {
    delay(2000);
    
    Serial.println(DHT11TempVar);                       //print the serial data recived
    Serial.println(DHT11HumidVar);
    Serial.println(TMP36temp);
    Serial.println(MQ2Var);
    
    temp = (DHT11TempVar + TMP36temp)/2;                //Temperture average between the 2 sensors
    RGBWrite(0,0,0);
    
    if (MQ2Var >= goodCO2) {
        RGBWrite(255,0,0);                           
    } else if (temp >= baseTemp + 5) {
        RGBWrite(0,0,255);
    } else if (temp <= baseTemp - 5 ) {                        
        RGBWrite(0,0,255);   
    } else if (DHT11HumidVar <= baseHumid - 10) {
        RGBWrite(255,255,0);
    } else if (DHT11HumidVar >= baseHumid + 10) {
        RGBWrite(0,255,255);
    } else {
        RGBWrite(0,0,0);
    }
    
}
