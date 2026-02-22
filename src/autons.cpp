#include "vex.h"

/**
 * Resets the constants for auton movement.
 * Modify these to change the default behavior of functions like
 * drive_distance(). For explanations of the difference between
 * drive, heading, turning, and swinging, as well as the PID and
 * exit conditions, check the docs.
 */

void default_constants(){
  // Each constant set is in the form of (maxVoltage, kP, kI, kD, startI).
  // SUPER LOW constants for new robot - start conservative and tune up
  chassis.set_drive_constants(
    12,          // slew
    0.55,      // P  (lower acceleration)
    0.0002,     // I  (stronger “finish the target” force)
    0.26,       // D  (much weaker braking)
    0           // start I
);
  chassis.set_heading_constants(6, .3, 0, 1, 0);  // Reduced: 0.2 kP, 0.5 kD
  chassis.set_turn_constants(8, .2, 0, 1.26, 15);  // Reduced: 6V max, 0.15 kP, 1.0 kD  
  chassis.set_swing_constants(12, .4, 0, 1.5, 15);  // Higher voltage and gains for swing
  // Each exit condition set is in the form of (settle_error, settle_time, timeout).
  // Looser settle conditions for new robot (more forgiving)
  chassis.set_drive_exit_conditions(1.8, 40, 2500);  // 5" error, 150ms settle, 4s timeout
  chassis.set_turn_exit_conditions(3, 40, 2000);  // 8° error, 150ms settle, 4s timeout
  chassis.set_swing_exit_conditions(3, 80, 4000);  // 5° error, 150ms settle, 4s timeout
}

/**
 * Sets constants to be more effective for odom movements.
 * For functions like drive_to_point(), it's often better to have
 * a slower max_voltage and greater settle_error than you would otherwise.
 */

void odom_constants(){
  default_constants();
  chassis.heading_max_voltage = 10;
  chassis.drive_max_voltage = 8;
  chassis.drive_settle_error = 3;
  chassis.boomerang_lead = .5;
  chassis.drive_min_voltage = 0;
}

/**
 * The expected behavior is to return to the start position.
 */

void drive_test(){
  chassis.drive_distance(6);
  chassis.drive_distance(12);
  chassis.drive_distance(18);
  chassis.drive_distance(-36);
}

/**
 * The expected behavior is to return to the start angle, after making a complete turn.
 */

void turn_test(){
  chassis.turn_to_angle(5);
  chassis.turn_to_angle(30);
  chassis.turn_to_angle(90);
  chassis.turn_to_angle(225);
  chassis.turn_to_angle(0);
}

/**
 * Should swing in a fun S shape.
 */

void swing_test(){
  chassis.left_swing_to_angle(90);
  chassis.right_swing_to_angle(0);
}

/**
 * A little of this, a little of that; it should end roughly where it started.
 */

void full_test(){
  chassis.drive_distance(24);
  chassis.turn_to_angle(-45);
  chassis.drive_distance(-36);
  chassis.right_swing_to_angle(-90);
  chassis.drive_distance(24);
  chassis.turn_to_angle(0);
}

/**
 * Doesn't drive the robot, but just prints coordinates to the Brain screen 
 * so you can check if they are accurate to life. Push the robot around and
 * see if the coordinates increase like you'd expect.
 */

void odom_test(){
  chassis.set_coordinates(0, 0, 0);
  while(1){
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(5,20, "X: %f", chassis.get_X_position());
    Brain.Screen.printAt(5,40, "Y: %f", chassis.get_Y_position());
    Brain.Screen.printAt(5,60, "Heading: %f", chassis.get_absolute_heading());
    Brain.Screen.printAt(5,80, "ForwardTracker: %f", chassis.get_ForwardTracker_position());
    Brain.Screen.printAt(5,100, "SidewaysTracker: %f", chassis.get_SidewaysTracker_position());
    task::sleep(20);
  }
}

/**
 * Should end in the same place it began, but the second movement
 * will be curved while the first is straight.
 */

void tank_odom_test(){
  odom_constants();
  chassis.set_coordinates(0, 0, 0);
  chassis.turn_to_point(24, 24);
  chassis.drive_to_point(24,24);
  chassis.drive_to_point(0,0);
  chassis.turn_to_angle(0);
}

