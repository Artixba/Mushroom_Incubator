/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

*************************************************************
  WARNING!
    It's very tricky to get it working. Please read this article:
    http://help.blynk.cc/hardware-and-libraries/arduino/esp8266-with-at-firmware

  This example shows how value can be pushed from Arduino to
  the Blynk App.

  NOTE:
  BlynkTimer provides SimpleTimer functionality:
    http://playground.arduino.cc/Code/SimpleTimer

  App project setup:
    Value Display widget attached to Virtual Pin V5
 *************************************************************/

#define BLYNK_PRINT Serial
// Hardware Serial on Mega, Leonardo, Micro...
#define EspSerial Serial1
// Your ESP8266 baud rate:
#define ESP8266_BAUD 115200

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <UTFT.h>
#include <URTouch.h>

//===================================================================
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "mKU7v6Mt6-aUA_vgBAg0ycJwpJ0aO_1n";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Celaya";
char pass[] = "FamiliaPerez5";
//char pass[] = "ni+bd+h3h+fg";

ESP8266 wifi(&EspSerial);
BlynkTimer timer;

//===================================================================
//==== Creating Objects
UTFT    myGLCD(ITDB32S_V2, 38, 39, 40, 41); //Parameters should be adjusted to your Display/Schield model
URTouch  myTouch( 6, 5, 4, 3, 2);

//==== Defining Variables
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegmentFull[];

int x, y;

char currentPage, selectedUnit;
boolean targetPlusPress = false;
boolean targetMinusPress = false;

//==== SHT30 Temp/Humid Sensor=======================================
#define VREF 5.0
#define TEMPERATURE_PIN A1
#define TEMPERATURE_PIN2 A2
#define HUMIDITY_PIN A0
#define HUMIDITY_PIN A3
#define ADC_RESOLUTION 1024

int targetF(0), tempF, targetRH(0), RH, concentration, targetCO2(0), targetLight(0), targetValTen(0), targetValTenL(0);
float analogVolt;
const int HeatCTRL = 8;
const int HumCTRL = 9;

//=== SEN0219 CO2 Sensor=============================================
int CO2In = A4;
const int FanOn = 10;

//=== Light Sensor===================================================
int LightIn = A5;
const int LightOn = 11;

//===================================================================
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.

void setup()
{
  // Debug console
  Serial.begin(9600);

  drawHomeScreen();  // Draws the Home Screen
  currentPage = '0'; // Indicates that we are at Home Screen
  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass);
  // You can also specify server:
  // Blynk.begin(auth, wifi, ssid, pass, "blynk-cloud.com", 80);
  // Blynk.begin(auth, wifi, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Initial setup
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);

  analogReference(DEFAULT); //Set the default voltage of the reference voltage
  pinMode(HeatCTRL, OUTPUT); // TEMPERATURE CONTROL MODULE
  pinMode(HumCTRL, OUTPUT);  // HUMIDITY CONTROL MODULE
  pinMode(FanOn, OUTPUT);  // FAN CONTROL MODULE
  pinMode(LightOn, OUTPUT);  // LIGHT CONTROL MODULE

  timer.setInterval(3000L, getTempReading); //========== Temperature
  timer.setInterval(3000L, getHumReading);  //========== Humidity
  timer.setInterval(3000L, getCO2Reading);  //========== CO2
  //timer.setInterval(3000L, getHumReading);  //========== Light

}

void loop()
{
  pageSelection();

  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}

