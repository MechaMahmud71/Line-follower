#include <SoftwareSerial.h>
#include <QTRSensors.h>

#define rightMotor1 6
#define rightMotor2 7
#define rightMotorPWM 9
#define leftMotor1 4
#define leftMotor2 5
#define leftMotorPWM 10
#define Kp .6
#define Ki 0
#define Kd 6
#define Kpw .6
#define Kiw 0
#define Kdw 6
#define Kpb .66
#define Kib 0
#define Kdb 6
#define M1_default_speed 100
#define M2_default_speed 100
#define M1_default_speedw 150
#define M2_default_speedw 150
#define M1_max_speed 255
#define M2_max_speed 255
#define NUM_SENSORS 8
#define EMITTER_PIN 37
#define NUM_SAMPLES_PER_SENSOR 2
#define leapTime 20
#define sen1 A8
#define sen2 A9

QTRSensorsAnalog qtra((unsigned char[]){7, 6, 5, 4, 3, 2, 1, 0}, NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);
SoftwareSerial mySerial(50, 51);

unsigned int sensors[NUM_SENSORS];
unsigned int sensor[NUM_SENSORS];
int identifier;
int M1_default_speedb = 220;
int M2_default_speedb = 220;

void setup()
{
    pinMode(rightMotor1, OUTPUT);
    pinMode(rightMotor2, OUTPUT);
    pinMode(rightMotorPWM, OUTPUT);
    pinMode(leftMotor1, OUTPUT);
    pinMode(leftMotor2, OUTPUT);
    pinMode(leftMotorPWM, OUTPUT);
    pinMode(sen1, INPUT);
    pinMode(sen2, INPUT);
    pinMode(A10, INPUT);
    initializeLeds();
    Auto_calibration();
    set_motors(0, 0);
    delay(2000);
    Serial.begin(115200);
    mySerial.begin(9600);
    identifier = (analogRead(A10) - 120);
    //Serial.println(identifier);
}

void initializeLeds()
{
    int i;
    for (i = 0; i <= 7; i++)
    {
        int pin = 22 + i;
        pinMode(pin, OUTPUT);
    }
}

int error = 0;
int lastError = 0;
int proportional = 0;
int derivative = 0;
int integral = 0;
int position;
int error1 = 0;
int lastError1 = 0;
int proportional1 = 0;
int derivative1 = 0;
int integral1 = 0;
int position1;
int errorb = 0;
int lastErrorb = 0;
int proportionalb = 0;
int derivativeb = 0;
int integralb = 0;
int positionb;
int errorw = 0;
int lastErrorw = 0;
int proportionalw = 0;
int derivativew = 0;
int integralw = 0;
int positionw;
int j = 0;
int x = 0;
int serialData;

void loop()
{

    if (mySerial.available())
    {
        serialData = mySerial.parseInt();
        //Serial.println(serialData);
        if (serialData == 1256)
        {
            line_follow();
        }
        else if (serialData == 1257)
        {
            turn_left0();
        }
        else if (serialData == 1258)
        {
            turn_right0();
        }
        else if (serialData == 1259)
        {
            lift_box();
        }
        else if (serialData == 1260)
        {
            turn_right0();
        }
        else
        {
            runInCave();
        }
    }
    else
    {
        line_follow();
    }
}

void line_follow()
{

    //Serial.println(analogRead(A9));
    //if(analogRead(A10)>identifier)
    //{

    sensor_reading_black();
    if ((sensors[2] > 4 || sensors[3] > 4 || sensors[4] > 4 || sensors[5] > 4) && sensors[0] < 4 && sensors[7] < 4)
    {
        position = sensor_reading_black();
        //Serial.println(position);
        error = position - 350;

        //Serial.println(error);
        proportional = error;
        derivative = error - lastError;
        integral += proportional;

        //Serial.print(proportional);
        //Serial.print("  ");
        //Serial.print(derivative);
        //Serial.print("  ");
        if (integral > 255)
        {
            integral = 255;
        }
        if (integral < -255)
        {
            integral = -255;
        }

        int motorSpeed = proportional * Kp + derivative * Kd + integral * Ki;

        //Serial.print(motorSpeed);
        //Serial.println("");
        lastError = error;

        int leftMotorSpeed;
        int rightMotorSpeed;

        leftMotorSpeed = M1_default_speed + motorSpeed;
        rightMotorSpeed = M2_default_speed - motorSpeed;

        set_motors(leftMotorSpeed, rightMotorSpeed);
    }

    else if ((sensors[2] < 4 || sensors[3] < 4 || sensors[4] < 4 || sensors[5] < 4) && sensors[0] > 4 && sensors[7] > 4)
    {
        position1 = sensor_reading_White();
        error1 = position1 - 350;
        proportional1 = error1;
        derivative1 = error1 - lastError1;
        integral1 += proportional1;
        if (integral1 > 255)
        {
            integral1 = 255;
        }
        if (integral1 < -255)
        {
            integral1 = -255;
        }
        int motorSpeed1 = proportional1 * Kp + derivative1 * Kd + integral1 * Ki;
        lastError1 = error1;
        int leftMotorSpeed1;
        int rightMotorSpeed1;

        leftMotorSpeed1 = M1_default_speed + motorSpeed1;
        rightMotorSpeed1 = M2_default_speed - motorSpeed1;

        set_motors(leftMotorSpeed1, rightMotorSpeed1);
    }
    else
    {
        if (analogRead(sen1) > 500 || analogRead(sen2) > 500)
        {
            turn();
        }
        else
        {
            turn2();
        }
    }
}

