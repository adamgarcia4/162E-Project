#ifndef SYNCHPID_H
#define SYNCHPID_H

class SynchPID {
public:
  SynchPID(double kp, double ki, double kd);
  ~SynchPID();
  double calculate(double input);
  void setSetpoint(double Setpoint);
  void setDeadband(double deadband);
  void setInputRange(double minimumInput, double maximumInput);
  void setOutputRange(double minimumOutput, double maximumOutput);
  
private:

};


#endif
