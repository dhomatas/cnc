 
//__________________________________________________________________
  
                void report_status_onDisplay()
//__________________________________________________________________
{
  display.clearDisplay();  display.display();delay(2000);
  display.setCursor(0,0);display.print("Sys "); 
  switch (sys.state) {
    case STATE_IDLE:        display.print("Idle"); break;            
    case STATE_MOTION_CANCEL: // Report run state.
    case STATE_CYCLE:       display.print("Run"); break;                     
    case STATE_HOLD:      display.print("Hold"); break;                   
    case STATE_HOMING:     display.print ("Home"); break;                             
    case STATE_ALARM:      display.print ("Alarm"); break;                  
    case STATE_CHECK_MODE: display.print ("Check"); break;     
  } 
 display.display(); delay(2000);
}
 
//-----------------------------------------------------------------
              void report_status_message( byte status_code) 
//-----------------------------------------------------------------
{
  if (status_code == 0) { // STATUS_OK
    Serial.println(" ok ");
  } else { 
    Serial.print("error: ");
     switch(status_code) {          
        case STATUS_EXPECTED_COMMAND_LETTER:
        Serial.println("Expected command letter"); break;
        case STATUS_BAD_NUMBER_FORMAT:
        Serial.println("Bad number format"); break;
        case STATUS_INVALID_STATEMENT:
        Serial.println("Invalid statement"); break;
        case STATUS_NEGATIVE_VALUE:
        Serial.println("Value < 0"); break;
        case STATUS_SETTING_DISABLED:
        Serial.println("Setting disabled"); break;
        case STATUS_SETTING_STEP_PULSE_MIN:
        Serial.println("Value < 3 usec"); break;
        case STATUS_SETTING_READ_FAIL:
        Serial.println("EEPROM read fail. Using defaults"); break;
        case STATUS_IDLE_ERROR:
        Serial.println("Not idle"); break;
        case STATUS_ALARM_LOCK:
        Serial.println("Alarm lock"); break;
        case STATUS_SOFT_LIMIT_ERROR:
        Serial.println("Homing not enabled"); break;
        case STATUS_OVERFLOW:
        Serial.println("Line overflow"); break;
        #ifdef MAX_STEP_RATE_HZ
          case STATUS_MAX_STEP_RATE_EXCEEDED: 
          Serial.println("Step rate > 30kHz"); break;
        #endif      
        // Common g-code parser errors.
        case STATUS_GCODE_MODAL_GROUP_VIOLATION:
        Serial.println("Modal group violation"); break;
        case STATUS_GCODE_UNSUPPORTED_COMMAND:
        Serial.println("Unsupported command"); break;
        case STATUS_GCODE_UNDEFINED_FEED_RATE:
        Serial.println("Undefined feed rate"); break;
         case STATUS_GCODE_COMMAND_VALUE_NOT_INTEGER:
        Serial.println("Gcode Command Value must be integer"); break;
        case STATUS_GCODE_AXIS_COMMAND_CONFLICT :
        Serial.println(" Axis Command conflict "); break;
         case STATUS_GCODE_WORD_REPEATED :
        Serial.println("Gcode word repeated"); break;
        case STATUS_GCODE_NO_AXIS_WORDS :
        Serial.println("Gcode No axis "); break;
        case STATUS_GCODE_VALUE_WORD_MISSING:
        Serial.println("Gcode Value missing"); break;
        case STATUS_GCODE_UNSUPPORTED_COORD_SYS :
        Serial.println("Gcode Unsupported coord system"); break;
        case STATUS_GCODE_G53_INVALID_MOTION_MODE :
        Serial.println("Gcode G53 Invalid Motion Mode"); break;
        case STATUS_GCODE_AXIS_WORDS_EXIST :
        Serial.println("Gcode axis word exist"); break;
        case STATUS_GCODE_NO_AXIS_WORDS_IN_PLANE :
        Serial.println("Gcode No axis words in plane"); break;
        case STATUS_GCODE_INVALID_TARGET :
        Serial.println("Gcode Invalid target"); break;
        case STATUS_GCODE_ARC_RADIUS_ERROR :
        Serial.println("Gcode Arc radius Error"); break;
        case STATUS_GCODE_NO_OFFSETS_IN_PLANE :
        Serial.println("Gcode no offsets in plane"); break;
        case STATUS_GCODE_UNUSED_WORDS :
        Serial.println("GcodeUnused words"); break;
        case STATUS_GCODE_G43_DYNAMIC_AXIS_ERROR :
        Serial.println("Gcode G43 Dynamic axis error"); break; 

      }
  }
}

