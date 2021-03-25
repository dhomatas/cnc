
//__________________________________________________________________
  
               void protocol_process()
//__________________________________________________________________
{
   
   int char_counter = 0; // Last character counter in line variable.
   boolean iscomment = false; // Comment/block delete flag for processor to ignore comment characters.
 if (myFile) {
    Serial.println ("Processing .....");
    
    while (myFile.available()) {

      char c = myFile.read();
       //   Serial.print (char_counter);  // DEBUG_GCODE_TEXT
     // Serial.print("Line from file ");   Serial.print (c);             // DEBUG_GCODE_TEXT
       if (sys.abort) return;
       switch (c){ 
         case -1: Serial.println("No char available ");return;break;
         case  10:  // End of line reached
                  if (char_counter > 0){ 
                  char_counter++;
                     line[char_counter] = '@';
             
                     bit_true_atomic(sys_rt_exec_state, EXEC_CYCLE_START);  // protocol_auto_cycle_start();
                  protocol_execute_line(line) ;
                     } 
                   else  report_status_message(STATUS_OK);
                   
        
       // reset line , char_counter and isComment variables 
                    for (int t = 0;t<=LINE_BUFFER_SIZE;t++)line[t]=' ';
                    char_counter = -1;
                    iscomment = false; 
                    break;
         case ' ' :char_counter--;                        break;
         case'(': iscomment = true;                       break;
         case')': iscomment = false;                      break;
         default :if (!iscomment) {line[char_counter] = c; break;}}
         if (!iscomment) char_counter++;
    } 
  Serial.println("EOF");
  myFile.close();
  Serial.println("File closed");
  sys.state == STATE_IDLE;
 }
  else  Serial.println("File not fount");
}
//_______________________________________________________________________

                  void printDirectory(File dir, int numTabs) 
//_______________________________________________________________________
{
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
//_____________________________________________________

                     void openGrblFile()
//______________________________________________________
{
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("Open test.txt ");

    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}





