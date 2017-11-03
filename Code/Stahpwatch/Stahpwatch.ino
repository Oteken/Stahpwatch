#define IN1  11
#define IN2  12
#define IN3  13
#define IN4  A0
int Steps = 0;
boolean Direction = true;

const int STEPPERMOTORSTEPS = 4096;
const int CLOCKSTEPS = 60;

// {RedPin, BluePin, GreenPin} -1 if no pin is attached
int ledArrayOne[] = {2, 3, -1};
int ledArrayTwo[] = {-1, 4, -1};
int ledArrayThree[] = {-1, 5, 6};
int ledArrayFour[] = {-1, -1, 7};
int ledArrayFive[] = {8, -1, 9};
int ledArraySix[] = {10, -1, -1};


int pinkCode[] = {1, 1, 0};
int blueCode[] = {0, 1, 0};
int aquaCode[] = {0, 1, 1};
int greenCode[] = {0, 0, 1};
int purpleCode[] = {1, 0, 1};
int redCode[] = {1, 0, 0};
int clearCode[] = {0, 0, 0};

int lightingPhase = 0;
int mode = 0;
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

boolean running = false;

void setup() {
  Serial.begin(9600);
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT); 
  pinMode(A1, INPUT);
  startTime = millis();
}

void loop() {
  buttonOperation();
  if(running){
    logicOperation();
    hardwareOperation();
  }
}

void buttonOperation(){
  if(running){
    if(digitalRead(A1) == HIGH){
      clearAllLedArrays();
      delay(1000);
      if(digitalRead(A1) == HIGH){
        while(digitalRead(A1) == HIGH){
          takeSteps(1, true);
        }
        motorPosition = 0;
      }
      shutdown();
      switch(mode){
        case 0:
          mode = 1;
          break;
        case 1:
          mode = 0;
          break;
      }
    }
  } else {
    if(digitalRead(A1) == HIGH){
      start();
      delay(1000);
    }
  }
}

void logicOperation(){
  actualTime = (secondsSinceStartTime() % 60);
  actualTimeMinute = ((secondsSinceStartTime() / 60) % 60);
}

void hardwareOperation(){
  clockhandOperation();
  lightingOperation();
  if(mode == 0);
    clockTime = actualTime;
}

void clockhandOperation(){
  if(mode == 0){    
    if(clockTime != actualTime){
      int motorStepTarget = (int)(getTimeDifference() * motorToClockStep);
      takeSteps(motorStepTarget, true);
    }
  }
  if(mode == 1){
    if(clockTimeMinute != actualTimeMinute){
      int motorStepTarget = (int)(getTimeDifferenceMinute() * motorToClockStep);
      takeSteps(motorStepTarget, true);
      clockTimeMinute = actualTimeMinute;
    }
  }
}

void lightingOperation(){
  if(clockTime != actualTime){
    if(((clockTime % 10) - (actualTime % 10)) > 0){
      lightNextPhase();
    }
    if(mode == 1)
      clockTime = actualTime;
  }
}

void lightNextPhase(){
  switch(lightingPhase){
    case 0:
      clearAllLedArrays();
      setColor(pinkCode, ledArrayOne);
      break;
    case 1:
      setColor(blueCode, ledArrayTwo);
      break;
    case 2:
      setColor(aquaCode, ledArrayThree);
      break;
    case 3:
      setColor(greenCode, ledArrayFour);
      break;
    case 4:
      setColor(purpleCode, ledArrayFive);
      break;
    case 5:
      setColor(redCode, ledArraySix);
      break;
  }
  lightingPhase++;
  if(lightingPhase > 5)
    lightingPhase = 0;
}

void clearAllLedArrays(){
  setColor(clearCode, ledArrayOne);
  setColor(clearCode, ledArrayTwo);
  setColor(clearCode, ledArrayThree);
  setColor(clearCode, ledArrayFour);
  setColor(clearCode, ledArrayFive);
  setColor(clearCode, ledArraySix);
}

void reset(){
  moveMotorToOrigin();
  lightingPhase = 0;
  lightNextPhase();
  startTime = millis();
  clockTime = 0;
  actualTime = 0;
  clockTimeMinute = 0;
  actualTimeMinute = 0;
}

void moveMotorToOrigin(){
  if(motorPosition != 0){
    if(motorPosition > (STEPPERMOTORSTEPS / 2))
      takeSteps(STEPPERMOTORSTEPS - motorPosition, true);
    else
      takeSteps(motorPosition, false);
  }
  motorPosition = 0;
}

void shutdown(){
  running = false;
  moveMotorToOrigin();
}

void start(){
  running = true;
  reset();
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

void setColor(int colorCode[], int pins[])
{
  for(int i = 0; i < sizeof(pins)+1; i++){
    digitalWrite(pins[i], colorCode[i]);
  }
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

