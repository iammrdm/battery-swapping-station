// Define analog input of Voltage Sensor
#define VOLTAGE_SENSOR_1 A0 
#define VOLTAGE_SENSOR_2 A1
#define VOLTAGE_SENSOR_3 A2

// Define analog input of Current Sensor
 
// Floats for ADC voltage & Input voltage
float adc_voltage = 0.0;
float in_voltage = 0.0;
 
// Floats for resistor values in divider (in ohms)
float R1 = 30000.0;
float R2 = 7500.0; 
 
// Float for Reference Voltage
float ref_voltage = 5.0;
 
// Integer for ADC value
int adc_value_1 = 0;
int adc_value_2 = 0;
int adc_value_3 = 0;
 
void setup(){
  // Setup Serial Monitor
  Serial.begin(9600);
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
  
  // Print results to Serial Monitor to 2 decimal places
  Serial.print("Input Voltage 1 = ");
  Serial.println(in_voltage_1, 2);

  Serial.print("Input Voltage 2 = ");
  Serial.println(in_voltage_2, 2);
  
  Serial.print("Input Voltage 3 = ");
  Serial.println(in_voltage_3, 2);
  
  // Short delay
  delay(500);
}