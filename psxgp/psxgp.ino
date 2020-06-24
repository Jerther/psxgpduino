#include <Psx.h>
#include <Gamepad.h>

#define PSX_DATA_PIN 15
#define PSX_CMD_PIN  14
#define PSX_ATT_PIN  10
#define PSX_CLK_PIN  16
#define PSX_DELAY    7

Psx Psx;
Gamepad gp;

void setup()
{
  Psx.setupPins(PSX_DATA_PIN, PSX_CMD_PIN, PSX_ATT_PIN, PSX_CLK_PIN, PSX_DELAY);
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
  gp.setButtonState(6, data & psxR1);
  gp.setButtonState(7, data & psxL1);
  gp.setButtonState(8, data & psxR2);
  gp.setButtonState(9, data & psxL2);
  unsigned long stop_watch = micros();
  Serial.println((stop_watch - start_watch));
  //faut cumuler les valeurs ici pis faire un seul send state a la fin
}
