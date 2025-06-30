#pragma region VEXcode Generated Robot Configuration
// Make sure all required headers are included.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


#include "vex.h"

using namespace vex;

// Brain should be defined by default
brain Brain;


// START V5 MACROS
#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
// END V5 MACROS


// Robot configuration code.
motor LeftDriveSmart = motor(PORT1, ratio18_1, false);
motor RightDriveSmart = motor(PORT2, ratio18_1, true);
drivetrain Drivetrain = drivetrain(LeftDriveSmart, RightDriveSmart, 319.19, 295, 40, mm, 1);

motor Claw = motor(PORT3, ratio18_1, false);

motor Arm = motor(PORT4, ratio36_1, false);

// AI Vision Color Descriptions
aivision::colordesc AIVision1__Blue(1, 27, 103, 204, 20, 0.4);
aivision::colordesc AIVision1__Red(2, 191, 2, 52, 20, 0.4);
aivision::colordesc AIVision1__Green(3, 34, 167, 75, 20, 0.4);
aivision::colordesc AIVision1__Orange(4, 203, 67, 64, 8, 0.1);
aivision::colordesc AIVision1__Red2(5, 233, 22, 36, 10, 0.2);
aivision::colordesc AIVision1__Red3(6, 233, 1, 27, 10, 0.2);
// AI Vision Code Descriptions
vex::aivision AIVision1(PORT13, AIVision1__Blue, AIVision1__Red, AIVision1__Green, AIVision1__Orange, AIVision1__Red2, AIVision1__Red3, aivision::ALL_TAGS);

controller Controller1 = controller(primary);
inertial Inertial1 = inertial(PORT7);

distance Distance1 = distance(PORT12);


// generating and setting random seed
void initializeRandomSeed(){
  int systemTime = Brain.Timer.systemHighResolution();
  double batteryCurrent = Brain.Battery.current();
  double batteryVoltage = Brain.Battery.voltage(voltageUnits::mV);

  // Combine these values into a single integer
  int seed = int(batteryVoltage + batteryCurrent * 100) + systemTime;

  // Set the seed
  srand(seed);
}



void vexcodeInit() {

  //Initializing random seed.
  initializeRandomSeed(); 
}


// Helper to make playing sounds from the V5 in VEXcode easier and
// keeps the code cleaner by making it clear what is happening.
void playVexcodeSound(const char *soundName) {
  printf("VEXPlaySound:%s\n", soundName);
  wait(5, msec);
}



// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
// define variables used for controlling motors based on controller inputs
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;