void set_motors(int motor1speed, int motor2speed)
{
    if (motor1speed > M1_max_speed)
    {
        motor1speed = M1_max_speed;
    }
    if (motor2speed > M2_max_speed)
    {
        motor2speed = M2_max_speed;
    }
    if (motor1speed < (-M1_max_speed))
    {
        motor1speed = -M1_max_speed;
    }
    if (motor2speed < (-M2_max_speed))
    {
        motor2speed = -M2_max_speed;
    }
    if (motor1speed >= 0 && motor2speed >= 0)
    {
        digitalWrite(leftMotor1, HIGH);
        digitalWrite(leftMotor2, LOW);
        analogWrite(leftMotorPWM, motor1speed);
        digitalWrite(rightMotor1, HIGH);
        digitalWrite(rightMotor2, LOW);
        analogWrite(rightMotorPWM, motor2speed);
    }
    if (motor1speed < 0 && motor2speed < 0)
    {
        digitalWrite(leftMotor1, LOW);
        digitalWrite(leftMotor2, HIGH);
        analogWrite(leftMotorPWM, -motor1speed);
        digitalWrite(rightMotor1, LOW);
        digitalWrite(rightMotor2, HIGH);
        analogWrite(rightMotorPWM, -motor2speed);
    }
    if (motor1speed >= 0 && motor2speed < 0)
    {
        digitalWrite(leftMotor1, HIGH);
        digitalWrite(leftMotor2, LOW);
        analogWrite(leftMotorPWM, motor1speed);
        digitalWrite(rightMotor1, LOW);
        digitalWrite(rightMotor2, HIGH);
        analogWrite(rightMotorPWM, -motor2speed);
    }
    if (motor1speed < 0 && motor2speed >= 0)
    {
        digitalWrite(leftMotor1, LOW);
        digitalWrite(leftMotor2, HIGH);
        analogWrite(leftMotorPWM, -motor1speed);
        digitalWrite(rightMotor1, HIGH);
        digitalWrite(rightMotor2, LOW);
        analogWrite(rightMotorPWM, motor2speed);
    }
}

void Auto_calibration()
{
    unsigned int counter;
    for (counter = 0; counter < 80; counter++)
    {
        if (counter <= 20 || counter >= 60)
        {
            set_motors(110, -110);
        }
        else
        {
            set_motors(-110, 110);
        }
        qtra.calibrate();
        delay(2);
    }
}

void breaking()
{
    digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, HIGH);
    digitalWrite(leftMotorPWM, HIGH);
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, HIGH);
    digitalWrite(rightMotorPWM, HIGH);
    delay(1);
}

int sensor_reading_black2()
{
    qtra.readLine(sensors);
    int i;

    for (i = 0; i <= 7; i++)
    {
        sensors[i] = (1024 - sensors[i]) * 10 / 1024;
        int pin = 22 + i;
        //if(sensors[i]>4)
        //{
        //  sensors[i]=9;
        //}
        //else if(sensors[i]<4)
        //{
        //  sensors[i]=0;
        //}
        if (sensors[i] > 4)
        {

            digitalWrite(pin, HIGH);
        }
        else
        {
            digitalWrite(pin, LOW);
        }
        // Serial.print("  ");
        // Serial.print(sensors[i]);
    }
    //Serial.println("");
    int position0 = (((0 * sensors[0]) + (100 * sensors[1]) + (200 * sensors[2]) + (300 * sensors[3]) + (400 * sensors[4]) + (500 * sensors[5]) + (600 * sensors[6]) + (700 * sensors[7])) / (sensors[0] + sensors[1] + sensors[2] + sensors[3] + sensors[4] + sensors[5] + sensors[6] + sensors[7]));
    if (sensors[7] > 4)
    {
        j = 1;
    }
    else if (sensors[0] > 4)
    {
        j = 2;
    }
    //Serial.println(j);
    if (position0 > 700 || position0 <= 0)
    {
        if (j == 1)
        {
            position0 = 700;
        }
        else if (j == 2)
        {
            position0 = 0;
        }
    }

    return position0;
}