//-----------------------------------------------------------------
          void report_alarm_message(int8_t alarm_code)
//-----------------------------------------------------------------
{ Serial.println( );
 Serial.print("ALARM: ");

    switch (alarm_code) {
      case ALARM_HARD_LIMIT_ERROR:  Serial.print("Hard limit");         break;
      case ALARM_SOFT_LIMIT_ERROR:  Serial.print("Soft limit");         break;
      case ALARM_ABORT_CYCLE:       Serial.print("Abort during cycle"); break;
      case ALARM_PROBE_FAIL:        Serial.print("Probe fail");         break;
      case ALARM_HOMING_FAIL:       Serial.print("Homing fail");        break;
    }

}

//-----------------------------------------------------------------
               void report_feedback_message( byte message_code)
//-----------------------------------------------------------------{
{  Serial.print(" [ ");
  switch(message_code) {
    case MESSAGE_CRITICAL_EVENT:    Serial.print("Reset to continue"); break;
    case MESSAGE_ALARM_LOCK:    Serial.print("'$H'|'$X' to unlock"); break;
    case MESSAGE_ALARM_UNLOCK:   Serial.print("Caution: Unlocked"); break;
    case MESSAGE_ENABLED:    Serial.print("Enabled"); break;
    case MESSAGE_DISABLED:    Serial.print("Disabled"); break; 
    case MESSAGE_PROGRAM_END:    Serial.print("Program End"); break;
    case MESSAGE_RESTORE_DEFAULTS:    Serial.print("Restoring defaults"); break;
  }
 Serial.println(" ] ");
}

//___________________________________________________________________

                    void report_grbl_help() {
//___________________________________________________________________

    Serial.println( );
    Serial.println( "********H E L P ********* ");
    Serial.println( "1                         ");
    Serial.println( "2                         ");
    Serial.println( "3                         ");
    Serial.println( "4                         ");
    Serial.println( "5  run homing cycle  ");
    Serial.println( "6  Open SD file #1  ");
    Serial.println( "7  Open SD file #2  ");
    Serial.println( "8  Open SD file #3  ");
    Serial.println( "9  Open SD file #4  ");
    Serial.println( "************************ ");
    Serial.println( );
}

//-----------------------------------------------------------------
void report_grbl_settings(){
//-----------------------------------------------------------------

      
    Serial.print(" step pulse, usec           "); Serial.println  (settings.pulse_microseconds);
    Serial.print(" step idle delay, msec      "); Serial.println( settings.stepper_idle_lock_time);
    Serial.print(" step port invert mask:     ");Serial.println(   settings.step_invert_mask ,BIN); 
    Serial.print(" dir port invert mask:      "); Serial.println(   settings.dir_invert_mask,BIN ); 
    Serial.print(" steppers pin invert, bool  "); Serial.println(  bit_istrue(settings.flags,BITFLAG_INVERT_ST_ENABLE),BIN);
    Serial.print(" probe pin invert, bool     "); Serial.println( bit_istrue(settings.flags,BITFLAG_INVERT_PROBE_PIN));
    Serial.print(" status report mask:        ");Serial.println(   settings.status_report_mask ,BIN);
    Serial.print(" junction deviation, mm     ");Serial.println(   settings.junction_deviation);
    Serial.print(" arc tolerance, mm          ");Serial.println( settings.arc_tolerance);
    Serial.print(" soft limits, bool          ");Serial.println(  bit_istrue(settings.flags,BITFLAG_SOFT_LIMIT_ENABLE));  
    Serial.print(" hard limits, bool          "); Serial.println(    bit_istrue(settings.flags,BITFLAG_HARD_LIMIT_ENABLE));
    Serial.print(" homing cycle, bool         ");Serial.println(   bit_istrue(settings.flags,BITFLAG_HOMING_ENABLE));
    Serial.print(" homing dir   mask:         "); Serial.println(   settings.homing_dir_mask ,BIN);  
    Serial.print(" homing feed, mm/min       ");Serial.println(   settings.homing_feed_rate); 
    Serial.print(" homing seek, mm/min       "); Serial.println ( settings.homing_seek_rate);
    Serial.print(" homing pull-off, mm)      ");Serial.println   (settings.homing_pulloff);

  
 Serial.println();
 Serial.println("Print axis settings");
 Serial.println( "        step/mm      max rate[mm/min]  accel[mm/sec^2]  max travel[mm]");
    
    for ( byte idx=0; idx<N_AXIS; idx++) {
      if (idx ==0 )Serial.print ("X       ");
      else if(idx ==1 )Serial.print ("Y       ");
      else if(idx ==2 )Serial.print ("Z       ");
          Serial.print (settings.steps_per_mm[idx]);
          Serial.print ("       ");
          Serial.print (settings.max_rate[idx]);
          Serial.print ("             ");
          Serial.print (settings.acceleration[idx]/(60*60));
          Serial.print ("             ");
          Serial.println (-settings.max_travel[idx]);
    }

}