// define a task that will handle monitoring inputs from Controller1
int rc_auto_loop_function_Controller1() {
  // process the controller input every 20 milliseconds
  // update the motors based on the input values
  while(true) {
    if(RemoteControlCodeEnabled) {
      
      // calculate the drivetrain motor velocities from the controller joystick axies
      // left = Axis3 + Axis1
      // right = Axis3 - Axis1
      int drivetrainLeftSideSpeed = Controller1.Axis3.position() + Controller1.Axis1.position();
      int drivetrainRightSideSpeed = Controller1.Axis3.position() - Controller1.Axis1.position();
      
      // check if the value is inside of the deadband range
      if (drivetrainLeftSideSpeed < 5 && drivetrainLeftSideSpeed > -5) {
        // check if the left motor has already been stopped
        if (DrivetrainLNeedsToBeStopped_Controller1) {
          // stop the left drive motor
          LeftDriveSmart.stop();
          // tell the code that the left motor has been stopped
          DrivetrainLNeedsToBeStopped_Controller1 = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the left motor nexttime the input is in the deadband range
        DrivetrainLNeedsToBeStopped_Controller1 = true;
      }
      // check if the value is inside of the deadband range
      if (drivetrainRightSideSpeed < 5 && drivetrainRightSideSpeed > -5) {
        // check if the right motor has already been stopped
        if (DrivetrainRNeedsToBeStopped_Controller1) {
          // stop the right drive motor
          RightDriveSmart.stop();
          // tell the code that the right motor has been stopped
          DrivetrainRNeedsToBeStopped_Controller1 = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the right motor next time the input is in the deadband range
        DrivetrainRNeedsToBeStopped_Controller1 = true;
      }
      
      // only tell the left drive motor to spin if the values are not in the deadband range
      if (DrivetrainLNeedsToBeStopped_Controller1) {
        LeftDriveSmart.setVelocity(drivetrainLeftSideSpeed, percent);
        LeftDriveSmart.spin(forward);
      }
      // only tell the right drive motor to spin if the values are not in the deadband range
      if (DrivetrainRNeedsToBeStopped_Controller1) {
        RightDriveSmart.setVelocity(drivetrainRightSideSpeed, percent);
        RightDriveSmart.spin(forward);
      }
    }
    // wait before repeating the process
    wait(20, msec);
  }
  return 0;
}

task rc_auto_loop_task_Controller1(rc_auto_loop_function_Controller1);

#pragma endregion VEXcode Generated Robot Configuration

// ----------------------------------------------------------------------------
//                                                                            
//    Project:        
//    Description:    
//    Configuration:  V5 Clawbot or Advanced TrainingBot (Drivetrain 2-motor, No Gyro)
//                    Claw Motor in Port 3
//                    Arm Motor in Port 8          
//                                                                            
// ----------------------------------------------------------------------------

#include "vex.h"

using namespace vex;
#include <iostream>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>
#include <string>

//Getting to_string
template <typename T>
std::string to_string(T value)
{
    std::ostringstream os;
    os << value ;
    return os.str() ;
}

//Creating Events
event checkRed = event();
event checkBlue = event();
event checkGreen = event();
event cLocate = event();
event aprilTags = event();
event logic = event();

//Variable storing whether or not the navigation is complete
bool complete = false;

//Variables for storing which cube is in which location
char topCube[] = "F";
char bottomCube[] = "F";

int blueCount = 0;
int redCount = 0;
int greenCount = 0;

//Determines the location of each cube
void colorLocatingCallback() 
{
  //Checking Whether Event Calls
  //Brain.Screen.setCursor(6, 1);
  //Brain.Screen.print("Event Called");


  //Grabbing the count of each object type
  AIVision1.takeSnapshot(AIVision1__Blue);
  /*for(int i = 0; i < AIVision1.objectCount; i++)
  {
    if(AIVision1.objects[i].centerX < 190 && AIVision1.objects[i].centerX < 130)
    {
      blueCount += 1;
    }
  }*/
  blueCount = AIVision1.objectCount;
 AIVision1.takeSnapshot(AIVision1__Red);
  /*  for(int i = 0; i < AIVision1.objectCount; i++)
  {
    if(AIVision1.objects[i].centerX < 190 && AIVision1.objects[i].centerX < 130)
    {
      redCount += 1;
    }
  }*/
  redCount = AIVision1.objectCount;
  //AIVision1.takeSnapshot(AIVision1__Red2);
  //redCount += AIVision1.objectCount;
  //AIVision1.takeSnapshot(AIVision1__Red3);
  //redCount += AIVision1.objectCount;
  AIVision1.takeSnapshot(AIVision1__Green);
  /*  for(int i = 0; i < AIVision1.objectCount; i++)
  {
    if(AIVision1.objects[i].centerX < 190 && AIVision1.objects[i].centerX < 130)
    {
      greenCount += 1;
    }
  }*/
  greenCount = AIVision1.objectCount;
  
  //Checking Whether Event Calls
  Brain.Screen.setCursor(10, 1);
  Brain.Screen.print("blueCount");

        Brain.Screen.setCursor(11, 1);
      Brain.Screen.print("greenCount");


  AIVision1.takeSnapshot(aivision::ALL_TAGS);
  int aprilCount = AIVision1.objectCount;

  for(int i = -1; i < aprilCount; i++)
  {
    if(AIVision1.objects[i].id == 20)
    {
      topCube[0] = 'O';
      bottomCube[0] = 'O';
    }
  }

  AIVision1.takeSnapshot(AIVision1__Orange);
  int orangeCount = AIVision1.objectCount;

  //Checking Whether it sees Orange (STOP)
  if ((orangeCount > 0 && AIVision1.largestObject.height > 150))
  {
    topCube[0] = 'O';
    bottomCube[0] = 'O';
  }
  //Checking Whether Blue|Red Combination
  else if (blueCount > 0 && redCount > 0)
  {
    //Finding Coordinates of Blue
    AIVision1.takeSnapshot(AIVision1__Blue);
    int coord1 = AIVision1.largestObject.centerY;

      Brain.Screen.setCursor(1, 1);
      Brain.Screen.print("Coord1 Var: ");
      Brain.Screen.print(coord1);

    //Finding Coordinates of Red
    AIVision1.takeSnapshot(AIVision1__Red);
    int coord2 = AIVision1.largestObject.centerY;
      Brain.Screen.setCursor(2, 1);
      Brain.Screen.print("Coord2 Var: ");
      Brain.Screen.print(coord2);

    //Assigning Location
    if(coord1 > coord2)
    {
      topCube[0] = 'R';
      bottomCube[0] = 'B';
    }
    else if (coord2 > coord1)
    {
      topCube[0] = 'B';
      bottomCube[0] = 'R';
    }
    else
    {
      Brain.Screen.setCursor(7, 1);
      Brain.Screen.print("SEND HELP, PLEASE! SOMETHING'S WRONG WITH THE FEELINGS");
    }
  }
  //Checking whether Red|Green Combination
  else if (redCount > 0 && greenCount > 0) 
  {
    //Finding Coordinates of Green
    AIVision1.takeSnapshot(AIVision1__Green);
    int coord1 = AIVision1.largestObject.centerY;

    //Finding Coordinates of Red
    AIVision1.takeSnapshot(AIVision1__Red);
    int coord2 = AIVision1.largestObject.centerY;

    //Assigning Location
    if(coord1 > coord2)
    {
      topCube[0] = 'R';
      bottomCube[0] = 'G';
    }
    else if (coord2 > coord1)
    {
      topCube[0] = 'G';
      bottomCube[0] = 'R';
    }
    else
    {
      Brain.Screen.setCursor(7, 1);
      Brain.Screen.print("SEND HELP, PLEASE! SOMETHING'S WRONG WITH THE FEELINGS");
    }
  }
  //Checking Whether Green|Blue Combination
  else if (greenCount > 0 && blueCount > 0)  
  {
    //Finding Coordinates of Blue
    AIVision1.takeSnapshot(AIVision1__Blue);
    int coord1 = AIVision1.largestObject.centerY;
    //Finding Coordinates of Green
    AIVision1.takeSnapshot(AIVision1__Green);
    int coord2 = AIVision1.largestObject.centerY;

    //Assigning Location
    if(coord1 > coord2)
    {
      topCube[0] = 'G';
      bottomCube[0] = 'B';
    }
    else if (coord2 > coord1)
    {
      topCube[0] = 'B';
      bottomCube[0] = 'G';
    }
    else
    {
      Brain.Screen.setCursor(7, 1);
      Brain.Screen.print("SEND HELP, PLEASE! SOMETHING'S WRONG WITH THE FEELINGS");
    }
  }
  //Green|Green Combination
  else if(greenCount > 0)
  {
    topCube[0] = 'G';
    bottomCube[0] = 'G';
  }
  //Blue|Blue Combination
  else if(blueCount > 0)
  {
    topCube[0] = 'B';
    bottomCube[0] = 'B';
  }
  //Red|Red Combination
  else if(redCount > 0)
  {
    topCube[0] = 'R';
    bottomCube[0] = 'R';
  }
  else
  {
    topCube[0] = 'F';
    bottomCube[0] = 'F';
  }

}





//April Tag Function Variables
std::vector<int> aprilIDs = {-1};
std::string toMessage = "";

//Function that manages the April Tags
void AprilTagsCallback() 
{
  //Taking Snapshot of all AIvision Tags
  AIVision1.takeSnapshot(aivision::ALL_TAGS);

  if (AIVision1.objectCount > 0)
  {
    //Variables for the "For Loop"
    int length = aprilIDs.size();
    bool exists = false;

    //Creating a variable that holds whether or not the April Tag is already in the array
    for (int i = 0; i <= length; i++)
    {
      if(int(AIVision1.largestObject.id) == aprilIDs[i]) {
        exists = true;
      }
    }

    //If April Tag is new and not previously in the array
    if (!exists)
    {
      //Adding April Tag ID to array of IDs
      aprilIDs.push_back(AIVision1.largestObject.id);

      //Debug Info for April Tag system
      Brain.Screen.setCursor(10, 1);
      Brain.Screen.print("Most Recent April Tag: ");
      Brain.Screen.print(aprilIDs.back());

      //Adding the ID to the Message
      int numb = aprilIDs.back();
      toMessage += (to_string(numb) + "\n");
    }

    //Uploading to SD Card
    std::ofstream MyFile("Message.txt");
    MyFile << toMessage;
    MyFile.close();
  }

}



int blueWidth = 0;
int blueDistance = 5000;
int redWidth = 0;
int redDistance = 5000;
int greenWidth = 0;
int greenDistance = 5000;
int orangeWidth = 0;
int orangeDistance = 5000;

void Adjust() 
{
  //Grabbing the width of each object type
  AIVision1.takeSnapshot(AIVision1__Blue);
  if(AIVision1.objectCount > 0)
  {
    blueWidth = AIVision1.largestObject.width;
    blueDistance = std::abs(AIVision1.largestObject.centerX - 150);
  }
  else
  {
    blueWidth = 0;
    blueDistance = 5000;
  }

  AIVision1.takeSnapshot(AIVision1__Red);
  if(AIVision1.objectCount > 0)
  {
    redWidth  = AIVision1.largestObject.width;
    redDistance = std::abs(AIVision1.largestObject.centerX - 150);
  }
  else
  {
    redWidth = 0;
    redDistance = 5000;
  }

  AIVision1.takeSnapshot(AIVision1__Green);
  if(AIVision1.objectCount > 0)
  {
    greenWidth = AIVision1.largestObject.width;
    greenDistance = std::abs(AIVision1.largestObject.centerX - 150);
  }
  else
  {
    greenWidth = 0;
    greenDistance = 5000;
  }

  AIVision1.takeSnapshot(AIVision1__Orange);
  if(AIVision1.objectCount > 0)
  {
    orangeWidth = AIVision1.largestObject.width;
    orangeDistance = std::abs(AIVision1.largestObject.centerX - 150);
  }
  else
  {
    orangeWidth = 0;
    orangeDistance = 5000;
  }


  if ((greenDistance < redDistance && greenDistance < blueDistance)) // && (greenWidth > redDistance && greenWidth > blueWidth)
  {
    // Take a snapshot to check for detected objects
    AIVision1.takeSnapshot(AIVision1__Green);

    // Check if there is an existing object at the start of the array
    if (AIVision1.objectCount > 0) {

      if (AIVision1.largestObject.centerX > 175.0) 
      {
        // turn in 5 degree increments to avoid the risk of turning too far
        Drivetrain.turnFor(right, 3.0, degrees, true);
        wait(0.1, seconds);
        Drivetrain.drive(forward);
      } 
      else if (AIVision1.largestObject.centerX < 145.0) 
      {
        // turn in 5 degree increments to avoid the risk of turning too far
        Drivetrain.turnFor(left, 3.0, degrees, true);
        wait(0.1, seconds);
        Drivetrain.drive(forward);
      } 
    }
  }
  else if((blueDistance < redDistance && blueDistance < greenDistance)) // && (blueWidth > redDistance && blueWidth > greenWidth)
  {
    // Take a snapshot to check for detected objects
    AIVision1.takeSnapshot(AIVision1__Blue);

    // Check if there is an existing object at the start of the array
    if (AIVision1.objectCount > 0) {

      if (AIVision1.largestObject.centerX > 175.0) 
      {
        // turn in 5 degree increments to avoid the risk of turning too far
        Drivetrain.turnFor(right, 3.0, degrees, true);
        wait(0.1, seconds);
        Drivetrain.drive(forward);
      } 
      else if (AIVision1.largestObject.centerX < 145.0) 
      {
        // turn in 5 degree increments to avoid the risk of turning too far
        Drivetrain.turnFor(left, 3.0, degrees, true);
        wait(0.1, seconds);
        Drivetrain.drive(forward);
      } 
    }
  }
  else if ((redDistance < greenDistance && redDistance < blueDistance)) //  && (redWidth > blueDistance && redWidth > greenWidth)
  {
    // Take a snapshot to check for detected objects
    AIVision1.takeSnapshot(AIVision1__Red);

    // Check if there is an existing object at the start of the array
    if (AIVision1.objectCount > 0) {

      if (AIVision1.largestObject.centerX > 175.0) 
      {
        // turn in 5 degree increments to avoid the risk of turning too far
        Drivetrain.turnFor(right, 3.0, degrees, true);
        wait(0.1, seconds);
        Drivetrain.drive(forward);
      } 
      else if (AIVision1.largestObject.centerX < 145.0) 
      {
        // turn in 5 degree increments to avoid the risk of turning too far
        Drivetrain.turnFor(left, 3.0, degrees, true);
        wait(0.1, seconds);
        Drivetrain.drive(forward);
      } 
    }
  }
  else if ((orangeDistance < greenDistance && orangeDistance < blueDistance)) //  && (redWidth > blueDistance && redWidth > greenWidth)
  {
    // Take a snapshot to check for detected objects
    AIVision1.takeSnapshot(AIVision1__Red);

    // Check if there is an existing object at the start of the array
    if (AIVision1.objectCount > 0) {

      if (AIVision1.largestObject.centerX > 175.0) 
      {
        // turn in 5 degree increments to avoid the risk of turning too far
        Drivetrain.turnFor(right, 3.0, degrees, true);
        wait(0.1, seconds);
        Drivetrain.drive(forward);
      } 
      else if (AIVision1.largestObject.centerX < 145.0) 
      {
        // turn in 5 degree increments to avoid the risk of turning too far
        Drivetrain.turnFor(left, 3.0, degrees, true);
        wait(0.1, seconds);
        Drivetrain.drive(forward);
      } 
    }
  }
}





//Functions for checking and adjusting for innacuracies in turning and distancing
/*
void InnacuracyTurn(int degree)
{
  Drivetrain.stop();
  wait(100, msec);

  //Lower velocity for more precision
  Drivetrain.setTurnVelocity(15, percent);

  //Identify the neccesary adjustments
  float head = Inertial1.heading(degrees);
  float newHead = std::abs(degree)-std::abs(head);

  Brain.Screen.setCursor(3, 1);
  Brain.Screen.print(newHead);
  Brain.Screen.setCursor(10, 1);
  Brain.Screen.print(head);

  if(newHead < -180)
  {
    newHead += 180;
  }
  else if(newHead > 180)
  {
    newHead -= 180;
  }

  //Adjust course
  if(newHead < 0 && std::abs(newHead) != 0)
  {
    Drivetrain.turnFor(left, std::abs(newHead), degrees);
  }
  else if (std::abs(newHead) != 0)
  {
    Drivetrain.turnFor(right, std::abs(newHead), degrees);
  }

  //Return to normal velocity
  Drivetrain.setTurnVelocity(50, percent);
}
void InnacuracyDistance() 
{
  Drivetrain.stop();
  wait(100, msec);

  //Lower velocity for more precision
  Drivetrain.setDriveVelocity(10, percent);

  //Identify the neccesary adjustments
  float dis = Distance1.objectDistance(inches);

  //Adjust course
  if(dis > 4)
  {
    Drivetrain.driveFor(forward, (4-dis), inches);
  }
  else if(dis < 4)
  {
    Drivetrain.driveFor(reverse, (dis-4), inches);
  }

  //Return to normal velocity
  Drivetrain.setDriveVelocity(35, percent);
} */

//Functions for Movement
void BlueMovement() {
  Brain.Screen.setCursor(11, 1);
  Brain.Screen.print(Inertial1.heading(degrees));

  Inertial1.setHeading(0, degrees);
  Drivetrain.turnFor(right, 80, degrees);
  //InnacuracyTurn(90);

  Drivetrain.drive(forward);
}
void RedMovement() {
  Brain.Screen.setCursor(11, 1);
  Brain.Screen.print(Inertial1.heading(degrees));

  Inertial1.setHeading(0, degrees);
  Drivetrain.turnFor(right, 190, degrees);
  //RightDriveSmart.spinFor(forward, 180, degrees);
  //InnacuracyTurn(180);

  Drivetrain.drive(forward);
}
void GreenMovement() {
  Brain.Screen.setCursor(11, 1);
  Brain.Screen.print(Inertial1.heading(degrees));

  Inertial1.setHeading(0, degrees);
  Drivetrain.turnFor(left, 80, degrees);
  //InnacuracyTurn(-90);

  Drivetrain.drive(forward);
}




//Logic Function -- The strings that weave the rest of the functions together, asking the questions that need to be asked before movements
void LogicCallback() 
{
  //Checking if Object is within range
  if(Distance1.objectDistance(inches) < 8)
  {
    //InnacuracyDistance();
    Brain.Screen.setCursor(11, 1);
    Brain.Screen.print("In Distance");  

    //If Orange Cube
    if(topCube[0] == 'O' && bottomCube[0] == 'O')
    {
      Controller1.Screen.setCursor(2, 1);
      Controller1.Screen.print("Stopping, Saw Orange       ");
      Brain.Screen.setCursor(7, 1);
      Brain.Screen.print("Stopping, Saw Orange       ");

      Drivetrain.stop();
      complete = true;
    }
    //If Red Cube on Top
    if ((topCube[0] == 'R' && bottomCube[0] == 'B') || (topCube[0] == 'R' && bottomCube[0] == 'G') || (topCube[0] == 'R' && bottomCube[0] == 'R'))
    {
      Drivetrain.stop();
      wait(100, msec);

      Controller1.Screen.setCursor(2, 1);
      Controller1.Screen.print("Turning Around, Saw Red     ");
      Brain.Screen.setCursor(7, 1);
      Brain.Screen.print("Turning Around, Saw Red     ");

      RedMovement();
    }
    //If Blue Cube on Top
    else if ((topCube[0] == 'B' && bottomCube[0] == 'R') || (topCube[0] == 'B' && bottomCube[0] == 'B') || (topCube[0] == 'B' && bottomCube[0] == 'B'))
    {
      Drivetrain.stop();
      wait(100, msec);

      Controller1.Screen.setCursor(2, 1);
      Controller1.Screen.print("Turning Right, Saw Blue     ");
      Brain.Screen.setCursor(7, 1);
      Brain.Screen.print("Turning Right, Saw Blue     ");

      BlueMovement();
    }
    //If Green Cube on Top
    else if ((topCube[0] == 'G' && bottomCube[0] == 'B') || (topCube[0] == 'G' && bottomCube[0] == 'G') || (topCube[0] == 'G' && bottomCube[0] == 'R'))
    {
      Drivetrain.stop();
      wait(100, msec);

      Controller1.Screen.setCursor(2, 1);
      Controller1.Screen.print("Turning Left, Saw Green     ");
      Brain.Screen.setCursor(7, 1);
      Brain.Screen.print("Turning Left, Saw Green     ");

      GreenMovement();
    }
  }
}




//Begin project code
int main() {
  complete = false;

  //Landon)) Slow The Robot
  Drivetrain.setDriveVelocity(25, percent);
  Drivetrain.setTurnVelocity(5, percent);
  //Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  //Calibrating Inertial Calibration
  Inertial1.calibrate();

  //Register event handlers
  cLocate(colorLocatingCallback);
  aprilTags(AprilTagsCallback);
  logic(LogicCallback);

  //Small delay to ensure that the event handlers are ready to be used AND calibration
  wait(3, sec);

  //Begin Movement
  Drivetrain.drive(forward);

  Arm.setStopping(hold);
  Arm.spinToPosition(110, degrees);

  while (!complete) {

    //Checking for new sensor data
    cLocate.broadcastAndWait();
    aprilTags.broadcastAndWait();
    logic.broadcastAndWait();
    wait(0.1, seconds);

    //********************
    //Brain Debug Messages
    //********************

    //Blue Object Check
    Brain.Screen.setCursor(4, 1);
    AIVision1.takeSnapshot(AIVision1__Blue);
    Brain.Screen.print("Number of Blue Objects: ");
    Brain.Screen.print(AIVision1.objectCount);
    //Red Object Check
    Brain.Screen.setCursor(5, 1);
    AIVision1.takeSnapshot(AIVision1__Red);
    Brain.Screen.print("Number of Red Objects: ");
    Brain.Screen.print(AIVision1.objectCount);

    //Location of the Cube
    Brain.Screen.setCursor(8, 1);
    Brain.Screen.print("Top Cube Var: ");
    Brain.Screen.print(topCube);
    Brain.Screen.setCursor(9, 1);
    Brain.Screen.print("Bottom Cube Var: ");
    Brain.Screen.print(bottomCube);

    //Current Heading
    Brain.Screen.setCursor(1, 1);
    Controller1.Screen.print("Current Heading: ");
    Controller1.Screen.print(Inertial1.heading(degrees));

    Adjust();
  }

  Drivetrain.stop();
}