#include "LDHT.h"
#define DHTPIN 3          // what pin we're connected to
#define DHTTYPE DHT22     // using DHT11 sensor

LDHT dht(DHTPIN, DHTTYPE);

int EP = 2; // SW-420 vibration sensor, using pin D2
volatile long vib = 0;
volatile boolean in_use = false;
volatile unsigned long now, past;

float tempC = 0.0, Humi = 0.0;
char readcharbuffer[20];
int readbuffersize;
char lora_status;
String sensorData;

// Interrupt Service Routine (ISR)
void vibrate() {
  past = millis();
  in_use = true;
}

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  dht.begin();
  pinMode(EP, INPUT); // set EP input for measurment
  Serial.print(DHTTYPE);
  Serial.println(" test!");
  past = millis();
  attachInterrupt(0, vibrate, RISING); // attach interrupt handler
}

void loop() {
  now = millis();
  Serial.print("now ");
  Serial.println(now);
  Serial.print("past ");
  Serial.println(past);
  Serial.print("now - past = ");
  Serial.println(now - past);
  if (now - past >= 2 * 60 * 1000 && in_use) { // 2 minutes for test
    in_use = false;
  }
  noInterrupts();
  delay(5000); // avoid generating dirty data to tempC and Humi
  if (dht.read()) {
    tempC = dht.readTemperature();
    Humi = dht.readHumidity();
    vib = TP_init();

    if (!in_use) { // in_use == false
      Serial.println("******************************");
      Serial.println("Not in use!");
      Serial.println("******************************");
      Serial.print("Temperature Celcius = ");
      Serial.print(String(tempC, 1));
      Serial.println("C");
      Serial.print("Humidity = ");
      Serial.print(String(Humi, 1));
      Serial.println("%");

      if (vib > 100) {
        Serial.print("Vibration = ");
        Serial.println(vib);
        vib = 1;
        in_use = true;
      }
      else {
        vib = 0;
        in_use = false;
        Serial.println("No vibration detected!");
      }
      past = now;
    }
    else { // in_use == true
      Serial.println("------------------------------");
      Serial.println("In use!");
      Serial.println("------------------------------");
      Serial.print("Temperature Celcius = ");
      Serial.print(String(tempC, 1));
      Serial.println("C");
      Serial.print("Humidity = ");
      Serial.print(String(Humi, 1));
      Serial.println("%");

      vib = 1;
    }

    sensorData = String(tempC * 10, 0) + String(Humi * 10, 0) + String(vib);
    Serial.println("Ready to Send");
    Serial.println("AT+DTX=" + String(sensorData.length()) + ",\"" + sensorData + "\"");
    Serial1.println("AT+DTX=" + String(sensorData.length()) + ",\"" + sensorData + "\"");

    readbuffersize = Serial1.available();
    while (readbuffersize) {
      lora_status = Serial1.read();
      Serial.print(lora_status);
      readbuffersize--;
    }
    Serial.println("things");
    interrupts();
    if (in_use) {
      Serial.println("Delay 1 minute......");
      delay(1 * 60 * 1000 -5000);
    }
    else {
      Serial.println("Delay 30 seconds...");
      delay(30 * 1000 -5000);
    }
  }
}


long TP_init() {
  delay(10);
  long measurement = pulseIn (EP, HIGH); // wait for the pin to get HIGH and returns measurement
  return measurement;
}