//*********************************************************************************************
void report_probe_parameters()
//*********************************************************************************************
{
   byte i;
  float print_position[N_AXIS];
 
  // Report in terms of machine position.
  Serial.print(" [ PRB:");
  for (i=0; i< N_AXIS; i++) {
 //   print_position[i] = system_convert_axis_steps_to_mpos(sys.probe_position,i);
    Serial.print(print_position[i]);
    if (i < (N_AXIS-1)) { Serial.print(","); }
  }
  Serial.print(":");
  Serial.print(sys.probe_succeeded);
  Serial.print(" ]");
}


// Prints Grbl NGC parameters (coordinate offsets, probing)
//***************************************************************
void report_ngc_parameters()
//***************************************************************
{
  float coord_data[N_AXIS];
   byte coord_select, i;
  for (coord_select = 0; coord_select <= SETTING_INDEX_NCOORD; coord_select++) { 
   // if (!(settings_read_coord_data(coord_select,coord_data))) { 
   //   report_status_message(STATUS_SETTING_READ_FAIL); 
  //    return;
 //   settings_read_coord_data(coord_select,coord_data);
    } 
    Serial.print("[G");
    switch (coord_select) {
      case 6: Serial.print("28"); break;
      case 7: Serial.print("30"); break;
      default: Serial.print(coord_select+54); break; // G54-G59
    }  
    Serial.print(":");         
    for (i=0; i<N_AXIS; i++) {
      Serial.print(coord_data[i]);
      if (i < (N_AXIS-1)) { Serial.print(","); }
      else { Serial.print("]"); }
    } 
  
  Serial.print("[G92:"); // Print G92,G92.1 which are not persistent in memory
  for (i=0; i<N_AXIS; i++) {
    Serial.print(values.coord_offset[i]);
    if (i < (N_AXIS-1)) { Serial.print(","); }
    else { Serial.print("]\r\n"); }
  } 
  Serial.print("[TLO:"); // Print tool length offset value
  Serial.print(values.tool_length_offset);
  Serial.print("] ");
  report_probe_parameters(); // Print probe parameters. Not persistent in memory.
}


// Print current gcode parser mode state
//***************************************************************
void report_gcode_modes(){
//***************************************************************
 
  Serial.print("[");
  
  switch (command.motion) {
    case MOTION_MODE_SEEK : Serial.print("G0"); break;
    case MOTION_MODE_LINEAR : Serial.print("G1"); break;
    case MOTION_MODE_CW_ARC : Serial.print("G2"); break;
    case MOTION_MODE_CCW_ARC : Serial.print("G3"); break;
    case MOTION_MODE_NONE : Serial.print("G80"); break;
    default: 
      Serial.print("G38.");
      Serial.print(command.motion - (MOTION_MODE_PROBE_TOWARD-2));
  }

  Serial.print(" G");
  Serial.print(command.coord_select+54);
  
  switch (command.plane_select) {
    case PLANE_SELECT_XY : Serial.print(" G17"); break;
    case PLANE_SELECT_ZX : Serial.print(" G18"); break;
    case PLANE_SELECT_YZ : Serial.print(" G19"); break;
  }
  
 
  
  if (command.distance == DISTANCE_MODE_ABSOLUTE) { Serial.print(" G90"); }
  else { Serial.print(" G91"); }
  
  if (command.feed_rate == FEED_RATE_MODE_INVERSE_TIME) { Serial.print(" G93"); }
  else { Serial.print(" G94"); }
    
  switch (command.program_flow) {
    case PROGRAM_FLOW_RUNNING : Serial.print(" M0"); break;
    case PROGRAM_FLOW_PAUSED : Serial.print(" M1"); break;
    case PROGRAM_FLOW_COMPLETED : Serial.print(" M2"); break;
  }

  switch (command.spindle) {
    case SPINDLE_ENABLE_CW : Serial.print(" M3"); break;
    case SPINDLE_ENABLE_CCW : Serial.print(" M4"); break;
    case SPINDLE_DISABLE : Serial.print(" M5"); break;
  }
  

 
  
  Serial.print(" F");
  Serial.print(values.f );
  
  #ifdef VARIABLE_SPINDLE
    Serial.print(" S");
    Serial.print(values.s );
  #endif

  Serial.println("]");
}



