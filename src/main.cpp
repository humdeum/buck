#include <Arduino.h>
#define PWMPIN 6
#define TRIM A0
#define SETPIN A1
#define N 500
#define HYST 10
#define SAMPLES 15

int value, outval=127, setval, m=0, n=0,
  val, set, up, down, k=0, i;
int vals[SAMPLES] = {0};

void printUp(void)
{
  Serial.print("Read ");
  Serial.print(value);
  Serial.print(", setval = ");
  Serial.print(setval);
  Serial.print(", regulating up to ");
  Serial.println(outval);
}

void printDown(void)
{
  Serial.print("Read ");
  Serial.print(value);
  Serial.print(", setval = ");
  Serial.print(setval);
  Serial.print(", regulating down to ");
  Serial.println(outval);
}

void printLevel(void)
{
  Serial.print("Read ");
  Serial.print(value);
  Serial.print(", setval = ");
  Serial.print(setval);
  Serial.print(", not regulating at ");
  Serial.print(outval);
  Serial.print(", max = ");
  Serial.println(m);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Analog out");
  pinMode(PWMPIN, OUTPUT);
  pinMode(TRIM, INPUT);
  pinMode(SETPIN, INPUT);
  //TCCR0B = (TCCR0B & B11111000) | B00000001; // 62.5kHz
}

void loop() {
  value = analogRead(TRIM);
  value = map(value, 0, 1023, 0, 255);
  for (i=0; i<SAMPLES; i++)
    vals[i+1] = vals[i];
  vals[0] = value;

  setval = analogRead(SETPIN);
  setval = map(setval, 0, 1023, 0, 255);

  set = setval;

  if (n == N)
  {
    k++;
    if (k == 100)
    {
      k = 0;
      m = 0;
    }
  }
  val = 0;
  for (i=0; i<SAMPLES; i++)
    val += vals[i];
  val = val/SAMPLES;


  if ((val < set - HYST) && (outval > 0))
  {
    if (up > m)
      m = up;
    outval--;
    down++;
    up = 0;
    if (n == N)
      printDown();
  }
  else if ((val > set + HYST) && (outval < 255))
  {
    if (down > m)
      m = down;
    outval++;
    up++;
    down = 0;
    if (n == N)
      printUp();
  } else if ((value == set) && (n == N))
    printLevel();
  if (n == N)
    n = 0;

  analogWrite(PWMPIN, outval);
  n++;
}