//===================================================================
// Function to switch between pages
void pageSelection() {
  if (currentPage == '0') {
    if (myTouch.dataAvailable()) {
      myTouch.read();
      x = myTouch.getX(); // X coordinate where the screen has been pressed
      y = myTouch.getY() * (-1) + 240; // Y coordinates where the screen has been pressed

      // If we press the Distance Sensor Button
      if ((x >= 35) && (x <= 285) && (y >= 90) && (y <= 130)) {
        drawFrame(35, 90, 285, 130); // Custom Function -Highlighs the buttons when it's pressed
        currentPage = '1'; // Indicates that we are the first example
        myGLCD.clrScr(); // Clears the screen
        drawTempHumSensor(); // It is called only once, because in the next iteration of the loop, this above if statement will be false so this funtion won't be called.
      }
      // If we press the CO2 Sensor Button
      if ((x >= 35) && (x <= 285) && (y >= 140) && (y <= 180)) {
        drawFrame(35, 140, 285, 180); // Custom Function -Highlighs the buttons when it's pressed
        currentPage = '2'; // Indicates that we are the first example
        myGLCD.clrScr(); // Clears the screen
        drawCO2Sensor(); // It is called only once, because in the next iteration of the loop, this above if statement will be false so this funtion won't be called. This function will draw the graphics of the first example.
      }
      // If we press the Light Sensor Button
      if ((x >= 35) && (x <= 285) && (y >= 190) && (y <= 230)) {
        drawFrame(35, 190, 285, 230); // Custom Function -Highlighs the buttons when it's pressed
        currentPage = '3'; // Indicates that we are the first example
        myGLCD.clrScr(); // Clears the screen
        drawLightSensor(); // It is called only once, because in the next iteration of the loop, this above if statement will be false so this funtion won't be called. This function will draw the graphics of the first example.
      }
    }
  }

  
  // HomeScreen
  if (currentPage == '0') {
    if (myTouch.dataAvailable()) {
      myTouch.read();
      x = myTouch.getX();
      y = myTouch.getY() * (-1) + 240;
    }
    //Sensor Background Monitoring Functions
    getTempHumControl();
  }

  // Temp/Humid Sensor Screen
  if (currentPage == '1') {
    drawTempValues();
    if (myTouch.dataAvailable()) {
      myTouch.read();
      x = myTouch.getX();
      y = myTouch.getY() * (-1) + 240;

      // If we press the Tmp Button we get F Degrees
      if ((x >= 10) && (x <= 60) && (y >= 125) && (y <= 175)) {
        selectedUnit = '0';
        //drawTempValues();
      }
      // If we press the Hmd Button we get humidity
      if ((x >= 10) && (x <= 60) && (y >= 180) && (y <= 230)) {
        selectedUnit = '1';
      }
      // If we press the Back Button
      if ((x >= 10) && (x <= 60) && (y >= 10) && (y <= 36)) {
        drawFrame(10, 10, 60, 36);
        currentPage = '0'; // Indicates we are at home screen
        myGLCD.clrScr();
        drawHomeScreen(); // Draws the home screen
      }
    }
  }

  // CO2 Sensor Screen
  if (currentPage == '2') {
    drawCO2Values(); //Gets CO2 value from the sensor and this function is repeatedly called
    if (myTouch.dataAvailable()) {
      myTouch.read();
      x = myTouch.getX();
      y = myTouch.getY() * (-1) + 240;
      //selectedUnit = '1';

      // Back Button
      if ((x >= 10) && (x <= 60) && (y >= 10) && (y <= 36)) {
        drawFrame(10, 10, 60, 36);
        currentPage = '0'; // Indicates we are at home screen
        myGLCD.clrScr();
        drawHomeScreen(); // Draws the home screen
      }
    }
  }

  // Light Sensor Screen
  if (currentPage == '3') {
    //drawLightValues(); // Gets light from the sensor and this function is repeatedly called
    if (myTouch.dataAvailable()) {
      myTouch.read();
      x = myTouch.getX();
      y = myTouch.getY() * (-1) + 240;

      // If we press the Back Button
      if ((x >= 10) && (x <= 60) && (y >= 10) && (y <= 36)) {
        drawFrame(10, 10, 60, 36);
        currentPage = '0'; // Indicates we are at home screen
        myGLCD.clrScr();
        drawHomeScreen(); // Draws the home screen
      }
    }
  }


}

//===================================================================
// ====== Custom Funtions ======
// drawHomeScreen - Custom Function
void drawHomeScreen() {
  // Title
  myGLCD.setBackColor(0, 0, 0); // Sets the background color of the area where the text will be printed to black
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.setFont(BigFont); // Sets font to big
  myGLCD.print("Mushroom Incubator", CENTER, 10); // Prints the string on the screen
  myGLCD.setColor(255, 0, 0); // Sets color to red
  myGLCD.drawLine(0, 32, 319, 32); // Draws the red line
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.setFont(SmallFont); // Sets the font to small
  myGLCD.print("by Team 5", CENTER, 41); // Prints the string
  myGLCD.setFont(BigFont);
  myGLCD.print("Select Sensor", CENTER, 64);

  // Button - Temp/Humid Sensor
  myGLCD.setColor(16, 167, 103); // Sets green color
  myGLCD.fillRoundRect (35, 90, 285, 130); // Draws filled rounded rectangle
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.drawRoundRect (35, 90, 285, 130); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(16, 167, 103); // Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("TEMP/HUM SENSOR", CENTER, 102); // Prints the string

  // Button - CO2 Sensor Screen
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRoundRect (35, 140, 285, 180);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (35, 140, 285, 180);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  myGLCD.print("CO2 SENSOR", CENTER, 152);

  // Button - Light Sensor
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRoundRect (35, 190, 285, 230);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (35, 190, 285, 230);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  myGLCD.print("LIGHT SENSOR", CENTER, 202);
}

