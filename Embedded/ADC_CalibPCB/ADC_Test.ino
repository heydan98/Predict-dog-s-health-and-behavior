// #include <Arduino.h>
// #include <driver/dac.h>
// #include "ADC_CalibResult.h"
// #define ADC_PIN 33


// // This example uses a DAC output (pin 25) as a source and feed into ADC (pin 35)
// // The calibrated value of the ADC is generated through LUT based on raw reading from the ADC

// void setup() {
//   dac_output_enable(DAC_CHANNEL_1);        // Enable DAC on pin 25
//   dac_output_voltage(DAC_CHANNEL_1, 0);    // Setup output voltage to 0
//   analogReadResolution(12);
//   Serial.begin(112500);
//   while (!Serial) {}
// }

// void loop() {
//   for (int i=1; i<255; i++) {
//     dac_output_voltage(DAC_CHANNEL_1, i);    // DAC output (8-bit resolution)
//     delayMicroseconds(100);
//     int rawReading = analogRead(ADC_PIN);                          // read value from ADC
//     int calibratedReading = (int)ADC_LUT[rawReading];    // get the calibrated value from LUT

//     // Run Serial Plotter to see the results
//     Serial.print(F("DAC = "));
//     Serial.print(i*16);
//     Serial.print(F(" rawReading = "));
//     Serial.print(rawReading);
//     Serial.print(F(" calibratedReading = "));
//     Serial.println(calibratedReading);
//     delay(10);
//   }
//   delay(10000);

// }