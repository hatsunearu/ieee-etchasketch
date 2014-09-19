int pulseSchedule[] = {1, 50, 3, 10, 2, 50};

unsigned long lastStep = 0; //time since last step
int dir = 0; //direction of motor step
int pulses = 0; //number of pulses to step in dir

// -- PIN ASSIGNMENTS --
int LR_CCW_PIN = 40, LR_CW_PIN = 41; 
int UD_CCW_PIN = 50, UD_CW_PIN = 51;
int DISABLE_PIN = 52;
// ---------------------

// -- PULSE CONFIGS --
int PW_US = 3; //pulse width in us 
int STEP_DELAY = 50; //period of pulses in ms
// -------------------

void setup() {
  pinMode(LR_CCW_PIN, OUTPUT);
  pinMode(LR_CW_PIN, OUTPUT);
  pinMode(UD_CCW_PIN, OUTPUT);
  pinMode(UD_CW_PIN, OUTPUT);
  pinMode(DISABLE_PIN, OUTPUT);
  
  digitalWrite(DISABLE_PIN, HIGH);
  
  Serial.begin(9600);
}

void loop() {
  stepperScheduler();
  stepperRun();
}

void stepperRun() { //runs the current schedule
  
  if(dir == 5) {
    stepn(5);
  }
  
  else if(millis() - lastStep > STEP_DELAY && pulses > 0) {
    stepn(dir);
    pulses--;
    lastStep = millis();
  }
}

void stepperScheduler() { //looks at pulseSchedule and finds next valid pulse schedule
  if(pulses <= 0) {
    for(int i=0; i < sizeof(pulseSchedule)/sizeof(pulseSchedule[0]); i++) {
      
      dir = 5; //disable if scheduler does not assign anything
      
      if(pulseSchedule[i] > 0) {
        dir = pulseSchedule[i];
        pulses = pulseSchedule[i+1];
        pulseSchedule[i] = 0;
        pulseSchedule[i+1] = 0;
        break;
      }
      
    }
  }
}

void stepn(int dir) { //dir = 1 left 2 = right 3 = up 4 = down 5 = disable
  switch (dir) {
    case 1: //LEFT
      digitalWrite(DISABLE_PIN, LOW);
      digitalWrite(LR_CW_PIN, HIGH); //pull other pin high
      digitalWrite(LR_CCW_PIN, LOW);
      delayMicroseconds(PW_US);
      digitalWrite(LR_CCW_PIN, HIGH);
      break;
    case 2: //RIGHT
      digitalWrite(DISABLE_PIN, LOW);
      digitalWrite(LR_CCW_PIN, HIGH); 
      digitalWrite(LR_CW_PIN, LOW);
      delayMicroseconds(PW_US);
      digitalWrite(LR_CW_PIN, HIGH);
      break;    
    case 3: //UP      
      digitalWrite(DISABLE_PIN, LOW);
      digitalWrite(UD_CCW_PIN, HIGH); 
      digitalWrite(UD_CW_PIN, LOW);
      delayMicroseconds(PW_US);
      digitalWrite(LR_CW_PIN, HIGH);
      break;
    case 4: //DOWN
      digitalWrite(DISABLE_PIN, LOW);
      digitalWrite(UD_CW_PIN, HIGH);
      digitalWrite(UD_CCW_PIN, LOW);
      delayMicroseconds(PW_US);
      digitalWrite(UD_CCW_PIN, HIGH);
      break;
    case 5:
      digitalWrite(DISABLE_PIN, HIGH);
      break;
  }
}