/**
 * Drives in a square while making a full turn in the process. Should
 * end where it started.
 */

void holonomic_odom_test(){
  odom_constants();
  chassis.set_coordinates(0, 0, 0);
  chassis.holonomic_drive_to_pose(0, 18, 90);
  chassis.holonomic_drive_to_pose(18, 0, 180);
  chassis.holonomic_drive_to_pose(0, 18, 270);
  chassis.holonomic_drive_to_pose(0, 0, 0);
}

void left_side_auton(){/*
  chassis.drive_distance(11);
  Intake1.spin(fwd, 100, pct);
  Intake2.stop();
  Intake2.setStopping(hold);
  chassis.turn_to_angle(-50);
  chassis.drive_max_voltage = 3;
  chassis.drive_distance(15);
  chassis.drive_distance(-2);
  chassis.turn_to_angle(45+180);*/
  chassis.set_coordinates(0,0,0);

  Intake1.spin(fwd, 100, pct);
  Intake2.stop();
  default_constants();
  chassis.drive_max_voltage = 4;
  
  Solenoid2.set(false);
  chassis.drive_to_point(-4.5,18);

   Solenoid.set(true);
  chassis.drive_to_point(-6,25);
     default_constants();
     chassis.drive_distance(-2);
  chassis.turn_to_angle(45+180);
  Intake2.stop();  //180 because it's a negative angle
  chassis.drive_distance(-17);
  
  Solenoid2.set(true);
Intake1.spin(fwd,100,pct);
  Intake2.spin(fwd, 100, pct);
  wait(1,sec);
  Intake1.stop();
  Intake2.stop();



  Solenoid2.set(false);
  chassis.drive_to_point(-29.5,0);
  Intake1.spin(fwd, 100, pct);
  chassis.turn_to_angle(180);
  chassis.drive_timeout=800;
  chassis.drive_distance(8.7);
  default_constants();
  chassis.turn_to_angle(180);
  chassis.drive_max_voltage = 8;
  wait(0.36,sec);
  Intake1.stop();
  //drive back
  
   chassis.turn_to_angle(180);
  chassis.drive_distance(-29.5);
  // Deactivate pneumatics
  Solenoid.set(false);
  Intake1.spin(fwd, 100, pct);
  Intake2.spin(fwd, 100, pct);
  wait(1.4,sec);
  Solenoid.set(false);
 Solenoid2.set(true);
 Intake2.stop();

 chassis.set_coordinates(0,0,0);
 chassis.drive_to_point(13,5);
  default_constants();
  chassis.turn_to_point(13,19);
  chassis.drive_to_point(13,-22);




 //chassis.drive_distance(10);
 //chassis.turn_to_angle(-90);
 //chassis.drive_distance(11.5);
 
 //chassis.turn_to_angle(180);
 //chassis.drive_distance(-24);

  
}

void testify(){
  chassis.drive_distance(24);
  chassis.drive_distance(-24);

}
void femboysolo(){

 chassis.drive_distance(28.75);

  Solenoid2.set(false);

 //Matchloader Pt.1
 Solenoid.set(true);
 chassis.turn_to_angle(90);
 Intake1.spin(fwd, 100, pct);


  chassis.drive_max_voltage = 6;
  chassis.drive_distance(8.5);

  wait(0.35,sec);
 chassis.turn_to_angle(90);
 //chassis.drive_max_voltage= 6;
 Intake1.stop();
 chassis.drive_distance(-29.5);
 Intake1.spin(fwd, 100, pct);
 Intake2.spin(fwd, 100, pct);

 Solenoid.set(false);
 
 wait(1.3,sec);
 Intake2.stop();
 Intake1.stop();
 chassis.drive_distance(10);
 chassis.set_coordinates(0,0,0);
 Intake1.spin(fwd, 100, pct);
 chassis.turn_to_angle(90);
 chassis.set_heading_constants(4,0.15,0,0.7,0);
 chassis.drive_to_point(22,-13);
 Solenoid.set(true);
 chassis.drive_to_point(24,-15);
 default_constants();
 chassis.turn_to_angle(90);
 Solenoid.set(false);
 chassis.drive_max_voltage = 6;
 chassis.drive_to_point(24*3,-15);
 default_constants();
 chassis.turn_to_angle(45);
 
  chassis.set_coordinates(-6,25,180+45);
 chassis.drive_distance(-17);
 
  Solenoid2.set(true);
Intake1.spin(fwd,100,pct);  
  Intake2.spin(fwd, 100, pct);
  wait(1,sec);
  Intake1.stop();
  Intake2.stop();

  Solenoid2.set(false);
  chassis.drive_to_point(-29.5,0);
  Intake1.spin(fwd, 100, pct);
  chassis.turn_to_angle(180);
  chassis.drive_distance(8.5);
  chassis.turn_to_angle(180);
  chassis.drive_max_voltage = 8;
  wait(0.36,sec);
  Intake1.stop();
  //drive back
  
   chassis.turn_to_angle(180);
  chassis.drive_distance(-29.5);
  // Deactivate pneumatics
  Solenoid.set(false);
  Intake1.spin(fwd, 100, pct);
  Intake2.spin(fwd, 100, pct);
  wait(1.4,sec);
  Solenoid.set(false);
 Solenoid2.set(true);
 Intake2.stop();


 
}



