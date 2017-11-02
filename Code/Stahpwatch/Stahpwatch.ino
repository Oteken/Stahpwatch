#define IN1  8
#define IN2  9
#define IN3  10
#define IN4  11
int Steps = 0;
boolean Direction = true;

const int STEPPERMOTORSTEPS = 4096;
const int CLOCKSTEPS = 60;

float startTime;
int motorPosition = 0;
// The ratio of clock step to motor step.
float motorToClockStep = STEPPERMOTORSTEPS / CLOCKSTEPS;
// The time represented on the timer in seconds.
int clockTime = 0;
// The actual time that should be represented on the timer in seconds.
int actualTime = 0;
// The time represented on the timer in minutes.
int clockTimeMinute = 0;
// The actual time that should be represented on the timer in minutes.
int actualTimeMinute = 0;


void setup() {
  Serial.begin(9600);
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT); 
  startTime = millis();
}

void loop() {
  logicOperation();
  hardwareOperation();
}

void logicOperation(){
  actualTime = (secondsSinceStartTime() % 60);
  actualTimeMinute = ((secondsSinceStartTime() / 60) % 60);
}

void hardwareOperation(){
  if(clockTimeMinute != actualTimeMinute){
    clockhandOperation();
    clockTimeMinute = actualTimeMinute;
  }
  if(clockTime != actualTime){
    lightingOperation();
    clockTime = actualTime;
  }
}

void clockhandOperation(){
  int motorStepTarget = (int)(getTimeDifferenceMinute() * motorToClockStep);
  takeSteps(motorStepTarget, true);
}

void lightingOperation(){
  
}

int secondsSinceStartTime(){
  return ((millis() - startTime)/1000);
}

int getTimeDifference(){
  if(actualTime > clockTime)
    return actualTime - clockTime;
  else 
    return (actualTime + 60) - clockTime;
}

int getTimeDifferenceMinute(){
  if(actualTimeMinute > clockTimeMinute)
    return actualTimeMinute - clockTimeMinute;
  else 
    return (actualTimeMinute + 60) - clockTimeMinute;
}

void takeSteps(int stepAmount, boolean direction){
  Direction = direction;
  int stepsTaken = 0;
  while(stepsTaken < stepAmount){
    stepper(1);
    delay(5);
    stepsTaken++;
  }
  if(direction)
    motorPosition = (motorPosition + stepAmount) % STEPPERMOTORSTEPS;
  else
    motorPosition = (motorPosition - stepAmount) % STEPPERMOTORSTEPS;
}

void stepper(int xw){
  for (int x=0;x<xw;x++){
    switch(Steps){
       case 0:
         digitalWrite(IN1, LOW); 
         digitalWrite(IN2, LOW);
         digitalWrite(IN3, LOW);
         digitalWrite(IN4, HIGH);
       break; 
       case 1:
         digitalWrite(IN1, LOW); 
         digitalWrite(IN2, LOW);
         digitalWrite(IN3, HIGH);
         digitalWrite(IN4, HIGH);
       break; 
       case 2:
         digitalWrite(IN1, LOW); 
         digitalWrite(IN2, LOW);
         digitalWrite(IN3, HIGH);
         digitalWrite(IN4, LOW);
       break; 
       case 3:
         digitalWrite(IN1, LOW); 
         digitalWrite(IN2, HIGH);
         digitalWrite(IN3, HIGH);
         digitalWrite(IN4, LOW);
       break; 
       case 4:
         digitalWrite(IN1, LOW); 
         digitalWrite(IN2, HIGH);
         digitalWrite(IN3, LOW);
         digitalWrite(IN4, LOW);
       break; 
       case 5:
         digitalWrite(IN1, HIGH); 
         digitalWrite(IN2, HIGH);
         digitalWrite(IN3, LOW);
         digitalWrite(IN4, LOW);
       break; 
         case 6:
         digitalWrite(IN1, HIGH); 
         digitalWrite(IN2, LOW);
         digitalWrite(IN3, LOW);
         digitalWrite(IN4, LOW);
       break; 
       case 7:
         digitalWrite(IN1, HIGH); 
         digitalWrite(IN2, LOW);
         digitalWrite(IN3, LOW);
         digitalWrite(IN4, HIGH);
       break; 
       default:
         digitalWrite(IN1, LOW); 
         digitalWrite(IN2, LOW);
         digitalWrite(IN3, LOW);
         digitalWrite(IN4, LOW);
       break; 
    }
  SetDirection();
  }
} 
void SetDirection(){
  if(Direction==1)
    Steps++;
  if(Direction==0)
    Steps--;
  if(Steps>7)
    Steps=0;
  if(Steps<0)
    Steps=7;
}

