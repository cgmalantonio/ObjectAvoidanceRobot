////////////////////////////////////////
//     Created by: Cory Malantonio    //
//     Object Avoidance Robot         //
//     September 7, 2012              //
//     Commentary - April 18, 2013    //
//     
////////////////////////////////////////

//ScanDirection, FindDirection, and HandleDirection are the heart and soul of the code, I ended up adding a directional joystick
//to manually steer the OAR and this added to a lot of confusing and probably unnecessary code


#include <AFMotor.h>
#include <Ultrasonic.h>
#include <Servo.h>

int Trigger = 14;
int Echo = 15;

int SensorDirect[] = {
  0,0,0,0,0};           //R/RF/F/LF/L//
int j;
int LNtest1;            //LNscan function
int LNtest2;            //3 readings
int LNtest3;            //
int LNresult;           //final reading before going into SensorDirect[]

int block[] = {         // array for "if path is blocked"
  0,0,0,0,0};           // ended up not using this

int closest = 20;       //Variable for my closest allowed distance
int FD;                 //Furthest Direction

int Right;              // cleans up my array
int FrontRight;         //
int Front;              // makes it easier to know 
int FrontLeft;          // where in the array the values
int Left;               // are stored


int buttonUP = 17;      // pins for parallax 5 position switch
int buttonDOWN = 18;    //
int buttonLEFT = 2;     //
int buttonRIGHT = 16;   //
int buttonCENTER = 19;  //
int buttoncount = 0;    // count the button presses for directional queue
int maxbuttoncount = 8; // same number as queue array

int UPcount = 0;
int DOWNcount = 0;
int LEFTcount = 0;
int RIGHTcount = 0;
int AutoToggle = 0;     // toggles auto mode
Servo myservo;          //creates object to control servo (max of 8 objects)

Ultrasonic ultrasonic(Trigger,Echo); //need the ultrasonic library in your arduino library folder


///////////////////////////SETUP///////////////////////////
void setup(){
  myservo.attach(9);

  Serial.begin(9600);

  pinMode(buttonCENTER, INPUT); //center
  pinMode(buttonUP, INPUT); //up
  pinMode(buttonLEFT, INPUT); //left
  pinMode(buttonDOWN, INPUT); //down
  pinMode(buttonRIGHT, INPUT);  //right

}

void loop(){
  int Queue[]={
    0,0,0,0,0,0,0,0    };
  myservo.write(90);

  Standby();
  ///////////////////////////manual steering/////////////////////////
  if (digitalRead(buttonUP) == LOW){
    Serial.println("Forward");
    Forward();
  }
  if (digitalRead(buttonDOWN) == LOW){
    Serial.println("Backward");
    TurnAround();
  }
  if (digitalRead(buttonLEFT) == LOW){
    Serial.println("Turn Left");
    TurnLeft();
  }
  if (digitalRead(buttonRIGHT) == LOW){
    Serial.println("Turn Right");
    TurnRight();
  }

  //////////////////center click////////8 directional input/////////////
  //idea behind this was a user could input 8 different directions    //
  //on the directional pad, the arduino would remember the directions //
  //then repeat it. Ended up being a lot of fun, trying to get the    //
  //OAR across the room to the next person, then back again.          //
  //////////////////////////////////////////////////////////////////////
  
  if (digitalRead(buttonCENTER) == LOW){
    Serial.println("Center");
    delay(500);
    do {
      if (digitalRead(buttonUP) == LOW) {  
        Queue[buttoncount] = 1;
        buttoncount = buttoncount++;
        Serial.println("Up");
        delay(100);
      }
      if (digitalRead(buttonDOWN) == LOW){ 
        Queue[buttoncount] = 2;
        buttoncount = buttoncount++;
        Serial.println("Down");
        delay(100);
      }
      if (digitalRead(buttonLEFT) == LOW){  
        Queue[buttoncount] = 3;
        buttoncount = buttoncount++;
        Serial.println("Left");
        delay(100);
      }
      if (digitalRead(buttonRIGHT) == LOW){
        Queue[buttoncount] = 4;
        buttoncount = buttoncount++;
        Serial.println("Right");
        delay(100);
      }
      ///////////////////////////Auto Mode///////////////////////////////////
      //This puts the OAR through the basic 3 functions in which it decides//
      //on its own which directions is best for it to travel               //
      ///////////////////////////////////////////////////////////////////////
      if (digitalRead(buttonCENTER) == LOW){
        if (buttoncount == 0) {
          int AutoToggle = 1;
          do{
            ScanDirection();    //OAR functions get called
            FindDirection();    //
            HandleDirection();  //here starts the "fun" :) 
            for (int p = 0; p < 50; p++){
              if (digitalRead(buttonCENTER) == LOW){
                AutoToggle = 0;
                buttoncount = 8;
              }
              delay(10);
            }
            delay(20);
          } 
          while (AutoToggle > 0);
        }     
      } //end if
      delay(100);
    } 
    while (buttoncount < maxbuttoncount);
    //delay(1000);
    buttoncount = 0;
    ///////////////////////////handles 8 directonal inputs/////////////////////
    for (int z = 0; z < maxbuttoncount; z++){
      if (Queue[z] == 1){
        Serial.println("UP");
        Forward();
        delay(500);
      }
      if (Queue[z] == 2){
        Serial.println("DOWN");
        TurnAround();
        delay(500);
      }
      if (Queue[z] == 3){
        Serial.println("LEFT");
        TurnLeft();
        delay(500);
      }
      if (Queue[z] == 4){
        Serial.println("RIGHT");
        TurnRight();
        delay(500);
      }
    }
    delay(2000);
  }
} //end loop