int sensor_reading_white2()
{
    qtra.readLine(sensors);
    int i;

    for (i = 0; i <= 7; i++)
    {
        sensors[i] = sensors[i] * 10 / 1024;
        int pin = 22 + i;
        //if(sensors[i]>4)
        //{
        //  sensors[i]=9;
        //}
        //else if(sensors[i]<4)
        //{
        // sensors[i]=0;
        //}
        if (sensors[i] > 4)
        {

            digitalWrite(pin, LOW);
        }
        else
        {
            digitalWrite(pin, HIGH);
        }
        // Serial.print("  ");
        // Serial.print(sensors[i]);
    }
    //Serial.println("");
    int position0 = (((0 * sensors[0]) + (100 * sensors[1]) + (200 * sensors[2]) + (300 * sensors[3]) + (400 * sensors[4]) + (500 * sensors[5]) + (600 * sensors[6]) + (700 * sensors[7])) / (sensors[0] + sensors[1] + sensors[2] + sensors[3] + sensors[4] + sensors[5] + sensors[6] + sensors[7]));

    //if(position!=0)
    //{
    //  error = position - 3500;
    //}
    //else
    //{
    //  error= 69;
    //}
    //if(sensors[0]>200)
    //{
    //  turn=1;
    //}
    //else if(sensors[7]>200)
    //{
    //  turn=0;
    //}
    //Serial.println(position0);
    if (sensors[7] > 4)
    {
        j = 1;
    }
    else if (sensors[0] > 4)
    {
        j = 2;
    }
    //Serial.println(j);
    if (position0 > 700 || position0 <= 0)
    {
        if (j == 1)
        {
            position0 = 700;
        }
        else if (j == 2)
        {
            position0 = 0;
        }
    }

    return position0;
}
int sensor_reading_black()
{
    qtra.readLine(sensors);
    int i;

    for (i = 0; i <= 7; i++)
    {
        sensors[i] = (1024 - sensors[i]) * 10 / 1024;
        int pin = 22 + i;
        if (sensors[i] > 4)
        {
            sensors[i] = 9;
        }
        else if (sensors[i] < 4)
        {
            sensors[i] = 0;
        }
        if (sensors[i] > 4)
        {
            digitalWrite(pin, HIGH);
        }
        else
        {
            digitalWrite(pin, LOW);
        }
        // Serial.print("  ");
        // Serial.print(sensors[i]);
    }
    //Serial.println("");
    int position0 = (((0 * sensors[0]) + (100 * sensors[1]) + (200 * sensors[2]) + (300 * sensors[3]) + (400 * sensors[4]) + (500 * sensors[5]) + (600 * sensors[6]) + (700 * sensors[7])) / (sensors[0] + sensors[1] + sensors[2] + sensors[3] + sensors[4] + sensors[5] + sensors[6] + sensors[7]));

    //if(position!=0)
    //{
    //  error = position - 3500;
    //}
    //else
    //{
    //  error= 69;
    //}
    //if(sensors[0]>200)
    //{
    //  turn=1;
    //}
    //else if(sensors[7]>200)
    //{
    //  turn=0;
    //}
    //Serial.println(position0);
    if (sensors[7] > 4)
    {
        j = 1;
    }
    else if (sensors[0] > 4)
    {
        j = 2;
    }
    //Serial.println(j);
    if (position0 > 700 || position0 <= 0)
    {
        if (j == 1)
        {
            position0 = 700;
        }
        else if (j == 2)
        {
            position0 = 0;
        }
    }

    return position0;
}

