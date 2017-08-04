#include <dht.h>
#include <Bridge.h>
#include <HttpClient.h>
#define dht_dpin A0 //no ; here. Set equal to channel sensor is on

dht DHT;
int led = 12;                // the pin that the LED is atteched to
int sensor = 2;              // the pin that the sensor is atteched to
int state = LOW;             // by default, no motion detected
int val = 0;                 // variable to store the sensor status (value)
String motion;

void setup(){
  delay(1000);//Let system settle, Wait rest of 1000ms recommended delay before
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH); 
  
  pinMode(led, OUTPUT);      // initalize LED as an output
  pinMode(sensor, INPUT);    // initialize sensor as an input
}//end "setup()"

void loop(){
  //This is the "heart" of the program.
  DHT.read11(dht_dpin);
  delay(500);

  val = digitalRead(sensor);   // read sensor value
  if (val == HIGH) {           // check if the sensor is HIGH
    digitalWrite(led, HIGH);   // turn LED ON
    delay(100);                // delay 100 milliseconds 
    
    if (state == LOW) {
      state = HIGH;       // update variable state to HIGH
      motion = "Yes";
    }
  } 
  else {
      digitalWrite(led, LOW); // turn LED OFF
      delay(200);             // delay 200 milliseconds 
      
      if (state == HIGH){
        state = LOW;       // update variable state to LOW
        motion = "No";
    }
  }
  
  HttpClient client; 
  String dataString;
  dataString += getTimeStamp();
  client.get("http://www.dweet.io/dweet/for/Ignite?Temperature="+String(DHT.temperature)+"&Humidity="+String(DHT.humidity)+"&Motion="+String(motion)+"&TimeStamp="+String(dataString));
// from the server, read response and print at the console:
  int i=0; // for degugging
  delay(1000);
  while (client.available()) {
  char c = client.read();
 }
  delay(1000); 
}// end loop()

String getTimeStamp() {
   String result;
   Process time;
   time.begin("date");
   time.addParameter("+%D+%T");  
   time.run(); 
   while(time.available()>0) {
     char c = time.read();
     if(c != '\n')
       result += c;
   }
   return result;
}
