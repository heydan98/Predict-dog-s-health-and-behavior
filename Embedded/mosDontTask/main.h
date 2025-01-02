#define ThermistorPin 35
#define readPin 34
double adcMax = 4095.0; // ADC resolution 12-bit (0-4095)
double Vs = 3.3;        // supply voltage
float battery_percent;
// float Tc;
#define BUFFER_LEN 600
long lastMsg = 0;
char msg[BUFFER_LEN];
uint64_t chipId;
unsigned int plength = 500;
char timestamp[64];
float timestamp_sort;

float latDf = 10.852333;
float longtitudeDf = 106.4623152;
float m;
float *lat = &latDf;
float *longtitude = &longtitudeDf;
long now = 0;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 1000;
String modeSelect = "";
String modeWifiSelect = "";
float id = 1;
String SSID_Mosquitto = "";
String PASSWORD_Mosquitto = "";
String mqtt_server_s = "";
String mqtt_topic_s = "";
// String mqtt_username_s = "heydan";
// String mqtt_password_s = "1";

unsigned long previousMillis_data = 0;
String SSID_AWS = "";
String PASSWORD_AWS = "";
String SIM4G = "";
String weight = "";

String receive_AT;
String response = "";
bool isModuleSIM = false;
bool DEBUG = true;
#define PIN_PWRKEY 5
#define RXD2 19 // SIM4G
#define TXD2 18
int count = 0;
String data_4G;

const char *mqtt_server = "172.28.31.201"; // Replace with the actual IP address of your MQTT broker
const char *mqtt_topic = "esp32/test";
const char *mqtt_username = "heydan";
const char *mqtt_password = "1";
// const char *cStringTopic ;

// const int mqttPort = 1883;
// strlen(data_.c_str()
unsigned long startTime = 0;
/////////// Thermistor/////////////////
double R1 = 10000.0;  // voltage divider resistor value
double Beta = 3950.0; // Beta value
double To = 298.15;   // Temperature in Kelvin for 25 degree Celsius
// double To = 323.15;    // Temperature in Kelvin for 25 degree Celsius
double Ro = 50000.0; // Resistance of Thermistor at 25 degree Celsius

// // float calib_factors[6][2] = {
// //     {0.99741949, -0.06529007},
// //     {1.00329363, -0.01231215},
// //     {0.97072881, -0.13793615},
// //     {1, -6.870460510253906},
// //     {1, -0.7690048217773438},
// //     {1, -0.6298065185546875}};

// // float calibrated_data[6] = {0, 0, 0, 0, 0, 0};

// // unsigned long lastDebounceTime = 0;
// // Function to calculate the calibrated value
// float calibrated_value(float input, float m, float b)
// {
//   return input * m + b;
// }

float ax1, ay1, az1, wx1, wy1, wz1;
float ax2, ay2, az2, wx2, wy2, wz2;
float ax1_filtered, ay1_filtered, az1_filtered, gx1_filtered, gy1_filtered, gz1_filtered;
float ax2_filtered, ay2_filtered, az2_filtered, gx2_filtered, gy2_filtered, gz2_filtered;



// float a_x1, a_y1, a_z1, w_x1, w_y1, w_z1;
// float a_x2, a_y2, a_z2, w_x2, w_y2, w_z2;


float alpha = 0.98;
// Previous raw sensor data
float ax1_prev, ay1_prev, az1_prev;
float gx1_prev, gy1_prev, gz1_prev;
float ax2_prev, ay2_prev, az2_prev;
float gx2_prev, gy2_prev, gz2_prev;
// float gx_prev, gy_prev, gz_prev;

// Previous filtered sensor data
float ax1_filtered_prev, ay1_filtered_prev, az1_filtered_prev;
float ax2_filtered_prev, ay2_filtered_prev, az2_filtered_prev;
float gx2_filtered_prev, gy2_filtered_prev, gz2_filtered_prev;
float gx1_filtered_prev, gy1_filtered_prev, gz1_filtered_prev;


// float *sensor_data_6050[6] = {&a_x1, &a_y1, &a_z1, &w_x1, &w_y1, &w_z1};
// float *sensor_data_9255[6] = {&a_x2, &a_y2, &a_z2, &w_x2, &w_y2, &w_z2};

// float *filtered_sensor_data_6050[6] = {&ax1_filtered, &ax1_filtered, &ax1_filtered, &gx1_filtered, &gy1_filtered, &gz1_filtered};
// float *filtered_sensor_data_9255[6] = {&ax2_filtered, &ax2_filtered, &ax2_filtered, &gx2_filtered, &gy2_filtered, &gz2_filtered};
const byte DNS_PORT = 53;
// const int buttonPin = 0;
#define buttonPin 4

bool buttonState = true;
bool webServerEnabled = false;
bool start_init = false;
bool dataSent = false;
boolean State;
bool send4Gdata = false;
bool sendWifidata = false;
#define PIN_ANALOG 34