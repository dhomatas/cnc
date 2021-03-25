//__________________________________________________________________

                             void setup() 
  
//__________________________________________________________________
{
 Serial.begin(9600);
 Akeypad.begin  ( makeKeymap(keys));
 pinMode(33,OUTPUT);  
 pinMode(35,OUTPUT);digitalWrite (35,HIGH);
  
   display.begin();
   display.setContrast(60);
   display.setTextSize(1);
   display.setTextColor(BLACK); 
   display.clearDisplay();
   display.display();delay(2000);
   
    display.setCursor(0,0);
    
   if (!SD.begin(33)) 
   {
     Serial.println("Sd initialization failed!");
    display.println("Sd init fail"); display.display();delay(2000);
   }
   else 
   {
     Serial.println("Sd initialization done.");
   display.println("Sd init OK"); display.display();delay(2000);
   }

    settings_restore(); 
   
    display.println("Setup Loaded"); display.display();delay(2000);
 
  pinMode(21, OUTPUT);digitalWrite(21,HIGH);               //x limit
  pinMode(20, OUTPUT);digitalWrite(20,HIGH);               //y limit
  pinMode(19, OUTPUT);digitalWrite(19,HIGH);               //z limit
  pinMode(18, OUTPUT);digitalWrite(18,HIGH);               //button
  pinMode(2, OUTPUT);digitalWrite(2,HIGH);               //button
  pinMode(3, OUTPUT);digitalWrite(3,HIGH);               //button 
  attachInterrupt(digitalPinToInterrupt(21), interrupts_triggered , FALLING);
  attachInterrupt(digitalPinToInterrupt(20), interrupts_triggered , FALLING);
   attachInterrupt(digitalPinToInterrupt(19), interrupts_triggered , FALLING);
    attachInterrupt(digitalPinToInterrupt(18), interrupts_triggered , FALLING);
     attachInterrupt(digitalPinToInterrupt(2), interrupts_triggered , FALLING);
      attachInterrupt(digitalPinToInterrupt(3), interrupts_triggered , FALLING);

 display.println("Ints OK"); display.display(); delay(2000);
  
     MCUSR &= ~(1<<WDRF);    //ENABLE_SOFTWARE_DEBOUNCE
     WDTCSR |= (1<<WDCE) | (1<<WDE);
     WDTCSR = (1<<WDP0);     // Set time-out at ~32msec
  
  pinMode(7,OUTPUT);  //spindle pin
    spindle_stop();
  pinMode(69,INPUT);digitalWrite(69,HIGH);// Probe pin
   sei();
 
}
 //__________________________________________________________________

                          void loop(){

//__________________________________________________________________

   memset(&sys, 0, sizeof(system_t));  // Clear all system variables
   sys.abort = true;   // Set abort to complete initialization

  
   if  (HOMING_INIT_LOCK){
    if (bit_istrue(settings.flags,BITFLAG_HOMING_ENABLE)) { sys.state = STATE_ALARM; }
     report_status_onDisplay();
   }
   if  (FORCE_INITIALIZATION_ALARM){
    sys.state = STATE_ALARM;
     report_status_onDisplay();
   }

   Serial.println(" Reset Grbl primary systems.");
    gc_init(); 

     report_status_onDisplay();

   Serial.println("Reset system variables.");
    sys.abort = false;
    sys_rt_exec_state = 0;
    sys_rt_exec_alarm = 0;
    sys.suspend = false;
    sys.soft_limit = false;
     report_status_onDisplay();          
    Serial.println(" Start Grbl main loop.");

  
  display.clearDisplay();
    //               12345678901234
    display.println(" 1 JIG");
    display.println(" 2 Cycle Start");
    display.println(" 3 Check Mode");
    display.println(" 4 GRBL Sets");
    display.println(" 5 Reset/Home");
    display.println(" 6 Files");
    display.display();delay(2000);
 
mark1:    
    char key1 = Akeypad.getKey();
   
switch (key1){
  case '1':  Serial.println(" 1"); 
       break;
  case '2':   Serial.println(" 2");
       break;
  case '3':   
            // Set check g-code mode [IDLE/CHECK]
                    // Perform reset when toggling off. Check g-code mode should only work if Grbl
                    // is idle and ready, regardless of alarm locks. This is mainly to keep things
                    // simple and consistent.
                    if ( sys.state == STATE_CHECK_MODE ) {                                      
                       Sys_reset();                                      
                      report_feedback_message(MESSAGE_DISABLED);
                    } else {
                      if (sys.state) { return(STATUS_IDLE_ERROR); } // Requires no alarm mode.
                      sys.state = STATE_CHECK_MODE;
                      report_feedback_message(MESSAGE_ENABLED);
                    }
                    break; 
       break;
  case '4':  
          if ( sys.state & (STATE_CYCLE | STATE_HOLD) ) { return(STATUS_IDLE_ERROR); } // Block during cycle. Takes too long to print.
          else { report_grbl_settings();report_gcode_modes(); 
                                        }
       break;
  case '5': 
  display.clearDisplay();
  display.print("Homing..."); display.display(); delay(2000);
     limits_go_home();
       break;
 case '6': 
           root = SD.open("/");
           myFile = SD.open("test.txt");
           protocol_process();
         // printDirectory(root, 0);
          Serial.println("End of printing"); 

 case'7': // Disable alarm lock [ALARM]
                    if (sys.state == STATE_ALARM) {                // Don't run startup script. Prevents stored moves in startup from causing accidents.
                      report_feedback_message(MESSAGE_ALARM_UNLOCK);// Otherwise, no effect.
                      sys.state = STATE_IDLE;} 
      break;       
 case '0' : report_grbl_help(); report_grbl_settings(); break; 
 default: goto mark1;         
      break;     
}

}    