int sensor_reading_White()
{
    qtra.readLine(sensors);
    int i;

    for (i = 0; i <= 7; i++)
    {
        sensors[i] = sensors[i] * 10 / 1024;
        int pin = 22 + i;
        if (sensors[i] > 4)
        {
            sensors[i] = 9;
        }
        else if (sensors[i] < 4)
        {
            sensors[i] = 0;
        }
        if (sensors[i] > 4)
        {
            digitalWrite(pin, LOW);
        }
        else
        {
            digitalWrite(pin, HIGH);
        }
        // Serial.print("  ");
        // Serial.print(sensors[i]);
    }
    //Serial.println("");
    int position0 = (((0 * sensors[0]) + (100 * sensors[1]) + (200 * sensors[2]) + (300 * sensors[3]) + (400 * sensors[4]) + (500 * sensors[5]) + (600 * sensors[6]) + (700 * sensors[7])) / (sensors[0] + sensors[1] + sensors[2] + sensors[3] + sensors[4] + sensors[5] + sensors[6] + sensors[7]));

    //if(position!=0)
    //{
    //  error = position - 3500;
    //}
    //else
    //{
    //  error= 69;
    //}
    //if(sensors[0]>200)
    //{
    //  turn=1;
    //}
    //else if(sensors[7]>200)
    //{
    //  turn=0;
    //}
    //Serial.println(position0);
    if (sensors[7] > 4)
    {
        j = 1;
    }
    else if (sensors[0] > 4)
    {
        j = 2;
    }
    //Serial.println(j);
    if (position0 > 700 || position0 <= 0)
    {
        if (j == 1)
        {
            position0 = 700;
        }
        else if (j == 2)
        {
            position0 = 0;
        }
    }

    return position0;
}
void turn()
{

    breaking();
    sensor_reading_black();
    if (sensors[0] > 4 && sensors[1] > 4 && sensors[2] > 4 && sensors[3] > 4 && sensors[4] > 4 && sensors[5] > 4 && sensors[6] > 4 && sensors[7] > 4)
    {
        set_motors(80, 80);
        delay(leapTime);

        breaking();
        sensor_reading_black();
        if (sensors[0] > 4 && sensors[1] > 4 && sensors[2] > 4 && sensors[3] > 4 && sensors[4] > 4 && sensors[5] > 4 && sensors[6] > 4 && sensors[7] > 4)
        {
            set_motors(80, 80);
            delay(leapTime);
            breaking();
            sensor_reading_black();
            if (sensors[0] > 4 && sensors[1] > 4 && sensors[2] > 4 && sensors[3] > 4 && sensors[4] > 4 && sensors[5] > 4 && sensors[6] > 4 && sensors[7] > 4)
            {
                set_motors(80, 80);
                delay(leapTime);
                breaking();
                sensor_reading_black();
                if (sensors[0] > 4 && sensors[1] > 4 && sensors[2] > 4 && sensors[3] > 4 && sensors[4] > 4 && sensors[5] > 4 && sensors[6] > 4 && sensors[7] > 4)
                {
                    finish();
                }
                else if (sensors[0] < 4 && sensors[1] < 4 && sensors[2] < 4 && sensors[3] < 4 && sensors[4] < 4 && sensors[5] < 4 && sensors[6] < 4 && sensors[7] < 4)
                {
                    turnRight();
                }
            }
            else if (sensors[0] < 4 && sensors[1] < 4 && sensors[2] < 4 && sensors[3] < 4 && sensors[4] < 4 && sensors[5] < 4 && sensors[6] < 4 && sensors[7] < 4)
            {
                turnRight();
            }
        }
        else if (sensors[0] < 4 && sensors[1] < 4 && sensors[2] < 4 && sensors[3] < 4 && sensors[4] < 4 && sensors[5] < 4 && sensors[6] < 4 && sensors[7] < 4)
        {
            turnRight();
        }
        sensor_reading_black();
        sensor_reading_black();
        if (sensors[0] > 4)
        {
            turnLeft();
        }
        sensor_reading_black();

        if (sensors[0] > 4)
        {
            sensor_reading_black();
            //while(sensors[3]>4||sensors[4]>4)
            //{
            // set_motors(80,80);
            // delay(5);
            // set_motors(0,0);
            // delay(0);
            // sensor_reading_black();
            //}
            turnRight();
        }
    }
    else if (sensors[7] > 4 && sensors[6] > 4 && sensors[5] > 4 && sensors[4] > 4 && sensors[0] < 4)
    {
        set_motors(80, 80);
        delay(leapTime);
        breaking();
        sensor_reading_black();
        if (sensors[0] > 4 && sensors[1] > 4 && sensors[2] > 4 && sensors[3] > 4 && sensors[4] > 4 && sensors[5] > 4 && sensors[6] > 4 && sensors[7] > 4)
        {
            set_motors(80, 80);
            delay(leapTime);
            breaking();
            sensor_reading_black();
            if (sensors[0] > 4 && sensors[1] > 4 && sensors[2] > 4 && sensors[3] > 4 && sensors[4] > 4 && sensors[5] > 4 && sensors[6] > 4 && sensors[7] > 4)
            {
                set_motors(80, 80);
                delay(leapTime);
                breaking();
                sensor_reading_black();
                if (sensors[0] > 4 && sensors[1] > 4 && sensors[2] > 4 && sensors[3] > 4 && sensors[4] > 4 && sensors[5] > 4 && sensors[6] > 4 && sensors[7] > 4)
                {
                    set_motors(80, 80);
                    delay(leapTime);
                    breaking();
                    sensor_reading_black();
                    if (sensors[0] > 4 && sensors[1] > 4 && sensors[2] > 4 && sensors[3] > 4 && sensors[4] > 4 && sensors[5] > 4 && sensors[6] > 4 && sensors[7] > 4)
                    {
                        finish();
                    }
                    else
                    {
                        turnRight();
                    }
                }
                else
                {
                    turnRight();
                }
            }
            else
            {
                turnRight();
            }
        }
        else
        {
            turnRight();
        }
    }
    else if (sensors[0] > 4 && sensors[1] > 4 && sensors[2] > 4 && sensors[3] > 4 && sensors[7] < 4)
    {
        set_motors(80, 80);
        delay(leapTime);
        breaking();
        sensor_reading_black();
        if (sensors[0] > 4 && sensors[1] > 4 && sensors[2] > 4 && sensors[3] > 4 && sensors[4] > 4 && sensors[5] > 4 && sensors[6] > 4 && sensors[7] > 4)
        {
            set_motors(80, 80);
            delay(leapTime);
            breaking();
            sensor_reading_black();
            if (sensors[0] > 4 && sensors[1] > 4 && sensors[2] > 4 && sensors[3] > 4 && sensors[4] > 4 && sensors[5] > 4 && sensors[6] > 4 && sensors[7] > 4)
            {
                set_motors(80, 80);
                delay(leapTime);
                breaking();
                sensor_reading_black();
                if (sensors[0] > 4 && sensors[1] > 4 && sensors[2] > 4 && sensors[3] > 4 && sensors[4] > 4 && sensors[5] > 4 && sensors[6] > 4 && sensors[7] > 4)
                {
                    set_motors(80, 80);
                    delay(leapTime);
                    breaking();
                    sensor_reading_black();
                    if (sensors[0] > 4 && sensors[1] > 4 && sensors[2] > 4 && sensors[3] > 4 && sensors[4] > 4 && sensors[5] > 4 && sensors[6] > 4 && sensors[7] > 4)
                    {
                        finish();
                    }
                    else
                    {
                        turnRight();
                    }
                }
                else
                {
                    turnRight();
                }
            }
            else
            {
                turnRight();
            }
        }
        else if (sensors[0] > 4 && sensors[7] < 4)
        {
            turnLeft();
        }
    }
    else if (sensors[7] > 4 && (sensors[6] < 4 || sensors[5] < 4) && (sensors[4] > 4 || sensors[3] > 4))
    {

        turnRight();
    }
    else if (sensors[0] > 4 && (sensors[1] < 4 || sensors[2] < 4) && (sensors[3] > 4 || sensors[4] > 4))
    {
        sensor_reading_black();
        while (sensors[3] > 4 || sensors[4] > 4)
        {
            set_motors(80, 80);
            delay(5);
            set_motors(0, 0);
            delay(1);
            sensor_reading_black();
        }
        sensor_reading_black();
        if (sensors[0] < 4 && sensors[1] < 4 && sensors[2] < 4 && sensors[3] < 4 && sensors[4] < 4 && sensors[5] < 4 && sensors[6] < 4 && sensors[7] < 4)
        {
            turnLeft();
        }
    }
    else if ((sensors[0] > 4 || sensors[1] > 4) && sensors[2] < 4 && sensors[3] < 4 && sensors[4] < 4 && sensors[5] < 4 && sensors[6] < 4 && sensors[7] < 4)
    {
        turnLeft();
    }
    else if ((sensors[7] > 4 || sensors[6] > 4) && sensors[0] < 4 && sensors[1] < 4 && sensors[2] < 4 && sensors[3] < 4 && sensors[4] < 4 && sensors[5] < 4)
    {
        turnRight();
    }
    else if (sensors[3] > 4 && sensors[4] > 4 && (sensors[2] > 4 || sensors[5] > 4) && sensors[0] < 4 && sensors[7] < 4)
    {
        turnRight();
    }

    else if (sensors[0] < 4 && sensors[1] < 4 && sensors[2] < 4 && sensors[3] < 4 && sensors[4] < 4 && sensors[5] < 4 && sensors[6] < 4 && sensors[7] < 4)
    {

        set_motors(80, 80);
        delay(50);
        set_motors(0, 0);
        delay(0);

        turnLeft();
    }

    else
    {
        set_motors(80, 80);
        delay(2);
        breaking();
    }
}

