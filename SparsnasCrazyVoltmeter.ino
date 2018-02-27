#include <ESP8266WiFi.h>


//// CONSTANTS ////

const int SPARSNASPULSE = 10000;  // SET "PULSE SETTING" IN SPARSNAS TO THIS VALUE

                                      
                                  // THERORETICAL FULL SCALE ADC ANALOG INPUT VOLTAGE (THAT EQUALS DIGITAL VALUE = "1024") IS ACCORDING TO THE 
                                  // WEMOS D1 MINI SPECIFICATIONS 3300 MILLIVOLT. WHICH IT ISN'T... IT'S RATHER SOMEWHERE BETWEEN 3000 AND 3100 MILLIVOLT

                                  // ADJUST HERE TO MATCH INPUT ANALOG VOLTAGE <-> VALUE PRINTED ON THE SERIAL PORT
                                  // ------------------------------------------------------------------------------
const float MAXVOLT = 3040;       // SET TO THE ACTUAL INPUT ANALOG VOLTAGE (IN MILLIVOLTS) THAT GENERATES A FULL SCALE DIGITAL VALUE = WHERE IT "BOTTOMS OUT"                       
const float VOLTOFFSET = -10;     // FINETUNE SERIAL PORT PRINTED DIGITAL VALUE IF ZERO VOLT ANALOG DOESN'T MATCH UP AS ZERO DIGITALLY. 



                                  // ADJUST HERE TO MATCH VALUE PRINTED ON THE SERIAL PORT <-> VALUE SHOWN ON SPARSNAS
                                  // ------------------------------------------------------------------------------
const float TUNE = 0.9989;        // 1 = NO ADJUSTMENT. 0.99 = LOWER NUMBER DISPLAYED
const float TUNEOFFSET = 0;       // MORE NEGATIVE = HIGHER NUMBER DISPLAYED



const int LEDOUTPUTPIN = 2;       // GPIO PIN FOR IR-LED (GPIO2 = D4)
const int ANALOGINPUTPIN = 0;     // GPIO PIN FOR ANALOG INPUT
const int UPDATETIMER = 100;      // TIME BETWEEN EACH READ OF ANALOG INPUT
const int PULSEWIDTH = 30;
const int BAUDRATE = 76800;



//// VARIABLES ////

int blinktimer = 0;               // TIME BETWEEN EACH BLINK (ms)
int sparsnasnumber = 0;           // NUMBER TO DISPLAY ON SPARSNAS
unsigned long now = 0;
unsigned long lastblink = 0;
unsigned long lastupdate = 0;




// ------------------------------------------------------
void setup() {
  Serial.begin(BAUDRATE);
  pinMode(LEDOUTPUTPIN, OUTPUT);
  digitalWrite(LEDOUTPUTPIN, HIGH);
  Serial.printf("\nSet pulse setting in Sparsnas to: %d\n\n", SPARSNASPULSE);
  Serial.println("Show:  Timer:");
}



// ------------------------------------------------------
void loop() {
  now = millis();
  if (now - lastblink >= blinktimer) {
    lastblink = now;
    blink();
  }
  if (now - lastupdate >= UPDATETIMER) {
//  sparsnasnumber = analogRead(ANALOGINPUTPIN);                               // SHOW RAW DATA INPUT 0-1024
    sparsnasnumber = ((analogRead(ANALOGINPUTPIN)*MAXVOLT)/1024)+VOLTOFFSET;   // SHOW INPUT VOLTAGE 0-3300mV (On Wemos D1 mini!)
    blinktimer = getblinktiming(sparsnasnumber);
  }
}



// ------------------------------------------------------
int getblinktiming(int sparsnasnumber) {
  int blinktimer = TUNEOFFSET + ( (10000/float(SPARSNASPULSE)) * 360000 * (1/(TUNE*float(sparsnasnumber))) );
  Serial.printf("%02d%11d\n", sparsnasnumber, blinktimer);
  return blinktimer;
}



// ------------------------------------------------------
void blink() {
  digitalWrite(LEDOUTPUTPIN, LOW);
  delay(PULSEWIDTH);
  digitalWrite(LEDOUTPUTPIN, HIGH);
}
