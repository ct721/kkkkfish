#include "vex.h"

using namespace vex;
competition Competition;

/*---------------------------------------------------------------------------*/
/*                             VEXcode Config                                */
/*                                                                           */
/*  Before you do anything else, start by configuring your motors and        */
/*  sensors. In VEXcode Pro V5, you can do this using the graphical          */
/*  configurer port icon at the top right. In the VSCode extension, you'll   */
/*  need to go to robot-config.cpp and robot-config.h and create the         */
/*  motors yourself by following the style shown. All motors must be         */
/*  properly reversed, meaning the drive should drive forward when all       */
/*  motors spin forward.                                                     */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                             JAR-Template Config                           */
/*                                                                           */
/*  Where all the magic happens. Follow the instructions below to input      */
/*  all the physical constants and values for your robot. You should         */
/*  already have configured your motors.                                     */
/*---------------------------------------------------------------------------*/

Drive chassis(

//Pick your drive setup from the list below:
//ZERO_TRACKER_NO_ODOM
//ZERO_TRACKER_ODOM
//TANK_ONE_FORWARD_ENCODER
//TANK_ONE_FORWARD_ROTATION
//TANK_ONE_SIDEWAYS_ENCODER
//TANK_ONE_SIDEWAYS_ROTATION
//TANK_TWO_ENCODER
//TANK_TWO_ROTATION
//HOLONOMIC_TWO_ENCODER
//HOLONOMIC_TWO_ROTATION
//
//Write it here:
ZERO_TRACKER_ODOM,

//Add the names of your Drive motors into the motor groups below, separated by commas, i.e. motor_group(Motor1,Motor2,Motor3).
//You will input whatever motor names you chose when you configured your robot using the sidebar configurer, they don't have to be "Motor1" and "Motor2".

//Left Motors:
motor_group(LeftFront, LeftMiddle, LeftBack),

//Right Motors:
motor_group(RightFront, RightMiddle, RightBack),

//Specify the PORT NUMBER of your inertial sensor, in PORT format (i.e. "PORT1", not simply "1"):
PORT21,

//Input your wheel diameter. (4" omnis are actually closer to 4.125"):
3.25,

//External ratio, must be in decimal, in the format of input teeth/output teeth.
//If your motor has an 84-tooth gear and your wheel has a 60-tooth gear, this value will be 1.4.
//If the motor drives the wheel directly, this value is 1:
0.75,

//Gyro scale, this is what your gyro reads when you spin the robot 360 degrees.
//For most cases 360 will do fine here, but this scale factor can be very helpful when precision is necessary.
355,

/*---------------------------------------------------------------------------*/
/*                                  PAUSE!                                   */
/*                                                                           */
/*  The rest of the drive constructor is for robots using POSITION TRACKING. */
/*  If you are not using position tracking, leave the rest of the values as  */
/*  they are.                                                                */
/*---------------------------------------------------------------------------*/

//If you are using ZERO_TRACKER_ODOM, you ONLY need to adjust the FORWARD TRACKER CENTER DISTANCE.

//FOR HOLONOMIC DRIVES ONLY: Input your drive motors by position. This is only necessary for holonomic drives, otherwise this section can be left alone.
//LF:      //RF:    
PORT1,     -PORT2,

//LB:      //RB: 
PORT3,     -PORT4,

//If you are using position tracking, this is the Forward Tracker port (the tracker which runs parallel to the direction of the chassis).
//If this is a rotation sensor, enter it in "PORT1" format, inputting the port below.
//If this is an encoder, enter the port as an integer. Triport A will be a "1", Triport B will be a "2", etc.
PORT9,

//Input the Forward Tracker diameter (reverse it to make the direction switch):

3.25,
//Input Forward Tracker center distance (a positive distance corresponds to a tracker on the right side of the robot, negative is left.)
//For a zero tracker tank drive with odom, put the positive distance from the center of the robot to the right side of the drive.
//This distance is in inches:
4.5,

//Input the Sideways Tracker Port, following the same steps as the Forward Tracker Port:
1,

//Sideways tracker diameter (reverse to make the direction switch):
-2.75,

//Sideways tracker center distance (positive distance is behind the center of the robot, negative is in front):
-5

);
// dumb4right 2 left 1 solo 5 femrugt 6
int current_auton_selection = 3;
bool auto_started = false;
// Driver state mirrored from Python config
bool is_tank_drive = true; // Set tank drive as default
bool is_reverse_arcade = false;
int drive_speed = 100;
bool pneumatics_extended = false; // placeholder, no direct solenoid mapped
bool pneumatics2_extended = false; // second pneumatics state
bool pneumatics3_extended = false; // third solenoid state

