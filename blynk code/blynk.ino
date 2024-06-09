#define BLYNK_TEMPLATE_ID "TMPL52VSlsBT"
#define BLYNK_DEVICE_NAME "HOME AUTOMATION"
#define BLYNK_AUTH_TOKEN process.env.TOKEN
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <AceButton.h>
#include <Preferences.h> // Include Preferences library
using namespace ace_button;

Preferences preferences; // Create Preferences object

// Define the GPIO connected with Relays
#define RelayPin1 D0    //D0__ It mention pinout on NODEMCU
#define RelayPin2 D1   //D1
#define RelayPin3 D2  //D2
#define RelayPin4 D3 //D3

// Define the GPIO connected with Swtch
#define SwitchPin1 D5    //D5__ It mention pinout on NODEMCU
#define SwitchPin2 D6   //D6 
#define SwitchPin3 D7  //D7
#define SwitchPin4 3  //RX

#define wifiLed   D4   //D4

//Change the virtual pins according the rooms
#define VPIN_BUTTON_1    V1 //For Switch 1 Button on mobile
#define VPIN_BUTTON_2    V2 //For Switch 2 Button on mobile
#define VPIN_BUTTON_3    V3 //For Switch 3 Button on mobile
#define VPIN_BUTTON_4    V4 //For Switch 4 Button on mobile
#define VPIN_BUTTON_5    V5 //For ALL Switch OFF  Button on mobile
#define VPIN_BUTTON_6    V6 //For ALL Switch ON Button on mobile


int toggleState_1 = preferences.getBool("Relay1", true); // Get last state from flash memory
int toggleState_2 = preferences.getBool("Relay2", true); // Get last state from flash memory
int toggleState_3 = preferences.getBool("Relay3", true); // Get last state from flash memory
int toggleState_4 = preferences.getBool("Relay4", true); // Get last state from flash memory

int wifiFlag = 0;

ButtonConfig config1;
AceButton button1(&config1);
ButtonConfig config2;
AceButton button2(&config2);
ButtonConfig config3;
AceButton button3(&config3);
ButtonConfig config4;
AceButton button4(&config4);

void handleEvent1(AceButton*, uint8_t, uint8_t);
void handleEvent2(AceButton*, uint8_t, uint8_t);
void handleEvent3(AceButton*, uint8_t, uint8_t);
void handleEvent4(AceButton*, uint8_t, uint8_t);


#define AUTH process.env.TOKEN   // You should get Auth Token in the Blynk App(or) G-Mail.  
#define WIFI_SSID process.env.SSID           //Enter Wifi SSID(Name)
#define WIFI_PASS process.env.PASS                //Enter wifi Password

BlynkTimer timer;

void all_SwitchOff(){
  toggleState_1 = 1; digitalWrite(RelayPin1, toggleState_1); Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1); preferences.putBool("Relay1", toggleState_1); delay(100);
  toggleState_2 = 1; digitalWrite(RelayPin2, toggleState_2); Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2); preferences.putBool("Relay2", toggleState_2); delay(100);
  toggleState_3 = 1; digitalWrite(RelayPin3, toggleState_3); Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3); preferences.putBool("Relay3", toggleState_3); delay(100);
  toggleState_4 = 1; digitalWrite(RelayPin4, toggleState_4); Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4); preferences.putBool("Relay4", toggleState_4); delay(100);
}

void all_SwitchOn(){
  toggleState_1 = 0; digitalWrite(RelayPin1, toggleState_1); Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1); preferences.putBool("Relay1", toggleState_1); delay(100);
  toggleState_2 = 0; digitalWrite(RelayPin2, toggleState_2); Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2); preferences.putBool("Relay2", toggleState_2); delay(100);
  toggleState_3 = 0; digitalWrite(RelayPin3, toggleState_3); Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3); preferences.putBool("Relay3", toggleState_3); delay(100);
  toggleState_4 = 0; digitalWrite(RelayPin4, toggleState_4); Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4); preferences.putBool("Relay4", toggleState_4); delay(100);
}

BLYNK_WRITE(VPIN_BUTTON_1) {
  toggleState_1 = param.asInt();
  digitalWrite(RelayPin1, toggleState_1);
  preferences.putBool("Relay1", toggleState_1); // Save state to flash memory
}

BLYNK_WRITE(VPIN_BUTTON_2) {
  toggleState_2 = param.asInt();
  digitalWrite(RelayPin2, toggleState_2);
  preferences.putBool("Relay2", toggleState_2); // Save state to flash memory
}