/////////////////////////////////////////////////////////////////////////////////////////////
////////////////////ScanDirection////////////passes off directions to SensorDirect Array/////
//ScanDirection sweeps from left to right stopping at 5 stops to take measurments with     //
//LNscan. It should then know at least the most open direction in which to move            //
//Here there is a lot of room for improvement. Its kinda redundant to sweep back a 2nd time//
/////////////////////////////////////////////////////////////////////////////////////////////

void ScanDirection(){

  int j = 0; //keep j working correctly

  for (int i = 0; i < 180; i = i + 45){  // move sensor from left to right
    myservo.write(i);
    delay(125);
    switch (j){
    case 0: 
      LNscan();
      SensorDirect[0] = LNresult;
      j = j + 1;                      //first time through, j = 1 by end
      break;
    case 1:
      LNscan();
      SensorDirect[1] = LNresult;
      j = j + 1;                     //second time through, j = 2
      break;
    case 2:
      LNscan();
      SensorDirect[2] = LNresult;
      j = j + 1;                     //third time through, j = 3
      break;
    case 3:
      LNscan();
      SensorDirect[3] = LNresult;
      j = j + 1;                     //fourth time through, j = 4
      break;
    }

  }
  for (int i = 180; i > 0; i = i - 45){  // move back from right to left
    myservo.write(i);
    delay(125);
    switch (j){
    case 4: 
      LNscan();
      SensorDirect[4] = LNresult;
      j = j - 1;                      // j = 3 by end
      break;
    case 3:
      LNscan();
      SensorDirect[3] = LNresult;
      j = j - 1;                     // j = 2
      break;
    case 2:
      LNscan();
      SensorDirect[2] = LNresult;
      j = j - 1;                     // j = 1
      break;
    case 1:
      LNscan();
      SensorDirect[1] = LNresult;
      j = j - 1;                     // j = 0
      break;
    }
  }
  myservo.write(90);
  delay(125);

  Serial.print("..");
  Serial.print(SensorDirect[0]);
  Serial.print("..");
  Serial.print(SensorDirect[1]);
  Serial.print("..");
  Serial.print(SensorDirect[2]);
  Serial.print("..");
  Serial.print(SensorDirect[3]);
  Serial.print("..");
  Serial.println(SensorDirect[4]);
} // end Scan Direction

//////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////LNscan//LowestNumberScan////////used by ScanDirection//////////////////////
//Takes in 3 readings, lowest (closest) reading is kept. Because most likely something is in    //
//the way of the sensor. ScanDirection moves the sensor in 5 directions and looks at all the    //
//closest numbers. Which ever way is most clear, it eventually drives in that direction         //
//////////////////////////////////////////////////////////////////////////////////////////////////
void LNscan(){
LNbegin:

  LNtest1 = ultrasonic.Ranging(INC);
  delay(50);
  LNtest2 = ultrasonic.Ranging(INC);
  delay(50);
  LNtest3 = ultrasonic.Ranging(INC);
  delay(50);

  if (LNtest1 >= LNtest2){    // if a > b
    LNtest1 = LNtest2;        // a = b
  }                           // if a > c
  if (LNtest1 >= LNtest3){   // a = c
    LNtest1 = LNtest3;        //
  }                          //    

  if (LNtest1 <= 0){
    goto LNbegin;
  }
  LNresult = LNtest1;        //pass off Lowest test
} // end LNscan


//////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////FindDirection////////////finds Furthest distance (FD) sets up block array/////
//I never did anything with my "Block array", was suppose to just give me extra information     //
//about where objects or bad pathways were, ended up not needing it. FindDirection pretty       //
//much just passes the measurements from LNscan, then ScanDirection to the variables            //
// HandleDirection uses.                                                                        //
//////////////////////////////////////////////////////////////////////////////////////////////////
void FindDirection(){
  FD = 0;

  for (int z = 0; z < 5; z++){                    // Gets furthest distance (FD)
    if (SensorDirect[z] >= closest){         // closest is a global variable i chose for closest allowed distance (~20 inches)
      block[z] = 0;                          
      if (SensorDirect[z] >= FD){            
        FD = SensorDirect[z];          
      }                             
    }                
    else{           
      block[z] = 1;                          // sets block array to 1 if something is blocked
    }                                  
  }         

  Right = SensorDirect[0]; 
  FrontRight = SensorDirect[1]; 
  Front = SensorDirect[2]; 
  FrontLeft = SensorDirect[3]; 
  Left = SensorDirect[4];
} //end Find Direction




