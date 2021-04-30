//IOT Project I.A.M.S Indoor Air Quality Monitor
//Nate Hardy - 2021 

#include <Adafruit_DHT.h>   //Include the Afafruit DHT library

#define DHTPIN 5            //pin for DHT11 sensor
#define DHTTYPE DHT11	    //setting the correct DHT sensor type

DHT dht(DHTPIN, DHTTYPE);   //inilize the DHT sensor with the DHT pin and type specified

const int TMP36Pin = A1;    //TMP-36 temp sensor pin
const int MQ2 = A5;          //MQ-2 CO2 sensor pin

double voltage, TMP36F, TMP36C, h, t, f, MQ2value;
int reading = 0;

void setup() {
    //Start serial and print a message!
	Serial.begin(9600); 
	Serial.println("IOT PROJECT IAMS Serial Test OKAY");
	
	pinMode(TMP36Pin, INPUT);   //Start the pin for the TMP36 in input mode
	pinMode(MQ2, INPUT);        //Start the pin for the MQ-2 in input mode 
    
	dht.begin(); //Initilize the DHT sensor
	
	//Start the Paricle.io variables
	
	Particle.variable("DHT-11 in degF", f);
	Particle.variable("DHT-11 in degC", t);
	Particle.variable("DHT-11 in %h", h);
	
	Particle.variable("TMP36 in degF", TMP36F);
	Particle.variable("TMP36 in degC", TMP36C);
	Particle.variable("TMP36 volts", voltage);
	
	Particle.variable("MQ-2 Data", reading);
}

void loop() {
	delay(2000); // Wait a few seconds between measurements.
	
    MQ2value = analogRead(MQ2);         //Reading the data from the MQ-2 sensor
    reading = analogRead(TMP36Pin);  //Reading the data from the TMP36

//Convert data from the TMP36
    voltage = (reading * 3.3) / 4095.0;     //Convert data into 10mv/deg
    TMP36C = (voltage - 0.5) * 100.0;       //Calculate degC value
    TMP36F = (TMP36C * (9/5) ) + 32.0;      //Calculate degF from the degC value

// Reading temperature or humidity takes about 250 milliseconds!
// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
	h = dht.getHumidity();      // Read humidity
	t = dht.getTempCelcius();   // Read temperature as Celsius
	f = dht.getTempFarenheit(); // Read temperature as Farenheit

// Check if any reads failed and exit early (to try again).
	if (isnan(h) || isnan(t) || isnan(f)) {
		Serial.println("Failed to read from DHT sensor!");
		//return;
	}
	
/*  ==============================================
                SENDING CLOUD DATA
    ============================================== */
	Particle.publish("DHT-11 in degF", String(f));
	Particle.publish("DHT-11 in %h", String(h));
	
	Particle.publish("TMP36 in degF", String(TMP36F));
   
    Particle.publish("MQ-2 Data", String(MQ2value));
    
/*  ==============================================
                SENDING SERIAL DATA
    ============================================== */
//MQ-2 Sensor Data
    Serial.println("MQ-2 DATA");
    Serial.println(MQ2value);
    Serial.println("");
    
//TMP-36 Sensor Data
    Serial.println("TMP-36 DATA");
    Serial.print("Voltage: ");
    Serial.println(voltage);
    
    Serial.print("degC");
    Serial.println(TMP36C);
    
    Serial.print("degF");
    Serial.print(TMP36F);

//DHT11 Sensor Data
    
	Serial.println("Humid: "); 
	Serial.print(h);
	Serial.print("% - ");
	Serial.print("Temp: "); 
	Serial.print(t);
	Serial.print("*C ");
	Serial.print(f);
	Serial.print("*F ");
	
// Compute heat index
// Must send in temp in Fahrenheit!
/*	float hi = dht.getHeatIndex();
	float dp = dht.getDewPoint();
	float k = dht.getTempKelvin(); */

	
/*	Serial.print(k);
	Serial.print("*K - ");
	Serial.print("DewP: ");
	Serial.print(dp);
	Serial.print("*C - ");
	Serial.print("HeatI: ");
	Serial.print(hi);
	Serial.println("*C");
	Serial.println(Time.timeStr()); */
}