void right_side_auton(){
  
 chassis.drive_distance(30);

  Solenoid2.set(false);

 //Matchloader Pt.1
 Solenoid.set(true);
 chassis.turn_to_angle(90);
 Intake1.spin(fwd, 100, pct);
 Intake2.spin(fwd,100,pct);

  chassis.drive_max_voltage = 8;
  chassis.drive_timeout = 900;
  chassis.drive_distance(12);
  default_constants();
  wait(0.15,sec);
 chassis.turn_to_angle(90);
 //chassis.drive_max_voltage= 6;
 chassis.drive_distance(-28.5);
 Intake1.spin(fwd, 100, pct);
 Intake2.spin(fwd, 100, pct);

 Intake3.spin(fwd, 100, pct);
 Solenoid.set(false);
 
 wait(1.4,sec);
 Intake3.stop();
chassis.set_coordinates(0,0,90);
Intake1.spin(fwd, 100, pct);
Intake2.spin(fwd,50,pct);
 chassis.turn_to_angle(220);
 chassis.drive_distance(13);
 chassis.drive_max_voltage=6;
 chassis.drive_distance(8);
 default_constants();
 Intake1.stop();
 Intake2.stop();
 chassis.drive_distance(10);
 Intake1.spin(reverse, 100, pct);
 Intake2.spin(reverse,100,pct);
 Intake3.spin(reverse,100,pct);
 wait(1.3, sec);
 chassis.drive_distance(-32);
 chassis.turn_to_angle(90);
 chassis.drive_distance(-30);
}



#include "vex.h"


/**
* Resets the constants for auton movement.
* Modify these to change the default behavior of functions like
* drive_distance(). For explanations of the difference between
* drive, heading, turning, and swinging, as well as the PID and
* exit conditions, check the docs.
*/




void right_side_dumbass4ball_auton(){

 chassis.drive_distance(31.5);

  Solenoid2.set(false);

 //Matchloader Pt.1
 Solenoid.set(true);
 chassis.turn_to_angle(90);
 Intake1.spin(fwd, 100, pct);
 Intake2.spin(fwd,100,pct);

  chassis.drive_max_voltage = 8;
  chassis.drive_timeout = 1000;
  chassis.drive_distance(13);
  default_constants();
  wait(0.05,sec);
 chassis.turn_to_angle(90);
 //chassis.drive_max_voltage= 6;
 Intake1.stop();
 chassis.drive_distance(-28.5);
 Intake1.spin(fwd, 100, pct);
 Intake2.spin(fwd, 100, pct);

 Intake3.spin(fwd, 100, pct);
 Solenoid.set(false);
 
 wait(1.4,sec);
 Intake2.stop();
 Intake1.stop();
 chassis.drive_distance(5);
 chassis.set_coordinates(0,0,0);
 chassis.drive_to_point(12,3);
 
  default_constants();
  chassis.drive_max_voltage=7;
  chassis.turn_to_point(12,19);
  chassis.drive_to_point(12,-26);
  
 chassis.drive_stop(hold);

}


