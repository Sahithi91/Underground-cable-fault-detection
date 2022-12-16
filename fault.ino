#include <LiquidCrystal.h>
LiquidCrystal lcd(2,3,4,5,6,7);
#include <SoftwareSerial.h>
#define sensor A0

#define RX 9
#define TX 10

int read_ADC;
int distance;

byte symbol[8] = {
        B00000,
        B00100,
        B00100,
        B00100,
        B11111,
        B01110,
        B00100,
        B00000
        };

String AP = "elegant";       // AP NAME
String PASS = "smartwork"; // AP PASSWORD
String API = "EDBHIBQE2VMS8GMY";   // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;
  
SoftwareSerial esp8266(RX,TX); 
  
void setup() {
  pinMode(sensor,INPUT); 
lcd.createChar(1, symbol);
    
lcd.begin(16, 2);
lcd.clear();
lcd.setCursor(0, 0); // set the cursor to column 0, line 2
lcd.print("Welcome to Cable");
lcd.setCursor(0, 1); // set the cursor to column 0, line 2
lcd.print("Fault  Detection");
delay(1000);
  Serial.begin(9600);
  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
}

void loop() {
  
 String getData = "GET /update?api_key="+ API +"&field1="+getdata();
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
lcd.clear();
lcd.setCursor(1,0);
lcd.print("R");
lcd.write(1);
delay(500);

getdata();
lcd.setCursor(0,1);
if(distance>0){lcd.print(distance); lcd.print("KM  ");}
else{lcd.print(" NF ");}
delay(500);

}

String getdata(){
read_ADC = analogRead(sensor);  
distance = read_ADC/100;

if(distance>9)distance = 0;

if(distance >= 7)
{
  sms();
delay(200);
 }
 return String(distance);
}

void sms()
{
  Serial.begin(9600); //Baud rate of the GSM/GPRS Module
  Serial.print("\r");
  delay(1000);
  //Serial.print(“AT+CMGF=1\r”);
  Serial.print("AT+CMGF=1\r");
  delay(1000);
 // Serial.print(“AT + CMGS = \”+919948896119\”\r”);
  Serial.print("AT+CMGS=\"+919381595215\"\r");    
  delay(1000);
  Serial.print("Fault detected...pls resolve");   
  delay(1000);
  Serial.write(0x1A);
  delay(1000);
}



void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
