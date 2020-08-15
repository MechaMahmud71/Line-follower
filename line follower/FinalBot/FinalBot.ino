#define Kp .08   //.156 
#define Ki 0
#define Kd .18   //.18

int LMpositive = 2;
int LMnegetive = 3;
int RMpositive = 4;
int RMnegetive = 5;
int LMpwm = 6;
int RMpwm = 7;
int sensorCount = 10;
int defaultRMspeed = 60;
int defaultLMspeed = 63;
int maxRMspeed = 250;
int maxLMspeed = 250;
int IRVal[10];
int LastIRVal[10];
int threshold = 400;
int proportional = 0;
int derivative = 0;
int integral = 0;
int error = 0;
int lastError = 0;
int currentError = 0;
int motorspeed = 0;
int meanWeight = 350;
int rightcounter = 0;
int leftcounter =0;
int zig = 0;

void setup()
{
    Serial.begin(9600);
    //pinMode(A0, INPUT);
    //pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
    pinMode(A6, INPUT);
    pinMode(A7, INPUT);
    pinMode(A8, INPUT);
    pinMode(A9, INPUT);
    pinMode(RMpwm, OUTPUT);
    pinMode(LMpwm, OUTPUT);
    pinMode(RMpositive, OUTPUT);
    pinMode(RMnegetive, OUTPUT);
    pinMode(LMpositive, OUTPUT);
    pinMode(LMnegetive, OUTPUT);
    /*setmotors(70, 40);
    delay(500);
    setmotors(40, 80);
    delay(1000);
    setmotors(0, 0);*/
}

void loop()
{
    //thresholdCalculation();
    //Serial.println(threshold);
    //setmotors(150, 150);
    //delay(20);
    //setmotors(0, 0);
    //PID();
    //Serial.println(analogRead(0));
    //delay(500);
    /*for(int i = 2; i < 10; ++i)
    {
        int x = analogRead(i);
        Serial.print(x < threshold);
    }
    Serial.println(" ");*/
    //delay(200);
    //errorCalculation();
    PID();
    //Serial.println(errorCalculation());
    //delay(500);
}

/*void thresholdCalculation(void)
{
    int IRValSum = 0;
    for(int i = 0; i < 1000; ++i)
    {
        for(int j = 0; j < 10; ++j)
        {
            IRValSum += analogRead(j);
        }
        threshold += (IRValSum / 10);
        if(i) threshold /= 2;
        IRValSum = 0;
    }
}*/

void PID(void)
{
    currentError = errorCalculation();
    //Serial.println(currentError);
    //delay(500);
    //Serial.print("Error - ");
    //Serial.println(error);
    proportional = currentError;
    //Serial.print("proportional - ");
    //Serial.println(proportional);
    //delay(200);
    derivative = currentError - lastError;
    //Serial.print("Derivative - ");
    //Serial.println(derivative);
    integral += currentError;
    //Serial.print("Integral - ");
    //Serial.println(integral);
    motorspeed = proportional * Kp + derivative * Kd + integral * Ki;
    //Serial.print("motorspeed - ");
    //Serial.println(motorspeed);
    //delay(500);
    //Serial.print("Motorspeed - ");
    //Serial.println(motorspeed);
    lastError = currentError;
    setmotors(defaultRMspeed + motorspeed, defaultLMspeed - motorspeed);
}

int errorCalculation(void)
{
    int totalSensorValue = 0;
    int weightedValue = 0;
    for (int i = 2; i < 10; ++i)
    {
        IRVal[i] = analogRead(i);
        //Serial.println(IRVal[i]);
        IRVal[i] = IRVal[i] < threshold;
        //Serial.println(IRVal[i]);
        //delay(500);
        weightedValue += ((i - 2) * 100 * IRVal[i]);
        totalSensorValue += IRVal[i];
    }
    if(totalSensorValue != 0)
    {
      for(int i = 2; i < 10; ++i)
      {
        LastIRVal[i] = IRVal[i];
      }
    }
    else
    {
      for(int i = 2; i < 10; ++i)
      {
        weightedValue += ((i - 2) * 100 * LastIRVal[i]);
        totalSensorValue += LastIRVal[i];
      }
      int weightedPos = weightedValue / totalSensorValue;
      if(rightcounter > 5 && rightcounter < 15)
      {
        setmotors(70, -70);
        delay(50);
        rightcounter++;
      }
      else if(weightedPos < 350)
      {
        setmotors(-70, 70);
        rightcounter++;
        delay(50);
      }
      else if(weightedPos > 350)
      {
        setmotors(70, -70);
        delay(50);
      }
      else
      {
        if(leftcounter >= 0 && leftcounter <= 10)
        {
          setmotors(0, 70);
          delay(50);
          leftcounter++;  
        }
        else if(leftcounter >= 0 && leftcounter <= 15)
        {
          setmotors(70, 0);
          delay(50);
          leftcounter += 1000;
        }
        else if(leftcounter >= 0)
        {
          setmotors(0, 70);
          delay(70);
          leftcounter = -9999999;
        }
      }
      //Serial.println(rightcounter);
    }
    int weightedPos = weightedValue / totalSensorValue;
    return weightedPos - meanWeight;
}
void setmotors(int RMspeed, int LMspeed)
{
    /*digitalWrite(RMpositive, HIGH);
    digitalWrite(RMnegetive, LOW);
    analogWrite(RMpwm, RMspeed);
    digitalWrite(LMpositive,HIGH);
    digitalWrite(LMnegetive,LOW);
    analogWrite(LMpwm, LMspeed);*/
    
    
    /*Serial.print("rightspeed - ");
    Serial.println(abs(RMspeed));
    //delay(500);
    digitalWrite(RMpositive, HIGH);
    digitalWrite(RMnegetive, LOW);
    analogWrite(RMpwm, abs(RMspeed));
    Serial.print("leftspeed - ");
    Serial.println(abs(LMspeed));
    //delay(500);
    digitalWrite(LMpositive, HIGH);
    digitalWrite(LMnegetive, LOW);
    analogWrite(LMpwm, abs(LMspeed));*/
    digitalWrite(RMpositive, RMspeed >= 0);
    digitalWrite(RMnegetive, !(RMspeed >= 0));
    analogWrite(RMpwm, RMspeed);
    //Serial.print("rightmotor - ");
    //Serial.println(RMspeed);
    //delay(500);
    digitalWrite(LMpositive, LMspeed >= 0);
    digitalWrite(LMnegetive, !(LMspeed >= 0));
    analogWrite(LMpwm, LMspeed);
    //Serial.print("leftmotor - ");
    //Serial.println(LMspeed);
    //delay(500);
}
