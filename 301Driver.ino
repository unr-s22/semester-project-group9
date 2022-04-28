#include <LiquidCrystal.h>

class SwampCooler;
class StateInterface;
class DisabledState;
class IdleState;
class RunningState;
class ErrorState;

int getWaterLev();
int getTemp();

void disableAll();
void setDisabledOutputs();
void setIdleOutputs();
void setRunningOutputs();
void setErrorOutputs();


int waterthreshold = 3;
int temphighthreshold = 143;
int templowthreshold = 140;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int waterlevel; //holds latest water level
int temperature;  //holds latest temperature

class StateInterface{
  protected:
    SwampCooler *sc;
  public:
    virtual void disable_enable();
    virtual void checkWater();
    virtual void checkTemp();
};

class DisabledState : public StateInterface{
  public:
    DisabledState(SwampCooler *s);
    void disable_enable();
    void checkWater();
    void checkTemp();
};

class IdleState : public StateInterface{
  public:
    IdleState(SwampCooler *s);
    void disable_enable();
    void checkWater();
    void checkTemp();
};

class RunningState : public StateInterface{
  public:
    RunningState(SwampCooler *s);
    void disable_enable();
    void checkWater();
    void checkTemp();
};

class ErrorState : public StateInterface{
  public:
    ErrorState(SwampCooler *s);
    void disable_enable();
    void checkWater();
    void checkTemp();
};


class SwampCooler{
  StateInterface *currentstate;
  DisabledState disabled;
  IdleState idle;
  RunningState running;
  ErrorState error;
  
  public:
    int state;
  
    SwampCooler();
    void update();
    void setDisabled();
    void setIdle();
    void setRunning();
    void setError();
};


//DisabledState
DisabledState::DisabledState(SwampCooler *s){
  sc = s;
}

void  DisabledState::disable_enable(){
  sc->state = 1;
  setIdleOutputs();
  sc->setIdle();
}
void DisabledState::checkWater(){
}
void DisabledState::checkTemp(){
}

//IdleState
IdleState::IdleState(SwampCooler *s){
  sc = s;
}

void  IdleState::disable_enable(){
  setDisabledOutputs();
  sc->state = 0;
  sc->setDisabled();
}
void IdleState::checkWater(){
  if(getWaterLev()<waterthreshold)
  {
    setErrorOutputs();
    sc->state = 3;
    sc->setError();
  }
}
void IdleState::checkTemp(){
  if(getTemp()>temphighthreshold)
  {
    setRunningOutputs();
    sc->state = 2;
    sc->setRunning();
  }
}

//RunningState
RunningState::RunningState(SwampCooler *s){
  sc = s;
}

void  RunningState::disable_enable(){
  setDisabledOutputs();
  sc->state = 0;
  sc->setDisabled();
}
void RunningState::checkWater(){
  if(getWaterLev()<waterthreshold)
  {
    setErrorOutputs();
    sc->state = 3;
    sc->setError();
  }
}
void RunningState::checkTemp(){
  if(getTemp()<templowthreshold)
  {
    setIdleOutputs();
    sc->state = 1;
    sc->setIdle();
  }
}

//ErrorState
ErrorState::ErrorState(SwampCooler *s){
  sc = s;
}

void  ErrorState::disable_enable(){
  setDisabledOutputs();
  sc->state = 0;
  sc->setDisabled();
}
void ErrorState::checkWater(){
  if(getWaterLev()>waterthreshold)
  {
    sc->state = 1;
    setIdleOutputs();
    sc->setIdle();
  }
}
void ErrorState::checkTemp(){
}


//SwampCooler
SwampCooler::SwampCooler() : disabled{this},idle{this},running{this},error{this}{
  state = 1;
  currentstate = &idle;
}

void SwampCooler::update(){
  //Serial.println(state);
  currentstate->checkWater();
  currentstate->checkTemp();
}
  
void SwampCooler::setDisabled(){
  currentstate = &disabled;
}
void SwampCooler::setIdle(){
  currentstate = &idle;
}
void SwampCooler::setRunning(){
  currentstate = &running;
}
void SwampCooler::setError(){
  currentstate = &error;
}


//Output
void disableAll(){
  Serial.println("Red off");
  Serial.println("Green off");
  Serial.println("Blue off");
  Serial.println("Yellow off");
  Serial.println("Motor off");
  //Serial.println("Servo to closed state");
}

void setDisabledOutputs(){
  disableAll();
  Serial.println("Yellow on");
}

void setIdleOutputs(){
  disableAll();
  Serial.println("Green on");
  Serial.print("Changed state to idle at: ");
  Serial.println(millis()/1000.0f);
}

void setRunningOutputs(){
  disableAll();
  Serial.print("Changed state to running at: ");
  Serial.println(millis()/1000.0f);
  Serial.println("Blue on");
}

void setErrorOutputs(){
  disableAll();
  Serial.println("Red on");
}


int getWaterLev(){
  waterlevel = analogRead(A0);
  return waterlevel;
}

int getTemp(){
   temperature = analogRead(A1);
   return temperature;
}

//main

SwampCooler swampcooler;
bool statechange = 1;  //check if state has changed
int staterequest = 0;
//int state = 0;
//states:
//0- disabled, 1-idle, 2-running, 3-error

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
}

void loop() {

  delay(200);
  swampcooler.update();
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Water Level: ");
  lcd.print(waterlevel);
  lcd.setCursor(0,1);
  lcd.print("Temperature: ");
  lcd.print(temperature);
  

}
