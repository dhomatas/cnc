
//____________________________________________________________________

                      void limits_go_home(){ //(byte cycle_mask) 
//____________________________________________________________________
 
  if (sys.abort)   return;   
 
  stepperX.setMaxSpeed(DEFAULT_HOMING_SEEK_RATE);
  stepperY.setMaxSpeed(DEFAULT_HOMING_SEEK_RATE);
    stepperX.setAcceleration(DEFAULT_HOMING_SEEK_RATE/2);
    stepperY.setAcceleration(DEFAULT_HOMING_SEEK_RATE/2);
  stepperX.moveTo(DEFAULT_X_MAX_TRAVEL*DEFAULT_X_STEPS_PER_MM);
  stepperY.moveTo(-DEFAULT_Y_MAX_TRAVEL*DEFAULT_Y_STEPS_PER_MM);
  Serial.print("DEFAULT_X_MAX_TRAVEL =");Serial.println( DEFAULT_X_MAX_TRAVEL);
   Serial.print("DEFAULT_HOMING_SEEK_RATE =");Serial.println( DEFAULT_HOMING_SEEK_RATE);

 if (HOMING_Z){
        stepperZ.setMaxSpeed(DEFAULT_HOMING_SEEK_RATE);
        stepperZ.setAcceleration(DEFAULT_HOMING_SEEK_RATE/2); 
        stepperZ.moveTo(-DEFAULT_Z_MAX_TRAVEL*DEFAULT_Y_STEPS_PER_MM);
        while (digitalRead(19)==HIGH){ 
          stepperZ.run();
        }
 }
 
 while (digitalRead(19)==HIGH){ 
    stepperX.run();
  }
  
  while (digitalRead(20)==HIGH){ 
    stepperY.run();
  }
    stepperX.setCurrentPosition(0);
    stepperY.setCurrentPosition(0);
    
    stepperX.setMaxSpeed(DEFAULT_HOMING_FEED_RATE);
    stepperY.setMaxSpeed(DEFAULT_HOMING_FEED_RATE);
    
    stepperX.setAcceleration(DEFAULT_HOMING_FEED_RATE/2);
    stepperY.setAcceleration(DEFAULT_HOMING_FEED_RATE/2);
    

        {stepperX.moveTo(-DEFAULT_HOMING_PULLOFF * DEFAULT_X_STEPS_PER_MM);
         stepperY.moveTo(DEFAULT_HOMING_PULLOFF * DEFAULT_Y_STEPS_PER_MM);}
         
if (HOMING_Z){
   stepperZ.setCurrentPosition(0);
   stepperZ.setMaxSpeed(DEFAULT_HOMING_FEED_RATE);
    stepperZ.setAcceleration(DEFAULT_HOMING_FEED_RATE/2);
    stepperZ.moveTo(DEFAULT_HOMING_PULLOFF * DEFAULT_Y_STEPS_PER_MM);
    while  (stepperZ.distanceToGo() != 0) stepperZ.run();
}
         
while  (stepperX.distanceToGo() != 0) stepperX.run();
while  (stepperY.distanceToGo() != 0) stepperY.run();   
  
 // Set machine positions for homed limit switches. 
  int32_t set_axis_position;
  for (byte idx=0; idx<N_AXIS; idx++) {
      if (HOMING_FORCE_SET_ORIGIN) set_axis_position = 0;
      else { 
        if ( bit_istrue(settings.homing_dir_mask,bit(idx)) ) {
          set_axis_position = lround((settings.max_travel[idx]+settings.homing_pulloff)*settings.steps_per_mm[idx]);
        } else {
          set_axis_position = lround(-settings.homing_pulloff*settings.steps_per_mm[idx]);
        }
      }
      sys.position[idx] = set_axis_position;
      Serial.print("axis position for axis  ");Serial.print(idx);Serial.print("  =  ");Serial.println( sys.position[idx]);
   }
   display.clearDisplay();

    //               12345678901234
    display.println("Homing OK");
    display.println("Coords reset");
    display.display();
    delay(2000);
    return;
  }
 

// Performs a soft limit check. Called from mc_line() only. Assumes the machine has been homed,
// the workspace volume is in all negative space, and the system is in normal operation.
//________________________________________________________________________________________________

                    void limits_soft_check(float *target){
//_______________________________________________________________________________________________

  byte idx;
  for (idx=0; idx<N_AXIS; idx++) {
   
     if ( HOMING_FORCE_SET_ORIGIN ){
      // When homing forced set origin is enabled, soft limits checks need to account for directionality.
      // NOTE: max_travel is stored as negative
      if (bit_istrue(settings.homing_dir_mask,bit(idx))) {
        if (target[idx] < 0 || target[idx] > -settings.max_travel[idx])   sys.soft_limit = true; 
      } else {
        if (target[idx] > 0 || target[idx] < settings.max_travel[idx])   sys.soft_limit = true;  
      }}
     else { 
      // NOTE: max_travel is stored as negative
      if (target[idx] > 0 || target[idx] < settings.max_travel[idx])  sys.soft_limit = true;  
     }
    
    if (sys.soft_limit) {
 
      if (sys.state == STATE_CYCLE) {
        bit_true_atomic(sys_rt_exec_state, EXEC_FEED_HOLD);
        do {
        
          protocol_execute_realtime();

          if (sys.abort) { return; }
        } while ( sys.state != STATE_IDLE );
      }
      Sys_reset();  
      bit_true_atomic(sys_rt_exec_alarm, (EXEC_ALARM_SOFT_LIMIT|EXEC_CRITICAL_EVENT));  

     protocol_execute_realtime();  

      return;
    }
  }
}



