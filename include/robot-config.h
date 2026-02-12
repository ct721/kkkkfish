using namespace vex;

extern brain Brain;

//To set up a motor called LeftFront here, you'd use
//extern motor LeftFront;

//Add your devices below, and don't forget to do the same in robot-config.cpp:
// Controller
extern controller Controller1;

// Drive motors
extern motor LeftFront;
extern motor LeftMiddle;
extern motor LeftBack;
extern motor RightFront;
extern motor RightMiddle;
extern motor RightBack;

// Intake
extern motor Intake1;
extern motor Intake2;
extern motor Intake3;

// Inertial sensor
extern inertial InertialSensor;

// Pneumatics
extern digital_out Solenoid;
extern digital_out Solenoid2;
extern digital_out Solenoid3;

void  vexcodeInit( void );