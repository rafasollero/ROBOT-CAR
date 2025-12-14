#define Trig A0
#define Echo A1
float distance;
float cm;

//Line order test function of trolley wheel
int D1 = 7;//right motor Definition of IO port
int D2 = 5;
int D3 = 10;//left motor Definition of IO port
int D4 = 9;
int speedLeft =180;
int speedRight =180;

//Function initialization
void setup()
{
   Serial.begin(9600);
   pinMode(D1, OUTPUT);
   pinMode(D2, OUTPUT);
   pinMode(D3, OUTPUT);
   pinMode(D4, OUTPUT);
   pinMode(Trig, OUTPUT);
   pinMode(Echo, INPUT);
}

//Principal function
void loop()
{
   cm = GetDistance();
   if(cm > 10 && cm <25){
   Forward();
   }
   else if(cm >25 || cm <5){
     Stop();
   }
   else if(cm <= 10){
    Backward();
   }
}

float GetDistance()
{
    // Send a low short pulse to Trig to trigger the ranging
  digitalWrite(Trig, LOW); //Send a low level to Trig
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  distance = pulseIn(Echo, HIGH) / 58.00;
  Serial.print("Distance = ");
  Serial.println(distance);//The serial output distance is converted into cm
  return distance;
}

void Forward()
{
  digitalWrite(D1, speedRight);
  analogWrite(D2, LOW);
  analogWrite(D4, speedLeft);
}
void Backward()
{
  digitalWrite(D1, LOW);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, LOW);
}
void TurnLeft()
{
  digitalWrite(D1, HIGH);
  digitalWrite(D2, LOW);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, LOW);
}
void TurnRight()
{
  digitalWrite(D1, LOW);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, LOW);
  digitalWrite(D4, HIGH);
}
void Stop()
{
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
}
