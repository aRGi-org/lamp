#define ledPin1     8
#define ledPin2    11

#define analogPin0  0
#define analogPin1  1

#define minpulse    0
#define maxpulse  125

#define mindimmer   0
#define maxdimmer 255

#define minanin     0
#define maxanin  1023


unsigned long int ms_counter;
int ledpulse = 0;
int dimmer = 0;
int anin0 = 0;
int anin1 = 0;

void setup() {
   
  Serial.begin(115200);

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  ms_counter = 0;
  // initialize Timer1
  cli();             // disable global interrupts
  TCCR1A = 0;        // set entire TCCR1A register to 0
  TCCR1B = 0;
  // Set CS10 and CS12 bits so timer runs at clock speed /1024:
  TCCR1B |= (1 << CS10); //|(1 << CS10);
 
  // enable Timer1 overflow interrupt:
  TIMSK1 = (1 << TOIE1);
  // enable global interrupts:
  sei();

  anin0 = analogRead(analogPin0);
  anin1 = analogRead(analogPin1);
}

ISR(TIMER1_OVF_vect)
{
  ms_counter++;
}

int r2s(int rangeVal) {
    if (rangeVal <= 32)
      return 0;
    if (rangeVal <= 64)
      return 64;
    if (rangeVal <= 96)
      return 96;
    if (rangeVal <= 128)
      return 128;
    if (rangeVal <= 160)
      return 160;
    if (rangeVal <= 192)
      return 192;
    if (rangeVal <= 214)
      return 214;
    return 255;  
}

void loop() {
  anin0 = analogRead(analogPin0);
  anin1 = analogRead(analogPin1);
  ledpulse = map(anin0,minanin,maxanin,minpulse,maxpulse);
  dimmer = r2s(map(anin1,minanin,maxanin,mindimmer,maxdimmer));
  analogWrite(ledPin2, dimmer);  
  if (ms_counter >= ledpulse) {
    ms_counter = 0;
    digitalWrite(ledPin1, !digitalRead(ledPin1));
  }
}