void turn2()
{

    breaking();
    sensor_reading_White();
    if (sensors[0] > 4 && sensors[1] > 4 && sensors[2] > 4 && sensors[3] > 4 && sensors[4] > 4 && sensors[5] > 4 && sensors[6] > 4 && sensors[7] > 4)
    {
        set_motors(80, 80);
        delay(leapTime);

        breaking();
        sensor_reading_White();
        if (sensors[0] > 4 && sensors[1] > 4 && sensors[2] > 4 && sensors[3] > 4 && sensors[4] > 4 && sensors[5] > 4 && sensors[6] > 4 && sensors[7] > 4)
        {
            set_motors(80, 80);
            delay(leapTime);

            breaking();
            sensor_reading_White();
            if (sensors[0] > 4 && sensors[1] > 4 && sensors[2] > 4 && sensors[3] > 4 && sensors[4] > 4 && sensors[5] > 4 && sensors[6] > 4 && sensors[7] > 4)
            {
                set_motors(80, 80);
                delay(leapTime);
                breaking();
                sensor_reading_White();
                if (sensors[0] > 4 && sensors[1] > 4 && sensors[2] > 4 && sensors[3] > 4 && sensors[4] > 4 && sensors[5] > 4 && sensors[6] > 4 && sensors[7] > 4)
                {
                    finish2();
                }
                else if (sensors[0] < 4 && sensors[1] < 4 && sensors[2] < 4 && sensors[3] < 4 && sensors[4] < 4 && sensors[5] < 4 && sensors[6] < 4 && sensors[7] < 4)
                {
                    turnRight2();
                }
            }
            else if (sensors[0] < 4 && sensors[1] < 4 && sensors[2] < 4 && sensors[3] < 4 && sensors[4] < 4 && sensors[5] < 4 && sensors[6] < 4 && sensors[7] < 4)
            {
                turnRight2();
            }
        }
        else if (sensors[0] < 4 && sensors[1] < 4 && sensors[2] < 4 && sensors[3] < 4 && sensors[4] < 4 && sensors[5] < 4 && sensors[6] < 4 && sensors[7] < 4)
        {
            turnRight2();
        }
    }
    else if (sensors[7] > 4 && sensors[6] > 4 && sensors[5] > 4 && sensors[4] > 4 && sensors[0] < 4)
    {
        set_motors(80, 80);
        delay(leapTime);
        breaking();
        sensor_reading_White();
        if (sensors[0] > 4 && sensors[1] > 4 && sensors[2] > 4 && sensors[3] > 4 && sensors[4] > 4 && sensors[5] > 4 && sensors[6] > 4 && sensors[7] > 4)
        {
            set_motors(80, 80);
            delay(leapTime);
            breaking();
            sensor_reading_White();
            if (sensors[0] > 4 && sensors[1] > 4 && sensors[2] > 4 && sensors[3] > 4 && sensors[4] > 4 && sensors[5] > 4 && sensors[6] > 4 && sensors[7] > 4)
            {
                set_motors(80, 80);
                delay(leapTime);
                breaking();
                sensor_reading_White();
                if (sensors[0] > 4 && sensors[1] > 4 && sensors[2] > 4 && sensors[3] > 4 && sensors[4] > 4 && sensors[5] > 4 && sensors[6] > 4 && sensors[7] > 4)
                {
                    set_motors(80, 80);
                    delay(leapTime);
                    breaking();
                    sensor_reading_White();
                    if (sensors[0] > 4 && sensors[1] > 4 && sensors[2] > 4 && sensors[3] > 4 && sensors[4] > 4 && sensors[5] > 4 && sensors[6] > 4 && sensors[7] > 4)
                    {
                        finish2();
                    }
                    else
                    {
                        turnRight2();
                    }
                }
                else
                {
                    turnRight2();
                }
            }
            else
            {
                turnRight2();
            }
        }
        else
        {
            turnRight2();
        }
    }
    else if (sensors[0] > 4 && sensors[1] > 4 && sensors[2] > 4 && sensors[3] > 4 && sensors[7] < 4)
    {
        set_motors(80, 80);
        delay(leapTime);
        breaking();
        sensor_reading_White();
        if (sensors[0] > 4 && sensors[1] > 4 && sensors[2] > 4 && sensors[3] > 4 && sensors[4] > 4 && sensors[5] > 4 && sensors[6] > 4 && sensors[7] > 4)
        {
            set_motors(80, 80);
            delay(leapTime);
            breaking();
            sensor_reading_White();
            if (sensors[0] > 4 && sensors[1] > 4 && sensors[2] > 4 && sensors[3] > 4 && sensors[4] > 4 && sensors[5] > 4 && sensors[6] > 4 && sensors[7] > 4)
            {
                set_motors(80, 80);
                delay(leapTime);
                breaking();
                sensor_reading_White();
                if (sensors[0] > 4 && sensors[1] > 4 && sensors[2] > 4 && sensors[3] > 4 && sensors[4] > 4 && sensors[5] > 4 && sensors[6] > 4 && sensors[7] > 4)
                {
                    set_motors(80, 80);
                    delay(leapTime);
                    breaking();
                    sensor_reading_White();
                    if (sensors[0] > 4 && sensors[1] > 4 && sensors[2] > 4 && sensors[3] > 4 && sensors[4] > 4 && sensors[5] > 4 && sensors[6] > 4 && sensors[7] > 4)
                    {
                        finish2();
                    }
                    else
                    {
                        turnRight2();
                    }
                }
                else
                {
                    turnRight2();
                }
            }
            else
            {
                turnRight2();
            }
        }
        else if (sensors[0] > 4 && sensors[7] < 4)
        {
            turnLeft2();
        }
    }
    else if (sensors[7] > 4 && (sensors[6] < 4 || sensors[5] < 4) && (sensors[4] > 4 || sensors[3] > 4))
    {

        turnRight2();
    }
    else if (sensors[0] > 4 && (sensors[1] < 4 || sensors[2] < 4) && (sensors[3] > 4 || sensors[4] > 4))
    {
        sensor_reading_White();
        while (sensors[3] > 4 || sensors[4] > 4)
        {
            set_motors(80, 80);
            delay(5);
            set_motors(0, 0);
            delay(1);
            sensor_reading_White();
        }
        sensor_reading_White();
        if (sensors[0] < 4 && sensors[1] < 4 && sensors[2] < 4 && sensors[3] < 4 && sensors[4] < 4 && sensors[5] < 4 && sensors[6] < 4 && sensors[7] < 4)
        {
            turnLeft2();
        }
    }
    else if ((sensors[0] > 4 || sensors[1] > 4) && sensors[2] < 4 && sensors[3] < 4 && sensors[4] < 4 && sensors[5] < 4 && sensors[6] < 4 && sensors[7] < 4)
    {
        turnLeft2();
    }
    else if ((sensors[7] > 4 || sensors[6] > 4) && sensors[0] < 4 && sensors[1] < 4 && sensors[2] < 4 && sensors[3] < 4 && sensors[4] < 4 && sensors[5] < 4)
    {
        turnRight2();
    }
    else if (sensors[3] > 4 && sensors[4] > 4 && (sensors[2] > 4 || sensors[5] > 4) && sensors[0] < 4 && sensors[7] < 4)
    {
        turnRight2();
    }

    else if (sensors[0] < 4 && sensors[1] < 4 && sensors[2] < 4 && sensors[3] < 4 && sensors[4] < 4 && sensors[5] < 4 && sensors[6] < 4 && sensors[7] < 4)
    {

        set_motors(80, 80);
        delay(50);
        set_motors(0, 0);
        delay(0);
        turnLeft2();
    }

    else
    {
        set_motors(80, 80);
        delay(2);
        breaking();
    }
}

