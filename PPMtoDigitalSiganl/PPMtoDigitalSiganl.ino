//define the pins and variables

#define RCEanblePin 2
//#define RCBuzzerPin 3

#define EnalblePin 4 //който сложим
//#define BuzzerPin 6 //който сложим

volatile unsigned long StartTimeEnalble = 0; 
volatile unsigned long CurrentTimeEnalble = 0;
volatile unsigned long PulsesEnalble = 0;
unsigned long PulseWidthEnalble = 0;

int PWMSignalEnalble = 0;

int directionEnalble = 0;

//volatile unsigned long StartTimeBuzzer = 0; 
//volatile unsigned long CurrentTimeBuzzer = 0;
//volatile unsigned long PulsesBuzzer = 0;
//unsigned long PulseWidthBuzzer = 0;

//int PWMSignalBuzzer = 0;

//int directionBuzzer = 0;

void setup() {
  pinMode (RCEanblePin, INPUT_PULLUP);
  //pinMode (RCBuzzerPin, INPUT_PULLUP);

  pinMode(EnalblePin, OUTPUT);

  attachInterrupt (digitalPinToInterrupt (RCEanblePin), PulseTimerEnalble, CHANGE); //вика функцията PulseTimer при промяна във фронта на пин RCPin - или когато настъпва rising edge или когао настъпва falling edge
  //attachInterrupt (digitalPinToInterrupt (RCBuzzerPin), PulseTimerBuzzer, CHANGE);
}


void loop(){

  cli();
  PulseWidthEnalble = PulsesEnalble;
  sei();
  if (PulseWidthEnalble > 950 && PulseWidthEnalble < 2050){ 
    if(PulseWidthEnalble > 950 && PulseWidthEnalble <= 1050){
      digitalWrite(EnalblePin, LOW);
    }
    else if(PulseWidthEnalble > 1950 && PulseWidthEnalble < 2050){
      digitalWrite(EnalblePin, HIGH);
    }
  }
/*
  cli();
  PulseWidthBuzzer = PulsesBuzzer;
  sei();
  if (PulseWidthBuzzer > 950 && PulseWidthBuzzer < 2050){ 
    if(PulseWidthBuzzer > 950 && PulseWidthBuzzer <= 1050){
      digitalWrite(BuzzerPin, LOW);
    }
    else if(PulseWidthBuzzer > 1950 && PulseWidthBuzzer < 2050){
      digitalWrite(BuzzerPin, HIGH);
    }
  }
*/
}

void PulseTimerEnalble(){

  CurrentTimeEnalble = micros(); //времето от началото на програмата

  if (CurrentTimeEnalble > StartTimeEnalble){ //StartTime - последният път, в който е бил засечен interrupt
    PulsesEnalble = CurrentTimeEnalble - StartTimeEnalble; // времето между interrupt-ите, при rising edge  ще бъде генериран interrupt и отново ще бъде генериран interrupt при falling edge - времето между двете е най-много 2000 us
                                                // времето между falling edge и rising edge ще бъде около 18000us (при 50Hz времето между rising edges e около 20000us -> 20000 - 2000 = 18000)
    StartTimeEnalble = CurrentTimeEnalble; //презаписва се последният път, в който е бил засечен interrupt
  }
}
/*
void PulseTimerBuzzer(){

  CurrentTimeBuzzer = micros(); //времето от началото на програмата

  if (CurrentTimeBuzzer > StartTimeBuzzer){ //StartTime - последният път, в който е бил засечен interrupt
    PulsesBuzzer = CurrentTimeBuzzer - StartTimeBuzzer; // времето между interrupt-ите, при rising edge  ще бъде генериран interrupt и отново ще бъде генериран interrupt при falling edge - времето между двете е най-много 2000 us
                                                // времето между falling edge и rising edge ще бъде около 18000us (при 50Hz времето между rising edges e около 20000us -> 20000 - 2000 = 18000)
    StartTimeBuzzer = CurrentTimeBuzzer; //презаписва се последният път, в който е бил засечен interrupt
  }
}*/