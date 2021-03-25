
//_________________________________________________________________________

               void interrupts_triggered(){ 
//_________________________________________________________________________

if (!(WDTCSR & (1<<WDIE))) { WDTCSR |= (1<<WDIE); } }


//_________________________________________________________________________

            ISR(WDT_vect){ // Watchdog timer ISR

//_________________________________________________________________________

    WDTCSR &= ~(1<<WDIE); // Disable watchdog timer.  

    if(digitalRead(18)== LOW){
      Serial.println("RESET pressed");                      
       Sys_reset();                
    } 

    else if(digitalRead(2)== LOW){
      Serial.println("CYCLE_START pressed");
      bit_true(sys_rt_exec_state, EXEC_CYCLE_START);
    }

      else if(digitalRead(3)== LOW){
      Serial.println("FEED_HOLD pressed");
        bit_true(sys_rt_exec_state, EXEC_FEED_HOLD); 
    } 
    
    if (digitalRead(21)== LOW)
          { Serial.println("Z limit Reached");
          bit_true(sys_rt_exec_alarm,EXEC_ALARM_HARD_LIMIT);}
    if (digitalRead(20)== LOW) 
          {Serial.println("Y limit Reached");
          bit_true(sys_rt_exec_alarm,EXEC_ALARM_HARD_LIMIT);}
    if (digitalRead(19)== LOW) 
          {Serial.println("X limit Reached");
          bit_true(sys_rt_exec_alarm,EXEC_ALARM_HARD_LIMIT);}
 }


// Method to ready the system to reset by setting the realtime reset command and killing any
// active processes in the system. This also checks if a system reset is issued while Grbl
// is in a motion state. If so, kills the steppers and sets the system alarm to flag position
// lost, since there was an abrupt uncontrolled deceleration. Called at an interrupt level by
// realtime abort command and hard limits. So, keep to a minimum.
//****************************************************************************************************
void Sys_reset()
//****************************************************************************************************
{
  if (bit_isfalse(sys_rt_exec_state, EXEC_RESET)) {
    bit_true_atomic(sys_rt_exec_state, EXEC_RESET);
 Serial.println(" Kill spindle and coolant. ");  
    spindle_stop();
Serial.println("   Kill steppers only if in any motion state");
    if ((sys.state & (STATE_CYCLE | STATE_HOMING)) || (sys.suspend == SUSPEND_ENABLE_HOLD)) {
      if (sys.state == STATE_HOMING) { bit_true_atomic(sys_rt_exec_alarm, EXEC_ALARM_HOMING_FAIL); }
      else { bit_true_atomic(sys_rt_exec_alarm, EXEC_ALARM_ABORT_CYCLE); }
      stepperX.stop();
      stepperY.stop();
      stepperZ.stop();
    }
  }
}
 


// Returns machine position of axis 'idx'. Must be sent a 'step' array.
// NOTE: If motor steps and machine position are not in the same coordinate frame, this function
//   serves as a central place to compute the transformation.
//************************************************************************************************
float system_convert_axis_steps_to_mpos(int32_t *steps,  byte idx)
//************************************************************************************************

{float pos = steps[idx]/settings.steps_per_mm[idx];
return(pos);
}
//************************************************************************************************
void system_convert_array_steps_to_mpos(float *position, int32_t *steps)
//************************************************************************************************
{
   byte idx;
  for (idx=0; idx<N_AXIS; idx++) {
    position[idx] = system_convert_axis_steps_to_mpos(steps, idx);
  }
  return;
}