void turnLeft()
{
    sensor_reading_black();
    while (sensors[3] > 4 || sensors[4] > 4)
    {
        set_motors(-150, 150);
        delay(5);
        set_motors(0, 0);
        delay(1);
        sensor_reading_black();
    }
    sensor_reading_black();
    while (sensors[3] < 4)
    {
        set_motors(-150, 150);
        delay(5);
        set_motors(0, 0);
        delay(1);
        sensor_reading_black();
    }
    breaking();
    delay(30);
}
void turnLeft2()
{
    sensor_reading_White();
    while (sensors[3] > 4 || sensors[4] > 4)
    {
        set_motors(-150, 150);
        delay(5);
        set_motors(0, 0);
        delay(1);
        sensor_reading_White();
    }
    sensor_reading_White();
    while (sensors[3] < 4)
    {
        set_motors(-150, 150);
        delay(5);
        set_motors(0, 0);
        delay(1);
        sensor_reading_White();
    }
    breaking();
    delay(30);
}

void turnRight()
{
    sensor_reading_black();
    while (sensors[3] > 4 || sensors[4] > 4)
    {
        set_motors(150, -150);
        delay(5);
        set_motors(0, 0);
        delay(1);
        sensor_reading_black();
    }
    sensor_reading_black();
    while (sensors[4] < 4)
    {
        set_motors(150, -150);
        delay(5);
        set_motors(0, 0);
        delay(1);
        sensor_reading_black();
    }
    breaking();
    delay(30);
}
void turnRight2()
{
    sensor_reading_White();
    while (sensors[3] > 4 || sensors[4] > 4)
    {
        set_motors(150, -150);
        delay(5);
        set_motors(0, 0);
        delay(1);
        sensor_reading_White();
    }
    sensor_reading_White();
    while (sensors[4] < 4)
    {
        set_motors(150, -150);
        delay(5);
        set_motors(0, 0);
        delay(1);
        sensor_reading_White();
    }
    breaking();
    delay(30);
}

