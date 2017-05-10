#include "Arduino.h"
#include "SynchPID.h" //include the declaration for this class

// Member Vars
double m_P;
double m_I;
double m_D;
double m_lastInput;
double m_deadband;
double m_error = 0;
double m_setpoint;
double m_minimumInput = -100;
double m_maximumInput = 100;
double m_minimumOutput = -100;
double m_maximumOutput = 100;
double m_totalError = 0;
double m_result;
double m_prevError;

// constructor
SynchPID::SynchPID(double Kp, double Ki, double Kd){
  m_P = Kp;
  m_I = Ki;
  m_D = Kd;
  m_lastInput = 0;
  m_error = 0;
  m_setpoint = 0;
  m_deadband = 0;
}

double SynchPID::calculate(double input) {
  m_lastInput = input;
  m_error = m_setpoint - input;

  // m_continuous logic??  Not doing right now

  if ((m_error * m_P < m_maximumOutput) && (m_error * m_P > m_minimumOutput)) {
      m_totalError += m_error;
   }
   else {
      m_totalError = 0;
   }

   // Don't blow away m_error so as to not break derivative
   double proportionalError = abs(m_error) < m_deadband ? 0 : m_error;

   m_result = (m_P * proportionalError + m_I * m_totalError + m_D * (m_error - m_prevError));
   m_prevError = m_error;

   if (m_result > m_maximumOutput){m_result = m_maximumOutput;}
   else if (m_result < m_minimumOutput){m_result = m_minimumOutput;}

   return m_result;
}

void SynchPID::setDeadband(double deadband){
   m_deadband = deadband;
}

void SynchPID::setInputRange(double minimumInput, double maximumInput) {
   if (minimumInput > maximumInput) {
      Serial.println("INPUT: Lower bound is greater than upper bound");
   }
   m_minimumInput = minimumInput;
   m_maximumInput = maximumInput;
   setSetpoint(m_setpoint);
}

void SynchPID::setOutputRange(double minimumOutput, double maximumOutput) {
   if (minimumOutput > maximumOutput) {
      Serial.println("OUTPUT: Lower bound is greater than upper bound");
   }
   m_minimumOutput = minimumOutput;
   m_maximumOutput = maximumOutput;
}

void SynchPID::setSetpoint(double setpoint) {
   if (m_maximumInput > m_minimumInput) {
      if (setpoint > m_maximumInput) {
         m_setpoint = m_maximumInput;
      }
      else if (setpoint < m_minimumInput) {
         m_setpoint = m_minimumInput;
      }
      else {
         m_setpoint = setpoint;
      }
   }
   else {
      m_setpoint = setpoint;
   }
}

//<<destructor>>
SynchPID::~SynchPID(){
  /*nothing to destruct*/
}
