#include <QTRSensors.h>

#define NUM_SENSORS             8
#define NUM_SAMPLES_PER_SENSOR  4
#define EMITTER_PIN             8
QTRSensorsAnalog qtra((unsigned char[]) {0, 1, 2, 3, 4, 5, 6, 7},  NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);

#define Kp 0.1
#define Ki 0
#define Kd 1


int LMp = 2;
int LMne = 3;
int RMpositive = 4;
int RMnegetive = 5;
int LMpwm = 6;
int RMpwm = 7;
int defaultRMspeed = 50;
int defaultLMspeed = 50;
int maxRMspeed = 250;
int maxLMspeed = 250;
int IRVal[NUM_SENSORS];
//int threshold = 400;
int proportional = 0;
int derivative = 0;
int integral = 0;
int error = 0;
int lastError = 0;
int currentError = 0;
int motorspeed = 0;
int meanWeight = 350;

void setup()
{
    Serial.begin(9600);
    /*pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
    pinMode(A6, INPUT);
    pinMode(A7, INPUT);
    pinMode(A8, INPUT);
    pinMode(A9, INPUT);*/
    pinMode(RMpwm, OUTPUT);
    pinMode(LMpwm, OUTPUT);
    pinMode(RMpositive, OUTPUT);
    pinMode(RMnegetive, OUTPUT);
    pinMode(LMpositive, OUTPUT);
    pinMode(LMnegetive, OUTPUT);
    for(int i = 0; i < 9; ++i)
    {
      pinMode(34 + i, OUTPUT);
    }
}

void loop()
{
    //thresholdCalculation();
    //Serial.println(threshold);
    //setmotors(65, -65);
    //PID();
    //Serial.println(analogRead(0));
    //delay(500);
    //delay(1000);
    //errorCalculation();
    //PID();
    //Serial.println(errorCalculation());
    //delay(500);
    qtra.calibrate();
    Serial.println(qtra.readLine(IRVal))
    Serial.print('\t');

}

void autoCalibtrate()                                //Copyright Imran Vai
{
    unsigned int counter;
    for (counter = 0; counter < 80; counter++)
    {
        if (counter <= 20 || counter >= 60)
        {
            setmotors(110, -110);
        }
        else
        {
            setmotors(-110, 110);
        }
        qtra.calibrate();
        delay(2);
    }
}

void thresholdCalculation(void)
{
    int IRValSum = 0;
    for(int i = 0; i < 1000; ++i)
    {
        for(int j = 0; j < 10; ++j)
        {
            IRValSum += readVal(j);
        }
        threshold += (IRValSum / 10);
        if(i) threshold /= 2;
        IRValSum = 0;
    }
}

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
    /*int i = 0;
    int totalSensorValue = 0;
    int weightedValue = 0;
    for (i = 2; i < 10; ++i)
    {
        IRVal[i] = readVal(i);
        //Serial.println(IRVal[i]);
        IRVal[i] = IRVal[i] < threshold;
        //Serial.println(IRVal[i]);
        //delay(500);
        digitalWrite(34 + i, IRVal[i]);
        weightedValue += (i * 100 * IRVal[i]);
        totalSensorValue += IRVal[i];
    }*/
    //int weightedPos = weightedValue / totalSensorValue;
    //Serial.println(weightedPos);
    //delay(500);
    return weightedPos - meanWeight;
}
void setmotors(int RMspeed, int LMspeed)
{
    /*Serial.print("rightspeed - ");
    Serial.println(abs(RMspeed));
    //delay(500);
    digitalWrite(RMpositive, HIGH);
    digitalWrite(RMnegetive, LOW);
    analogWrite(RMpwm, RMspeed);
    Serial.print("leftspeed - ");
    Serial.println(abs(LMspeed));
    //delay(500);
    digitalWrite(LMpositive, HIGH);
    digitalWrite(LMnegetive, LOW);
    analogWrite(LMpwm, LMspeed);*/
    digitalWrite(RMpositive, RMspeed >= 0);
    digitalWrite(RMnegetive, !(RMspeed >= 0));
    analogWrite(RMpwm, RMspeed);
    //Serial.print("rightmotor - ");
    //Serial.println(abs(RMspeed));
    //delay(500);
    digitalWrite(LMpositive, LMspeed >= 0);
    digitalWrite(LMnegetive, !(LMspeed >= 0));
    analogWrite(LMpwm, LMspeed);
    //Serial.print("leftmotor - ");
    //Serial.println(LMspeed);
    //delay(500);
    /*if(RMspeed > 0)
    {
      digitalWrite(RMpositive, HIGH);
      digitalWrite(RMnegetive, LOW);
      analogWrite(RMpwm, RMspeed);
    }
    else if(RMspeed < 0)
    {
      digitalWrite(RMpositive, HIGH);
      digitalWrite(RMnegetive, HIGH);
      analogWrite(RMpwm, 0);
    }
    else
    {
      digitalWrite(RMpositive, HIGH);
      digitalWrite(RMnegetive, HIGH);
      analogWrite(RMpwm, 0);
    }
    if(LMspeed > 0)
    {
      digitalWrite(LMpositive, HIGH);
      digitalWrite(LMnegetive, LOW);
      analogWrite(LMpwm, LMspeed);
    }
    else if(RMspeed < 0)
    {
      digitalWrite(LMpositive, HIGH);
      digitalWrite(LMnegetive, HIGH);
      analogWrite(LMpwm, 0);
    }
    else
    {
      digitalWrite(LMpositive, HIGH);
      digitalWrite(LMnegetive, HIGH);
      analogWrite(LMpwm, 0);
    }*/
}
