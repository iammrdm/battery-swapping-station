// Define analog input of Voltage Sensor
#define VOLTAGE_SENSOR_1 A0 
#define VOLTAGE_SENSOR_2 A1
#define VOLTAGE_SENSOR_3 A2

// Define analog input of Current Sensor
#define CURRENT_SENSOR_1 A3
#define CURRENT_SENSOR_2 A4
#define CURRENT_SENSOR_3 A5

// Define float for current sensor

 
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

  // Current sensors
  acs_value_1 = analogRead(CURRENT_SENSOR_1);
  acs_value_2 = analogRead(CURRENT_SENSOR_2);
  acs_value_3 = analogRead(CURRENT_SENSOR_3);

  for(int x = 0; x < 150; x++) { // Get 150 samples test
    acs_value_1 = acs_value_1; // Read current sensor value on 1
    acs_value_2 = acs_value_2; // Read current sensor value on 2
    acs_value_3 = acs_value_3; // Read current sensor value on 3
    
    get_samples_1 = Samples + acs_value_1; // Add samples together 1
    get_samples_2 = Samples + acs_value_2; // Add samples together 2
    get_samples_3 = Samples + acs_value_3; // Add samples together 3

    delay(3); // let ADC settle before next sample 3ms
  }

//((AvgAcs * (5.0 / 1024.0)) is converitng the read voltage in 0-5 volts
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

  // Current print values
  Serial.print("Current 1 = ");
  Serial.println(acs_value_f_1, 2);
  Serial.print("Current 2 = ");
  Serial.println(acs_value_f_2, 2);
  Serial.print("Current 3 = ");
  Serial.println(acs_value_f_3, 2);
  
  // Print results to Serial Monitor to 2 decimal places for Voltage
  Serial.print("Input Voltage 1 = ");
  Serial.println(in_voltage_1, 2);

  Serial.print("Input Voltage 2 = ");
  Serial.println(in_voltage_2, 2);

  Serial.print("Input Voltage 3 = ");
  Serial.println(in_voltage_3, 2);

  // Short delay update it to your liking
  delay(1500);
}