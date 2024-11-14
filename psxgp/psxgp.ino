#include <Psx.h>
#include <Gamepad.h>

Psx Psx;
Gamepad gp;

void setup()
{
  Serial.begin(115200);
  delay(2000);
  Serial.println("START!");
}

void loop()
{
  unsigned long start_watch = micros();
  unsigned int data = Psx.read();
  
  if (data & psxLeft)
    gp.setLeftXaxis(-127);
  else if (data & psxRight)
    gp.setLeftXaxis(127);
  else
    gp.setLeftXaxis(0);

  if (data & psxUp)
    gp.setLeftYaxis(127);
  else if (data & psxDown)
    gp.setLeftYaxis(-127);
  else
    gp.setLeftYaxis(0);

  gp.setButtonState(0, data & psxStrt);
  gp.setButtonState(1, data & psxSlct);
  gp.setButtonState(2, data & psxSqu);
  gp.setButtonState(3, data & psxX);
  gp.setButtonState(4, data & psxO);
  gp.setButtonState(5, data & psxTri);
  gp.setButtonState(6, data & psxL1);
  gp.setButtonState(7, data & psxR1);
  gp.setButtonState(8, data & psxL2);
  gp.setButtonState(9, data & psxR2);
  if(gp.sendUpdate() < 0)
    delay(1000);
  unsigned long stop_watch = micros();
  Serial.println((stop_watch - start_watch));

}