//===================================================================
void drawTempHumSensor() {
  //Title lines
  myGLCD.setColor(100, 155, 203);
  myGLCD.fillRoundRect (10, 10, 60, 36);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (10, 10, 60, 36);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(100, 155, 203);
  myGLCD.print("<-", 18, 15);

  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(SmallFont);
  myGLCD.print("Back to Main Menu", 70, 18);
  myGLCD.setFont(BigFont);
  myGLCD.print("Temp Humid Sensor", CENTER, 45);
  myGLCD.print("SHT30", CENTER, 68);
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawLine(0, 95, 319, 95);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setFont(SmallFont);
  myGLCD.print("Select Sensor", 10, 104);
  myGLCD.setFont(BigFont);
  myGLCD.print("Measurement:", 120, 110);

  //Temp Button
  myGLCD.setColor(223, 77, 55);
  myGLCD.fillRoundRect (10, 125, 60, 175);
  myGLCD.setColor(225, 255, 255);
  myGLCD.drawRoundRect (10, 125, 60, 175);
  myGLCD.setBackColor(223, 77, 55);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Tmp", 11, 140);

  //Humid Button
  myGLCD.setColor(223, 77, 55);
  myGLCD.fillRoundRect (10, 180, 60, 230);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (10, 180, 60, 230);
  myGLCD.setBackColor(223, 77, 55);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Hmd", 11, 195);
  myGLCD.setBackColor(0, 0, 0);

  drawTargetControl();
}

//====================================================
void drawCO2Sensor() {
  //Title lines
  myGLCD.setColor(100, 155, 203);
  myGLCD.fillRoundRect (10, 10, 60, 36);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (10, 10, 60, 36);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(100, 155, 203);
  myGLCD.print("<-", 18, 15);

  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(SmallFont);
  myGLCD.print("Back to Main Menu", 70, 18);
  myGLCD.setFont(BigFont);
  myGLCD.print("CO2 Sensor", CENTER, 45);
  myGLCD.print("SEN0219", CENTER, 68);
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawLine(0, 95, 319, 95);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setFont(BigFont);
  myGLCD.print("Measurement in ppm:", CENTER, 100);

  drawTargetControl();
}

//====================================================
void drawLightSensor() {
  //Title lines
  myGLCD.setColor(100, 155, 203);
  myGLCD.fillRoundRect (10, 10, 60, 36);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (10, 10, 60, 36);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(100, 155, 203);
  myGLCD.print("<-", 18, 15);

  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(SmallFont);
  myGLCD.print("Back to Main Menu", 70, 18);
  myGLCD.setFont(BigFont);
  myGLCD.print("Light Sensor", CENTER, 45);
  myGLCD.print("SEN0219", CENTER, 68);
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawLine(0, 95, 319, 95);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setFont(BigFont);
  myGLCD.print("Measurement:", CENTER, 100);

  drawTargetControl();
}

//===================================================================
// Create a function that uses a timer that send info every 3 seconds
// The info sent will be the sensor info, the info received is the target control

void getTempReading() {
  // Analog voltage read-in
  analogVolt = (float)analogRead(TEMPERATURE_PIN) / ADC_RESOLUTION * VREF;
  // Convert voltage to temperature (°F, fahrenheit )
  tempF = -88.375 + 131.25 * analogVolt;
  Blynk.virtualWrite(V1, tempF);
}

//===================================================================
// Create a function that uses a timer that send info every 3 seconds
// The info sent will be the sensor info, the info received is the target control

void getHumReading() {
  analogVolt = (float)analogRead(HUMIDITY_PIN) / ADC_RESOLUTION * VREF;
  // Convert voltage to relative humidity (%)
  RH = -12.5 + 41.667 * analogVolt;
  // Prints the actual humidity in %RH

  Blynk.virtualWrite(V3, RH);
}

//===================================================================
// Sensor and target values are printed on the screen

