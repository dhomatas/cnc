 void plotLineLow(int x0,int y0, int x1,int y1){ 
  int dx = x1 - x0;
  int dy = y1 - y0;
  byte yi = 1;
  if (dy < 0){
    yi = -1;
    dy = -dy;}

 int  D = 2*dy - dx;
  int y = y0;

  for (int x=x0; x<=x1;x++){
  stepperX.moveTo (1);stepperX.run();
  stepperY.moveTo (y);stepperY.run(); //plot(1,y);
    if (D > 0){
       y = y + yi;
       D = D - 2*dx;}
    D = D + 2*dy;
  }
}

void plotLineHigh  (int x0,int y0, int x1,int y1){
  
  int   dx = x1 - x0;
 int  dy = y1 - y0;
 byte  xi = 1;
  if (dx < 0){
    xi = -1;
    dx = -dx;}
  
  int D = 2*dx - dy;
  int x = x0;

  for (int y=y0 ;y<=y1;y++){
   stepperX.moveTo(x);stepperX.run();
  stepperY.moveTo(1);stepperY.run();//  plot(x,1)
    if (D > 0){
       x = x + xi;
       D = D - 2*dy;}
    D = D + 2*dx;
  }
}

void plotLine(int x0,int y0, int x1,int y1)
/*
  // If enabled, check for soft limit violations. Placed here all line motions are picked up
  // from everywhere in Grbl.
  if (bit_istrue(settings.flags,BITFLAG_SOFT_LIMIT_ENABLE)) { limits_soft_check(target); }    
      
  // If in check gcode mode, prevent motion by blocking planner. Soft limits still work.
  if (sys.state == STATE_CHECK_MODE) { return; }
   protocol_auto_cycle_start(); */
{
  if (abs(y1 - y0) < abs(x1 - x0))
  {
    if (x0 > x1)  plotLineLow(x1, y1, x0, y0);
    else          plotLineLow(x0, y0, x1, y1);
  }
  else
  { 
    if (y0 > y1)  plotLineHigh(x1, y1, x0, y0);
    else          plotLineHigh(x0, y0, x1, y1);
  }
} 

// Execute dwell in seconds.
//****************************************************************************************************
void mc_dwell(int seconds) 
//****************************************************************************************************
{ 
   if (sys.state == STATE_CHECK_MODE)   return;  
   
 //  uint16_t i = floor(1000/DWELL_TIME_STEP*seconds);
   cli();
  // protocol_buffer_synchronize();
   //******************THE USE OF DELAY WILL BLOCK THE PROCESOR*************************
   delay(100*seconds);//(floor(1000*seconds-i*DWELL_TIME_STEP)); // Delay millisecond remainder.
   //***********************************************************************************
 //  while (i-- > 0) {
     // NOTE: Check and execute realtime commands during dwell every <= DWELL_TIME_STEP milliseconds.
//     protocol_execute_realtime();
 //    if (sys.abort) { return; }
 //    _delay_ms(DWELL_TIME_STEP); // Delay DWELL_TIME_STEP increment
 return;
   }



  