void skills_auton(){
  //new auton (?)
  chassis.drive_distance(27);
  Solenoid2.set(false);

 //Matchloader #1
 Solenoid.set(true);
 chassis.turn_to_angle(90);
 Intake1.spin(fwd, 100, pct);
 Intake2.spin(fwd,100,pct);
 chassis.drive_timeout = 800;
 chassis.drive_max_voltage = 3;
 chassis.drive_distance(11);
 wait(0.92, sec);
 default_constants();
 wait(0.15,sec);
 chassis.drive_max_voltage = 3;
 chassis.turn_to_angle(90);
 
 //Get ready for transition from 1st matchload to wall 
 chassis.drive_distance(-17);
 chassis.turn_to_angle(180);
 chassis.drive_distance(15);
 chassis.turn_to_angle(-90);
 Intake1.stop();
 Intake2.stop();

 //1st -> 2nd Matchload Right Wall
 chassis.drive_distance(84);
 chassis.turn_to_angle(0);
 chassis.drive_distance(18);
 chassis.turn_to_angle(-90);
 chassis.drive_distance(-21);

 //Scoring the blocks from 1st matchload to long goal in top right
 Intake1.spin(fwd, 100, pct);
 Intake2.spin(fwd, 100, pct);
 Intake3.spin(fwd, 100, pct);
 wait(1.1, sec);
 Intake1.stop();
 chassis.drive_distance(1);
 chassis.turn_to_angle(-90);
 
 //Matchload #2
 Solenoid.set(true);
 Intake1.spin(fwd, 100, pct);
 Intake2.spin(fwd, 100, pct);
 chassis.drive_distance(1);
 //hassis.turn_to_angle(-75);
 chassis.drive_max_voltage = 3;
 chassis.drive_distance(26.3);
 wait(0.94, sec);
 chassis.turn_to_angle(-90);
 chassis.drive_max_voltage = 3;

 //Scoring 2nd matchload blocks in long goal
 chassis.drive_distance(-25);
 Intake3.spin(fwd, 100, pct);
 wait(0.9, sec);
 Intake1.stop();
 Intake2.stop();
 Intake3.stop();
 chassis.drive_distance(9);
 chassis.turn_to_angle(180);

 //2nd -> 3rd Matchload Top Wall
 chassis.drive_distance(43);
 chassis.turn_to_angle(180);
 chassis.drive_distance(50);
 chassis.turn_to_angle(-90);

 //Matchload #3
 Intake1.spin(fwd, 100, pct);
 Intake2.spin(fwd, 100, pct);
 Solenoid.set(true);
 chassis.drive_distance(19);
 Intake1.spin(fwd, 100, pct);
 Intake2.spin(fwd, 100, pct);
 wait(0.92, sec);

 //
 chassis.drive_distance(-10);
 chassis.turn_to_angle(0);
 /*
 chassis.drive_distance(20);
 chassis.turn_to_angle(90);
 chassis.drive_distance(86.9);
 chassis.turn_to_angle(180);
 chassis.drive_distance(17);
 chassis.turn_to_angle(90);
 chassis.drive_distance(19);
 Intake3.spin(fwd, 100, pct);
 wait(1,sec);
 Intake3.stop();
 Solenoid.set(true);
 chassis.turn_to_angle(90);
 chassis.drive_distance(20);

 wait(1,sec);
 Intake3.stop();
chassis.set_coordinates(0,0,90);
*/
}

void left_side_sped_auton(){
 /*I hate niggers*/
}