bool prev_toggle_combo = false;
bool prev_speed_toggle = false;
bool prev_pneumatics_press = false;
bool prev_pneumatics2_press = false;
bool prev_solenoid3_press = false;
bool prev_reverse_arcade_combo = false;

/**
 * Function before autonomous. It prints the current auton number on the screen
 * and tapping the screen cycles the selected auton by 1. Add anything else you
 * may need, like resetting pneumatic components. You can rename these autons to
 * be more descriptive, if you like.
 */

void pre_auton() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
  // IMMEDIATELY retract pneumatics when program starts
  Solenoid.set(false);        // Force Port H pneumatics retracted
  Solenoid2.set(false);        // Force Port A pneumatics ON by defafult
  wait(50, msec);             // Brief delay to ensure command processes
  
  // Calibrate gyro sensor
  InertialSensor.calibrate();
  while (InertialSensor.isCalibrating()) {
    wait(10, msec);
  }
  wait(500, msec);
  

  default_constants();
  
  // Initialize pneumatics to retracted position (safety default)
  Solenoid.set(false);        // A pneumatics - ON by default
  pneumatics_extended = false;
  
  // Ensure pneumatics are fully retracted with a small delay
  wait(100, msec);
  Solenoid.set(false);        // Double-check Port H pneumatics retracted
  Solenoid2.set(false);        // Double-check Port A pneumatics ON

  while(!auto_started){
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(5, 20, "JAR Template v1.2.0");
    Brain.Screen.printAt(5, 40, "Battery Percentage:");
    Brain.Screen.printAt(5, 60, "%d", Brain.Battery.capacity());
    Brain.Screen.printAt(5, 80, "Chassis Heading Reading:");
    Brain.Screen.printAt(5, 100, "%f", chassis.get_absolute_heading());
    Brain.Screen.printAt(5, 120, "Selected Auton:");
    switch(current_auton_selection){
      case 0:
        Brain.Screen.printAt(5, 140, "Auton 1");
        break;
      case 1:
        Brain.Screen.printAt(5, 140, "Auton 2");
        break;
      case 2:
        Brain.Screen.printAt(5, 140, "Auton 3");
        break;
      case 3:
        Brain.Screen.printAt(5, 140, "Auton 4");
        break;
      case 4:
        Brain.Screen.printAt(5, 140, "Auton 5");
        break;
      case 5:
        Brain.Screen.printAt(5, 140, "Auton 6");
        break;
      case 6:
        Brain.Screen.printAt(5, 140, "Auton 7");
        break;
      case 7:
        Brain.Screen.printAt(5, 140, "Auton 8");
        break;
    }
    if(Brain.Screen.pressing()){
      while(Brain.Screen.pressing()) {}
      current_auton_selection ++;
    } else if (current_auton_selection == 8){
      current_auton_selection = 0;    //THIS IS NOT WHERE U CHNAGE IT IT LIES TO YOU AND IT WILL GASLIGHT YOU AND WASTE UR LIFE AWAY change it at line 108 fuck u gay gya 
    }
    wait(10, msec);
  }
}

/**
 * Auton function, which runs the selected auton. Case 0 is the default,
 * and will run in the brain screen goes untouched during preauton. Replace
 * drive_test(), for example, with your own auton function you created in
 * autons.cpp and declared in autons.h.
 */

