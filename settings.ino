
//----------------------------------------------------------------
// Method to restore EEPROM-saved Grbl global settings back to defaults. 
void settings_restore() 
//-----------------------------------------------------------------
{  
  settings.pulse_microseconds = DEFAULT_STEP_PULSE_MICROSECONDS;
  settings.stepper_idle_lock_time = DEFAULT_STEPPER_IDLE_LOCK_TIME;
  settings.step_invert_mask = DEFAULT_STEPPING_INVERT_MASK;
  settings.dir_invert_mask = DEFAULT_DIRECTION_INVERT_MASK;
  settings.status_report_mask = DEFAULT_STATUS_REPORT_MASK;
  settings.junction_deviation = DEFAULT_JUNCTION_DEVIATION;
  settings.arc_tolerance = DEFAULT_ARC_TOLERANCE;
  
  settings.homing_dir_mask = DEFAULT_HOMING_DIR_MASK;
  
  settings.homing_feed_rate = DEFAULT_HOMING_FEED_RATE;
  settings.homing_seek_rate = DEFAULT_HOMING_SEEK_RATE;
  settings.homing_pulloff = DEFAULT_HOMING_PULLOFF;

  settings.flags = 0;
  if (DEFAULT_INVERT_ST_ENABLE)  { settings.flags |= BITFLAG_INVERT_ST_ENABLE;  }
 // if (DEFAULT_INVERT_LIMIT_PINS) { settings.flags |= BITFLAG_INVERT_LIMIT_PINS; }
  if (DEFAULT_SOFT_LIMIT_ENABLE) { settings.flags |= BITFLAG_SOFT_LIMIT_ENABLE; }
  if (DEFAULT_HARD_LIMIT_ENABLE) { settings.flags |= BITFLAG_HARD_LIMIT_ENABLE; }
  if (DEFAULT_HOMING_ENABLE)     { settings.flags |= BITFLAG_HOMING_ENABLE;     }
  
  settings.steps_per_mm[X_AXIS] = DEFAULT_X_STEPS_PER_MM;
  settings.steps_per_mm[Y_AXIS] = DEFAULT_Y_STEPS_PER_MM;
  settings.steps_per_mm[Z_AXIS] = DEFAULT_Z_STEPS_PER_MM;
  settings.max_rate[X_AXIS] =     DEFAULT_X_MAX_RATE;
  settings.max_rate[Y_AXIS] =     DEFAULT_Y_MAX_RATE;
  settings.max_rate[Z_AXIS] =     DEFAULT_Z_MAX_RATE;
  settings.acceleration[X_AXIS] = DEFAULT_X_ACCELERATION;
  settings.acceleration[Y_AXIS] = DEFAULT_Y_ACCELERATION;
  settings.acceleration[Z_AXIS] = DEFAULT_Z_ACCELERATION;
  settings.max_travel[X_AXIS] =  -DEFAULT_X_MAX_TRAVEL ;
  settings.max_travel[Y_AXIS] =  -DEFAULT_Y_MAX_TRAVEL ;
  settings.max_travel[Z_AXIS] =  -DEFAULT_Z_MAX_TRAVEL ;    

  values.f  = DEFAULT_HOMING_FEED_RATE;
  values.seek_rate = DEFAULT_HOMING_SEEK_RATE;
  }

