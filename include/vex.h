/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       vex.h                                                     */
/*    Author:       Vex Robotics                                              */
/*    Created:      1 Feb 2019                                                */
/*    Description:  Default header for V5 projects                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/
//

#pragma once
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

// Ensure common math symbols are available in the global scope for legacy C-style code
// (many files in this project use fabs, sin, cos, sqrt, pow and M_PI).
#ifndef M_PI
static constexpr double M_PI = 3.14159265358979323846;
#endif

// Prefer calling sin/cos/fabs/sqrt/pow unqualified (they exist in the global namespace
// for the embedded SDK). If you prefer explicit std:: versions, include <cmath>
// and qualify them in implementation files.

#include "v5.h"
#include "v5_vcs.h"

#include "robot-config.h"
#include "JAR-Template/odom.h"
#include "JAR-Template/drive.h"
#include "JAR-Template/util.h"
#include "JAR-Template/PID.h"
#include "autons.h"

#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)