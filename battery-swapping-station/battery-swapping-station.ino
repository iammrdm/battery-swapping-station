
#include <LiquidCrystal_I2C.h>
#include  <Wire.h>
#include <Servo.h>

// Button
int switchPin =2; //button is connected to digital pin 2
static int hits = 0;

// variable to hold the value of the switchPin
 int switchState = 0;

// variable to hold previous value of the switchpin
int prevSwitchState = 0;

// Define analog input of Voltage Sensor
#define VOLTAGE_SENSOR_1 A0 
#define VOLTAGE_SENSOR_2 A1
#define VOLTAGE_SENSOR_3 A2

// Define analog input of Current Sensor
#define CURRENT_SENSOR_1 A3
// #define CURRENT_SENSOR_2 A4
// #define CURRENT_SENSOR_3 A5

// LED Indicator
const int LED_BATTERY_RED_1 = 3;
const int LED_BATTERY_YELLOW_1 = 4;
const int LED_BATTERY_GREEN_1 = 5;
const int LED_BATTERY_RED_2 = 6;
const int LED_BATTERY_YELLOW_2 = 7;
const int LED_BATTERY_GREEN_2 = 8;
const int LED_BATTERY_RED_3 = 9;
const int LED_BATTERY_YELLOW_3 = 10;
const int LED_BATTERY_GREEN_3 = 11;


// LiquidCrystal
LiquidCrystal_I2C lcd(0x27,  16, 2);

 
// Floats for ADC voltage & Input voltage
float adc_voltage_1 = 0.0;
float adc_voltage_2 = 0.0;
float adc_voltage_3 = 0.0;
float in_voltage_1 = 0.0;
float in_voltage_2 = 0.0;
float in_voltage_3 = 0.0;

// Floats for ADC Current
int adc_current_1 = 0.0;
int adc_current_2 = 0.0;
int adc_current_3 = 0.0;

// Floats for resistor values in divider (in ohms)
float R1 = 30000.0;
float R2 = 7500.0; 
 
// Float for Reference Voltage
float ref_voltage = 5.0;
 
// Integer for ADC value
int adc_value_1 = 0;
int adc_value_2 = 0;
int adc_value_3 = 0;
 
// Current

float acs_value_1 = 0.0;
float acs_value_2 = 0.0;
float acs_value_3 = 0.0;
float get_samples_1 = 0.0;
float  get_samples_2 = 0.0;
float get_samples_3 = 0.0;
float avg_acs_1 = 0.0;
float avg_acs_2 = 0.0;
float avg_acs_3 = 0.0;
float acs_value_f_1 = 0.0;
float acs_value_f_2 = 0.0;
float acs_value_f_3 = 0.0;
float AcsValue = 0.0;
float Samples = 0.0;
float AvgAcs = 0.0;
float AcsValueF = 0.0;

// Servo 
int ServoPos = 0; // Default Servo position, Meaning door is closed
Servo ServoBattery1;
Servo ServoBattery2;
Servo ServoBattery3;


void setup(){
  // Setup Serial Monitor
  ServoBattery1.attach(12); // Servo pin on 12
  ServoBattery2.attach(13); // Servo pin on 13
  ServoBattery3.attach(14); // Servo pin on 14
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Battery Monitor"); 
}
 
