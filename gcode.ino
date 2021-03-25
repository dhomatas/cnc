
  

//____________________________________________________________________

                              void gc_init() 
//____________________________________________________________________
                              
{
 // memset(&values, 0, sizeof(values));
  
  Serial.println(" Load default G54 coordinate system.");
  // if (!(settings_read_coord_data(command.coord_select,values.coord_system))) { 
//    settings_read_coord_data(command.coord_select,values.coord_system);
    //report_status_message(STATUS_SETTING_READ_FAIL); 
    
   
}

/*
//____________________________________________________________________
// Sets g-code parser position in mm. Input in steps. Called by the system abort and hard
// limit pull-off routines.
                        void gc_sync_position() 
//____________________________________________________________________

{system_convert_array_steps_to_mpos(values.position,sys.position);}

*/
//____________________________________________________________________

         byte gc_check_same_position(float *pos_a, float *pos_b) 

//____________________________________________________________________
{
  byte idx;
  for (idx=0; idx<N_AXIS; idx++) {
    if (pos_a[idx] != pos_b[idx]) { return(false); }
  }
  return(true);
}



//____________________________________________________________________

          byte gc_execute_line(char *line) 

//____________________________________________________________________

{
 
 // memset(&values, 0, sizeof(values)); // Initialize the parser block struct.
  memcpy(&command,&saved_command,sizeof(command)); // Copy current modes
   byte axis_command = AXIS_COMMAND_NONE;
   byte axis_0, axis_1, axis_linear;
   byte coord_select = 0; // Tracks G10 P coordinate selection for execution
  float coordinate_data[N_AXIS]; // Multi-use variable to store coordinate data for execution
  float parameter_data[N_AXIS]; // Multi-use variable to store parameter data for execution
  float Fr;
  // Initialize bitflag tracking variables
   byte axis_words = 0; // XYZ tracking
   byte ijk_words = 0; // IJK tracking 
   int command_words = 0; 
   int value_words;

   byte word_bit; 
   counter = 0;  
  char letter;
  EndOL=false;
   float h_x2_div_d ;

  while (line[counter] != '@' ) { 
    if (EndOL== true) {letter ='@';goto mark2;}
    letter = line[counter];
   #ifdef DEBUG_GCODE_TEXT
   
                     Serial.print("G: Commant char  ");
                    Serial.print (letter); Serial.print(" at poz #  ");
                     Serial.println(counter);
   #endif                 
    if((letter < 'A') || (letter > 'Z')) { return(STATUS_EXPECTED_COMMAND_LETTER); } // [Expected word letter]
    
    counter++;
    
    if (!read_float())   return(STATUS_BAD_NUMBER_FORMAT);   // [Expected word value]
   // if (EndOL== true) break;

  #ifdef DEBUG_GCODE_TEXT
        Serial.print  ( akeraio);
    if (dekadiko != 0){   Serial.print  (".");Serial.println  (dekadiko);}
    else                  Serial.println( );
  #endif 
      value = value+akeraio;
       if(isNegativ) { 
                      value =  (-1)* value ;
                      isNegativ = false;}  
 #ifdef DEBUG_GCODE_TEXT                     
       Serial.print  ( "value  = ");
      Serial.println  ( value);
 #endif 
//       if (EndOL== true) break;
//      counter++;
//   }
mark2:
    switch(letter) { 
      case 'G':
        if (dekadiko > 0) { return(STATUS_GCODE_COMMAND_VALUE_NOT_INTEGER); } // [No Gxx.x commands]
        switch(akeraio) {
           case 0: command.motion = MOTION_MODE_SEEK;   
                   word_bit = MODAL_GROUP_G1;
                   if (axis_command) { return(STATUS_GCODE_AXIS_COMMAND_CONFLICT); } 
                   axis_command = AXIS_COMMAND_MOTION_MODE; 
                   break; 
          case 1: command.motion = MOTION_MODE_LINEAR; 
                   word_bit = MODAL_GROUP_G1;
                   if (axis_command) { return(STATUS_GCODE_AXIS_COMMAND_CONFLICT); } 
                   axis_command = AXIS_COMMAND_MOTION_MODE; 
                   break; 
          case 2: command.motion = MOTION_MODE_CW_ARC;   
                   word_bit = MODAL_GROUP_G1;
                   if (axis_command) { return(STATUS_GCODE_AXIS_COMMAND_CONFLICT); } 
                   axis_command = AXIS_COMMAND_MOTION_MODE; 
                   break; 
          case 3: command.motion = MOTION_MODE_CCW_ARC; 
                   word_bit = MODAL_GROUP_G1;
                   if (axis_command) { return(STATUS_GCODE_AXIS_COMMAND_CONFLICT); } 
                   axis_command = AXIS_COMMAND_MOTION_MODE; 
                   break; 
            case 4: word_bit = MODAL_GROUP_G0; 
                    command.non_command = NON_MODAL_DWELL;  
                    break; 
          case 10: if (axis_command) { return(STATUS_GCODE_AXIS_COMMAND_CONFLICT); } 
                      axis_command = AXIS_COMMAND_NON_MODAL;
                      command.non_command = NON_MODAL_SET_COORDINATE_DATA;  
                   break;
          case 28: if (axis_command) { return(STATUS_GCODE_AXIS_COMMAND_CONFLICT); } 
                      axis_command = AXIS_COMMAND_NON_MODAL;
                   break; 
           case 30:  if (axis_command) { return(STATUS_GCODE_AXIS_COMMAND_CONFLICT); } 
                      axis_command = AXIS_COMMAND_NON_MODAL;
                      switch(dekadiko) {
                        case 0: command.non_command = G30_NON_MODAL_GO_HOME_0;  break; 
                        case 10: command.non_command =  G30_1_NON_MODAL_SET_HOME_1;   break; 
                        default: return(STATUS_GCODE_UNSUPPORTED_COMMAND); break; 
                      }
                      dekadiko = 0; 
                  break;
          case 38:         word_bit = MODAL_GROUP_G1;
                           if (axis_command) { return(STATUS_GCODE_AXIS_COMMAND_CONFLICT); } 
                            axis_command = AXIS_COMMAND_MOTION_MODE; 
                          switch(dekadiko) {
                            case 20: command.motion = MOTION_MODE_PROBE_TOWARD;   break;
                            case 30: command.motion = MOTION_MODE_PROBE_TOWARD_NO_ERROR; break; 
                            case 40: command.motion = MOTION_MODE_PROBE_AWAY; break; 
                            case 50: command.motion = MOTION_MODE_PROBE_AWAY_NO_ERROR; break; 
                            default: return(STATUS_GCODE_UNSUPPORTED_COMMAND); break; 
                          }
                          dekadiko = 0; 
                          break;        
          case 53:  command.non_command = NON_MODAL_ABSOLUTE_OVERRIDE;
                    word_bit = MODAL_GROUP_G0; 
                    
                         
          
         case 92:  
                      if (axis_command) { return(STATUS_GCODE_AXIS_COMMAND_CONFLICT); } 
                      axis_command = AXIS_COMMAND_NON_MODAL;
                      switch(dekadiko) {
                        case 0: command.non_command = NON_MODAL_SET_COORDINATE_OFFSET;   break; 
                        case 10: command.non_command = NON_MODAL_RESET_COORDINATE_OFFSET;   break; 
                        default: return(STATUS_GCODE_UNSUPPORTED_COMMAND); break; 
                      }
                      dekadiko = 0; 
                     
                 break;
          
          case 80: command.motion = MOTION_MODE_NONE; break; 
          case 17: case 18: case 19: 
                      word_bit = MODAL_GROUP_G2; 
                      switch(akeraio) {
                        case 17: command.plane_select = PLANE_SELECT_XY; break;
                        case 18: command.plane_select = PLANE_SELECT_ZX; break;
                        case 19: command.plane_select = PLANE_SELECT_YZ; break;
                      }
                   break;
          case 90: case 91: 
                      if (dekadiko == 0) {
                        word_bit = MODAL_GROUP_G3; 
                        if (akeraio == 90)   command.distance = DISTANCE_MODE_ABSOLUTE; 
                        else   command.distance = DISTANCE_MODE_INCREMENTAL;} 
                      else {
                        word_bit = MODAL_GROUP_G4;
                        if ((dekadiko != 10) || (akeraio == 90))  return(STATUS_GCODE_UNSUPPORTED_COMMAND);   
                        dekadiko = 0; 
                      }
                   break;
          case 93: case 94: 
                      word_bit = MODAL_GROUP_G5; 
                      if (akeraio == 94)  command.feed_rate = FEED_RATE_MODE_INVERSE_TIME;  
                      else if (akeraio == 93) command.feed_rate = FEED_RATE_MODE_UNITS_PER_MIN; 
                   break;
          case 40:
                      word_bit = MODAL_GROUP_G7;
                    break;
          case 43: case 49:
                      word_bit = MODAL_GROUP_G8;
                      if (axis_command)   return(STATUS_GCODE_AXIS_COMMAND_CONFLICT);   
                      axis_command = AXIS_COMMAND_TOOL_LENGTH_OFFSET;
                      if (akeraio == 49)  command.tool_length = TOOL_LENGTH_OFFSET_CANCEL;  
                      else if (dekadiko == 10)command.tool_length = TOOL_LENGTH_OFFSET_ENABLE_DYNAMIC; 
                      else   return(STATUS_GCODE_UNSUPPORTED_COMMAND);  
                      dekadiko = 0; 
                   break;
          case 54: case 55: case 56: case 57: case 58: case 59: 
                      word_bit = MODAL_GROUP_G12;
                      command.coord_select = akeraio-54; 
                      break;
          case 61:
                      word_bit = MODAL_GROUP_G13;
                      if (dekadiko != 0) { return(STATUS_GCODE_UNSUPPORTED_COMMAND); } 
                   break;
          default:
                      return(STATUS_GCODE_UNSUPPORTED_COMMAND); 
                      break;
        }              
        if ( bit_istrue(command_words,bit(word_bit)) ) { return(STATUS_GCODE_MODAL_GROUP_VIOLATION); }
        command_words |= bit(word_bit);
        
        
      case 'M':
      
        if (dekadiko > 0) { return(STATUS_GCODE_COMMAND_VALUE_NOT_INTEGER); } 
        switch(akeraio) {
          case 0: case 1: case 2: case 30: 
                  word_bit = MODAL_GROUP_M4; 
                  switch(akeraio) {
                    case 0: command.program_flow = PROGRAM_FLOW_PAUSED; break; 
                    case 1: break; // Optional stop not supported. Ignore.
                    case 2: case 30: command.program_flow = PROGRAM_FLOW_COMPLETED; break; 
                  }
                 break;

          case 3: case 5:
            word_bit = MODAL_GROUP_M7; 
            switch(akeraio) {
              case 3: command.spindle = SPINDLE_ENABLE_CW; break;
              case 5: command.spindle = SPINDLE_DISABLE; break;
            }
            break;            
          case 8: case 9:
            word_bit = MODAL_GROUP_M8; 
            return(STATUS_GCODE_UNSUPPORTED_COMMAND); 
            break;
        }
        break;
      
        // ***********************Check for more than one command per command group violations *************
        if ( bit_istrue(command_words,bit(word_bit)) ) { return(STATUS_GCODE_MODAL_GROUP_VIOLATION); }
        command_words = command_words | bit(word_bit);
        
           value_words = 0;
          case 'F': word_bit = WORD_F; value_words = value_words | 1 ; values.f = value ; Fr = value; break;
          case 'I': word_bit = WORD_I; value_words = value_words | 2 ; values.ijk[X_AXIS] = value; ijk_words |= (1<<X_AXIS); break;
          case 'J': word_bit = WORD_J; value_words = value_words | 4 ; values.ijk[Y_AXIS] = value; ijk_words |= (1<<Y_AXIS); break;
          case 'K': word_bit = WORD_K; value_words = value_words | 8 ; values.ijk[Z_AXIS] = value; ijk_words |= (1<<Z_AXIS); break;
          case 'L': word_bit = WORD_L; value_words = value_words | 16 ; values.l = akeraio; break;
          case 'P': word_bit = WORD_P; value_words = value_words | 64 ; values.p = value; break;
          case 'R': word_bit = WORD_R; value_words = value_words | 128 ; values.r = value; break;
          case 'S': word_bit = WORD_S; value_words = value_words | 256 ; values.s = value; break;
          case 'T': word_bit = WORD_T; value_words = value_words | 512 ;break;  
          case 'X': word_bit = WORD_X; value_words = value_words | 1024 ; values.xyz[X_AXIS] = value; axis_words |= (1<<X_AXIS); break;
          case 'Y': word_bit = WORD_Y; value_words = value_words | 2048 ; values.xyz[Y_AXIS] = value; axis_words |= (1<<Y_AXIS); break;
          case 'Z': word_bit = WORD_Z; value_words = value_words | 4096 ; values.xyz[Z_AXIS] = value; axis_words |= (1<<Z_AXIS); break;
          case 'O':  Serial.print(" G: Program : ");Serial.println(akeraio); break;
          case '@': 
          #ifdef   DEBUG_GCODE_MODE
          Serial.println(" G: End Of Line detected ");
          #endif
          counter = 0;EndOL = false; goto mark3; break; 
          default: return(STATUS_GCODE_UNSUPPORTED_COMMAND);break;
          letter = ' ';EndOL = false;

        
        //**************************** Word repeated check *****************************************************
        if (bit_istrue(value_words,bit(word_bit))) { return(STATUS_GCODE_WORD_REPEATED); } 
        
         //***************** Negative values for words F, N, P, T, and S. check ********************************
        if ( bit(word_bit) & (bit(WORD_F)|bit(WORD_P)|bit(WORD_T)|bit(WORD_S)) )
             {if (value < 0.0) { /*Serial.println("DEBUG_GCODE_TEXT");*/return(STATUS_NEGATIVE_VALUE);}  
             }

        
    } 
       counter++;    
  }
  mark3:

//******************** if No G , xhen XYZ mode = motion ***********************************************  
  if (axis_words)
    {if (!axis_command)   axis_command = AXIS_COMMAND_MOTION_MODE; } 
  
 
//**********************************  F word missing  check************************************************** 

  if (   command.feed_rate == FEED_RATE_MODE_INVERSE_TIME) { //  G94
    if (             axis_command == AXIS_COMMAND_MOTION_MODE 
        && (command.motion != MOTION_MODE_NONE ||
            command.motion != MOTION_MODE_SEEK)) //  G01 G02 G03 G04
        {
            if (bit_isfalse(value_words,bit(WORD_F))) { values.f = saved_values.f;}//Fr;}//{ return(STATUS_GCODE_UNDEFINED_FEED_RATE); } //  F word missing 
        }
    if (command.motion == MOTION_MODE_SEEK )  values.f = values.seek_rate;  //G00

   else if (command.feed_rate == FEED_RATE_MODE_UNITS_PER_MIN){  // G93   
       if (bit_isfalse(value_words,bit(WORD_F)))   values.f = saved_values.f;  // if No f take setup value
   
    bit_false(value_words,bit(WORD_F));    // bit(F)= 0
  }
  }

  #ifdef   DEBUG_GCODE_MODE
  Serial.println ("**********************DEBUG*****************************" );
  Serial.print (" G: axis_command  "); Serial.println ( axis_command,BIN);
  Serial.print (" G: Modal Motion  "); Serial.println ( command.motion,BIN);
  Serial.print (" G: Value_word  "); Serial.println ( value_words,BIN);
  Serial.println (" ");
  Serial.print    (" Saved Seek rate    "); Serial.println (values.seek_rate );
  Serial.print    (" Saved Feed rate    "); Serial.println ( saved_values.f );
  Serial.print    (" Feed rate check No1  "); Serial.println ( values.f );

  #endif






  
//********************************** Set spindle speed check ************************************************************  
  if (bit_isfalse(value_words,bit(WORD_S))) {  values.s = saved_values.s; }
    bit_false(value_words,bit(WORD_S)); // bit(S)= 0
    
//**********************************Dwell  P value missing   ************************************************************  
  if (command.non_command == NON_MODAL_DWELL) 
    {if (bit_isfalse(value_words,bit(WORD_P))) { return(STATUS_GCODE_VALUE_WORD_MISSING); } } 
    bit_false(value_words,bit(WORD_P));   // bit(P)= 0
  
  
  //******************** Set active plane *****************************************************************************
  switch (command.plane_select) {
    case PLANE_SELECT_XY:
      axis_0 = X_AXIS;
      axis_1 = Y_AXIS;
      axis_linear = Z_AXIS;
      break;
    case PLANE_SELECT_ZX:
      axis_0 = Z_AXIS;
      axis_1 = X_AXIS;
      axis_linear = Y_AXIS;
      break;
    case PLANE_SELECT_YZ:
      axis_0 = Y_AXIS;
      axis_1 = Z_AXIS;
      axis_linear = X_AXIS;
  }   
 
  if (axis_command == AXIS_COMMAND_TOOL_LENGTH_OFFSET ) {  
    if (command.tool_length == TOOL_LENGTH_OFFSET_ENABLE_DYNAMIC) {   
       if (axis_words ^ (1<<TOOL_LENGTH_OFFSET_AXIS)) {return(STATUS_GCODE_G43_DYNAMIC_AXIS_ERROR); }
    }
  }
 
//  memcpy(coordinate_data,values.coord_system,sizeof(values.coord_system));
  if ( bit_istrue(command_words,bit(MODAL_GROUP_G12)) ) {  
    if (command.coord_select > N_COORDINATE_SYSTEM) { return(STATUS_GCODE_UNSUPPORTED_COORD_SYS); } // [Greater than N sys]
    if (command.coord_select != command.coord_select) {
//       if (!(settings_read_coord_data(command.coord_select,coordinate_data))) { return(STATUS_SETTING_READ_FAIL); } 
    }
  } 

  switch (command.non_command) {
    case NON_MODAL_SET_COORDINATE_DATA:   
      if (!axis_words) { return(STATUS_GCODE_NO_AXIS_WORDS); }  
      if (bit_isfalse(value_words,((1<<WORD_P)|(1<<WORD_L)))) { return(STATUS_GCODE_VALUE_WORD_MISSING); }  
      coord_select = trunc( values.p);  
      if (coord_select > N_COORDINATE_SYSTEM) { return(STATUS_GCODE_UNSUPPORTED_COORD_SYS); }  
      if ( values.l != 20) {
        if ( values.l == 2) {
          if (bit_istrue(value_words,bit(WORD_R))) {return(STATUS_GCODE_UNSUPPORTED_COMMAND); }  
        } else { return(STATUS_GCODE_UNSUPPORTED_COMMAND); }  
      }
      bit_false(value_words,(bit(WORD_L)|bit(WORD_R)));  // bit(L)= 0  bit(R)= 0
      
 //     // Determine coordinate system to change and try to load from EEPROM.
 //     if (coord_select > 0) { coord_select--; } // Adjust P1-P6 index to EEPROM coordinate data indexing.
//      else { coord_select = command.coord_select; } // Index P0 as the active coordinate system
//       if (!settings_read_coord_data(coord_select,parameter_data)) {return(STATUS_SETTING_READ_FAIL); } // [EEPROM read fail]
    
      // Pre-calculate the coordinate data changes. NOTE: Uses parameter_data since coordinate_data may be in use by G54-59.
      for (int idx=0; idx<N_AXIS; idx++) { // Axes indices are consistent, so loop may be used.
        // Update axes defined only in block. Always in machine coordinates. Can change non-active system.
        if (bit_istrue(axis_words,bit(idx)) ) {
          if ( values.l == 20) {
            // L20: Update coordinate system axis at current position (with modifiers) with programmed value
            parameter_data[idx] = values.position[idx]-values.coord_offset[idx]- values.xyz[idx];
 
             if (idx == TOOL_LENGTH_OFFSET_AXIS) { parameter_data[idx] -= values.tool_length_offset; }
           } else {
            // L2: Update coordinate system axis to programmed value.
            parameter_data[idx] =  values.xyz[idx]; 
          }
        }
      }
      break;
    case NON_MODAL_SET_COORDINATE_OFFSET:
      if (!axis_words) { return(STATUS_GCODE_NO_AXIS_WORDS); } 
    
      // Update axes defined only in block. Offsets current system to defined value. Does not update when
      // active coordinate system is selected, but is still active unless G92.1 disables it. 
      for (int idx=0; idx<N_AXIS; idx++) {
        if (bit_istrue(axis_words,bit(idx)) ) {
           values.xyz[idx] = values.position[idx]-coordinate_data[idx]- values.xyz[idx];
           if (idx == TOOL_LENGTH_OFFSET_AXIS) {  values.xyz[idx] -= values.tool_length_offset; }
         } else {
           values.xyz[idx] = values.coord_offset[idx];
        }
      }
      break;
      
    default:
 
      if (axis_command != AXIS_COMMAND_TOOL_LENGTH_OFFSET ) {  
        if (axis_words) {
          for (int idx=0; idx<N_AXIS; idx++) { // Axes indices are consistent, so loop may be used to save flash space.
            if ( bit_isfalse(axis_words,bit(idx)) ) {
               values.xyz[idx] = values.position[idx]; // No axis word in block. Keep same axis position.
            } else {
              // Update specified value according to distance mode or ignore if absolute override is active.
              // NOTE: G53 is never active with G28/30 since they are in the same command group.
              if (command.non_command != NON_MODAL_ABSOLUTE_OVERRIDE) {
                // Apply coordinate offsets based on distance mode.
                if (command.distance == DISTANCE_MODE_ABSOLUTE) {
                   values.xyz[idx] += coordinate_data[idx] + values.coord_offset[idx]; 
                  if (idx == TOOL_LENGTH_OFFSET_AXIS) {  values.xyz[idx] += values.tool_length_offset; }
                } else {  // Incremental mode
                   values.xyz[idx] += values.position[idx];
                }
              }
            }
          }
        }
      }break;
  }
      switch (command.non_command) {        
        case G28_NON_MODAL_GO_HOME_0: 
          if (!axis_words) { axis_command = AXIS_COMMAND_NONE; } // Set to none if no intermediate motion.
//       if (!settings_read_coord_data(SETTING_INDEX_G28,parameter_data)) { return(STATUS_SETTING_READ_FAIL); }
          break;
        case G30_NON_MODAL_GO_HOME_0:
          if (!axis_words) { axis_command = AXIS_COMMAND_NONE; } // Set to none if no intermediate motion.
 //        if (!settings_read_coord_data(SETTING_INDEX_G30,parameter_data)) { return(STATUS_SETTING_READ_FAIL); }
          break;
        case  G28_1_NON_MODAL_SET_HOME_0: case  G30_1_NON_MODAL_SET_HOME_1:
        
          break;
        case NON_MODAL_RESET_COORDINATE_OFFSET: 
          // NOTE: If axis words are passed here, they are interpreted as an implicit motion mode.
          break;
        case NON_MODAL_ABSOLUTE_OVERRIDE:
          // [G53 Errors]: G0 and G1 are not active. Cutter compensation is enabled.
          // NOTE: All explicit axis word commands are in this command group. So no implicit check necessary.
          if (!(command.motion == MOTION_MODE_SEEK || command.motion == MOTION_MODE_LINEAR)) {
            return(STATUS_GCODE_G53_INVALID_MOTION_MODE); // [G53 G0/1 not active]
          }
          break;
      
      }

  if (command.motion == MOTION_MODE_NONE) {
    if ((axis_words) && (axis_command != AXIS_COMMAND_NON_MODAL)) { 
      return(STATUS_GCODE_AXIS_WORDS_EXIST); 
    }


  } else if ( axis_command == AXIS_COMMAND_MOTION_MODE ) {
  
    if (command.motion == MOTION_MODE_SEEK) {
      if (!axis_words) { axis_command = AXIS_COMMAND_NONE; }

    } else { 
      #ifdef   DEBUG_GCODE_MODE
  Serial.print (" No2  "); Serial.println ( values.f );
  Serial.println ("**********************DEBUG*****************************" );
  #endif       
      if ( values.f == 0.0) { return(STATUS_GCODE_UNDEFINED_FEED_RATE); }    
      switch (command.motion) {
        case MOTION_MODE_LINEAR: 
                  if (!axis_words) { axis_command = AXIS_COMMAND_NONE; }
                 break;
        case MOTION_MODE_CW_ARC:
        case MOTION_MODE_CCW_ARC:
        
          if (!axis_words) { return(STATUS_GCODE_NO_AXIS_WORDS); } // [No axis words]
          if (!(axis_words & (bit(axis_0)|bit(axis_1)))) { return(STATUS_GCODE_NO_AXIS_WORDS_IN_PLANE); } // [No axis words in plane]
        
          
          float x,y;
          x =  values.xyz[axis_0]-values.position[axis_0];  
          y =  values.xyz[axis_1]-values.position[axis_1];  

          if (value_words & bit(WORD_R)) bit_false(value_words,bit(WORD_R));
            if (gc_check_same_position(values.position,  values.xyz)) { return(STATUS_GCODE_INVALID_TARGET); }  
          

  
            h_x2_div_d = 4.0 *  values.r *  values.r - x*x - y*y;
//___________________________________Βγαζει error _____________________________________________________________________
 //           if (h_x2_div_d < 0) {return(STATUS_GCODE_ARC_RADIUS_ERROR); } 
//___________________________________Βγαζει error _____________________________________________________________________    
          
             h_x2_div_d = -sqrt(h_x2_div_d)/hypot_f(x,y); // == -(h * 2 / d) 
            if (command.motion == MOTION_MODE_CCW_ARC) { h_x2_div_d = -h_x2_div_d; }  

            if ( values.r < 0) { 
                h_x2_div_d = -h_x2_div_d; 
                 values.r = - values.r;  
                     
             values.ijk[axis_0] = 0.5*(x-(y*h_x2_div_d));
             values.ijk[axis_1] = 0.5*(y+(x*h_x2_div_d));
          
          } else {  
            if (!(ijk_words & (bit(axis_0)|bit(axis_1)))) { return(STATUS_GCODE_NO_OFFSETS_IN_PLANE); } // [No offsets in plane]
            bit_false(value_words,(bit(WORD_I)|bit(WORD_J)|bit(WORD_K)));  
           
            x -=  values.ijk[axis_0];  
            y -=  values.ijk[axis_1]; 
             float target_r = hypot_f(x,y); 
 
              values.r = hypot_f( values.ijk[axis_0],  values.ijk[axis_1]); 
             
             float delta_r = fabs(target_r- values.r);
             
            if (delta_r > 0.005) { 
              if (delta_r > 0.5) {return(STATUS_GCODE_INVALID_TARGET); }  
              if (delta_r > (0.001* values.r)) { return(STATUS_GCODE_INVALID_TARGET); }  
            }
          }
          break;
     
        case MOTION_MODE_PROBE_TOWARD: case MOTION_MODE_PROBE_TOWARD_NO_ERROR:
        case MOTION_MODE_PROBE_AWAY: case MOTION_MODE_PROBE_AWAY_NO_ERROR: 
             if (!axis_words) { return(STATUS_GCODE_NO_AXIS_WORDS); }  
              if (gc_check_same_position(values.position,  values.xyz)) { return(STATUS_GCODE_INVALID_TARGET); }  
         break;
  }}}
#ifdef   DEBUG_GCODE_MODE
  // resetting variables
  Serial.println ("RESET/SAVE");
#endif  
  value_words = 0;
  
  bit_false(value_words,( bit(WORD_F)|bit(WORD_S)|bit(WORD_T)));   
  if (axis_command) { bit_false(value_words,(bit(WORD_X)|bit(WORD_Y)|bit(WORD_Z))); }  
  if (value_words) { return(STATUS_GCODE_UNUSED_WORDS); } // [Unused words]
 
  saved_command.feed_rate = command.feed_rate;
  saved_values.f  =  values.f; 
  saved_values.seek_rate = values.seek_rate;
  if (saved_values.s !=  values.s) { 
                if (command.spindle != SPINDLE_DISABLE) spindle_run();
                saved_values.s =  values.s; 
  }
  if (saved_command.spindle != command.spindle) {
             spindle_run();
              saved_command.spindle = command.spindle;    
  }
//_________________________________MISPLACED_____________________________________________________________  
 if (command.non_command == NON_MODAL_DWELL) { mc_dwell( values.p); } //do a dwell
//_______________________________________________________________________________________________ 
  saved_command.plane_select = command.plane_select;  
if (axis_command == AXIS_COMMAND_TOOL_LENGTH_OFFSET ) {      // Indicates a change.
    saved_command.tool_length = command.tool_length;
    if (command.tool_length == TOOL_LENGTH_OFFSET_ENABLE_DYNAMIC) { 
    values.tool_length_offset =  values.xyz[TOOL_LENGTH_OFFSET_AXIS];
} else { 
      values.tool_length_offset = 0.0;
    }
  }
  
  if (saved_command.coord_select != command.coord_select) {
    saved_command.coord_select = command.coord_select;
    memcpy(values.coord_system,coordinate_data,sizeof(coordinate_data));
  }
  saved_command.distance = command.distance;
  switch(command.non_command) {
    case NON_MODAL_SET_COORDINATE_DATA:    
 //     settings_write_coord_data(coord_select,parameter_data);
      if (command.coord_select == coord_select) { memcpy(values.coord_system,parameter_data,sizeof(parameter_data)); }
      break;

// Homing option G28 G30
      
    case G28_NON_MODAL_GO_HOME_0: case G30_NON_MODAL_GO_HOME_0: 

      if (axis_command) {
//********************************************************************
//          mc_line( values.xyz, -1.0, false);
//********************************************************************
      }
//********************************************************************
//        mc_line(parameter_data, -1.0, false); 
//********************************************************************
      memcpy(values.position, parameter_data, sizeof(parameter_data));
      break;
    case  G28_1_NON_MODAL_SET_HOME_0: 
//      settings_write_coord_data(SETTING_INDEX_G28,values.position);
      break;
    case  G30_1_NON_MODAL_SET_HOME_1:
//      settings_write_coord_data(SETTING_INDEX_G30,values.position);
      break;



      
    case NON_MODAL_SET_COORDINATE_OFFSET:
      memcpy(values.coord_offset, values.xyz,sizeof( values.xyz));
      break;
    case NON_MODAL_RESET_COORDINATE_OFFSET: 
      clear_vector(values.coord_offset); // Disable G92 offsets by zeroing offset vector.
      break;
  }

  saved_command.motion = command.motion;
  if (command.motion != MOTION_MODE_NONE) {
    if (axis_command == AXIS_COMMAND_MOTION_MODE) {
      switch (command.motion) {
        case MOTION_MODE_SEEK:
//********************************************************************
//            mc_line( values.xyz, -1.0, false);
//********************************************************************      
          break;
        case MOTION_MODE_LINEAR:
//********************************************************************
//            mc_line( values.xyz, values.feed_rate, command.feed_rate);
//********************************************************************
          break;
        case MOTION_MODE_CW_ARC: 
//********************************************************************
//            mc_arc(values.position,  values.xyz,  values.ijk,  values.r, 
//              values.feed_rate, command.feed_rate, axis_0, axis_1, axis_linear, true); 
//********************************************************************
          break;        
        case MOTION_MODE_CCW_ARC:
//********************************************************************
//            mc_arc(values.position,  values.xyz,  values.ijk,  values.r, 
//              values.feed_rate, command.feed_rate, axis_0, axis_1, axis_linear, false); 
//********************************************************************
          break;
        case MOTION_MODE_PROBE_TOWARD: 
          // NOTE:  values.xyz is returned from mc_probe_cycle with the updated position value. So
          // upon a successful probing cycle, the machine position and the returned value should be the same.
//********************************************************************
//            mc_probe_cycle( values.xyz, values.feed_rate, command.feed_rate, false, false);
//********************************************************************
          break;
        case MOTION_MODE_PROBE_TOWARD_NO_ERROR:
//********************************************************************
//            mc_probe_cycle( values.xyz, values.feed_rate, command.feed_rate, false, true);
//********************************************************************
          break;
        case MOTION_MODE_PROBE_AWAY:
//********************************************************************
//            mc_probe_cycle( values.xyz, values.feed_rate, command.feed_rate, true, false);
//********************************************************************
          break;
        case MOTION_MODE_PROBE_AWAY_NO_ERROR:
 //********************************************************************      
 //           mc_probe_cycle( values.xyz, values.feed_rate, command.feed_rate, true, true);
//******************************************************************** 
          break;
      }
   
      // As far as the parser is concerned, the position is now == target. In reality the
      // motion control system might still be processing the action and the real tool position
      // in any intermediate location.
      memcpy(values.position,  values.xyz, sizeof( values.xyz)); // values.position[] =  values.xyz[]
    }
  }
 //     // if (EndOL== true) break;
//       counter++;

 // saved_command.program_flow = command.program_flow;

  if (command.program_flow == PROGRAM_FLOW_PAUSED) {
    if (sys.state != STATE_CHECK_MODE) {
    bit_true_atomic(sys_rt_exec_state, EXEC_FEED_HOLD); // Use feed hold for program pause.
    protocol_execute_realtime(); // Execute suspend.
    }
  }
  if(command.program_flow == PROGRAM_FLOW_COMPLETED){ 
      report_feedback_message(MESSAGE_PROGRAM_END);
      return(STATUS_OK);
      }
  
  else {
          saved_command.motion = MOTION_MODE_LINEAR;
          saved_command.plane_select = PLANE_SELECT_XY;
          saved_command.distance = DISTANCE_MODE_ABSOLUTE;
          saved_command.feed_rate = FEED_RATE_MODE_UNITS_PER_MIN;
          saved_command.coord_select = 0; 
          saved_command.spindle = SPINDLE_DISABLE;
    
          if (sys.state != STATE_CHECK_MODE) {
                  memcpy(values.coord_system,coordinate_data,sizeof(coordinate_data));
                  spindle_stop();
          }
          saved_command.program_flow = PROGRAM_FLOW_RUNNING; // Reset program flow.
    }
    
  return(STATUS_OK);
}