void finish()
{
    sensor_reading_black();
    set_motors(100, 100);
    delay(10);
    if (sensors[0] > 4 && sensors[7] > 4)
    {
        set_motors(100, 100);
        delay(100);
        Stop();
    }
}
void finish2()
{
    sensor_reading_White();
    set_motors(100, 100);
    delay(10);
    if (sensors[0] > 4 && sensors[7] > 4)
    {
        set_motors(100, 100);
        delay(100);
        Stop();
    }
}

void Stop()
{
    set_motors(0, 0);
    delay(1000);
    Stop();
}

void crossTheBridge()
{
    set_motors(0, 0);
    while (sensor_reading_black2() < 300)
    {
        set_motors(0, 120);
        delay(3);
        set_motors(0, 0);
    }
    while (sensor_reading_black2() > 400)
    {
        set_motors(120, 0);
        delay(3);
        set_motors(0, 0);
    }
    while (analogRead(A10) < identifier)
    {
        set_motors(180, 190);
        delay(5);
        set_motors(0, 0);
    }
    while (analogRead(A10) > identifier)
    {
        // straightPID();
        straight();
    }
    while (analogRead(A10) < identifier)
    {
        set_motors(180, 190);
        delay(5);
        set_motors(0, 0);
    }
}

void straightPID()
{
    positionb = sensor_reading_black2();
    Serial.println(positionb);
    errorb = positionb - 350;
    //Serial.println(error);
    proportionalb = errorb;
    lastErrorb;
    derivativeb = errorb - lastErrorb;
    integralb += proportionalb;
    //Serial.print(proportional);
    //Serial.print("  ");
    //Serial.print(derivative);
    //Serial.print("  ");
    if (integralb > 255)
    {
        integralb = 255;
    }
    if (integralb < -255)
    {
        integralb = -255;
    }
    int motorSpeedb = proportionalb * Kpb + derivativeb * Kdb + integralb * Kib;
    //Serial.print(motorSpeed);
    //Serial.println("");
    lastErrorb = errorb;

    int leftMotorSpeedb;
    int rightMotorSpeedb;

    if (sensors[1] < 4 && sensors[6] < 4 && sensors[0] < 4 && sensors[7] < 4)
    {
        leftMotorSpeedb = M1_default_speedb + motorSpeedb;
        rightMotorSpeedb = M2_default_speedb - motorSpeedb;

        set_motors(leftMotorSpeedb, rightMotorSpeedb);
    }
    else if (sensors[0] < 4 && sensors[1] < 4 && sensors[2] < 4 && sensors[3] < 4 && sensors[4] < 4 && sensors[5] < 4 && sensors[6] < 4 && sensors[7] < 4)
    {
        set_motors(180, 180);
        delay(10);
    }
    else
    {

        set_motors(180, 180);
        delay(20);
        M1_default_speedb = 70;
        M2_default_speedb = 70;
    }
}

void straight()
{

    if (sensor_reading_black2() < 400 && sensor_reading_black2() > 300)
    {
        set_motors(180, 180);
        delay(7);
        //breaking();
        //delay(2);
        return;
    }
    if (sensor_reading_black2() < 300 && sensor_reading_black2() > 200)
    {

        set_motors(130, 180);
        delay(5);
        //breaking();
        //delay(1);

        return;
    }
    if (sensor_reading_black2() < 200 && sensor_reading_black2() > 100)
    {

        set_motors(80, 180);
        delay(5);
        //breaking();
        //delay(1);

        return;
    }
    if (sensor_reading_black2() < 100 && sensor_reading_black2() > 0)
    {

        set_motors(50, 180);
        delay(5);
        // breaking();
        //delay(1);

        return;
    }
    if (sensor_reading_black2() < 500 && sensor_reading_black2() > 400)
    {

        set_motors(180, 130);
        delay(5);
        //breaking();
        // delay(1);

        return;
    }
    if (sensor_reading_black2() < 600 && sensor_reading_black2() > 500)
    {

        set_motors(180, 80);
        delay(5);
        // breaking();
        //delay(1);

        return;
    }
    if (sensor_reading_black2() < 700 && sensor_reading_black2() > 600)
    {

        set_motors(180, 50);
        delay(5);
        // breaking();

        return;
    }
}

void straight2()
{

    if (sensor_reading_white2() < 400 && sensor_reading_white2() > 300)
    {
        set_motors(180, 180);
        delay(7);
        //breaking();
        //delay(2);
        return;
    }
    if (sensor_reading_white2() < 300 && sensor_reading_white2() > 200)
    {

        set_motors(130, 180);
        delay(5);
        //breaking();
        //delay(1);

        return;
    }
    if (sensor_reading_white2() < 200 && sensor_reading_white2() > 100)
    {

        set_motors(80, 180);
        delay(5);
        //breaking();
        //delay(1);

        return;
    }
    if (sensor_reading_white2() < 100 && sensor_reading_white2() > 0)
    {

        set_motors(50, 180);
        delay(5);
        // breaking();
        //delay(1);

        return;
    }
    if (sensor_reading_white2() < 500 && sensor_reading_white2() > 400)
    {

        set_motors(180, 130);
        delay(5);
        //breaking();
        // delay(1);

        return;
    }
    if (sensor_reading_white2() < 600 && sensor_reading_white2() > 500)
    {

        set_motors(180, 80);
        delay(5);
        // breaking();
        //delay(1);

        return;
    }
    if (sensor_reading_white2() < 700 && sensor_reading_white2() > 600)
    {

        set_motors(180, 50);
        delay(5);
        // breaking();

        return;
    }
}