//***************************************************************
void report_echo_line_received(char *line){
//***************************************************************

  Serial.print("Line:  "); 
  Serial.println(line);
}

 //***************************************************************
void report_realtime_status(){
//***************************************************************

 
   byte idx;
  int32_t current_position[N_AXIS]; // Copy current state of the system position variable
  memcpy(current_position,sys.position,sizeof(sys.position));
  float print_position[N_AXIS];
// display.setCursor(0,0);display.print("Sys ");
  // Report current machine state
  switch (sys.state) {
    case STATE_IDLE:        Serial.print("<Idle"); break;           // display.print("IDLE"); break;
    case STATE_MOTION_CANCEL: // Report run state.
    case STATE_CYCLE:       Serial.print("<Run"); break;                    //display.print("RUN"); break;
    case STATE_HOLD:       Serial.print("<Hold"); break;                  //display.print("HOLD"); break;
    case STATE_HOMING:     Serial.print ("<Home"); break;                             //display.print("HOME");break;
    case STATE_ALARM:      Serial.print ("<Alarm"); break;                 //display.print("ALARM");break;
    case STATE_CHECK_MODE: Serial.print ("<Check"); break;                      // display.print("CHECK");break;
  }
 // display.display();
/* 
  if (bit_istrue(settings.status_report_mask,(BITFLAG_RT_STATUS_MACHINE_POSITION | BITFLAG_RT_STATUS_WORK_POSITION))) {
    system_convert_array_steps_to_mpos(print_position,current_position);
  }
  
  // Report machine position
  if (bit_istrue(settings.status_report_mask,BITFLAG_RT_STATUS_MACHINE_POSITION)) {
    Serial.print(",MPos:"); 
    for (idx=0; idx< N_AXIS; idx++) {
      Serial.print(print_position[idx]);
      if (idx < (N_AXIS-1)) { Serial.print(","); }
    }
  }
  
  // Report work position
  if (bit_istrue(settings.status_report_mask,BITFLAG_RT_STATUS_WORK_POSITION)) {
    Serial.print(",WPos:"); 
    for (idx=0; idx< N_AXIS; idx++) {
      // Apply work coordinate offsets and tool length offset to current position.
      print_position[idx] -= values.coord_system[idx]+values.coord_offset[idx];

 if (idx == TOOL_LENGTH_OFFSET_AXIS) { print_position[idx] -= values.tool_length_offset; }    

      
      
      Serial.print(print_position[idx]);
      if (idx < (N_AXIS-1)) { Serial.print(","); }
    }
  }
        
  // Returns the number of active blocks are in the planner buffer.
  if (bit_istrue(settings.status_report_mask,BITFLAG_RT_STATUS_PLANNER_BUFFER)) {
    Serial.print(",Buf:");
//    Serial.println(plan_get_block_buffer_count());
  }
  
  #ifdef USE_LINE_NUMBERS
    // Report current line number
    Serial.print(",Ln:"); 
    int32_t ln=0;
//    plan_block_t * pb = plan_get_current_block();
//    if(pb != NULL) {
//      ln = pb->line_number; 
//    } 
    Serial.println(ln);
  #endif
    
  #ifdef REPORT_REALTIME_RATE
    // Report realtime rate 
    Serial.print(",F:"); 
//    Serial.print(st_get_realtime_rate());
  #endif    
  
  if (bit_istrue(settings.status_report_mask,BITFLAG_RT_STATUS_LIMIT_PINS)) {
    Serial.print(",Lim:");
    Serial.print(limits_get_state());//,2,N_AXIS);
  }
  
  #ifdef REPORT_CONTROL_PIN_STATE 
    Serial.print(",Ctl:");
    Serial.print((CONTROL_PIN & CONTROL_MASK),BIN);
  #endif
  
  Serial.println(" >");
  */
}

