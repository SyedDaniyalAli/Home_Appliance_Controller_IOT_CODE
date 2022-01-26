#define fanActuator D1
#define lightActuator D2
#define light2Actuator D3
#define fan2Actuator D4

#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>


// Provide the token generation process info.
#include "addons/TokenHelper.h"

// Insert Firebase project API Key
#define API_KEY "AIzaSyD-Ut9L9v-JAnKw8O353uMqBvnyU-TyISo"

/* 3. Define the project ID */
#define FIREBASE_PROJECT_ID "home-appliance-controlle-75983"

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "USER_EMAIL"
#define USER_PASSWORD "USER_PASSWORD"


#define WIFI_SSID "DreamNetSDA" // enter the wifi address
#define WIFI_PASSWORD "Daniyal444" // enter it's password

String DEVICE1_ID = "123456"; //Unique identity of machine
String DEVICE2_ID = "123457"; //Unique identity of machine
String DEVICE3_ID = "123458"; //Unique identity of machine
String DEVICE4_ID = "123459"; //Unique identity of machine

bool DEVICE1_STATE = false;
bool DEVICE2_STATE = false;
bool DEVICE3_STATE = false;
bool DEVICE4_STATE = false;


// Define Firebase objects~~~~~~~~~~~~~~
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;


FirebaseJson json;
FirebaseJsonData jsonData;


bool signupOK = false;



// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;

// constants won't change:
const long intervalForGettingDataFromFirebase = 5000;           // how long grinder may work (milliseconds)


bool isDataUpdated =  false; // For first time load of data


// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}



void setup() {

  Serial.begin(9600);

  pinMode(fanActuator, OUTPUT);
  pinMode(lightActuator, OUTPUT);
  pinMode(light2Actuator, OUTPUT);
  pinMode(fan2Actuator, OUTPUT);


  digitalWrite(fanActuator, HIGH);
  digitalWrite(lightActuator, HIGH);
  digitalWrite(light2Actuator, HIGH);
  digitalWrite(fan2Actuator, HIGH);

  // Initialize WiFi
  initWiFi();

  //Firebase ~~~~~~~~~~~~~~~~~START~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;


  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Signed up (ok)");
    signupOK = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  //Firebase ~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~~~~~~~~~~~

}

// Loop Started~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void loop() {

  // check to see if it's time to on the grinder; that is, if the difference
  // between the current time and last time you turned on the machine is bigger than
  // the interval at which you want to turn on the grinder.
  unsigned long currentMillis = millis();


  // you can adjust the delay time from here~~~~~~~~~~~~~~~~~~~~~~~~
  if (currentMillis - previousMillis > 2000) {

    previousMillis = currentMillis;

    isDataUpdated = getDataFromFirebase();
    digitalWrite(fanActuator, !DEVICE1_STATE);
    digitalWrite(lightActuator, !DEVICE2_STATE);
    digitalWrite(light2Actuator, !DEVICE3_STATE);
    digitalWrite(fan2Actuator, !DEVICE4_STATE);

  }

}
// Loop Ended~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


bool getDataFromFirebase() {

  String documentPath1 = "machines/" + DEVICE1_ID;
  String documentPath2 = "machines/" + DEVICE2_ID;
  String documentPath3 = "machines/" + DEVICE3_ID;
  String documentPath4 = "machines/" + DEVICE4_ID;

  Serial.print("Get a document... ");

  //  For Device 1~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath1.c_str()))
  {

    //Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
    json.setJsonData(fbdo.payload());
    json.get(jsonData, "fields/state/booleanValue");
    //    Serial.println(jsonData.type);
    Serial.print("Device 1: ");
    Serial.println(jsonData.boolValue);
    DEVICE1_STATE = jsonData.boolValue;
    json.clear();

    //  For Device 2~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath2.c_str());
    //Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
    json.setJsonData(fbdo.payload());
    json.get(jsonData, "fields/state/booleanValue");
    //    Serial.println(jsonData.type);
    Serial.print("Device 2: ");
    Serial.println(jsonData.boolValue);
    DEVICE2_STATE = jsonData.boolValue;
    json.clear();


    //  For Device 3~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath3.c_str());
    //Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
    json.setJsonData(fbdo.payload());
    json.get(jsonData, "fields/state/booleanValue");
    //    Serial.println(jsonData.type);
    Serial.print("Device 3: ");
    Serial.println(jsonData.boolValue);
    DEVICE3_STATE = jsonData.boolValue;
    json.clear();

    //  For Device 4~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath4.c_str());
    //Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
    json.setJsonData(fbdo.payload());
    json.get(jsonData, "fields/state/booleanValue");
    //    Serial.println(jsonData.type);
    Serial.print("Device 4: ");
    Serial.println(jsonData.boolValue);
    DEVICE4_STATE = jsonData.boolValue;
    json.clear();

    return true;

  }
  else
  {
    Serial.println(fbdo.errorReason());
    return false;
  }
}
