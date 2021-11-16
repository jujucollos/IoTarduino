int analogPin = 1; //connect water sensor to analog interface 0
int val = 0; //define the initial value of variable ‘val’ as 0
int data = 0; //define the initial value of variable ‘data’ as 0

void setup(){
Serial.begin(9600);
Serial.println();
pinMode(led, OUTPUT);
}


void loop(){
Serial.print("Moisture Sensor Value:");
val = analogRead(analogPin); //read and assign analog value to variable ’val’ 
data = val; //variable ’val’ assigns value to variable ‘data’
Serial.println(analogRead(0));
Serial.print("Water Level Value:");
Serial.println(data); //print variable ‘data’ by Serial.print

delay(1000);
}
