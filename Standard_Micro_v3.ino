#include <Keyboard.h>

//input pins
#define DISPATCH 2
#define B_SW 3
#define C_SW 4
#define A_BT 5
#define B_BT 6
#define C_BT 7
#define ESTOP_BT 8
#define ESTOP_SW 9
#define KEY 10

//outputs pins
#define RESET_LT 16
#define DISPATCH_LT A3
#define A_LT A2 //toggle 2
#define B_LT A1 //toggle 3
#define C_LT A0 //toggle 4
#define ESTOP_LT 15 //toggle 1
#define KEY_LT 14 //toggle 0

//key mappings
#define DISPATCH_KEY 48
#define ESTOP_KEY 49
#define A_On 50
#define A_Off 51
#define B_On 52
#define B_Off 53
#define C_On 54
#define C_Off 55

int
  dispatchWait = 0,
  blinkCount = 20,
  eStopSwtDefault;

boolean
  prev[10],
  toggle[5];

void setup()
{ 
  //set inputs to pullup
  pinMode(DISPATCH,INPUT_PULLUP);
  pinMode(B_SW,INPUT_PULLUP);
  pinMode(C_SW,INPUT_PULLUP);
  pinMode(A_BT,INPUT_PULLUP);
  pinMode(B_BT,INPUT_PULLUP);
  pinMode(C_BT,INPUT_PULLUP);
  pinMode(ESTOP_BT,INPUT_PULLUP);
  pinMode(ESTOP_SW,INPUT_PULLUP);
  pinMode(KEY,INPUT_PULLUP);

  //set pins to outputs
  pinMode(DISPATCH_LT,OUTPUT);
  pinMode(A_LT,OUTPUT);
  pinMode(B_LT,OUTPUT);
  pinMode(C_LT,OUTPUT);
  pinMode(ESTOP_LT,OUTPUT);
  pinMode(KEY_LT,OUTPUT);
  pinMode(RESET_LT,OUTPUT);

  //set to key to unlocked
  toggle[0] = true;
  prev[0] = digitalRead(KEY);

  //get inital e-stop switch position
  eStopSwtDefault = digitalRead(ESTOP_SW);

  //open port
  Keyboard.begin();
}

void loop()
{
  int state, holdCount = 0;
    
  //------process key switch------
  
  //get switch state
  state = digitalRead(KEY);
  
  //test for switch changed
  if(prev[0] != state)
  {
    toggle[0] = !toggle[0];
    ProcessLights(false);//set lighting
  }

  prev[0] = state;
  
  
  //------process e-stops------
  
  //get switch state
  state = digitalRead(ESTOP_SW);
  
  //test for switch changed
  if(prev[1] != state)
  {
    PressKey(ESTOP_KEY); //press estop key
    toggle[1] = !toggle[1];
    ProcessLights(true);//set lighting
  }

  prev[1] = state;


  //get button pressed
  state = digitalRead(ESTOP_BT);
  
  //test for button pressed
  if((prev[2] != state) && (state == LOW))
  {
    PressKey(ESTOP_KEY); //press estop key
    toggle[1] = !toggle[1];
    ProcessLights(true);//set lighting
  }

  prev[2] = state;
  

  //------process dispatch button------
  
  state = digitalRead(DISPATCH); //get input state for button
  
  //see if button has been pressed
  if((state != prev[3]) && (state == LOW) && toggle[0])
  {
      PressKey(DISPATCH_KEY); //press dispatch key
      dispatchWait = 400;
      ProcessLights(true);//set lighting
  }

  prev[3] = state;
  

  //------process function buttons and switches------

  //process aux buttons
  ProcessButton(A_BT, A_On, A_Off, prev[4], toggle[2]);
  ProcessButton(B_BT, B_On, B_Off, prev[5], toggle[3]);
  ProcessButton(C_BT, C_On, C_Off, prev[6], toggle[4]);

  //process aux switches
  ProcessSwitch(B_SW, B_On, B_Off, prev[8]);
  ProcessSwitch(C_SW, C_On, C_Off, prev[9]);

  //------process lights------

  ProcessLights(true);//set lighting
  delay(50); //wait for 0.1 seconds
}


//press a key
void PressKey(int keyNumber)
{
  Keyboard.press(keyNumber); //press key
  delay(50); //wait for 0.05 seconds
  Keyboard.releaseAll(); //release key
}


//process an aux button
void ProcessButton(int pin, int onBtn, int offBtn, bool &prevState, bool &toggleState)
{
  //test if locked
  if(!toggle[0])
    return;
  
  int state = digitalRead(pin); //get input state
  bool decWait = false;

  //test for button pressed
  if((state != prevState) && (state == LOW))
  {
    toggleState = !toggleState;
    
    if(toggleState) //test if aux is on
      PressKey(onBtn); //on
    else
      PressKey(offBtn); //off

    decWait = true;
  }  

  prevState = state;
  ProcessLights(decWait); //update lighting output
}

//process an aux switch
void ProcessSwitch(int pin, int onBtn, int offBtn, bool &prevState)
{
  //test if locked
  if(!toggle[0])
    return;
  
  int state = digitalRead(pin); //get input state
  bool decWait = false;

  //test for switch changed
  if(state != prevState)
  {
    if(state == LOW) //test if aux is on
      PressKey(onBtn); //on
    else
      PressKey(offBtn); //off

    decWait = true;
  }  

  prevState = state;
  ProcessLights(decWait); //update lighting output
}

//set lights to correct on or off
void ProcessLights(boolean decWaits)
{  
  bool
    blinkOn = (blinkCount < 10),
    dispatching = (dispatchWait >= 0);
  
  //set aux and estop lights
  digitalWrite(KEY_LT, toggle[0]);
  digitalWrite(ESTOP_LT, toggle[1] && (digitalRead(ESTOP_SW) == HIGH ||  blinkOn));
  digitalWrite(RESET_LT, toggle[0] && toggle[1] && digitalRead(ESTOP_SW) == eStopSwtDefault && blinkOn);
  digitalWrite(A_LT, toggle[0] && !toggle[1] && !dispatching && (toggle[2] || blinkOn));
  digitalWrite(B_LT, toggle[0] && !toggle[1] && !dispatching && (toggle[3] || blinkOn));
  digitalWrite(C_LT, toggle[0] && !toggle[1] && !dispatching && (toggle[4] || blinkOn));  

  //set dispatch light
  digitalWrite(DISPATCH_LT, toggle[0] && !dispatching && !toggle[1] && !toggle[2] && !toggle[3] && !toggle[4] && blinkOn);
  
  //test if counts should be dec'ed
  if(decWaits)
  {
    dispatchWait = (dispatchWait >= 0) ? dispatchWait - 1 : dispatchWait;
    blinkCount = (blinkCount >= 0) ? blinkCount - 1 : blinkCount;
  }
  
  blinkCount = (blinkCount <= 0) ? 20 : blinkCount;
}
