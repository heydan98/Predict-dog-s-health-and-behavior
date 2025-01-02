// #include "ADC_CalibResult.h"

// int ThermistorPin;
// double adcMax, Vs;

// double R1 = 10000.0;   // voltage divider resistor value
// double Beta = 3950.0;  // Beta value
// double To = 298.15;    // Temperature in Kelvin for 25 degree Celsius
// // double To = 323.15;    // Temperature in Kelvin for 25 degree Celsius
// double Ro = 50000.0;   // Resistance of Thermistor at 25 degree Celsius


// void setup() {
//     Serial.begin(115200);
//     ThermistorPin = 35;
//     adcMax = 4095.0; // ADC resolution 12-bit (0-4095)
//     Vs = 3.3;        // supply voltage
//     analogReadResolution(12);
// }

// void loop() {
//     double Vout, Rt = 0;
//     double T, Tc, Tf = 0;

//     double adc = 0;
//     adc = analogRead(ThermistorPin);
//     adc = ADC_LUT[(int)adc];
//     Vout = adc * Vs/adcMax;
//     Rt = R1 * Vout / (Vs - Vout);

//     T = 1/(1/To + log(Rt/Ro)/Beta) + 4;    // Temperature in Kelvin
//     Tc = T - 273.15;                   // Celsius
//     Tf = Tc * 9 / 5 + 32;              // Fahrenheit
//     Serial.println(Tc);
//     delay(2000);
// }