void loop(){
  // Read the Analog Input
  adc_value_1 = analogRead(VOLTAGE_SENSOR_1);
  adc_value_2 = analogRead(VOLTAGE_SENSOR_2);
  adc_value_3 = analogRead(VOLTAGE_SENSOR_3);
  
  // Determine voltage at ADC input
  adc_voltage_1  = (adc_value_1 * ref_voltage) / 1024.0;
  adc_voltage_2  = (adc_value_2 * ref_voltage) / 1024.0;
  adc_voltage_3  = (adc_value_2 * ref_voltage) / 1024.0;
  
  // Calculate voltage at divider input
  in_voltage_1 = adc_voltage_1*(R1+R2)/R2;
  in_voltage_2 = adc_voltage_2*(R1+R2)/R2;
  in_voltage_3 = adc_voltage_3*(R1+R2)/R2;

  // Current sensors
  acs_value_1 = analogRead(CURRENT_SENSOR_1);
  // acs_value_2 = analogRead(CURRENT_SENSOR_2);
  // acs_value_3 = analogRead(CURRENT_SENSOR_3);

  for(int x = 0; x < 150; x++) { // Get 150 samples test
    acs_value_1 = acs_value_1; // Read current sensor value on 1
    acs_value_2 = acs_value_2; // Read current sensor value on 2
    acs_value_3 = acs_value_3; // Read current sensor value on 3
    
    get_samples_1 = Samples + acs_value_1; // Add samples together 1
    get_samples_2 = Samples + acs_value_2; // Add samples together 2
    get_samples_3 = Samples + acs_value_3; // Add samples together 3

    delay(3); // let ADC settle before next sample 3ms
  }

//((avg_acs_1 * (5.0 / 1024.0)) is converitng the read voltage in 0-5 volts
//2.5 is offset(I assumed that arduino is working on 5v so the viout at no current comes
//out to be 2.5 which is out offset. If your arduino is working on different voltage than 
//you must change the offset according to the input voltage)
//0.100v(100mV) is rise in output voltage when 1A current flows at input

  avg_acs_1 = get_samples_1/150.0; // Getting the avg samples
  avg_acs_2 = get_samples_2/150.0;
  avg_acs_3 = get_samples_3/150.0;

  acs_value_f_1 = (2.5 - (avg_acs_1 * (5.0/1024.0)))/0.100;
  acs_value_f_2 = (2.5 - (avg_acs_2 * (5.0/1024.0)))/0.100;
  acs_value_f_3 = (2.5 - (avg_acs_3 * (5.0/1024.0)))/0.100;

  // For Button
  switchState = digitalRead(switchPin);
   Serial.print("switchState:");Serial.println(switchState);
  if (switchState != prevSwitchState) {
    if (switchState == HIGH) {
          hits = hits + 1;
          delay(10);
    }
  }
  
  
  Serial.print("hits:");Serial.println(hits);
  if(hits==1)
  {
    // Current print values
  // Print results to Serial Monitor to 2 decimal places for Voltage
    Serial.print("Input Voltage 1 = ");
    Serial.println(in_voltage_1, 2);
    Serial.print("Current 1 = ");
    Serial.println(acs_value_f_1, 2);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("V1=");
    lcd.setCursor(3,0);
    lcd.print(in_voltage_1, 2);
    lcd.setCursor(8, 0);
    lcd.print("I1=");
    lcd.setCursor(11,0);
    lcd.print(acs_value_f_1, 2);
    lcd.setCursor(0,1);
    lcd.print("Battery: ");
    // if (in_voltage_1 )
    if (in_voltage_1 <= 10.5) { 
      // Servo Door will close at 0% battery
      if ( in_voltage_1 <= 12.42 ) {
        ServoBattery1.write(ServoPos);  
      } else {
        ServoBattery1.write(180);              // tell servo to go to position in variable 'ServoPos', calibrate this
        delay(15);                       // waits 15 ms for the servo to reach the position
      }
      Serial.print("Battery: 0%");
      lcd.setCursor(8,1);
      lcd.print("0%");
      // LED Codes
        digitalWrite(LED_BATTERY_RED_1, HIGH);  
        digitalWrite(LED_BATTERY_YELLOW_1, LOW);   
        digitalWrite(LED_BATTERY_GREEN_1, LOW); 
    } else if (in_voltage_1 >= 10.5 && in_voltage_1 <= 11.31) {
      Serial.print("10%");
      lcd.setCursor(8,1);
      lcd.print("10%");
      // LED Codes
        digitalWrite(LED_BATTERY_RED_1, LOW);  
        digitalWrite(LED_BATTERY_YELLOW_1, HIGH);   
        digitalWrite(LED_BATTERY_GREEN_1, LOW); 
    } else if (in_voltage_1 >= 11.32 && in_voltage_1 <= 11.58) {
      Serial.print("20%");
      lcd.setCursor(8,1);
      lcd.print("20%");
      // LED Codes
        digitalWrite(LED_BATTERY_RED_1, LOW);  
        digitalWrite(LED_BATTERY_YELLOW_1, HIGH);   
        digitalWrite(LED_BATTERY_GREEN_1, LOW); 
    } else if (in_voltage_1 >= 11.59 && in_voltage_1 <= 11.74) {
      Serial.print("30%");
      lcd.setCursor(8,1);
      lcd.print("30%");
      // LED Codes
        digitalWrite(LED_BATTERY_RED_1, LOW);  
        digitalWrite(LED_BATTERY_YELLOW_1, HIGH);   
        digitalWrite(LED_BATTERY_GREEN_1, LOW); 
    } else if (in_voltage_1 >= 11.75 && in_voltage_1 <= 11.89) {
      Serial.print("40%");
      lcd.setCursor(8,1);
      lcd.print("40%");
      // LED Codes
        digitalWrite(LED_BATTERY_RED_1, LOW);  
        digitalWrite(LED_BATTERY_YELLOW_1, HIGH);   
        digitalWrite(LED_BATTERY_GREEN_1, LOW); 
    } else if (in_voltage_1 >= 11.90 && in_voltage_1 <= 12.059) {
      Serial.print("50%");
      lcd.setCursor(8,1);
      lcd.print("50%");
      // LED Codes
        digitalWrite(LED_BATTERY_RED_1, LOW);  
        digitalWrite(LED_BATTERY_YELLOW_1, HIGH);   
        digitalWrite(LED_BATTERY_GREEN_1, LOW); 
    } else if (in_voltage_1 >= 12.06 && in_voltage_1 <= 12.19) {
      Serial.print("60%");
      lcd.setCursor(8,1);
      lcd.print("60%");
      // LED Codes
        digitalWrite(LED_BATTERY_RED_1, LOW);  
        digitalWrite(LED_BATTERY_YELLOW_1, HIGH);   
        digitalWrite(LED_BATTERY_GREEN_1, LOW); 
    } else if (in_voltage_1 >= 12.20 && in_voltage_1 <= 12.31) {
      Serial.print("70%");
      lcd.setCursor(8,1);
      lcd.print("70%");
      // LED Codes
        digitalWrite(LED_BATTERY_RED_1, LOW);  
        digitalWrite(LED_BATTERY_YELLOW_1, HIGH);   
        digitalWrite(LED_BATTERY_GREEN_1, LOW); 
    } else if (in_voltage_1 >= 12.32 && in_voltage_1 <= 12.41) {
      Serial.print("80%");
      lcd.setCursor(8,1);
      lcd.print("80%");
      // LED Codes
        digitalWrite(LED_BATTERY_RED_1, LOW);  
        digitalWrite(LED_BATTERY_YELLOW_1, HIGH);   
        digitalWrite(LED_BATTERY_GREEN_1, LOW); 
    } else if (in_voltage_1 >= 12.42 && in_voltage_1 <= 12.49) {
      Serial.print("80%");
      lcd.setCursor(8,1);
      lcd.print("90%");
      // LED Codes
        digitalWrite(LED_BATTERY_RED_1, LOW);  
        digitalWrite(LED_BATTERY_YELLOW_1, LOW);   
        digitalWrite(LED_BATTERY_GREEN_1, HIGH); 
    } else if (in_voltage_1 >= 12.50) {
      Serial.print("100%");
      lcd.setCursor(8,1);
      lcd.print("100%");
      // LED Codes
        digitalWrite(LED_BATTERY_RED_1, LOW);  
        digitalWrite(LED_BATTERY_YELLOW_1, LOW);   
        digitalWrite(LED_BATTERY_GREEN_1, HIGH); 
      // Servo Codes try it first, if battery is 100% it will automatically open
      for (ServoPos = 0; ServoPos <= 180; ServoPos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
        Serial.print("Battery Full");
        ServoBattery1.write(ServoPos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15 ms for the servo to reach the position
      }
    } else {
      Serial.print("err");
    }
  }
  else if(hits==2)
  {
    // Current print values
  // Print results to Serial Monitor to 2 decimal places for Voltage
    Serial.print("Input Voltage 2 = ");
    Serial.println(in_voltage_2, 2);
    Serial.print("Current 2 = ");
    Serial.println(acs_value_f_2, 2);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("V2=");
    lcd.setCursor(3,0);
    lcd.print(in_voltage_2, 2);
    lcd.setCursor(8, 0);
    lcd.print("I2=");
    lcd.setCursor(11,0);
    lcd.print(acs_value_f_2, 2);
    lcd.setCursor(0,1);
    lcd.print("Battery %: ");
    // if (in_voltage_2 <= 10.5) {
    //   Serial.print("0%");
    // } else if (10.5 => in_voltage_2 <= 11.31) {
    //   Serial.print("gg1");
    // } else {
    //   Serial.print("gg2");
    // }
  }
  else if ( hits==3)
  {
    // Current print values
  // Print results to Serial Monitor to 2 decimal places for Voltage
    Serial.print("Input Voltage 3 = ");
    Serial.println(in_voltage_3, 2);
    Serial.print("Current 3 = ");
    Serial.println(acs_value_f_3, 2);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("V3=");
    lcd.setCursor(3,0);
    lcd.print(in_voltage_3, 2);
    lcd.setCursor(8, 0);
    lcd.print("I3=");
    lcd.setCursor(11,0);
    lcd.print(acs_value_f_3, 2);
    lcd.setCursor(0,1);
    lcd.print("Battery %: ");
  }
  else if ( hits>=4)
  {
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Battery Monitor"); 
       hits = 0;
  }
  Serial.println("...............");  

  // Short delay update it to your liking
  delay(300);
}