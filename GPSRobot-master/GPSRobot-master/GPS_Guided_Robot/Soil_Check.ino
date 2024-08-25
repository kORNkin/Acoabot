#include <Stepper.h>

#define IN1 13
#define IN2 12
#define IN3 14
#define IN4 27

const int sp = 100;
Stepper mp(sp, IN1, IN2, IN3, IN4);

void checkSoil()
{
  mp.setSpeed(200);
  for(int i = 0;i < 35;i++)
  {
    mp.step(sp);
  }
}

void uncheckSoil()
{
  mp.setSpeed(-200);
  for(int i = 0;i < 35;i++)
  {
    mp.step(sp);
  }  
}