void drawTempValues() {
  if (selectedUnit == '0') {
    myGLCD.setFont(SevenSegmentFull);
    myGLCD.setColor(0, 255, 0);
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.printNumI(tempF, 70, 135, 3, ' ');
    myGLCD.setFont(BigFont);
    myGLCD.print("F  ", 270, 168);

    //Print TARGET temp as shown above and the target side by side differnt colors
    targetF = getSwitchTarget(targetF);
    myGLCD.setFont(SevenSegmentFull);
    myGLCD.setColor(255, 0, 0);
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.printNumI(targetF, 176, 135, 3, ' ');
    Blynk.virtualWrite(V2, targetF);
    Blynk.syncVirtual(V2);
  }
  if (selectedUnit == '1') {
    myGLCD.setFont(SevenSegmentFull);
    myGLCD.setColor(0, 255, 0);
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.printNumI(RH, 70, 135, 3, ' ');
    myGLCD.setFont(BigFont);
    myGLCD.print("%RH", 270, 168);

    //Print TARGET humidity as shown above and the target side by side differnt colors
    targetRH = getSwitchTarget(targetRH);
    myGLCD.setFont(SevenSegmentFull);
    myGLCD.setColor(255, 0, 0);
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.printNumI(targetRH, 176, 135, 3, ' ');
    myGLCD.setFont(BigFont);
    Blynk.virtualWrite(V4, targetRH);
  }
  getTempHumControl(); 
}

//===================================================================
// Temperature and humdity module control
void getTempHumControl() {
  // Need control of the heating module, use targetT & targetRH to control OUTPUT based on tempF
  // If-else-if loop used to check the actual value and compare it to the target, if +- 3 itll turn ON or OFF module
  int rangePF(0), rangeMF(0), rangePH(0), rangeMH(0);   //rangePF = range plus F, range minus F, rangePH = range plus Hum, rangeMH = range minus Hum
  rangePF = targetF + 3;
  rangeMF = targetF - 3;
  if (tempF > rangePF) {  // When temperature is 3 degrees higher than target temp,HEATING OFF(low)
    digitalWrite(HeatCTRL, LOW); // TEMPERATURE CONTROL OFF
  }
  else if (tempF < rangeMF) { // When temperature is 3 degrees lower than terget temp, HEATING ON
    digitalWrite(HeatCTRL, HIGH);  // TEMPERATURE CONTROL ON
  }
  else {  // When temperature is within 3 degrees target temp, HEATING OFF
    digitalWrite(HeatCTRL, LOW); // TEMPERATURE CONTROL OFF
  }

  rangePH = targetRH + 3;
  rangeMH = targetRH - 3;
  if (RH > rangePH) {  // When humidty is 3 percent higher than target humidity, humidifier OFF
    digitalWrite(HumCTRL, LOW); // HUMIDITY CONTROL OFF
  }
  else if (RH < rangeMH) { // When temperature is 3 percent lower than target humidity, humidifier ON
    digitalWrite(HumCTRL, HIGH);  // HUMIDITY CONTROL ON
  }
  else{   //When humidity is withtin 3 percent of target humdity
    digitalWrite(HumCTRL, LOW);
  }
}

//===================================================================
// Create a function that uses a timer that send info every 3 seconds
// The info sent will be the sensor info, the info received is the target control
void getCO2Reading() {
  //Read voltage
  int sensorValue = analogRead(CO2In);

  // The analog signal is converted to a voltage
  int voltage = sensorValue * (5000 / 1024);
  int voltage_diference = voltage - 400;
  int concentration = voltage_diference * 50 / 16;

  Blynk.virtualWrite(V5, concentration);
}
//===================================================================
// CO2 sensor values are printed on the LCD screen
void drawCO2Values() {

  // Print ACTUAL CO2 level in GREEN
  myGLCD.setFont(SevenSegmentFull);
  myGLCD.setColor(0, 255, 0);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.printNumI(concentration, 10, 135, 4, ' ');
  myGLCD.setFont(BigFont);

  // Print TARGET CO2 level in RED
  targetCO2 = getSwitchTarget10(targetCO2);
  myGLCD.setFont(SevenSegmentFull);
  myGLCD.setColor(255, 0, 0);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.printNumI(targetCO2, 160, 135, 4, ' ');
  Blynk.virtualWrite(V6, targetCO2);
}

//===================================================================
// Create a function that uses a timer that send info every 3 seconds
// The info sent will be the sensor info, the info received is the target control

/*void getLightReading(){
 * 
 *Blynk.virtualWrite(V7, ); 
 }*/

