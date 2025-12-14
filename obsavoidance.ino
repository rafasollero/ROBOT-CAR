//2023.6.7
int D1 = 7;//right motor Definition of IO port
int D2 = 5;
int D3 = 10;//left motor Definition of IO port
int D4 = 9;

//Definition of ultrasonic pin
#define Trig A0
#define Echo A1

//Data variable definition
int MotSpeed1 = 0;
int MotSpeed2 = 0;
int Speed_adjustment = 120;
int stop_bit = 0;

//Program initialization
void setup()
{
   Serial.begin(9600);
   pinMode(5, OUTPUT);
   pinMode(6, OUTPUT);
   pinMode(9, OUTPUT);
   pinMode(10, OUTPUT);
// Define ultrasonic sensor pins
   pinMode(Trig,OUTPUT);
   pinMode(Echo,INPUT);
}

//Principal function
void loop()
{
   avoidance(18);//In brackets is the obstacle avoidance distance in cm
}

/*
Function: obtain ultrasonic sensor ranging data
Parameters: Trig, Echo
Parameter description: sensor connected to the motherboard pin port A4,A5
Trig --------> pin A4
Echo --------> pin A5
*/
float GetDistance()
{
   float distance;
   digitalWrite(Trig, LOW);
   delayMicroseconds(2);
   digitalWrite(Trig, HIGH);
   delayMicroseconds(10);
   digitalWrite(Trig, LOW);
   distance = pulseIn(Echo, HIGH) / 58.00;
   return distance;//Return distance
}

/*
* Function: Obstacle avoidance
* Parameter: set_dis sets the obstacle avoidance distance
*/

void Forward(int speed1, int speed2)
{
   analogWrite(D1, speed1);
   digitalWrite(D2, LOW);
   digitalWrite(D3, LOW);
   analogWrite(D4, speed2);
}
void Backward(int speed1, int speed2)
{
   digitalWrite(D1, LOW);
   analogWrite(D2, speed1);
   analogWrite(D3, speed2);
   digitalWrite(D4, LOW);
}
void TurnLeft(int speed1, int speed2)
{
   analogWrite(D1, speed1);
   digitalWrite(D2, LOW);
   analogWrite(D3, speed2);
   digitalWrite(D4, LOW);
}
void TurnRight(int speed1, int speed2)
{
   digitalWrite(D1, LOW);
   analogWrite(D2, speed1);
   digitalWrite(D3, LOW);
   analogWrite(D4, speed2);
}
void Stop()
{
   digitalWrite(D1, LOW);
   digitalWrite(D2, LOW);
   digitalWrite(D3, LOW);
   digitalWrite(D4, LOW);
}

//Obstacle avoidance function
void avoidance(int set_dis)
{
   int distance;
   distance = GetDistance();
   Serial.println(distance);//Print distance in serial port
   if(distance < 30)//Judge that the distance is less than 30cm and start to decelerate
   {
     Speed_adjustment--;
     if(Speed_adjustment<=120)Speed_adjustment=120;
     MotSpeed1 = Speed_adjustment+20;//In order to make up for the car's yaw, a value can be added according to the actual situation //as:Speed_adjustment+20
     MotSpeed2 = Speed_adjustment;
   }

  if(distance > 30)//Judge if the distance is more than 30cm and start to accelerate
   {
     Speed_adjustment++;
     if(Speed_adjustment>=235)Speed_adjustment=235;//The maximum limit is 255
     MotSpeed1 = Speed_adjustment+20;//In order to make up for the car's yaw, a value can be added according to the actual situation //as:Speed_adjustment+20
     MotSpeed2 = Speed_adjustment;
   }
  
   if(distance < set_dis)//Judge that the car is less than the obstacle avoidance distance and start to turn backward
   {
     delay(10);
     distance = GetDistance();
     if(distance < set_dis)
     {
       if(stop_bit==0)
       {
         Stop();//stop
         delay(300);
        stop_bit=1;
       }
       Forward(140,140);//The car moves forward
       delay(600);
       TurnLeft(140,140);//The car turns left
       delay(200);
       Speed_adjustment=120;
     }
   }
   else
   {
     Backward(MotSpeed1,MotSpeed2);//If there is no less than the obstacle avoidance distance, move backward
    stop_bit=0;
   }
}
