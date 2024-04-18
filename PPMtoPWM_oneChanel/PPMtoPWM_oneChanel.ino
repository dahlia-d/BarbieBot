//define the pins and variables

#define RCPin 2

#define directionMotorPin 6 //който сложим
#define speedMotorPin 5 //който сложим

volatile unsigned long StartTime = 0; 
volatile unsigned long CurrentTime = 0;
volatile unsigned long Pulses = 0;
unsigned long PulseWidth = 0;

int PWMSignal = 0;

void setup() {
  //Serial.begin(115200);

  pinMode (RCPin, INPUT_PULLUP);
  pinMode (directionMotorPin, OUTPUT);
  pinMode (speedMotorPin, OUTPUT);

  attachInterrupt (digitalPinToInterrupt (RCPin), PulseTimer, CHANGE); //вика функцията PulseTimer при промяна във фронта на пин RCPin - или когато настъпва rising edge или когао настъпва falling edge
}


void loop(){

  cli();
  PulseWidth = Pulses;
  sei();
  if (PulseWidth > 950 && PulseWidth <= 2000){ 
    if(PulseWidth > 950 && PulseWidth <= 1469){
      // първия мотор се движи назад, май трябва да икараме на един цифров изход 1 или 0 за посоката и на един PWM изход или 0 или 255(ако се не се движат или се движат с максимална скорост) или съответно map-натата стойност
      digitalWrite(directionMotorPin, LOW);
      PWMSignal = map(PulseWidth, 1469, 950, 0, 255); // май така би трябвало да се map-не - analogWrite приема стойности от 0 до 255
      analogWrite(speedMotorPin, PWMSignal);
    }
    else if(PulseWidth > 1469 && PulseWidth <= 1560){
      // първия мотор не се движи
      //digitalWrite(directionMotorPin, LOW);
      analogWrite(speedMotorPin, 0);
    }
    else if(PulseWidth > 1560 && PulseWidth <= 2000){
      // първия мотор се движи напред
      digitalWrite(directionMotorPin, HIGH);
      PWMSignal = map(PulseWidth, 1560, 2000, 0, 255); // май така би трябвало да се map-не - analogWrite приема стойности от 0 до 255
      analogWrite(speedMotorPin, PWMSignal);
    }
  }

}

void PulseTimer(){

//measure the time between interrupts

  CurrentTime = micros(); //времето от началото на програмата

  if (CurrentTime > StartTime){ //StartTime - последният път, в който е бил засечен interrupt
    Pulses = CurrentTime - StartTime; // времето между interrupt-ите, при rising edge  ще бъде генериран interrupt и отново ще бъде генериран interrupt при falling edge - времето между двете е най-много 2000 us
                                                // времето между falling edge и rising edge ще бъде около 18000us (при 50Hz времето между rising edges e около 20000us -> 20000 - 2000 = 18000)
    StartTime = CurrentTime; //презаписва се последният път, в който е бил засечен interrupt
  }
}