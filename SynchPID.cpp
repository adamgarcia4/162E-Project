#include "Arduino.h"
#include "SynchPID.h" //include the declaration for this class

// Member Vars
double m_P;
double m_I;
double m_D;
double m_lastInput;
double m_error = 0;
double m_setpoint;
double m_maximumOutput = 1;
double m_minimumOutput = -1;
double m_totalError = 0;

// constructor
SynchPID::SynchPID(double Kp, double Ki, double Kd){
  m_P = Kp;
  m_I = Ki;
  m_D = Kd;
  m_lastInput = 0;
  m_error = 0;
  m_setpoint = 0;

}

SynchPID::calculate(double input) {
  m_lastInput = input;
  m_error = m_setpoint - input;

  // m_continuous logic??  Not doing right now

  // Increment total error logic
  if ((m_error * m_P < m_maximumOutput) && (m_error * m_P > m_minimumOutput)) {
            m_totalError += m_error;
        } else {
            m_totalError = 0;
        }
   }

  // adjust prop error just in case cray cray

  m_result = (m_P * proportionalError + m_I * m_totalError + m_D * (m_error - m_prevError));

  // Add Motor Deadbands

  return m_result;
}

//<<destructor>>
SynchPID::SynchPID(){
  /*nothing to destruct*/
}
