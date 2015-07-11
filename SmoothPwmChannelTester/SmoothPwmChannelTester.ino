#define INPUT_PIN 10

void setup() {
  pinMode(INPUT_PIN, INPUT);
  Serial.begin(115200);
}

void loop() {
  long inSignal = getSignal();
  Serial.println(inSignal);
}

long oldSignal = 1500;
unsigned long getSignal() {
  unsigned long signal = pulseIn(INPUT_PIN, HIGH, 20000);
  if (signal < 1000 || signal > 2000) signal = oldSignal;
  else oldSignal = signal;
  return signal;
}