//////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////Handle Direction//////////////tells motors to move ///////////////////////////
//This ended up becoming a lot uglier than i am sure was needed. The random numbers were all    //
//kinda fine tuned to my OAR based on trial and error. I tried always having the OAR move in the// 
//furthest direction but found it sitting in the middle of the room going in circles. So i tuned//
//the code to have it choose the forward direction more often, and only turn when it gets to a  //
//wall or object.                                                                               //
//////////////////////////////////////////////////////////////////////////////////////////////////
void HandleDirection(){
  if (FD <= 19){
    TurnAround();
  }
  if (Front >= 40){
    if (FrontLeft >= 20){
      if (FrontRight >= 20){ //if FL and FR are clear
        Forward(); //GO FOWARD
        delay(500);
        Forward();
        Serial.println("FORWARD Twice");
        goto done;
      }
    }
  }
  if (FD <= Front + 10){
    if (FrontLeft >= 10){
      if (FrontRight >= 10){ //if FL and FR are clear
        Forward(); //GO FOWARD
        Serial.println("FORWARD");
        goto done;
      }
    }
  }
  if (FD == FrontRight){
    if (Right >= 20){
      HeadTurnRight();
      SlightRight(); //TURN HALF RIGHT
      goto done;
      Serial.println("Slight RIGHT");
    }
    else{
      if (Left >= 20 && FrontLeft >= 20){

        Serial.println("front right blocked, left turn");
        HeadTurnLeft();
        TurnLeft();
        goto done;
      }
      else{
        Serial.println("turn around");
        TurnAround();
        goto done;
      }
    }
  }
  if (FD == FrontLeft){
    if (Left >= 20){
      HeadTurnLeft();
      SlightLeft(); //TURN HALF LEFT
      goto done;
      Serial.println("Slight LEFT");
    }
    else{
      if (Right >= 20 && FrontRight >= 20){
        Serial.println("front left blocked, right turn");
        HeadTurnRight();
        TurnRight();
        goto done;
      }
      else{
        Serial.println("turn around");
        TurnAround();
        goto done;
      }
    }
  }
  if (FD == Right){  
    if (FrontRight >= 20){  
      HeadTurnRight();
      TurnRight();  //TURN RIGHT
      goto done;
      Serial.println("RIGHT");
    }
    else{
      if (Left >= 20 && FrontLeft >= 20){
        Serial.println("Right didnt work, turning left");
        HeadTurnLeft();
        TurnLeft();
        goto done;
      }
      else{
        Serial.println("turn around");
        TurnAround();
        goto done;
      }
    }
  }
  if (FD == Left){
    if (FrontLeft >= 20){
      HeadTurnLeft();
      TurnLeft(); //TURN LEFT
      goto done;
      Serial.println("LEFT");
    }
    else{
      if (Right >= 20 && FrontRight >=20){
        Serial.println("Left didnt work, turning right");
        HeadTurnRight();
        TurnRight();
        goto done;
      }
      else{
        Serial.println("turn around");
        TurnAround();
        goto done;
      }
    }
  }
  if (Front <= 10){
    if (FrontLeft <= 15 || Left <= 15){
      if (FrontRight <= 15 || Right <= 15){
        ShakeHead();
        Backward();
        delay(500);
        TurnAround();
        Serial.println("Back up and turn around");
      }
    }
  }

done: 

  Serial.print(FD);
  Serial.println("..FD");

} //end HandleDirection

//////////////////////////////////////robot response functions//////////////////////////////////
//Wanted to try and make the OAR a little more user friendly                                  //
////////////////////////////////////////////////////////////////////////////////////////////////

void Standby(){
  LNscan(); 
  if (LNresult <= 5){
    Backward();
    delay(500);
    ShakeHead();
    Serial.println("backup, your too close");
  }  
}//end Standby();

void ShakeHead(){

  for (int UD = 70; UD < 111; UD++){
    myservo.write(UD);
    delay(10);
  }
  for (int UD = 110; UD > 69; UD--){
    myservo.write(UD);
    delay(10);
  }  
  for (int UD = 70; UD < 111; UD++){
    myservo.write(UD);
    delay(10);
  }
  for (int UD = 110; UD > 69; UD--){
    myservo.write(UD);
    delay(10);
  }  
  myservo.write(90);  //bring back to center
}

void HeadTurnRight(){
  for (int UD = 90; UD > 50; UD--){
    myservo.write(UD);
    delay(10);
  }
  myservo.write(90);  //bring back to center
  delay(10);
}

void HeadTurnLeft(){
  for (int UD = 90; UD < 130; UD++){
    myservo.write(UD);
    delay(10);
  }
  myservo.write(90);  //bring back to center
  delay(10);
}






