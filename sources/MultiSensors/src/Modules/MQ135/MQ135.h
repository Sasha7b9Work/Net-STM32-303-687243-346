/**************************************************************************/
/*!
@file     MQ135.h
@author   G.Krocker (Mad Frog Labs)
@license  GNU GPLv3

First version of an Arduino Library for the MQ135 gas sensor
TODO: Review the correction factor calculation. This currently relies on
the datasheet but the information there seems to be wrong.

@section  HISTORY

v1.0 - First release
*/
/**************************************************************************/
#ifndef MQ135_H
#define MQ135_H
//#if ARDUINO >= 100
// #include "Arduino.h"
//#else
// #include "WProgram.h"
//#endif

/// The load resistance on the board
#define RLOAD 10.0f
/// Calibration resistance at atmospheric CO2 level
#define RZERO 76.63f
/// Parameters for calculating ppm of CO2 from sensor resistance
#define PARA 116.6020682f
#define PARB 2.769034857f

/// Parameters to model temperature and humidity dependence
#define CORA 0.00035f
#define CORB 0.02718f
#define CORC 1.39538f
#define CORD 0.0018f

/// Atmospheric CO2 level for calibration purposes
#define ATMOCO2 397.13f

class MQ135 {
 private:
  uint8 _pin;

 public:
  MQ135(uint8_t pin);
  float getCorrectionFactor(float t, float h);
  float getResistance();
  float getCorrectedResistance(float t, float h);
  float getPPM();
  float getCorrectedPPM(float t, float h);
  float getRZero();
  float getCorrectedRZero(float t, float h);
};
#endif
