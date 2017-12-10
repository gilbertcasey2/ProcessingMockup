
/**
 * File: HappyButton_0.ino
 * 
 * THIS BUTTON IS AT LOCATION 0 (The MAGIC Center)
 * 
 * Author: Casey E Gilbert
 * 
 * Credited: Open source Energia code for connecting to thinkspeak:
 * https://github.com/energia/Energia/blob/master/libraries
 * /SimplelinkWifi/examples/ThingSpeakClient/ThingSpeakClient.ino#L100
 * 
 * Created: 14MAR2017
 * 
 * Purpose: This program takes in input from 5 different buttons. If the button
 * input pin reads HIGH , then the button has been pressed. The program records
 * which button has been pressed and adds a new reading to the happybutton channel
 * on thinkspeak. The data entered is the location (a final variable specific to the 
 * program) and which button was pressed. Thinkspeak keeps track of the date that
 * the data was entered and assigns each entry an ID for simple sorting.
 * 
 * The thinkspeak data is made public at https://thingspeak.com/channels/237760 .
 * The data can also be downloaded into an excel spreadsheet or accessed in real time
 * through the use of API commands.
 * 
 * The overarching goal of this program is to collect data on happiness.
 * 
 * */

#include <SPI.h>
#include <ESP8266WiFi.h>

/*
 * Enter all internet connections used below. Simply uncomment the correct
 * internet if it is already listed. Also add passwords under password.
*/
const char* ssid = "RIT-Legacy";
//const char* ssid = "Verizon-MiFi5510L-5578";

//const char* ssid = "time";


/*
 * Variables
 */

int pressCount = 0; // the number of times a button has been pushed (cumulative)
int pressCounts[5]; // the number of times any of the buttons has been pushed

/*
 * All of the button pin set up
 */

const int pinNum = 5; // the number of buttons we have

/*-------------CHANGE ME DEPENDING ON LOCATION ------------------------*/
// We've already used 0 for the SAU so use either 1 or 2
const int location = 0; // which location is this program running

// An array holding all of the input pins recording button data (five total)
const int pins[] = {2,15,4,5,16};
//const int pins[] = {5,16};

// an array holding output pins for the lighting
const int lightPins[] = {0,3,13,12,14};

// an array holding the state of all the buttons (to digital read the pins)
int buttonStates[pinNum]; 

/*
 *  Create the Wifi server and client
 */
WiFiServer server(80);
WiFiClient client;

/*
 *  Think speak settings
 */
char thingSpeakAddress[] = "api.thingspeak.com";  // the address
String writeAPIKey = "9P3XOH30ACU9E8CJ";  // the key to our specific channel

long lastConnectionTime = 0;
boolean lastConnected = false;
int failedCounter = 0;


void setup() {
  Serial.begin(115200);
  delay(10);

 // Get the pin modes ready
 for (int i = 0; i < pinNum; i++) {
  pinMode(pins[i], INPUT);
  Serial.print(pins[i]);
 }
 Serial.println("");

 // Get the pin modes ready
 for (int i = 0; i < pinNum; i++) {
  pinMode(lightPins[i], OUTPUT);
 }
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid);

  // Wait for the wi fi to connect
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");

 // Here we have the IP address to connect to the page online
 // that can print any updates or whatever 
 // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop() {
    // make client
  client = server.available();

  // make sure all the lights are on
  for (int i= 0; i < pinNum; i++) {
    digitalWrite(lightPins[i], HIGH);
  }

  // Build the web page that will give us responses
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<br><br>");
  client.println("<p> How many presses today? </p>");

   // Try to read all of the buttons
  for (int i = 0; i < pinNum; i++) {
    if (digitalRead(pins[i]) == HIGH) {
      buttonStates[i] = HIGH;
      //Serial.println("high");
    }
    else {
      buttonStates[i] = LOW;
    }
  }
  client.println("<p> We tried to read buttons! </p>");

  //int happyLevel = random(1,10);  // for testing purposes DLETE ME

  // Now check the button state, and if it is high, then
  // record in thinkspeak
  String info;
  for (int i = 0; i < pinNum; i++) {
    client.println("<p> Checking for each button high: </p>");

    if (buttonStates[i] == HIGH)  {
      client.println(i);
      Serial.print("Button pressed! ");
      Serial.print(i);
      Serial.println();
      client.println("<p> Before ThingSpeak </p>");

      info = "field1="+(String)i+"&field2="+location; // build string of data to send to thinkspeak
      updateThingSpeak(info); // pass in string
      //client = server.available();
      client.println("<p> Updated ThingSpeak </p>");

      // we'll activate the doLight buttons when we are ready for light fun
      doLights2(i);
      
      pressCounts[i]++;
      Serial.println(pressCount);
      delay(300);
      client.println("<p>After delay </p>");

      digitalWrite(lightPins[i], HIGH);
      client.println("<p>done with  </p>");
      client.println(i);
    }
  }

  // finish off the page
  for (int i = 0; i < pinNum; i++) {
    client.print("<p> Button ");
    client.print(i);
    client.print(": ");
    client.println(pressCounts[i]);
  }

  client.print(" </p>");
  client.print("<br>");
  client.println("</html>");
 

}

/**
 * Name: updateThingSpeak
 * @peram: the string that holds the fields and data to update
 * @purpose: This function connects to thingspeak using appropriate key,
 *           and then posts the string of field data to update the channel.
 * @return: none
 */
void updateThingSpeak(String tsData)
{
  if (client.connect(thingSpeakAddress, 80))
  {     

        // connect to thinkspeak
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    // enter appropriate ky
    client.print("X-THINGSPEAKAPIKEY: "+writeAPIKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");

    // print the data to the client and print it so we can see 
    Serial.println(tsData);
    client.print(tsData);
  }
  else
  {
    // if it doesn't work
    failedCounter++;
    
    Serial.println("Connection to ThingSpeak Failed ("+String(failedCounter, DEC)+")");   
    Serial.println();
    
    lastConnectionTime = millis(); 
  }
  Serial.println("coming out of thinkspeak");

}

/**
 * Name: doLights
 * @peram: int pressed : which button was pressed
 * @return: none
 * 
 * Purpose: This function causes the lights of the 
 * buttons to blink and light up and do their thing
 */
void doLights(int pressed) {
  digitalWrite(lightPins[pressed], LOW);
  delay(2000);
}

/**
 * Name: doLights2
 * @peram: int pressed : which button was pressed
 * @return: none
 * 
 * Purpose: This function causes the lights of the 
 * buttons to blink and light up in a beautiful way
 */
void doLights2(int pressed) {
   Serial.println("in do lights 2");
  // make the light on the button that was pressed blink
  for(int i = 0; i < 2; i++) {
    digitalWrite(lightPins[pressed], LOW);
    delay(300);
    digitalWrite(lightPins[pressed],HIGH);
    delay(300);
  }
  // from left to right sweep the lights out
  for (int i = 0; i < pinNum; i++) {
    digitalWrite(lightPins[i], LOW);
    delay(200);
  }
  // wait 2 seconds with lights off
  delay(2000);
  
  // then sweep lights on from right to left
  for (int i = pinNum-1; i >= 0; i--) {
    digitalWrite(lightPins[i], HIGH);
    delay(200);
  }
}

