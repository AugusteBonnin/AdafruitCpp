#include "PWM.h"

PWM::PWM(int bus,int address)
{
    _i2c = new I2C(bus,address);


    // zero all PWM ports
    setAllPWM(0,0);

    _i2c->write_byte( __MODE2, __OUTDRV);
    _i2c->write_byte( __MODE1, __ALLCALL);

    int mode1 = _i2c->read_byte( __MODE1);
    mode1 = mode1 & ~__SLEEP;
    _i2c->write_byte( __MODE1, mode1);

    setPWMFreq(60);
}

PWM::~PWM()
{
delete _i2c;
}

void
PWM::setPWMFreq(int freq)
{
    float prescaleval = 25000000;
    prescaleval /= 4096.0;
    prescaleval /= (float)freq;
    prescaleval -= 1.0;
    int prescale = floor(prescaleval + 0.5);

    int oldmode = _i2c->read_byte( __MODE1);
    int newmode = (oldmode & 0x7F) | 0x10;
    _i2c->write_byte( __MODE1, newmode);
    _i2c->write_byte( __PRESCALE, floor(prescale));
    _i2c->write_byte( __MODE1, oldmode);

    _i2c->write_byte( __MODE1, oldmode | 0x80);
}

void
PWM::setPWM(int channel, int on, int off)
{
    _i2c->write_byte( __LED0_ON_L+4*channel, on & 0xFF);
    _i2c->write_byte( __LED0_ON_H+4*channel, on >> 8);
    _i2c->write_byte( __LED0_OFF_L+4*channel, off & 0xFF);
    _i2c->write_byte( __LED0_OFF_H+4*channel, off >> 8);
}

void
PWM::setAllPWM(int on, int off)
{
    _i2c->write_byte( __ALL_LED_ON_L, on & 0xFF);
    _i2c->write_byte( __ALL_LED_ON_H, on >> 8);
    _i2c->write_byte( __ALL_LED_OFF_L, off & 0xFF);
    _i2c->write_byte( __ALL_LED_OFF_H, off >> 8);
}