//===================================================================
// Get light values
void drawLightValues() {
  // Actual light sensor value
  int lightSensorVal;
  lightSensorVal = 1000;
  myGLCD.setFont(SevenSegmentFull);
  myGLCD.setColor(0, 255, 0);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.printNumI(lightSensorVal, 10, 135, 4, ' ');

  // Target control variable
  targetLight = getSwitchTarget10(targetLight);
  myGLCD.setFont(SevenSegmentFull);
  myGLCD.setColor(255, 0, 0);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.printNumI(targetLight, 160, 135, 4, ' ');

}

//===================================================================
// BLYNK_WRITE used for pushing info from app to arduino via virtual pins
// A BLYNK_WRITE statement for each sensor
BLYNK_WRITE(V9) {
  targetF = targetF + param.asInt();
  Blynk.virtualWrite(V2, targetF);
}

BLYNK_WRITE(V10) {
  targetRH = targetRH + param.asInt();
  Blynk.virtualWrite(V4, targetRH);
}

BLYNK_WRITE(V11) {
  targetCO2 = targetCO2 + param.asInt();
  Blynk.virtualWrite(V6, targetCO2);
}

//BLYNK_WRITE(V12) {
//  targetRH = targetRH + param.asInt();
//  Blynk.virtualWrite(V8, targetLight);
//}
//===================================================================
void drawTargetControl() {
  //Minus Button
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRoundRect (110, 190, 180, 230);
  myGLCD.setColor(225, 255, 255);
  myGLCD.drawRoundRect (110, 190, 180, 230);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print(" _ ", 122, 195);

  //Plus Button
  myGLCD.setColor(0, 255, 0);
  myGLCD.fillRoundRect (200, 190, 270, 230);
  myGLCD.setColor(225, 255, 255);
  myGLCD.drawRoundRect (200, 190, 270, 230);
  myGLCD.setBackColor(0, 255, 0); //
  myGLCD.setColor(255, 255, 255);
  myGLCD.print(" + ", 212, 203);
}

//===================================================================
// When the plus or minus buttons are pressed the input variable will
// be increased or decreased by ONE
int getSwitchTarget(int targetVal) {
  // If loop to check for touch input
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY() * (-1) + 240;

    // If we press the minus button the target variable is decreased by 1
    if ((x >= 110) && (x <= 180) && (y >= 190) && (y <= 230)) {
      targetMinusPress = true;
    }
    else {
      targetMinusPress = false;
    }
    // If we press the plus button the target variable is increased by 1
    if ((x >= 200) && (x <= 270) && (y >= 190) && (y <= 230)) {
      targetPlusPress = true;
    }
    else {
      targetPlusPress = false;
    }
  }

  // Switch case to ensure that the variable can only be updated if
  // button is pressed if not, to allow the app to change it
  switch (targetMinusPress) {
    case true:  //Minus button pressed
      if (myTouch.dataAvailable() && !targetPlusPress) {
        targetVal = targetVal - 1;
      }
      break;
  }
  switch (targetPlusPress) {
    case true:  //Minus button pressed
      if (myTouch.dataAvailable() && targetPlusPress) {
        targetVal = targetVal + 1;
      }
      break;
  }
  return targetVal;
}

//===================================================================
// When the plus or minus buttons are pressed the input variable will
// be increased or decreased by TEN
int getSwitchTarget10(int targetVal) {
  // If loop to check for touch input
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY() * (-1) + 240;

    // If we press the minus button the target variable is decreased by 1
    if ((x >= 110) && (x <= 180) && (y >= 190) && (y <= 230)) {
      targetMinusPress = true;
    }
    else {
      targetMinusPress = false;
    }
    // If we press the plus button the target variable is increased by 1
    if ((x >= 200) && (x <= 270) && (y >= 190) && (y <= 230)) {
      targetPlusPress = true;
    }
    else {
      targetPlusPress = false;
    }
  }

  // Switch case to ensure that the variable can only be updated if
  // button is pressed if not, to allow the app to change it
  switch (targetMinusPress) {
    case true:  //Minus button pressed
      if (myTouch.dataAvailable() && !targetPlusPress) {
        targetVal = targetVal - 10;
      }
      break;
  }
  switch (targetPlusPress) {
    case true:  //Minus button pressed
      if (myTouch.dataAvailable() && targetPlusPress) {
        targetVal = targetVal + 10;
      }
      break;
  }
  return targetVal;
}

//===================================================================
// Highlights the button when pressed
void drawFrame(int x1, int y1, int x2, int y2) {
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawRoundRect (x1, y1, x2, y2);
  while (myTouch.dataAvailable())
    myTouch.read();
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (x1, y1, x2, y2);
}
