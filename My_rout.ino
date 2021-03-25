
//____________________________________________________________________

                           boolean  read_float () 
//____________________________________________________________________
{

  boolean FirstDigit = true;
  
   akeraio = 888888;
   dekadiko = 0;
   value = 0.;
  float diviz;
  char c = line[counter];
  EndOL=false;
// akeraio               
       if (c=='-'){isNegativ = true;
                    counter++;
                  c = line[counter];}
       while (c != '.' ){
             
            if (c>='A'&& akeraio==888888 ){
             goto returnFalse;}
            if (c>='A'&& akeraio!=888888 ){
              goto returnTrue;} 
            if (c=='@'){EndOL=true;
             #ifdef DEBUG_GCODE_TEXT
                    Serial.print("R: End Of Line detected  ");
              #endif  
              
              goto returnTrue; 
              }
              
            if (FirstDigit){
              #ifdef DEBUG_GCODE_TEXT
                    Serial.print("R: first digit at char # ");
                    Serial.print(counter);
                    Serial.print ( "  =  ");
                    Serial.println(c);
              #endif
                    akeraio = c - '0';
                    FirstDigit = false;}
            else {
              #ifdef DEBUG_GCODE_TEXT
                    Serial.print ( "R: next digit at char #  ");
                    Serial.print(counter);
                    Serial.print ( "  =  ");
                    Serial.println(c);
              #endif
                  akeraio = akeraio*10 + (c - '0');}
                  counter++;
            c = line[counter];
            if ((c>='0'&& c<='9' )|| (c>='A' && c<='Z' )|| c=='.' )continue ;
            else {EndOL=true; counter =counter-1;
              goto returnTrue;}
     }
               #ifdef DEBUG_GCODE_TEXT
                    Serial.print("sign . at # ");
                    Serial.println(counter);
              #endif
 
  //dekadiko

               
   counter++;           
  c = line[counter];
   if ((c>='0'&& c<='9' )|| (c>='A' && c<='Z' ) )goto IterStart ;
            else {EndOL=true; counter =counter-1; 
             goto returnTrue;}
             
IterStart:    if (  c=='@') {EndOL=true; counter =counter-1;
              goto returnTrue;}
    FirstDigit = true;
      while (c >='0' && c<='9'  ){
          if (FirstDigit) {
              #ifdef DEBUG_GCODE_TEXT
                    Serial.print("R: Dekadiko at char # ");
                    Serial.print(counter);
                    Serial.print ( "  =  ");
                    Serial.println(c);
              #endif
              dekadiko = c - '0';
                diviz=10.;
              value = float(dekadiko)/diviz;
              
              FirstDigit = false;
            }
            else {
              #ifdef DEBUG_GCODE_TEXT
                    Serial.print ( "R: next digit at char #  ");
                    Serial.print(counter);
                    Serial.print ( "  =  ");
                    Serial.println(c);
              #endif
              dekadiko = dekadiko*10 + (c - '0');
            diviz = diviz*10.;
            value = value +float((c-'0'))/diviz ;}
            counter++;
            c = line[counter];
            if ((c>='0'&& c<='9' )|| (c>='A' && c<='Z' ) )continue ;
            else {EndOL=true; counter =counter-1;
              goto returnTrue;}
  }
  if (c=='@' )EndOL=true;
   else { 
   #ifdef DEBUG_GCODE_TEXT
        Serial.print  ("R: akeraio ");Serial.println (akeraio);
        Serial.print  ("R: dekadiko ");Serial.println (dekadiko);
        Serial.print  ("R: value ");Serial.println (value); 
  #endif
   }
 

returnTrue:              
  counter =counter-1;
  return(true);
  
returnFalse:
counter =counter-1;
return(false);        
       
}