void left_side_dumbass4ball_auton(){

 chassis.drive_distance(29.75);

  Solenoid2.set(false);

 //Matchloader Pt.1
 Solenoid.set(true);
 chassis.turn_to_angle(-90);
 Intake1.spin(fwd, 100, pct);


 //chassis.drive_max_voltage = 4;
 //Matchloader Pt.2
  // Drive forward with collision detection - drive at full speed until hitting something, then back up 1 inch
  {
    float max_voltage = 6.0; // Full voltage
    float target_distance = 8.5; // Target distance in inches
    float collision_velocity_threshold = 5.0; // RPM threshold - if velocity drops below this, consider it a collision
    float collision_check_time = 60; // ms - how long velocity must be low to confirm collision
    float start_position = (chassis.get_left_position_in() + chassis.get_right_position_in()) / 2.0;
    float current_position = start_position;
    bool collision_detected = false;
    int low_velocity_count = 0;
    int start_time = Brain.timer(msec);
    int timeout = 800; // 3 second timeout
    
    // Drive forward at full speed
    while (!collision_detected && (Brain.timer(msec) - start_time) < timeout) {
      current_position = (chassis.get_left_position_in() + chassis.get_right_position_in()) / 2.0;
      float distance_traveled = current_position - start_position;
      
      // Check if we've reached the target distance
      if (distance_traveled >= target_distance) {
        break;
      }
      
      // Check motor velocities for collision detection
      float left_vel = (LeftFront.velocity(rpm) + LeftMiddle.velocity(rpm) + LeftBack.velocity(rpm)) / 3.0;
      float right_vel = (RightFront.velocity(rpm) + RightMiddle.velocity(rpm) + RightBack.velocity(rpm)) / 3.0;
      float avg_velocity = (left_vel + right_vel) / 2.0;
      
      // If velocity is very low while trying to drive, we're hitting something
      if (fabs(avg_velocity) < collision_velocity_threshold) {
        low_velocity_count++;
        if (low_velocity_count > (collision_check_time / 10)) { // Check for collision_check_time ms
          collision_detected = true;
          break;
        }
      } else {
        low_velocity_count = 0; // Reset counter if velocity recovers
      }
      
      // Drive forward at full voltage
      chassis.drive_with_voltage(max_voltage, max_voltage);
      wait(10, msec);
    }
    
    // Stop motors
    chassis.drive_stop(hold);
    wait(50, msec);
    
    // If collision detected, drive back 1 inch
    if (collision_detected) {
      chassis.drive_distance(1);
    }
  }
  wait(0.35,sec);
  chassis.drive_timeout = 300;
  chassis.drive_distance(2);
  default_constants();
 chassis.turn_to_angle(-90);
 //chassis.drive_max_voltage= 6;
 Intake1.stop();
 chassis.drive_distance(-29.5);
 Intake1.spin(fwd, 100, pct);
 Intake2.spin(fwd, 100, pct);

 Solenoid.set(false);
 
 wait(2,sec);
 Intake2.stop();
 Intake1.stop();
 chassis.turn_to_angle(90);
 chassis.drive_distance(5);
 chassis.set_coordinates(0,0,0);
 chassis.drive_to_point(13,3);
 
 Solenoid2.set(true);
  
  default_constants();
  chassis.drive_max_voltage=7;
  chassis.turn_to_point(13,19);
  chassis.drive_to_point(13,-26);
  
 chassis.drive_stop(hold);




}
void JBCsolo(){
 
 chassis.drive_distance(30);

  Solenoid2.set(false);

 //Matchloader Pt.1
 Solenoid.set(true);
 chassis.turn_to_angle(90);
 Intake1.spin(fwd, 100, pct);
 Intake2.spin(fwd,100,pct);

  chassis.drive_timeout = 800;
  chassis.drive_distance(12);
  default_constants();
  wait(0.15,sec);
 chassis.turn_to_angle(90);
 //chassis.drive_max_voltage= 6;
 
 chassis.drive_distance(-28.5);
 Intake1.spin(fwd, 100, pct);
 Intake2.spin(fwd, 100, pct);

 Intake3.spin(fwd, 100, pct);
 Solenoid.set(false);
 
 wait(1,sec);
 Intake3.stop();
chassis.set_coordinates(0,0,90);
Intake1.spin(fwd, 100, pct);
Intake2.spin(fwd,50,pct);
chassis.drive_distance(18);

 chassis.turn_to_angle(225);
 chassis.drive_distance(28);
 chassis.drive_distance(8);
 chassis.turn_to_angle(180);
 chassis.drive_distance(48);
 chassis.turn_to_angle(135);
 
 Intake3.spin(fwd,100,pct);
 chassis.drive_distance(-14);
 Intake2.spin(fwd,100,pct);
 Intake1.spin(fwd,100,pct);
 Intake3.stop();
 chassis.drive_distance(44);
 Solenoid.set(true);
 
 chassis.turn_to_angle(90);
 chassis.drive_distance(12);
 wait(0.3,sec);
 chassis.drive_distance(-28.5);
 Intake3.spin(fwd,100,pct);

}
