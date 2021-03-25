

/*

// Define step pulse output pins. NOTE: All step bit pins must be on the same port.
#define STEP_DDR      DDRA
#define STEP_PORT     PORTA
#define STEP_PIN      PINA
#define X_STEP_BIT    2 // MEGA2560 Digital Pin 24
#define Y_STEP_BIT    3 // MEGA2560 Digital Pin 25
#define Z_STEP_BIT    4 // MEGA2560 Digital Pin 26
#define STEP_MASK ((1<<X_STEP_BIT)|(1<<Y_STEP_BIT)|(1<<Z_STEP_BIT)) // All step bits

// Define step direction output pins. NOTE: All direction pins must be on the same port.
#define DIRECTION_DDR     DDRC
#define DIRECTION_PORT    PORTC
#define DIRECTION_PIN     PINC
#define X_DIRECTION_BIT   7 // MEGA2560 Digital Pin 30
#define Y_DIRECTION_BIT   6 // MEGA2560 Digital Pin 31
#define Z_DIRECTION_BIT   5 // MEGA2560 Digital Pin 32
#define DIRECTION_MASK ((1<<X_DIRECTION_BIT)|(1<<Y_DIRECTION_BIT)|(1<<Z_DIRECTION_BIT)) // All direction bits

// Define stepper driver enable/disable output pin.
#define STEPPERS_DISABLE_DDR   DDRB
#define STEPPERS_DISABLE_PORT  PORTB
#define STEPPERS_DISABLE_BIT   7 // MEGA2560 Digital Pin 13
#define STEPPERS_DISABLE_MASK (1<<STEPPERS_DISABLE_BIT)


#define LINE_BUFFER_SIZE  100

  #define SEGMENT_BUFFER_SIZE 6

#ifdef REPORT_REALTIME_RATE
float st_get_realtime_rate();
#endif
#define BLOCK_BUFFER_SIZE 18

char line[LINE_BUFFER_SIZE]; // Line to be executed. Zero-terminated.


// Define spindle enable and spindle direction output pins.
#define SPINDLE_ENABLE_DDR      DDRH
#define SPINDLE_ENABLE_PORT     PORTH
#define SPINDLE_ENABLE_BIT      3 // MEGA2560 Digital Pin 6
#define SPINDLE_DIRECTION_DDR   DDRE
#define SPINDLE_DIRECTION_PORT  PORTE
#define SPINDLE_DIRECTION_BIT   3 // MEGA2560 Digital Pin 5

 
// Define probe switch input pin.
#define PROBE_DDR       DDRK
#define PROBE_PIN       PINK
#define PROBE_PORT      PORTK
#define PROBE_BIT       7  // MEGA2560 Analog Pin 15
#define PROBE_MASK      (1<<PROBE_BIT)

// Start of PWM & Stepper Enabled Spindle
 #ifdef VARIABLE_SPINDLE
  // Advanced Configuration Below You should not need to touch these variables
  // Set Timer up to use TIMER4B which is attached to Digital Pin 7
   #define PWM_MAX_VALUE       65535.0
   #define TCCRA_REGISTER    TCCR4A
   #define TCCRB_REGISTER    TCCR4B
   #define OCR_REGISTER    OCR4B
  
   #define COMB_BIT      COM4B1
   #define WAVE0_REGISTER    WGM40
   #define WAVE1_REGISTER    WGM41
   #define WAVE2_REGISTER    WGM42
   #define WAVE3_REGISTER    WGM43
  
   #define SPINDLE_PWM_DDR   DDRH
   #define SPINDLE_PWM_PORT    PORTH
   #define SPINDLE_PWM_BIT   4 // MEGA2560 Digital Pin 7
 #endif // End of VARIABLE_SPINDLE



//---------------------SETTINGS ------------------------

// Define EEPROM address indexing for coordinate parameters
#define N_COORDINATE_SYSTEM 6  // Number of supported work coordinate systems (from index 1)
#define SETTING_INDEX_NCOORD N_COORDINATE_SYSTEM+1 // Total number of system stored (from index 0)
// NOTE: Work coordinate indices are (0=G54, 1=G55, ... , 6=G59)




//---------------------LIMITS ------------------------

#define HOMING_AXIS_SEARCH_SCALAR  1.5 // Must be > 1 to ensure limit switch will be engaged.
#define HOMING_AXIS_LOCATE_SCALAR  5.0 // Must be > 1 to ensure limit switch is cleared.
#define SETTING_INDEX_G28    N_COORDINATE_SYSTEM    // Home position 1
#define SETTING_INDEX_G30    N_COORDINATE_SYSTEM+1  // Home position 2
// #define SETTING_INDEX_G92    N_COORDINATE_SYSTEM+2  // Coordinate offset (G92.2,G92.3 not supported)

//------------------------------------------------------ 



// Conversions
#define MM_PER_INCH (25.40)
#define INCH_PER_MM (0.0393701)
#define TICKS_PER_MICROSECOND (F_CPU/1000000)
 
// Define different comment types for pre-parsing.
#define COMMENT_NONE 0
#define COMMENT_TYPE_PARENTHESES 1
#define COMMENT_TYPE_SEMICOLON 2

#define CMD_STATUS_REPORT '?'
#define CMD_FEED_HOLD '!'
#define CMD_CYCLE_START '~'
#define CMD_RESET 0x18 // ctrl-x.

// ---------------------------------------------------------------------------------------
// COMPILE-TIME ERROR CHECKING OF DEFINE VALUES:
// Some useful constants.
#define DT_SEGMENT (1.0/(ACCELERATION_TICKS_PER_SECOND*60.0)) // min/segment 
#define REQ_MM_INCREMENT_SCALAR 1.25                                   
#define RAMP_ACCEL 0
#define RAMP_CRUISE 1
#define RAMP_DECEL 2


#define MAX_AMASS_LEVEL 3
// AMASS_LEVEL0: Normal operation. No AMASS. No upper cutoff frequency. Starts at LEVEL1 cutoff frequency.
#define AMASS_LEVEL1 (F_CPU/8000) // Over-drives ISR (x2). Defined as F_CPU/(Cutoff frequency in Hz)
#define AMASS_LEVEL2 (F_CPU/4000) // Over-drives ISR (x4)
#define AMASS_LEVEL3 (F_CPU/2000) // Over-drives ISR (x8)



// Values that define the probing state machine.  
#define PROBE_OFF     0 // Probing disabled or not in use. (Must be zero.)
#define PROBE_ACTIVE  1 // Actively watching the input pin.


*/

 





