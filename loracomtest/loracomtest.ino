#include "LDHT.h"

#define DHTPIN 2          // what pin we're connected to
#define DHTTYPE DHT22     // using DHT11 sensor

LDHT dht(DHTPIN, DHTTYPE);

int EP = 9; // SW-420 vibration sensor


float tempC = 0.0, Humi = 0.0;
char readcharbuffer[20];
int readbuffersize;
char lora_status;
String sensorData;

void setup() {

  Serial.begin(9600);
  Serial1.begin(9600);

  dht.begin();
  pinMode(EP, INPUT); //set EP input for measurment
  Serial.print(DHTTYPE);
  Serial.println(" test!");
}
void loop() {
  if (dht.read())
  {
    tempC = dht.readTemperature();
    Humi = dht.readHumidity();
    long measurement = TP_init();

    Serial.println("------------------------------");
    Serial.print("Temperature Celcius = ");
    Serial.print(String(tempC, 1));
    Serial.println("C");

    Serial.print("Humidity = ");
    Serial.print(String(Humi, 1));
    Serial.println("%");

    Serial.print("Vibration = ");
    Serial.println(measurement);
    if (measurement > 100) {
      measurement = 1;
      Serial.println("Vibration detected!");
    }
    else{
      measurement = 0;
      Serial.println("No vibration detected!");
    }

    sensorData = String(tempC*10, 0) + String(Humi*10, 0) + String(measurement);
    
    Serial.println("Ready to Send");
    Serial.println("AT+DTX="+ String(sensorData.length()) + ",\"" + sensorData + "\"");
    // Serial.println("AT+DTX="+ String(sensorData.length()) + "," + sensorData);

    Serial1.println("AT+DTX="+ String(sensorData.length()) + ",\"" + sensorData + "\"");
    // Serial1.println("AT+DTX="+ String(sensorData.length()) + "," + sensorData);

    
    // Serial.println("AT+DTX=16,1234567890abcdef");
    // Serial1.println("AT+DTX=16,1234567890abcdef");
    // Serial1.println("AT+DTX=11,\"12345ABCdef\"");


  }

  /*
    Serial.println("Ready to Send");
    Serial1.println("AT+DTX=11,\"12345ABCdef\"");
    delay(1000);*/
  /*readbuffersize = Serial1.available();
    while (readbuffersize) {
    lora_status = Serial1.read();
    Serial.print(lora_status);
    readbuffersize--;
    }
    delay(9000);*/
  readbuffersize = Serial1.available();
  while (readbuffersize) {
    lora_status = Serial1.read();
    Serial.print(lora_status);
    readbuffersize--;
  }
  Serial.println("things");
  delay(30000);
}

long TP_init() {
  delay(10);
  long measurement = pulseIn (EP, HIGH); //wait for the pin to get HIGH and returns measurement
  return measurement;
}