void autonomous(void) {
  auto_started = true;
  
  // Enable auton mode (reduces left side by 5% to compensate for drift)
  
  
  // Set drive motors to HOLD mode for precise autonomous movements
  LeftFront.setStopping(hold);
  LeftMiddle.setStopping(hold);
  LeftBack.setStopping(hold);
  RightFront.setStopping(hold);
  RightMiddle.setStopping(hold);
  RightBack.setStopping(hold);
  
  switch(current_auton_selection){ 
    case 0: 
    femboysolo();
    break;
    
    break;
    case 1:
    left_side_auton();
       break;



    case 2:
      right_side_dumbass4ball_auton();
      break;
    case 3:
      skills_auton();
      break;
    case 4:
      testify();
      break;
    case 5:
    JBCsolo();
      break;
    case 6:
      right_side_auton();
      break;
    case 7:
    chassis.left_swing_to_angle(90);
      break;
 }
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // Disable auton mode (normal operation for driver control)
  chassis.set_auton_mode(false);
  
  // Set drive motors to hold mode for user control
  LeftFront.setStopping(coast);
  LeftMiddle.setStopping(coast);
  LeftBack.setStopping(coast);
  RightFront.setStopping(coast);
  RightMiddle.setStopping(coast);
  RightBack.setStopping(coast);
  
  // Reset pneumatics when entering user control
  Solenoid.set(false);        // L2 pneumatics - retracted
  Solenoid2.set(true);        // A pneumatics - ON by default
  Solenoid3.set(false);       // Third solenoid default: retracted
  pneumatics_extended = false;
  pneumatics2_extended = true;

  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    // === Intake Control ===
      
    if (Controller1.ButtonR1.pressing()){
      Intake1.spin(fwd, 100, pct);
      Intake2.spin(fwd, 100, pct);
      Intake3.spin(fwd,100,pct);
      Intake3.setStopping(coast);
    }
    
    else if (Controller1.ButtonLeft.pressing()){
      Intake1.spin(fwd, 60, pct);
      Intake2.spin(fwd, 60, pct);
      Intake3.spin(reverse,60,pct);
      Intake3.setStopping(coast);
      Solenoid3.set(true);
      // Activate third solenoid while holding Left
    }
    else if (Controller1.ButtonRight.pressing()){
      Intake1.spin(reverse, 40, pct);
      Intake2.spin(reverse, 70, pct);
      Intake3.spin(reverse,100,pct);
      Intake3.setStopping(coast);
      // Activate third solenoid while holding Left
    }
    else if (Controller1.ButtonR2.pressing()){
      // intake1 forward only
      Intake1.spin(fwd, 100, pct);
      Intake2.spin(fwd,100,pct);
      Intake3.stop();
      Intake3.setStopping(hold);\
    } else if (Controller1.ButtonL1.pressing()){
      Intake1.spin(reverse, 60, pct);
      Intake2.spin(reverse, 100, pct);
      Intake3.spin(reverse, 100, pct);
      Intake3.setStopping(coast);
    } else {
      Intake1.stop();
      Intake2.stop();
      Intake3.stop();
      Intake3.setStopping(coast);
      // Respect Y toggle state when no intake buttons pressed
      Solenoid3.set(pneumatics3_extended);
    }

    // === Drive Mode Toggle (Y + Right) ===
    bool toggle_combo = Controller1.ButtonY.pressing() && Controller1.ButtonRight.pressing();
    if (toggle_combo && !prev_toggle_combo){
      is_tank_drive = !is_tank_drive;
      is_reverse_arcade = false; // Reset reverse arcade when switching modes
      Brain.Screen.clearScreen();
      if (is_tank_drive) Brain.Screen.print("Drive Mode: Tank"); else Brain.Screen.print("Drive Mode: Arcade");
    }
    prev_toggle_combo = toggle_combo;

    // === Reverse Arcade Toggle (Up + X) ===
    bool reverse_arcade_combo = Controller1.ButtonUp.pressing() && Controller1.ButtonX.pressing();
    if (reverse_arcade_combo && !prev_reverse_arcade_combo){
      is_reverse_arcade = !is_reverse_arcade;
      is_tank_drive = false; // Switch to arcade mode when enabling reverse arcade
      Brain.Screen.clearScreen();
      if (is_reverse_arcade) Brain.Screen.print("Drive Mode: singlestick"); else Brain.Screen.print("Drive Mode: Arcade");
    }
    prev_reverse_arcade_combo = reverse_arcade_combo;

    // === Speed Toggle (B) ===
    bool curr_speed_toggle = Controller1.ButtonB.pressing();
    if (curr_speed_toggle && !prev_speed_toggle){
      drive_speed = (drive_speed == 100) ? 80 : 100;
      float scale = (float)drive_speed / 100.0f;
      chassis.set_driver_scale(scale);
      Brain.Screen.clearScreen();
      Brain.Screen.printAt(5,20, "Speed: %d%%", drive_speed);
    }
    prev_speed_toggle = curr_speed_toggle;

    // === Drive Control ===
    int left_speed = 0;
    int right_speed = 0;
    float current_fwd = 0.0;
    float current_turn = 0.0;
    
    // Helper function to apply quadratic curve (x^2) to joystick input
    // Input range: -100 to 100, output range: -100 to 100
    auto apply_precision_scale = [](float raw_value) -> float {
      if (fabs(raw_value) < 0.01) return 0.0; // Deadband for very small values
      float normalized = raw_value / 100.0f; // Normalize to -1 to 1
      float squared = normalized * normalized; // Apply x^2 curve
      // Preserve sign for quadratic curve
      float sign = (normalized >= 0.0f) ? 1.0f : -1.0f;
      return sign * squared * 100.0f; // Scale back to -100 to 100
    };
    
    // === Forward Movement (Down Button) ===
    if (Controller1.ButtonDown.pressing()) {
      // Move straight forward at 80% speed
      int forward_speed = (int)(80 * drive_speed / 100.0);
      left_speed = forward_speed;
      right_speed = forward_speed;
      current_fwd = 80;
      current_turn = 0;
    } else {
      // Normal joystick control
      if (is_tank_drive){
        float left_raw = deadband(Controller1.Axis3.position(), 0.05);
        float right_raw = deadband(Controller1.Axis2.position(), 0.05);
        left_speed = (int)(apply_precision_scale(left_raw) * drive_speed / 100.0);
        right_speed = (int)(apply_precision_scale(right_raw) * drive_speed / 100.0);
      } else if (is_reverse_arcade) {
        // Reverse arcade: Axis4 (left/right) for left/right, Axis2 (left/right) for forward/back
        float fwd_raw = deadband(Controller1.Axis3.position(), 0.05);
        float turn_raw = deadband(Controller1.Axis4.position(), 0.01);
        float fwd = apply_precision_scale(fwd_raw);
        float turn = apply_precision_scale(turn_raw) * 1; // Reduce turning sensitivity to 70%
        current_fwd = fwd;
        current_turn = turn;
        left_speed = (int)((fwd + turn) * drive_speed / 100.0);
        right_speed = (int)((fwd - turn) * drive_speed / 100.0);
      } else {
        // Normal arcade: Axis3 (forward/back) for forward/back, Axis1 (left/right) for left/right
        float fwd_raw = deadband(Controller1.Axis3.position(), 0.05);
        float turn_raw = deadband(Controller1.Axis1.position(), 0.05);
        float fwd = apply_precision_scale(fwd_raw);
        float turn = apply_precision_scale(turn_raw) * 0.7; // Reduce turning sensitivity to 70%
        current_fwd = fwd;
        current_turn = turn;
        left_speed = (int)((fwd + turn) * drive_speed / 100.0);
        right_speed = (int)((fwd - turn) * drive_speed / 100.0);
      }
    }

    // === Dynamic Motor Stopping Mode for Arcade Drive ===
    // Behavior:
    // - If a turn was just released (joystick turn went from significant -> near-zero) and
    //   forward input is small, set motors to HOLD to keep heading.
    // - If forward input becomes active, set motors to COAST for smooth driving.
    if (!is_tank_drive && !is_reverse_arcade) {
      static float prev_turn = 0.0f;
      static float prev_fwd = 0.0f;
      const float turn_release_thresh = 12.0f;   // previously considered a 'turn'
      const float turn_deadband = 6.0f;          // considered released when below this
      const float forward_deadband = 10.0f;      // forward considered active above this

      bool was_turning = fabs(prev_turn) > turn_release_thresh;
      bool turn_released = was_turning && fabs(current_turn) <= turn_deadband;
      bool forward_active = fabs(current_fwd) > forward_deadband;

      if (Controller1.ButtonDown.pressing()) {
        // Special case: forced forward button -> use coast
        LeftFront.setStopping(coast);
        LeftMiddle.setStopping(coast);
        LeftBack.setStopping(coast);
        RightFront.setStopping(coast);
        RightMiddle.setStopping(coast);
        RightBack.setStopping(coast);
      } else if (turn_released && !forward_active) {
        // Just finished a turn and not driving forward: hold to keep heading
        LeftFront.setStopping(hold);
        LeftMiddle.setStopping(hold);
        LeftBack.setStopping(hold);
        RightFront.setStopping(hold);
        RightMiddle.setStopping(hold);
        RightBack.setStopping(hold);
      } else if (forward_active) {
        // Driving forward or curve: coast for smooth motion
        LeftFront.setStopping(coast);
        LeftMiddle.setStopping(coast);
        LeftBack.setStopping(coast);
        RightFront.setStopping(coast);
        RightMiddle.setStopping(coast);
        RightBack.setStopping(coast);
      }

      prev_turn = current_turn;
      prev_fwd = current_fwd;
    }

  // Apply to motors individually for smooth direct control
  float leftVolt = to_volt((float)left_speed);
  float rightVolt = to_volt((float)right_speed);
  LeftFront.spin(fwd, leftVolt, volt);
  LeftMiddle.spin(fwd, leftVolt, volt);
  LeftBack.spin(fwd, leftVolt, volt);
  RightFront.spin(fwd, rightVolt, volt);
  RightMiddle.spin(fwd, rightVolt, volt);
  RightBack.spin(fwd, rightVolt, volt);

    // === Pneumatics toggle (L2) ===
    bool pneu2_press = Controller1.ButtonL2.pressing();
    if (pneu2_press && !prev_pneumatics2_press){
      pneumatics2_extended = !pneumatics2_extended;
      Solenoid2.set(pneumatics2_extended);
      Brain.Screen.clearScreen();
      if (pneumatics2_extended) Brain.Screen.print("Pneumatics 2: Extended"); else Brain.Screen.print("Pneumatics 2: Retracted");
    }
    prev_pneumatics2_press = pneu2_press;
    
    // === Second Pneumatics toggle (A) ===
    bool pneu_press = Controller1.ButtonA.pressing();
    if (pneu_press && !prev_pneumatics_press){
      pneumatics_extended = !pneumatics_extended;
      Solenoid.set(pneumatics_extended);
      Brain.Screen.clearScreen();
      if (pneumatics_extended) Brain.Screen.print("Pneumatics: Extended"); else Brain.Screen.print("Pneumatics: Retracted");
    }
    prev_pneumatics_press = pneu_press;
    
    // === Solenoid 3 Toggle (Y) ===
    bool solenoid3_press = Controller1.ButtonY.pressing();
    if (solenoid3_press && !prev_solenoid3_press){
      pneumatics3_extended = !pneumatics3_extended;
      Solenoid3.set(pneumatics3_extended);
      Brain.Screen.clearScreen();
      if (pneumatics3_extended) Brain.Screen.print("Solenoid 3: Extended"); else Brain.Screen.print("Solenoid 3: Retracted");
    }
    prev_solenoid3_press = solenoid3_press;
    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}