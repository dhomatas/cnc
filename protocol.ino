
//__________________________________________________________________
  
               void protocol_execute_line(char *line) 
//__________________________________________________________________
{      
  protocol_execute_realtime(); // Runtime command check point.
  if (sys.abort)  return; // Bail to calling function upon system abort  

  #ifdef REPORT_ECHO_LINE_RECEIVED
    Serial.println("  ");
    report_echo_line_received(line);
  #endif
 if      (line[0] == 0  )   report_status_message(STATUS_OK);  // Empty or comment line. Send status message for syncing purposes.
  else if (sys.state == STATE_ALARM) report_status_message(STATUS_ALARM_LOCK); // Everything else is gcode. Block if in alarm mode.
  else    report_status_message(gc_execute_line(line));// Parse and execute g-code block!*/
}


//_______________________________________________________________________________________________

                      void protocol_execute_realtime()
//_______________________________________________________________________________________________

{
  byte rt_exec; // Temp variable to avoid calling volatile multiple times.

  do { 
  rt_exec = sys_rt_exec_alarm; 
  if (rt_exec) { 
    sys.state = STATE_ALARM; // Set system alarm state
    if (rt_exec & EXEC_ALARM_HARD_LIMIT) {
      report_alarm_message(ALARM_HARD_LIMIT_ERROR); 
    } else if (rt_exec & EXEC_ALARM_SOFT_LIMIT) {
      report_alarm_message(ALARM_SOFT_LIMIT_ERROR);
    } else if (rt_exec & EXEC_ALARM_ABORT_CYCLE) {      
      report_alarm_message(ALARM_ABORT_CYCLE);
    } else if (rt_exec & EXEC_ALARM_PROBE_FAIL) {
      report_alarm_message(ALARM_PROBE_FAIL);
    } else if (rt_exec & EXEC_ALARM_HOMING_FAIL) {
      report_alarm_message(ALARM_HOMING_FAIL);
    }
    if (rt_exec & EXEC_CRITICAL_EVENT) {
      report_feedback_message(MESSAGE_CRITICAL_EVENT);  // prints "Reset to continue"
      bit_false_atomic(sys_rt_exec_state,EXEC_RESET);    // αναβει το bit reset
      do { 

      } while (bit_isfalse(sys_rt_exec_state,EXEC_RESET));  // περιμενει να σβησει  
    }
    
    bit_false_atomic(sys_rt_exec_alarm,0xFF);   // σβηνει τα bits toy αλαρμ
  }
  
  rt_exec = sys_rt_exec_state; 
  if (rt_exec) { 
    if (rt_exec & EXEC_RESET) {sys.abort = true;  return;   }
    else if (rt_exec & EXEC_STATUS_REPORT) { 
      report_realtime_status();
      bit_false_atomic(sys_rt_exec_state,EXEC_STATUS_REPORT);
      }
    else if (rt_exec & (EXEC_MOTION_CANCEL | EXEC_FEED_HOLD  )) {
      if ((sys.state == STATE_IDLE) || (sys.state & (STATE_CYCLE | STATE_HOMING | STATE_MOTION_CANCEL | STATE_HOLD  ))) {
        if (sys.state == STATE_CYCLE) {
         // st_update_plan_block_parameters(); 
          sys.suspend = SUSPEND_ENABLE_HOLD; 
        }
        if (sys.state == STATE_IDLE) { sys.suspend = SUSPEND_ENABLE_READY; }
        if (rt_exec & EXEC_MOTION_CANCEL) {

          if (sys.state == STATE_CYCLE) { sys.state = STATE_MOTION_CANCEL; }
          sys.suspend |= SUSPEND_MOTION_CANCEL; 
        
        }
         
      }
      bit_false_atomic(sys_rt_exec_state,(EXEC_MOTION_CANCEL | EXEC_FEED_HOLD ));      
    }

    if (rt_exec & EXEC_CYCLE_START) {
      if (!(rt_exec & (EXEC_FEED_HOLD | EXEC_MOTION_CANCEL ))) { 
        if ((sys.state == STATE_IDLE) || ((sys.state & (STATE_HOLD | STATE_MOTION_CANCEL)) && (sys.suspend & SUSPEND_ENABLE_READY))) {
          if (sys.suspend & SUSPEND_ENERGIZE) { 
 //           if (command.spindle != SPINDLE_DISABLE) { 
 //             spindle_set_state(command.spindle, values.spindle_speed); 
 //            }
            }
          }
      }
    }

 
          

          
          else {  sys.state = STATE_IDLE;       }
          sys.suspend = SUSPEND_DISABLE; // Break suspend state.
        }
          
      bit_false_atomic(sys_rt_exec_state,EXEC_CYCLE_START);
   
       
    if (rt_exec & EXEC_CYCLE_STOP) {
      if (sys.state & (STATE_HOLD   && !(sys.soft_limit))) {
        if (sys.suspend & SUSPEND_ENERGIZE) { 
//           spindle_stop();

        }
        bit_true(sys.suspend,SUSPEND_ENABLE_READY);
      } else {
        sys.suspend = SUSPEND_DISABLE;
        sys.state = STATE_IDLE;
      }
      bit_false_atomic(sys_rt_exec_state,EXEC_CYCLE_STOP);
    }
    
  if (sys.state & (STATE_CYCLE | STATE_HOLD | STATE_MOTION_CANCEL   | STATE_HOMING)) {} //st_prep_buffer(); }  
  


}  while(sys.suspend); 
  
}  
 /*
//******************************************************************************************************************
void protocol_buffer_synchronize()
//*********************************************************************************************
{}/*
 protocol_auto_cycle_start();
  do {
  //  protocol_execute_realtime();   
    if (sys.abort) { return; } 
  } while (plan_get_current_block() || (sys.state == STATE_CYCLE));
}
*/ 
//__________________________________________________________________

               void protocol_auto_cycle_start() 
//__________________________________________________________________
{
  bit_true_atomic(sys_rt_exec_state, EXEC_CYCLE_START);
} 


