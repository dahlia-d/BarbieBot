//define the pins and variables

#define RCPinCh2 2

#define RCPinCh3 3

#define directionMotor1 4 //който сложим
#define directionMotor2 5 //който сложим
#define speedMotor1 6 //който сложим
#define speedMotor2 7 //който сложим

volatile unsigned long StartTimeCh2 = 0; 
volatile unsigned long CurrentTimeCh2 = 0;
volatile unsigned long PulsesCh2 = 0;
unsigned long PulseWidthCh2 = 0;

volatile unsigned long StartTimeCh3 = 0; 
volatile unsigned long CurrentTimeCh3 = 0; 
volatile unsigned long PulsesCh3 = 0;
unsigned long PulseWidthCh3 = 0;

int PWMSignal2 = 0;
int PWMSignal3 = 0;

int direction1 = 0;
int direction2 = 0;

void setup() {

  //set up the serial monitor, pin mode, and external interrupt. 
  Serial.begin(115200);

  pinMode (RCPinCh2, INPUT_PULLUP);

  pinMode (RCPinCh3, INPUT_PULLUP);

  attachInterrupt (digitalPinToInterrupt (RCPinCh2), PulseTimerCh2, CHANGE); //вика функцията PulseTimerCh2 при промяна във фронта на пин RCPinCh2 - или когато настъпва rising edge или когао настъпва falling edge

  attachInterrupt (digitalPinToInterrupt (RCPinCh3), PulseTimerCh3, CHANGE); //вика функцията PulseTimerCh3 при промяна във фронта на пин RCPinCh3 - или когато настъпва rising edge или когао настъпва falling edge
}


void loop(){

//only save pulse lengths that are less than 2000 microseconds 2000) (
  cli();
  PulseWidthCh2 = PulsesCh2;
  sei();
  if (PulseWidthCh2 > 950 && PulseWidthCh2 < 2000){ 
    if(PulseWidthCh2 > 950 && PulseWidthCh2 <= 1469){
      // първия мотор се движи назад, май трябва да икараме на един цифров изход 1 или 0 за посоката и на един PWM изход или 0 или 255(ако се не се движат или се движат с максимална скорост) или съответно map-натата стойност
      digitalWrite(directionMotor1, LOW);
      direction1 = 1;
      PWMSignal2 = map(PulseWidthCh2, 1469, 1000, 0, 255); // май така би трябвало да се map-не - analogWrite приема стойности от 0 до 255
      analogWrite(speedMotor1, PWMSignal2);
    }
    else if(PulseWidthCh2 > 1469 && PulseWidthCh2 <= 1561){
      // първия мотор не се движи
      direction1 = 0;
      analogWrite(speedMotor1, 0);
    }
    else if(PulseWidthCh2 > 1561 && PulseWidthCh2 < 2000){
      // първия мотор се движи напред
      direction1 = -1;
      digitalWrite(directionMotor1, HIGH);
      PWMSignal2 = map(PulseWidthCh2, 1561, 2000, 0, 255); // май така би трябвало да се map-не - analogWrite приема стойности от 0 до 255
      analogWrite(speedMotor1, PWMSignal2);
    }
    //Serial.print("Ch 2 - "); 
    //Serial.print("PWM: "); 
    Serial.print(PWMSignal2);
    Serial.print(" ");
    //Serial.print("direction: ");
    Serial.print(direction1); 
    Serial.print(" ");
    //Serial.print("Time: "); 
    Serial.print(PulseWidthCh2); 
    Serial.print("     ");
  }
  


  cli();
  PulseWidthCh3 = PulsesCh3;
  sei();
  if (PulseWidthCh3 > 950 && PulseWidthCh3 < 2000){
    if(PulseWidthCh3 > 950 && PulseWidthCh3 <= 1469){
      // втория мотор се движи назад
      digitalWrite(directionMotor2, LOW);
      direction2 = 1;
      PWMSignal3 = map(PulseWidthCh3, 1469, 1000, 0, 255); // май така би трябвало да се map-не - analogWrite приема стойности от 0 до 255
      analogWrite(speedMotor2, PWMSignal3);
    }
    else if(PulseWidthCh3 > 1469 && PulseWidthCh3 <= 1561){
      // втория мотор не се движи
      direction2 = 0;
      analogWrite(speedMotor2, 0);
    }
    else if(PulseWidthCh3 > 1561 && PulseWidthCh3 < 2000){
      // втория мотор се движи напред
      digitalWrite(directionMotor1, HIGH);
      direction2 = -1;
      PWMSignal3 = map(PulseWidthCh3, 1561, 2000, 0, 255); // май така би трябвало да се map-не - analogWrite приема стойности от 0 до 255
      analogWrite(speedMotor1, PWMSignal3);
    }
    //Serial.print("Ch 3 - "); 
    //Serial.print("PWM: "); 
    Serial.print(PWMSignal3);
    Serial.print(" ");
    //Serial.print("direction: ");
    Serial.print(direction2); 
    Serial.print(" ");
    //Serial.print("Time: "); 
    Serial.println(PulseWidthCh3); 
  }
  
  //Serial.print("\n");

  //delay(100);

}

void PulseTimerCh2(){

//measure the time between interrupts

  CurrentTimeCh2 = micros(); //времето от началото на програмата

  if (CurrentTimeCh2 > StartTimeCh2){ //StartTimeCh2 - последният път, в който е бил засечен interrupt
    PulsesCh2 = CurrentTimeCh2 - StartTimeCh2; // времето между interrupt-ите, при rising edge  ще бъде генериран interrupt и отново ще бъде генериран interrupt при falling edge - времето между двете е най-много 2000 us
                                                // времето между falling edge и rising edge ще бъде около 18000us (при 50Hz времето между rising edges e около 20000us -> 20000 - 2000 = 18000)
    StartTimeCh2 = CurrentTimeCh2; //презаписва се последният път, в който е бил засечен interrupt
  }
}

void PulseTimerCh3(){

//measure the time between interrupts

  CurrentTimeCh3 = micros();

  if (CurrentTimeCh3 > StartTimeCh3) {
    PulsesCh3 = CurrentTimeCh3 - StartTimeCh3;
    StartTimeCh3 = CurrentTimeCh3;
  }
}
