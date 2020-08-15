#define leftMotor1 1
#define leftMotor2 2
#define rightMotor1 3
#define rightMotor2 4
#define RMpwm 8
#define LMpwm 7
#define sensorCount 10
#define Kp .6
#define Ki 0
#define Kd 0
#define defaultRMspeed 100
#define defaultLMspeed 100
#define maxRMspeed 250
#define maxLMspeed 250

int ir[10];
int threshold = 111;
int proportional = 0;
int derivative = 0;
int integral = 0;
int error = 0;
int last_error = 0;
int current_error = 0;
int desired_deviation = 0;
int motorspeed;
int meanWeight = 4500;

void setup()
{
    Serial.begin(9600);
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
    pinMode(A6, INPUT);
    pinMode(A7, INPUT);
    pinMode(A8, INPUT);
    pinMode(A9, INPUT);
    pinMode(leftMotor1, OUTPUT);
    pinMode(leftMotor2, OUTPUT);
    pinMode(rightMotor1, OUTPUT);
    pinMode(rightMotor2, OUTPUT);
    pinMode(LMpwm, OUTPUT);
    pinMode(RMpwm, OUTPUT);
}

void loop()
{
    //thresholdCalc();
    //Serial.println(threshold);
    //setmotors(250, 250);
    //PID();
    for(int i = 0; i < 10; ++i)
    {
      int x = analogRead(i);
      Serial.println(x);
      delay(200);
    }
}

void thresholdCalc()
{
    int sensorValSum = 0;
    for(int i = 0; i < 1000; ++i)
    {
        for(int j = 0; j < 10; ++j)
        {
            sensorValSum += analogRead(i);
        }
        threshold += (sensorValSum / 10);
        if(i) threshold /= 2;
        sensorValSum = 0;
    }
}

void PID()
{
    current_error = error_calculation();
    //Serial.println(current_error);
    //delay(500);
    error = current_error - meanWeight;
    //Serial.print("Error - ");
    //Serial.println(error);
    proportional = error;
    derivative = error - last_error;
    //Serial.print("Derivative - ");
    //Serial.println(derivative);
    integral += error;
    //Serial.print("Integral - ");
    //Serial.println(integral);
    motorspeed = proportional * Kp + derivative * Kd + integral * Ki;
    //Serial.print("Motorspeed - ");
    //Serial.println(motorspeed);
    last_error = error;
    setmotors(defaultRMspeed + motorspeed, defaultLMspeed - motorspeed);
}

int error_calculation()
{
    int i = 0;
    int totalSensorValue = 0;
    int weightedValue = 0;
    for (i = 0; i < 10; ++i)
    {
        ir[i] = analogRead(i);
        Serial.println(ir[i]);
        ir[i] = ir[i] < threshold;
        //Serial.println(ir[i]);
        delay(500);
        weightedValue += (i * 1000 * ir[i]);
        totalSensorValue += ir[i];
    }
    int weightedPos = weightedValue / totalSensorValue;
    return weightedPos;
}
void setmotors(int RMspeed, int LMspeed)
{
    digitalWrite(leftMotor1, LOW);
    digitalWrite(leftMotor2, HIGH);
    analogWrite(LMpwm, LMspeed);
    digitalWrite(rightMotor1, LOW);
    digitalWrite(rightMotor2, HIGH);
    analogWrite(RMpwm, RMspeed);
}


