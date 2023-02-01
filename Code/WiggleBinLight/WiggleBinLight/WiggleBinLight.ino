#include <TinyWireS.h>

int state = 0;
const int led = 4;

void setup()
{
  pinMode(led, OUTPUT);
  TinyWireS.begin(13);
  TinyWireS.onReceive(receiveEvent);
  TinyWireS.onRequest(requestEvent);
}

void loop()
{
  TinyWireS_stop_check();

  analogWrite(led, state);
}

void receiveEvent(int howMany)
{
  state = TinyWireS.receive();
}

void requestEvent(int howMany)
{
  TinyWireS.send(state);
}