BLYNK_WRITE(VPIN_BUTTON_3) {
  toggleState_3 = param.asInt();
  digitalWrite(RelayPin3, toggleState_3);
  preferences.putBool("Relay3", toggleState_3); // Save state to flash memory
}

BLYNK_WRITE(VPIN_BUTTON_4) {
  toggleState_4 = param.asInt();
  digitalWrite(RelayPin4, toggleState_4);
  preferences.putBool("Relay4", toggleState_4); // Save state to flash memory
}

BLYNK_WRITE(VPIN_BUTTON_5) {
  all_SwitchOff();
}

BLYNK_WRITE(VPIN_BUTTON_6) {
  all_SwitchOn();
}

void checkBlynkStatus() { // called every 3 seconds by SimpleTimer

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    wifiFlag = 1;
    Serial.println("Blynk Not Connected");
  }
  if (isconnected == true) {
    wifiFlag = 0;
    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
    Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
    Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
    Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
    Serial.println("NODEMCU is Connected to Blynk Server ");
  }
}
void setup()
{
  Serial.begin(9600);

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);

  pinMode(wifiLed, OUTPUT);

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);

  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, toggleState_1);
  digitalWrite(RelayPin2, toggleState_2);
  digitalWrite(RelayPin3, toggleState_3);
  digitalWrite(RelayPin4, toggleState_4);

  digitalWrite(wifiLed, HIGH);

  config1.setEventHandler(button1Handler);
  config2.setEventHandler(button2Handler);
  config3.setEventHandler(button3Handler);
  config4.setEventHandler(button4Handler);
  
  button1.init(SwitchPin1);
  button2.init(SwitchPin2);
  button3.init(SwitchPin3);
  button4.init(SwitchPin4);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  timer.setInterval(3000L, checkBlynkStatus); // check if Blynk server is connected every 3 seconds
  Blynk.config(AUTH);
  delay(1000);

  Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
  Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
  Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
  Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
}

void loop()
{  
  if (WiFi.status() != WL_CONNECTED)
  {
    //Serial.println("WiFi Not Connected");
    digitalWrite(wifiLed, HIGH);
  }
  else
  {
    //+Serial.println("WiFi Connected");
    digitalWrite(wifiLed, LOW); //Turn on WiFi LED
    Blynk.run();
  }

  timer.run(); // Initiates SimpleTimer

  button1.check();
  button2.check();
  button3.check();
  button4.check();
}

void button1Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("Switch 1");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("Manual Switch 1 Pressed");
      toggleState_1 = 0;
      Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);   // Update Button Widget
      digitalWrite(RelayPin1, LOW);
      preferences.putBool("Relay1", toggleState_1); // Save state to flash memory
      break;
    case AceButton::kEventReleased:
      Serial.println("Manual Switch 1 Released");
      toggleState_1 = 1;
      Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);   // Update Button Widget
      digitalWrite(RelayPin1, HIGH);
      preferences.putBool("Relay1", toggleState_1); // Save state to flash memory
      break;
  }
}

void button2Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("Switch 2");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("Manual Switch 2 Pressed");
      toggleState_2 = 0;
      Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
      digitalWrite(RelayPin2, LOW);
      preferences.putBool("Relay2", toggleState_2); // Save state to flash memory
      break;
    case AceButton::kEventReleased:
      Serial.println("Manual Switch 2 Released");
      toggleState_2 = 1;
      Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
      digitalWrite(RelayPin2, HIGH);
      preferences.putBool("Relay2", toggleState_2); // Save state to flash memory
      break;
  }
}

void button3Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("Switch 3");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("Manual Switch 3 Pressed");
      toggleState_3 = 0;
      Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
      digitalWrite(RelayPin3, LOW);
      preferences.putBool("Relay3", toggleState_3); // Save state to flash memory
      break;
    case AceButton::kEventReleased:
      Serial.println("Manual Switch 3 Released");
      toggleState_3 = 1;
      Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
      digitalWrite(RelayPin3, HIGH);
      preferences.putBool("Relay3", toggleState_3); // Save state to flash memory
      break;
  }
}

void button4Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("Switch 4");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("Manual Switch 4 Pressed");
      toggleState_4 = 0;
      Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
      digitalWrite(RelayPin4, LOW);
      preferences.putBool("Relay4", toggleState_4); // Save state to flash memory
      break;
    case AceButton::kEventReleased:
      Serial.println("Manual Switch 4 Released");
      toggleState_4 = 1;
      Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
      digitalWrite(RelayPin4, HIGH);
      preferences.putBool("Relay4", toggleState_4); // Save state to flash memory
      break;
  }
}