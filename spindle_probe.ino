
//***********************************************************************
                 void spindle_stop(){
//***********************************************************************
  digitalWrite (7,LOW); 
}


//***********************************************************************
                   void spindle_run(){
//***********************************************************************
 if (sys.state == STATE_CHECK_MODE) { return; }
  digitalWrite (7,HIGH);
}

// Perform tool length probe cycle. Requires probe switch.
// NOTE: Upon probe failure, the program will be stopped and placed into ALARM state.
//****************************************************************************************************
  void mc_probe_cycle(float *target, float feed_rate, uint8_t invert_feed_rate, uint8_t is_probe_away, uint8_t is_no_error){
 
//****************************************************************************************************
  }
/*  
  if (sys.state == STATE_CHECK_MODE) { return; }
  //  Probing control variables
  sys.probe_succeeded = false;  
  probe_configure_invert_mask(is_probe_away);
  
  // After syncing, check if probe is already triggered. If so, halt and issue alarm.
  // NOTE: This probe initialization error applies to all probing cycles.
  if ( probe_get_state() ) { // Check probe pin state.
    bit_true_atomic(sys_rt_exec_alarm, EXEC_ALARM_PROBE_FAIL);
    protocol_execute_realtime();
  }
  if (sys.abort) { return; }  

    mc_line(target, feed_rate, invert_feed_rate);

  
  // Activate the probing state monitor in the stepper module.
  sys_probe_state = PROBE_ACTIVE;

  // Perform probing cycle. Wait here until probe is triggered or motion completes.
  bit_true_atomic(sys_rt_exec_state, EXEC_CYCLE_START);
  do {
    protocol_execute_realtime(); 
    if (sys.abort) { return; } // Check for system abort
  } while (sys.state != STATE_IDLE);
  
  // Probing cycle complete!
  
  // Set state variables and error out, if the probe failed and cycle with error is enabled.
  if (sys_probe_state == PROBE_ACTIVE) {
    if (is_no_error) { memcpy(sys.probe_position, sys.position, sizeof(float)*N_AXIS); }
    else { bit_true_atomic(sys_rt_exec_alarm, EXEC_ALARM_PROBE_FAIL); }
  } else { 
    sys.probe_succeeded = true; // Indicate to system the probing cycle completed successfully.
  }
  sys_probe_state = PROBE_OFF; // Ensure probe state monitor is disabled.
  protocol_execute_realtime();   // Check and execute run-time commands
  if (sys.abort) { return; } // Check for system abort

  // Reset the stepper and planner buffers to remove the remainder of the probe motion.
  st_reset(); // Reest step segment buffer.
  plan_reset(); // Reset planner buffer. Zero planner positions. Ensure probing motion is cleared.
  plan_sync_position(); // Sync planner position to current machine position.

  // TODO: Update the g-code parser code to not require this target calculation but uses a gc_sync_position() call.
  // NOTE: The target[] variable updated here will be sent back and synced with the g-code parser.
  system_convert_array_steps_to_mpos(target, sys.position);

  #ifdef MESSAGE_PROBE_COORDINATES
    // All done! Output the probe position as message.
    report_probe_parameters();
  #endif
}
*/

