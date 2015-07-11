
#define INPUT_PIN 10
#define OUTPUT_PIN 11

#define MIN_SIGNAL 1000
#define MAX_SIGNAL 2000
#define MIDDLE_SIGNAL 1500
#define SPEED 0.5

//Smooth tilt beta version fully functional JMMaupin 2014-09-21 
float SliderPos = 0;    // Internal value from Input Slider desired position
float SliderAcc = 0;    // Smooth ghost from SliderPos
float DeltaSldAcc = 0;
float AbsDeltaSldAcc = 0;
int DirSliderAcc = 0; // Direction from SliderPos
float CurrentPos = 0;   // Live out position
float DeltaPos = 0;
float DeltaPosR2 = 0;   // DeltaPosSqr
float AbsDeltaPos = 0;
int DirectPos = 0;    // Direction

// Behaviour parameters. Results depend from the running cycle time approximately every 10ms (Mesured)
float AccRatio = 0.02; //1/50;    // Impact acceleration
float SpeedRatio = 0.5; //1/2;  // Impact speed and deceleration (High speed is clipped by MaxSpeed)
float SExtSpeed = 0;       // Internal value from Input Slider or Switch speed profile
int MaxSpeed = 100;      // Clipped the high speed

int DutyTime = (MIN_SIGNAL>>3); // this wloud give us minimum angle signal
// X>>1 = X/2, X>>2 = X/4 and X>>3 = X/8...

void setup() {
  pinMode(INPUT_PIN, INPUT); // set pin 10 to input
  pinMode(OUTPUT_PIN, OUTPUT); // set pin 11 to output
     
  TCCR2A |= _BV(COM2A1); // this connects Comperator 2A to its pin
  // note the _BV(REGISTER) is the same as (1<<REGISTER) its just a Arduino wise
  OCR2A = DutyTime;

  Serial.begin(115200);
}

void loop() {
  long inSignal = getSignal();
  long outSignal = generateSignal(inSignal);

  Serial.print(inSignal);
  Serial.print(" ");
  Serial.println(outSignal);

  DutyTime = (outSignal>>3);
  OCR2A = DutyTime;
}

float mapfloat(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}

float generateSignal(float position) {
  SliderPos = position; // Get the Desired target position
  // Compute the desired global speed
  SExtSpeed = SPEED; //(1024 + speed / 2) /1024; // 0.5 for -1024 | 1 for 0 | 1.5 for 1024
  
  // 1-st smooth the Slider acceleration and speed
  DeltaSldAcc = SliderAcc - SliderPos;    // Difference between Slider and internal slider smooth acc
  AbsDeltaSldAcc=abs(DeltaSldAcc);   // Absolute value
  DirSliderAcc = DeltaSldAcc > 0 ? 1 : -1;  // What is the direction ?
  // Cyclic Smooth slider computing
  SliderAcc = SliderAcc - DirSliderAcc * AbsDeltaSldAcc * AccRatio * SExtSpeed;
  // End smooth Slider
  //---------------------------------------------------------------------------------
  
  // 2-nd Compute final position
  DeltaPos = CurrentPos - SliderAcc;    // Difference between Slider smooth and internal and Output position
  AbsDeltaPos = abs(DeltaPos);   // Absolute value
  DirectPos = DeltaPos > 0 ? 1 : -1;    // What is the direction ?
  DeltaPosR2 = sqrt(AbsDeltaPos);  // Root square from AbsDeltaPos to shorten the deceleration
  if (DeltaPosR2 > MaxSpeed) {      // When DeltaPosR2 is high final speed is too high then
    DeltaPosR2 = MaxSpeed;        // Clipped DeltaPosR2
  }
  // Cyclic final output position computing
  CurrentPos = CurrentPos - (DirectPos * DeltaPosR2 * SpeedRatio * SExtSpeed);
//  Serial.print(DirectPos);
//  Serial.print(" ");
//  Serial.print(DeltaPosR2);
//  Serial.print(" ");
//  Serial.print(SpeedRatio);
//  Serial.print(" ");
//  Serial.println(SExtSpeed);
  // End Compute final position
  
  // Function return results
  //return CurrentPos, SliderAcc, DeltaPos, SExtSpeed * 10.24

  return CurrentPos;
}

long oldSignal = MIDDLE_SIGNAL;
unsigned long getSignal() {
  unsigned long signal = pulseIn(INPUT_PIN, HIGH, 20000);
  if (signal < 1000 || signal > 2000) signal = oldSignal;
  else oldSignal = signal;
  return signal;
}

void putSignal(unsigned long outSignal) {
  
}

