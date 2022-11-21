#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <comm.h>

// #define SDA_1 21
// #define SCL_1 22

// called this way, it uses the default address 0x40
// Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
// you can also call it with a different address and I2C interface
// Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN 1300    // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 4096 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN 600     // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX 2400    // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

// our servo # counter
uint8_t servonum = 0;

void setupMotors()
{
    pwm.begin();
    /*
     * In theory the internal oscillator (clock) is 25MHz but it really isn't
     * that precise. You can 'calibrate' this by tweaking this number until
     * you get the PWM update frequency you're expecting!
     * The int.osc. for the PCA9685 chip is a range between about 23-27MHz and
     * is used for calculating things like writeMicroseconds()
     * Analog servos run at ~50 Hz updates, It is importaint to use an
     * oscilloscope in setting the int.osc frequency for the I2C PCA9685 chip.
     * 1) Attach the oscilloscope to one of the PWM signal pins and ground on
     *    the I2C PCA9685 chip you are setting the value for.
     * 2) Adjust setOscillatorFrequency() until the PWM update frequency is the
     *    expected value (50Hz for most ESCs)
     * Setting the value here is specific to each individual I2C PCA9685 chip and
     * affects the calculations for the PWM update frequency.
     * Failure to correctly set the int.osc value will cause unexpected PWM results
     */
    pwm.setOscillatorFrequency(27000000);
    pwm.setPWMFreq(SERVO_FREQ); // Analog servos run at ~50 Hz updates

    delay(10);
}

// You can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. It's not precise!
void setServoPulse(uint8_t n, double pulse)
{
    double pulselength;

    pulselength = 1000000;     // 1,000,000 us per second
    pulselength /= SERVO_FREQ; // Analog servos run at ~60 Hz updates
    Serial.print(pulselength);
    Serial.println(" us per period");
    pulselength /= 4096; // 12 bits of resolution
    Serial.print(pulselength);
    Serial.println(" us per bit");
    pulse *= 1000000; // convert input seconds to us
    pulse /= pulselength;
    Serial.println(pulse);
    pwm.setPWM(n, 0, pulse);
}

void motorTest()
{
    Serial.println("Servo test!");
    // Drive each servo one at a time using setPWM()
    std::string txt("driving motor " + std::to_string(servonum));
    char *res = &txt[0];
    Serial.println(res);

    Serial.println("pulselen++");
    for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++)
    {
        Serial.println(pulselen);
        pwm.setPWM(servonum, 0, pulselen);
    }

    delay(500);
    Serial.println("pulselen--");
    for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--)
    {
        Serial.println(pulselen);
        pwm.setPWM(servonum, 0, pulselen);
    }

    delay(2000);

    Serial.println("microsec++");
    // Drive each servo one at a time using writeMicroseconds(), it's not precise due to calculation rounding!
    // The writeMicroseconds() function is used to mimic the Arduino Servo library writeMicroseconds() behavior.
    for (uint16_t microsec = USMIN; microsec < USMAX; microsec++)
    {
        pwm.writeMicroseconds(servonum, microsec);
    }

    delay(500);
    Serial.println("microsec++");
    for (uint16_t microsec = USMAX; microsec > USMIN; microsec--)
    {
        pwm.writeMicroseconds(servonum, microsec);
    }

    delay(500);

    // servonum++;
    // if (servonum > 15)
    //     servonum = 0; // Testing the first 8 servo channels
}

void driveMotors(int values[], int size)
{
    for (int i = 0; i < size; i++)
    {
        Serial.print("driving ");
        Serial.print(i);
        Serial.print(" at ");
        Serial.println(values[i]);
        pwm.setPWM(i, 0, values[i]);
    }
}

void Scanner()
{
    Serial.println();
    Serial.println("I2C scanner. Scanning ...");
    byte count = 0;

    Wire.begin();
    for (byte i = 8; i < 120; i++)
    {
        Wire.beginTransmission(i);       // Begin I2C transmission Address (i)
        if (Wire.endTransmission() == 0) // Receive 0 = success (ACK response)
        {
            Serial.print("Found address: ");
            Serial.print(i, DEC);
            Serial.print(" (0x");
            Serial.print(i, HEX); // PCF8574 7 bit address
            Serial.println(")");
            count++;
        }
    }
    Serial.print("Found ");
    Serial.print(count, DEC); // numbers of devices
    Serial.println(" device(s).");
}

void runScanner()
{
    Scanner();
    delay(100);
}