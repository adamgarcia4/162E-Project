//#include "Arduino.h"
//#include "SynchPID.h" //include the declaration for this class
//
//// Member Vars
//double m_P;
//double m_I;
//double m_D;
//double m_lastInput;
//double m_error;
//double m_setpoint;
//
//// constructor
//SynchPID::SynchPID(double Kp, double Ki, double Kd){
//   m_P = Kp;
//   m_I = Ki;
//   m_D = Kd;
//   m_lastInput = 0;
//   m_error = 0;
//   m_setpoint = 0;
//}
//
//SynchPID::calculate(double input) {
//   m_lastInput = input;
//   m_error = m_setpoint - input;
//
//   // m_continuous logic??
//
//   // Increment total error logic
//
//   // adjust prop error just in case cray cray
//
//   m_result = (m_P * proportionalError + m_I * m_totalError + m_D * (m_error - m_prevError));
//
//   // Add Motor Deadbands
//
//   return m_result;
//}
//
////<<destructor>>
//SynchPID::SynchPID(){
//   /*nothing to destruct*/
//}