void straightUp()
{

    if (sensor_reading_black2() < 400 && sensor_reading_black2() > 300)
    {
        set_motors(180, 180);
        delay(7);
        //breaking();
        //delay(2);
        return;
    }
    if (sensor_reading_black2() < 300 && sensor_reading_black2() > 200)
    {

        set_motors(130, 180);
        delay(5);
        //breaking();
        //delay(1);

        return;
    }
    if (sensor_reading_black2() < 200 && sensor_reading_black2() > 100)
    {

        set_motors(80, 180);
        delay(5);
        //breaking();
        //delay(1);

        return;
    }
    if (sensor_reading_black2() < 100 && sensor_reading_black2() > 0)
    {

        set_motors(50, 180);
        delay(5);
        // breaking();
        //delay(1);

        return;
    }
    if (sensor_reading_black2() < 500 && sensor_reading_black2() > 400)
    {

        set_motors(180, 130);
        delay(5);
        //breaking();
        // delay(1);

        return;
    }
    if (sensor_reading_black2() < 600 && sensor_reading_black2() > 500)
    {

        set_motors(180, 80);
        delay(5);
        // breaking();
        //delay(1);

        return;
    }
    if (sensor_reading_black2() < 700 && sensor_reading_black2() > 600)
    {

        set_motors(180, 50);
        delay(5);
        // breaking();

        return;
    }
}

void straightDown()
{

    if (sensor_reading_black2() < 400 && sensor_reading_black2() > 300)
    {
        set_motors(80, 80);
        delay(7);
        //breaking();
        //delay(2);
        return;
    }
    if (sensor_reading_black2() < 300 && sensor_reading_black2() > 200)
    {

        set_motors(30, 80);
        delay(5);
        //breaking();
        //delay(1);

        return;
    }
    if (sensor_reading_black2() < 200 && sensor_reading_black2() > 100)
    {

        set_motors(30, 100);
        delay(5);
        //breaking();
        //delay(1);

        return;
    }
    if (sensor_reading_black2() < 100 && sensor_reading_black2() > 0)
    {

        set_motors(30, 100);
        delay(5);
        // breaking();
        //delay(1);

        return;
    }
    if (sensor_reading_black2() < 500 && sensor_reading_black2() > 400)
    {

        set_motors(100, 60);
        delay(5);
        //breaking();
        // delay(1);

        return;
    }
    if (sensor_reading_black2() < 600 && sensor_reading_black2() > 500)
    {

        set_motors(100, 30);
        delay(5);
        // breaking();
        //delay(1);

        return;
    }
    if (sensor_reading_black2() < 700 && sensor_reading_black2() > 600)
    {

        set_motors(100, 30);
        delay(5);
        // breaking();

        return;
    }
}

void turn_left0()
{
    while (mySerial.parseInt() == 1257)
    {
        set_motors(-170, 170);
        delay(5);
        set_motors(0, 0);
        delay(1);
    }
    while (mySerial.parseInt() > 1000)
    {
        set_motors(-170, 170);
        delay(5);
        set_motors(0, 0);
        delay(1);
    }
    breaking();
}

void turn_right0()
{
    while (mySerial.parseInt() == 1258)
    {
        set_motors(170, -170);
        delay(5);
        set_motors(0, 0);
        delay(1);
    }
    while (mySerial.parseInt() > 1000)
    {
        set_motors(170, -170);
        delay(5);
        set_motors(0, 0);
        delay(1);
    }
    breaking();
}

void runInCave()
{
    positionw = mySerial.parseInt();
    errorw = positionw - 150;
    proportionalw = errorw;
    derivativew = errorw - lastErrorw;
    integralw += proportionalw;
    if (integralw > 255)
    {
        integralw = 255;
    }
    if (integralw < -255)
    {
        integralw = -255;
    }
    int motorSpeedw = proportionalw * Kpw + derivativew * Kdw + integralw * Kiw;
    //Serial.print(motorSpeed);
    //Serial.println("");
    lastErrorw = errorw;
    int leftMotorSpeedw;
    int rightMotorSpeedw;
    leftMotorSpeedw = M1_default_speedw + motorSpeedw;
    rightMotorSpeedw = M2_default_speedw - motorSpeedw;
    set_motors(leftMotorSpeedw, rightMotorSpeedw);
}

void obstacle_avoid()
{
    breaking();
    delay(200);
    set_motors(120, -120);
    delay(400);
    breaking();
    delay(100);
    set_motors(100, 100);
    delay(400);
    breaking();
    delay(100);
    set_motors(-120, 120);
    delay(300);
    breaking();
    delay(100);
    set_motors(120, 120);
    delay(1200);
    breaking();
    delay(100);
    set_motors(0, 120);
    delay(300);
    sensor_reading_black();
    while (sensors[0] < 3 && sensors[1] < 3 && sensors[2] < 3 && sensors[3] < 3 && sensors[4] < 3 && sensors[5] < 3 && sensors[6] < 3 && sensors[7] < 3)
    {
        set_motors(120, 120);
        delay(5);
        set_motors(0, 0);
        sensor_reading_black();
    }
}
