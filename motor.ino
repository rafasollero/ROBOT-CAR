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
}

//Principal function
void loop()
{
   Forward();//Car forward
   delay(2000);
   Stop();//Car stop
   delay(500);//delayed

   Backward();//Car back
   delay(2000);
   Stop();//Car stop
   delay(500);//delayed

   TurnLeft();//Car turnleft
   delay(2000);
   Stop();//Car stop
   delay(500);//delayed

   TurnRight();//Car turnright
   delay(2000); 
   Stop();//Car stop
   delay(500);//delayed
}

void Forward()
{
   analogWrite(D1, speedRight);
   digitalWrite(D2, LOW);
   digitalWrite(D3, LOW);
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


