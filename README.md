# ProcessingMockup
  

  Author: Casey E Gilbert
 
  Credited: Open source Energia code for connecting to thinkspeak:
  https://github.com/energia/Energia/blob/master/libraries
  /SimplelinkWifi/examples/ThingSpeakClient/ThingSpeakClient.ino#L100
  
  Created: 14MAR2017
  
  Purpose: This program takes in input from 5 different buttons. If the button
  input pin reads HIGH , then the button has been pressed. The program records
  which button has been pressed and adds a new reading to the happybutton channel
  on thinkspeak. The data entered is the location (a final variable specific to the 
  program) and which button was pressed. Thinkspeak keeps track of the date that
  the data was entered and assigns each entry an ID for simple sorting.
  
  The thinkspeak data is made public at https://thingspeak.com/channels/237760 .
  The data can also be downloaded into an excel spreadsheet or accessed in real time
  through the use of API commands.
  
  The overarching goal of this program is to collect data on happiness.
