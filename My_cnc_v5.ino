 #include <SPI.h>
#include <SD.h>
 //SD set up variables 
 Sd2Card card;
 SdVolume volume;
File root;
File myFile ;
 
 #include <Adafruit_GFX.h>
 #include <Adafruit_PCD8544.h>
 Adafruit_PCD8544 display = Adafruit_PCD8544(35,27,29);

#include <AccelStepper.h>
#include <MultiStepper.h>
// Define the steppers and the pins the will use
// The X Stepper pins
#define STEPPERX_DIR_PIN 32
#define STEPPERX_STEP_PIN 26

// The Z stepper pins
#define STEPPERZ_DIR_PIN 31
#define STEPPERZ_STEP_PIN 25

// The Y stepper pins
 #define STEPPERY_DIR_PIN 30
 #define STEPPERY_STEP_PIN 24

AccelStepper stepperX(AccelStepper::DRIVER, STEPPERX_STEP_PIN, STEPPERX_DIR_PIN);
AccelStepper stepperY(AccelStepper::DRIVER, STEPPERY_STEP_PIN, STEPPERY_DIR_PIN);
AccelStepper stepperZ(AccelStepper::DRIVER, STEPPERZ_STEP_PIN, STEPPERZ_DIR_PIN);

 
// Keypad  
#include <Keypad.h>
#include <Key.h>
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {34,36,38,40}; 
byte colPins[COLS] = {28,37,39}; 
Keypad Akeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
 


// Axis array index values. Must start with 0 and be continuous.
#define N_AXIS 3 // Number of axes
#define X_AXIS 0 // Axis indexing value. 
#define Y_AXIS 1
#define Z_AXIS 2
#define AXIS_N_SETTINGS          4// Number of settings parameter per  axe

#define LINE_BUFFER_SIZE  100

// Define Grbl status codes.
#define STATUS_OK 0
#define STATUS_EXPECTED_COMMAND_LETTER 1
#define STATUS_BAD_NUMBER_FORMAT 2
#define STATUS_INVALID_STATEMENT 3
#define STATUS_NEGATIVE_VALUE 4
#define STATUS_SETTING_DISABLED 5
#define STATUS_SETTING_STEP_PULSE_MIN 6
#define STATUS_SETTING_READ_FAIL 7
#define STATUS_IDLE_ERROR 8
#define STATUS_ALARM_LOCK 9
#define STATUS_SOFT_LIMIT_ERROR 10
#define STATUS_OVERFLOW 11
#define STATUS_MAX_STEP_RATE_EXCEEDED 12
#define STATUS_GCODE_UNSUPPORTED_COMMAND 20
#define STATUS_GCODE_MODAL_GROUP_VIOLATION 21
#define STATUS_GCODE_UNDEFINED_FEED_RATE 22
#define STATUS_GCODE_COMMAND_VALUE_NOT_INTEGER 23
#define STATUS_GCODE_AXIS_COMMAND_CONFLICT 24
#define STATUS_GCODE_WORD_REPEATED 25
#define STATUS_GCODE_NO_AXIS_WORDS 26
#define STATUS_GCODE_INVALID_LINE_NUMBER 27
#define STATUS_GCODE_VALUE_WORD_MISSING 28
#define STATUS_GCODE_UNSUPPORTED_COORD_SYS 29
#define STATUS_GCODE_G53_INVALID_MOTION_MODE 30
#define STATUS_GCODE_AXIS_WORDS_EXIST 31
#define STATUS_GCODE_NO_AXIS_WORDS_IN_PLANE 32
#define STATUS_GCODE_INVALID_TARGET 33
#define STATUS_GCODE_ARC_RADIUS_ERROR 34
#define STATUS_GCODE_NO_OFFSETS_IN_PLANE 35
#define STATUS_GCODE_UNUSED_WORDS 36
#define STATUS_GCODE_G43_DYNAMIC_AXIS_ERROR 37

// Define Grbl alarm codes.
#define ALARM_HARD_LIMIT_ERROR 1
#define ALARM_SOFT_LIMIT_ERROR 2
#define ALARM_ABORT_CYCLE 3
#define ALARM_PROBE_FAIL 4
#define ALARM_HOMING_FAIL 5

// Define Grbl feedback message codes.
#define MESSAGE_CRITICAL_EVENT 1
#define MESSAGE_ALARM_LOCK 2
#define MESSAGE_ALARM_UNLOCK 3
#define MESSAGE_ENABLED 4
#define MESSAGE_DISABLED 5
#define MESSAGE_PROGRAM_END 7
#define MESSAGE_RESTORE_DEFAULTS 8

  // Grbl generic default settings.  
  #define DEFAULT_X_STEPS_PER_MM 250.0
  #define DEFAULT_Y_STEPS_PER_MM 250.0
  #define DEFAULT_Z_STEPS_PER_MM 250.0
  
  #define DEFAULT_X_MAX_RATE 500.0 // mm/min
  #define DEFAULT_Y_MAX_RATE 500.0 // mm/min
  #define DEFAULT_Z_MAX_RATE 500.0 // mm/min
  
  #define DEFAULT_X_ACCELERATION (10.0*60*60) // 10*60*60 mm/min^2 = 10 mm/sec^2
  #define DEFAULT_Y_ACCELERATION (10.0*60*60) // 10*60*60 mm/min^2 = 10 mm/sec^2
  #define DEFAULT_Z_ACCELERATION (10.0*60*60) // 10*60*60 mm/min^2 = 10 mm/sec^2
  
  #define DEFAULT_X_MAX_TRAVEL 200.0 // mm
  #define DEFAULT_Y_MAX_TRAVEL 200.0 // mm
  #define DEFAULT_Z_MAX_TRAVEL 10.0 // mm
  
  #define DEFAULT_STEP_PULSE_MICROSECONDS 10
  #define DEFAULT_STEPPING_INVERT_MASK 0
  #define DEFAULT_DIRECTION_INVERT_MASK 0
  #define DEFAULT_STEPPER_IDLE_LOCK_TIME 25 // msec (0-254, 255 keeps steppers enabled)
  #define DEFAULT_STATUS_REPORT_MASK ((BITFLAG_RT_STATUS_MACHINE_POSITION)|(BITFLAG_RT_STATUS_WORK_POSITION))
  #define DEFAULT_JUNCTION_DEVIATION 0.01 // mm
  #define DEFAULT_ARC_TOLERANCE 0.002 // mm
  #define DEFAULT_INVERT_ST_ENABLE 0 // false
  #define DEFAULT_SOFT_LIMIT_ENABLE 0 // false
  #define DEFAULT_HARD_LIMIT_ENABLE 0  // false
  
  #define DEFAULT_HOMING_ENABLE 1  // false
  
  #define DEFAULT_HOMING_DIR_MASK 0 // move positive dir
  
  #define DEFAULT_HOMING_FEED_RATE 250.0 // mm/min
  #define DEFAULT_HOMING_SEEK_RATE 500.0 // mm/min
  #define DEFAULT_HOMING_PULLOFF 1.0 // mm

// Define bit flag masks for the boolean settings in settings.flag.
   #define BITFLAG_INVERT_ST_ENABLE   bit(2)
   #define BITFLAG_HARD_LIMIT_ENABLE  bit(3)
   #define BITFLAG_HOMING_ENABLE      bit(4)
   #define BITFLAG_SOFT_LIMIT_ENABLE  bit(5)
   #define BITFLAG_INVERT_PROBE_PIN   bit(7)

// Define status reporting boolean enable bit flags in settings.status_report_mask
#define BITFLAG_RT_STATUS_MACHINE_POSITION  bit(0)
#define BITFLAG_RT_STATUS_WORK_POSITION     bit(1)
#define BITFLAG_RT_STATUS_PLANNER_BUFFER    bit(2)
#define BITFLAG_RT_STATUS_SERIAL_RX         bit(3)
#define BITFLAG_RT_STATUS_LIMIT_PINS        bit(4)

// critical events.
#define STATE_IDLE          0      // Must be zero. No flags.
#define STATE_ALARM         bit(0) // In alarm state. Locks out all g-code processes. Allows settings access.
#define STATE_CHECK_MODE    bit(1) // G-code check mode. Locks out planner and motion only.
#define STATE_HOMING        bit(2) // Performing homing cycle
#define STATE_CYCLE         bit(3) // Cycle is running or motions are being executed.
#define STATE_HOLD          bit(4) // Active feed hold
#define STATE_MOTION_CANCEL bit(6) // Motion cancel by feed hold and return to idle. 

// Define system executor bit map. default 0
#define EXEC_STATUS_REPORT  bit(0) // bitmask 00000001
#define EXEC_CYCLE_START    bit(1) // bitmask 00000010
#define EXEC_CYCLE_STOP     bit(2) // bitmask 00000100
#define EXEC_FEED_HOLD      bit(3) // bitmask 00001000
#define EXEC_RESET          bit(4) // bitmask 00010000
#define EXEC_MOTION_CANCEL  bit(6) // bitmask 01000000
// Alarm executor bit map.
#define EXEC_CRITICAL_EVENT     bit(0) // bitmask 00000001 (SPECIAL FLAG. See NOTE:)
#define EXEC_ALARM_HARD_LIMIT   bit(1) // bitmask 00000010
#define EXEC_ALARM_SOFT_LIMIT   bit(2) // bitmask 00000100
#define EXEC_ALARM_ABORT_CYCLE  bit(3) // bitmask 00001000
#define EXEC_ALARM_PROBE_FAIL   bit(4) // bitmask 00010000
#define EXEC_ALARM_HOMING_FAIL  bit(5) // bitmask 00100000
// Define system suspend states.
#define SUSPEND_DISABLE       0      // Must be zero.
#define SUSPEND_ENABLE_HOLD   bit(0) // Enabled. Indicates the cycle is active and currently undergoing a hold.
#define SUSPEND_ENABLE_READY  bit(1) // Ready to resume with a cycle start command.
#define SUSPEND_ENERGIZE      bit(2) // Re-energizes output before resume.
#define SUSPEND_MOTION_CANCEL bit(3) // Cancels resume motion. Used by probing routine.

// Useful macros
#define int_disable(regbit)  (0 <<regbit) 
#define int_enable(regbit)  (1 <<regbit) 
#define clear_vector(a) memset(a, 0, sizeof(a))
#define clear_vector_float(a) memset(a, 0.0, sizeof(float)*N_AXIS)
// #define clear_vector_long(a) memset(a, 0.0, sizeof(long)*N_AXIS)
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
float hypot_f(float x, float y) { return(sqrt(x*x + y*y)); }
// Bit field and masking macros
#define bit(n) (1 << n) 
#define bit_true_atomic(x,mask) {byte sreg = SREG; cli(); (x) |= (mask); SREG = sreg; }
#define bit_false_atomic(x,mask) {byte sreg = SREG; cli(); (x) &= ~(mask); SREG = sreg; }
#define bit_toggle_atomic(x,mask) {byte sreg = SREG; cli(); (x) ^= (mask); SREG = sreg; }
#define bit_true(x,mask) (x) |= (mask)
#define bit_false(x,mask) (x) &= ~(mask)
#define bit_istrue(x,mask) ((x & mask) != 0)
#define bit_isfalse(x,mask) ((x & mask) == 0)

#define SERIAL_NO_DATA 0xff
#define MAX_LINE_NUMBER 9999999
#define MAX_INT_DIGITS 8 // Maximum number of digits in int32 (and float)
#define SOME_LARGE_VALUE 1.0E+38 

#define AXIS_COMMAND_NONE 0
#define AXIS_COMMAND_NON_MODAL 1 
#define AXIS_COMMAND_MOTION_MODE 2
#define AXIS_COMMAND_TOOL_LENGTH_OFFSET 3 // *Undefined but required

#define MODAL_GROUP_G0 0 // [G4,G10,G28,G28.1,G30,G30.1,G53,G92,G92.1] Non-command
#define MODAL_GROUP_G1 1 // [G0,G1,G2,G3,G38.2,G38.3,G38.4,G38.5,G80] Motion
#define MODAL_GROUP_G2 2 // [G17,G18,G19] Plane selection
#define MODAL_GROUP_G3 3 // [G90,G91] Distance mode
#define MODAL_GROUP_G4 4 // [G91.1] Arc IJK distance mode
#define MODAL_GROUP_G5 5 // [G93,G94] Feed rate mode
//#define MODAL_GROUP_G6 6 // [G20,G21] Units
#define MODAL_GROUP_G7 7 // [G40] Cutter radius compensation mode. G41/42 NOT SUPPORTED.
#define MODAL_GROUP_G8 8 // [G43.1,G49] Tool length offset
#define MODAL_GROUP_G12 9 // [G54,G55,G56,G57,G58,G59] Coordinate system selection
#define MODAL_GROUP_G13 10 // [G61] Control mode
#define MODAL_GROUP_M4 11  // [M0,M1,M2,M30] Stopping
#define MODAL_GROUP_M7 12 // [M3,M4,M5] Spindle turning
#define MODAL_GROUP_M8 13 // [M7,M8,M9] Coolant control

  #define OTHER_INPUT_F 14
  #define OTHER_INPUT_S 15
  #define OTHER_INPUT_T 16

// Define parameter word mapping.
#define WORD_F  0
#define WORD_I  1
#define WORD_J  2
#define WORD_K  3
#define WORD_L  4
#define WORD_P  6
#define WORD_R  7
#define WORD_S  8
#define WORD_T  9
#define WORD_X  10
#define WORD_Y  11
#define WORD_Z  12
// Modal Group G0: Non-command actions
#define NON_MODAL_NO_ACTION 0 // (Default: Must be zero)
#define NON_MODAL_DWELL 1 // G4
#define NON_MODAL_SET_COORDINATE_DATA 2 // G10
#define G28_NON_MODAL_GO_HOME_0 3 // G28
#define  G28_1_NON_MODAL_SET_HOME_0 4 // G28.1
#define G30_NON_MODAL_GO_HOME_0 5 // G30
#define  G30_1_NON_MODAL_SET_HOME_1 6 // G30.1
#define NON_MODAL_ABSOLUTE_OVERRIDE 7 // G53
#define NON_MODAL_SET_COORDINATE_OFFSET 8 // G92
#define NON_MODAL_RESET_COORDINATE_OFFSET 9 //G92.1

// Modal Group G1: Motion modes
#define MOTION_MODE_SEEK 0 // G0 (Default: Must be zero)
#define MOTION_MODE_LINEAR 1 // G1
#define MOTION_MODE_CW_ARC 2  // G2
#define MOTION_MODE_CCW_ARC 3  // G3
#define MOTION_MODE_PROBE_TOWARD 4 // G38.2 NOTE: G38.2, G38.3, G38.4, G38.5 must be sequential. See report_gcode_modes().
#define MOTION_MODE_PROBE_TOWARD_NO_ERROR 5 // G38.3
#define MOTION_MODE_PROBE_AWAY 6 // G38.4
#define MOTION_MODE_PROBE_AWAY_NO_ERROR 7 // G38.5
#define MOTION_MODE_NONE 8 // G80

// Modal Group G2: Plane select
#define PLANE_SELECT_XY 0 // G17 (Default: Must be zero)
#define PLANE_SELECT_ZX 1 // G18
#define PLANE_SELECT_YZ 2 // G19

// Modal Group G3: Distance mode
#define DISTANCE_MODE_ABSOLUTE 0 // G90 (Default: Must be zero)
#define DISTANCE_MODE_INCREMENTAL 1 // G91

// Modal Group G4: Arc IJK distance mode
#define DISTANCE_ARC_MODE_INCREMENTAL 0 // G91.1 (Default: Must be zero)

// Modal Group M4: Program flow
#define PROGRAM_FLOW_RUNNING 0 // (Default: Must be zero)
#define PROGRAM_FLOW_PAUSED 1 // M0, M1
#define PROGRAM_FLOW_COMPLETED 2 // M2, M30

// Modal Group G5: Feed rate mode
#define FEED_RATE_MODE_UNITS_PER_MIN 0 // G94 (Default: Must be zero)
#define FEED_RATE_MODE_INVERSE_TIME 1 // G93



// Modal Group G7: Cutter radius compensation mode
#define CUTTER_COMP_DISABLE 0 // G40 (Default: Must be zero)

// Modal Group G13: Control mode
#define CONTROL_MODE_EXACT_PATH 0 // G61 (Default: Must be zero)

// Modal Group M7: Spindle control
#define SPINDLE_DISABLE 0 // M5 (Default: Must be zero)
#define SPINDLE_ENABLE_CW 1 // M3
#define SPINDLE_ENABLE_CCW 2 // M4



// Modal Group G8: Tool length offset
#define TOOL_LENGTH_OFFSET_CANCEL 0 // G49 (Default: Must be zero)
#define TOOL_LENGTH_OFFSET_ENABLE_DYNAMIC 1 // G43.1

   int akeraio = 0;
  int dekadiko = 0; 
   boolean isNegativ = false;
   boolean EndOL=false;
  char line[LINE_BUFFER_SIZE];  
     byte counter ;
     float value; 
     
boolean stepperXIsStopped = false;
boolean stepperYIsStopped = false;

   struct settings_t{          // Global persistent settings (Stored from byte EEPROM_ADDR_GLOBAL onwards)
  // Axis settings
  float steps_per_mm[N_AXIS];
  float max_rate[N_AXIS];
  float acceleration[N_AXIS];
  float max_travel[N_AXIS];
  // Remaining Grbl settings
  byte pulse_microseconds;
  byte step_invert_mask;
  byte dir_invert_mask;
  byte stepper_idle_lock_time; // If max value 255, steppers do not disable.
  byte status_report_mask;     // Mask to indicate desired report data.
  float junction_deviation;
  float arc_tolerance;
  byte flags;                  // Contains default boolean settings
  byte homing_dir_mask;
  float homing_feed_rate;
  float homing_seek_rate;
  float homing_pulloff;
} ;
struct settings_t settings;   


 struct system_t {            // Define global system variables
  byte abort;                 // System abort flag. Forces exit back to main loop for reset.
  byte state;                 // Tracks the current state of Grbl.
  byte suspend;               // System suspend bitflag variable that manages holds, cancels, and safety door.
  long position[N_AXIS];      // Real-time machine (aka home) position vector in steps.                             
  long probe_position[N_AXIS]; // Last probe position in machine coordinates and steps.
  boolean soft_limit;            // Tracks soft limit errors for the state machine. (boolean)
  boolean probe_succeeded;        // Tracks if last probing cycle was successful.
} ;
struct system_t sys;           // Declare system global variable structure
 
volatile byte sys_probe_state;   // Probing state value.  Used to coordinate the probing cycle with stepper ISR.
volatile byte sys_rt_exec_state;  // Global realtime executor bitflag variable for state management. See EXEC bitmasks.
volatile byte sys_rt_exec_alarm;  // Global realtime executor bitflag variable for setting various alarm
 
struct command_t {
  byte motion;          // {G0,G1,G2,G3,G38.2,G80}
  byte feed_rate;       // {G93,G94}
  byte distance;        // {G90,G91}
  byte plane_select;    // {G17,G18,G19}
  byte cutter_comp;     // {G40} NOTE: Don't track. Only default supported.
  byte tool_length;     // {G43.1,G49}
  byte coord_select;    // {G54,G55,G56,G57,G58,G59}
  byte program_flow;    // {M0,M1,M2,M30}
  byte coolant;         // {M7,M8,M9}
  byte spindle;         // {M3,M4,M5}
  byte non_command;
} ;
struct command_t command;
struct command_t  saved_command;

  struct values_t{
  float f;         // Feed Millimeters/min
  float seek_rate;  // Millimeters/min
  float ijk[3];    // I,J,K Axis arc offsets
  byte l;       // G10 or canned cycles parameters
  long n;       // Line number
  float p;         // G10 or dwell parameters
  // float q;      // G82 peck drilling
  float r;         // Arc radius
  float s;         // Spindle speed rpm
  byte t;       // Tool selection
  float xyz[3];    // X,Y,Z Translational axes
  float position[N_AXIS];       // Where the interpreter considers the tool to be at this point in the code
  float coord_system[N_AXIS];   // Current work coordinate system (G54+). Stores offset from absolute machine
                                // position in mm. Loaded from EEPROM when called.  
  float coord_offset[N_AXIS];   // Retains the G92 coordinate offset (work coordinates) relative to
                                // machine zero in mm. Non-persistent. Cleared upon reset and boot.    
  float tool_length_offset;     // Tracks tool length offset value when enabled.
  }  ;
struct  values_t values;
struct  values_t saved_values;


/*
// This struct stores a linear movement of a g-code block motion with its critical "nominal" values
// are as specified in the source g-code. 
 struct plan_block_t{
  byte direction_bits;    // The direction bit set for this block (refers to *_DIRECTION_BIT in config.h)
   long steps[N_AXIS];    // Step count along each axis
   long step_event_count; // The maximum step axis count and number of steps required to complete this block. 
  float entry_speed_sqr;         // The current planned entry speed at block junction in (mm/min)^2
  float max_entry_speed_sqr;     // Maximum allowable entry speed based on the minimum of junction limit and  neighboring nominal speeds with overrides in (mm/min)^2
  float max_junction_speed_sqr;  // Junction entry speed limit based on direction vectors in (mm/min)^2
  float nominal_speed_sqr;       // Axis-limit adjusted nominal speed for this block in (mm/min)^2
  float acceleration;            // Axis-limit adjusted line acceleration in (mm/min^2)
  float millimeters;             // The remaining distance for this block to be executed in (mm)
} ;
plan_block_t  plan_block;
/*
static plan_block_t block_buffer[BLOCK_BUFFER_SIZE];  // A ring buffer for motion instructions
static byte block_buffer_tail;     // Index of the block to process now
static byte block_buffer_head;     // Index of the next block to be pushed
static byte next_buffer_head;      // Index of the next buffer head
static byte block_buffer_planned;  // Index of the optimally planned block

//**************************************** steppers vars   ******************************************
/*
  struct st_block_t{  
  uint8_t direction_bits;
  uint32_t steps[N_AXIS];
  uint32_t step_event_count;
} ;
  struct st_block_t st_block_buffer[SEGMENT_BUFFER_SIZE-1];


  struct segment_t{
  uint16_t n_step;          // Number of step events to be executed for this segment
  uint8_t st_block_index;   // Stepper block data index. Uses this information to execute this segment.
  uint16_t cycles_per_tick; // Step distance traveled per ISR tick, aka step rate.
  #ifdef ADAPTIVE_MULTI_AXIS_STEP_SMOOTHING
    uint8_t amass_level;    // Indicates AMASS level for the ISR to execute this segment
  #else
    uint8_t prescaler;      // Without AMASS, a prescaler is required to adjust for slow timing.
  #endif
}  ;
  struct segment_t segment_buffer[SEGMENT_BUFFER_SIZE];

// Stepper ISR data struct. Contains the running data for the main stepper ISR.
  struct stepper_t{
  // Used by the bresenham line algorithm
  uint32_t counter_x,        // Counter variables for the bresenham line tracer
           counter_y, 
           counter_z;
  #ifdef STEP_PULSE_DELAY
    uint8_t step_bits;  // Stores out_bits output to complete the step pulse delay
  #endif
  
  uint8_t execute_step;     // Flags step execution for each interrupt.
  uint8_t step_pulse_time;  // Step pulse reset time after step rise
  uint8_t step_outbits;         // The next stepping-bits to be output
  uint8_t dir_outbits;
  #ifdef ADAPTIVE_MULTI_AXIS_STEP_SMOOTHING
    uint32_t steps[N_AXIS];
  #endif

  uint16_t step_count;       // Steps remaining in line segment motion  
  uint8_t exec_block_index; // Tracks the current st_block index. Change indicates new block.
  st_block_t *exec_block;   // Pointer to the block data for the segment being executed
  segment_t *exec_segment;  // Pointer to the segment being executed
} ;
 struct stepper_t st;

// Step segment ring buffer indices
static volatile uint8_t segment_buffer_tail;
static uint8_t segment_buffer_head;
static uint8_t segment_next_head;

// Step and direction port invert masks. 
static uint8_t step_port_invert_mask;
static uint8_t dir_port_invert_mask;

// Used to avoid ISR nesting of the "Stepper Driver Interrupt". Should never occur though.
static volatile uint8_t busy;   

// Pointers for the step segment being prepped from the planner buffer. Accessed only by the
// main program. Pointers may be planning segments or planner blocks ahead of what being executed.
static plan_block_t *pl_block;     // Pointer to the planner block being prepped
static st_block_t *st_prep_block;  // Pointer to the stepper block data being prepped 

// Segment preparation data struct. Contains all the necessary information to compute new segments
// based on the current executing planner block.
  struct st_prep_t{
  uint8_t st_block_index;  // Index of stepper common data block being prepped
  uint8_t flag_partial_block;  // Flag indicating the last block completed. Time to load a new one.

  float steps_remaining;
  float step_per_mm;           // Current planner block step/millimeter conversion scalar
  float req_mm_increment;
  float dt_remainder;
  
  uint8_t ramp_type;      // Current segment ramp state
  float mm_complete;      // End of velocity profile from end of current planner block in (mm).
                          // NOTE: This value must coincide with a step(no mantissa) when converted.
  float current_speed;    // Current speed at the end of the segment buffer (mm/min)
  float maximum_speed;    // Maximum speed of executing block. Not always nominal speed. (mm/min)
  float exit_speed;       // Exit speed of executing block (mm/min)
  float accelerate_until; // Acceleration ramp end measured from end of block (mm)
  float decelerate_after; // Deceleration ramp start measured from end of block (mm)
} ;
  struct st_prep_t prep;
*/
 
// -------------------------DEBUG OPTIONS-------------------------------------------------------------- 
 // #define DEBUG_GCODE_TEXT
 //  #define DEBUG_GCODE_MODE
// -------------------------HOMING CONF--------------------------------------------------------------   
 boolean HOMING_INIT_LOCK        = false; // if true program always starts with homing
 boolean HOMING_FORCE_SET_ORIGIN = true;  // if true Homing poz is 0,0,0
 boolean HOMING_Z = false;   //if true Z is homming first
                            // if false only XY are homing
  boolean FORCE_INITIALIZATION_ALARM = false;   //if true start prog with Alarm status

 #define REPORT_REALTIME_RATE 

#define MESSAGE_PROBE_COORDINATES 
// #define INVERT_ALL_CONTROL_PINS 

// #define INVERT_SPINDLE_ENABLE_PIN 
// #define REPORT_CONTROL_PIN_STATE 
 

// ------------------------------------ADVANCED CONFIGURATION OPTIONS:--------------------------------------------------

// #define REPORT_GUI_MODE // Default disabled. Uncomment to enable.
#define ACCELERATION_TICKS_PER_SECOND 100 
#define ADAPTIVE_MULTI_AXIS_STEP_SMOOTHING  // Default enabled. Comment to disable.
// #define MAX_STEP_RATE_HZ 30000 // Hz
#define TOOL_LENGTH_OFFSET_AXIS Z_AXIS // Default z-axis. Valid values are X_AXIS, Y_AXIS, or Z_AXIS.
#define REPORT_ECHO_LINE_RECEIVED  
#define MINIMUM_JUNCTION_SPEED 0.0 // (mm/min)
#define MINIMUM_FEED_RATE 1.0 // (mm/min)
#define N_ARC_CORRECTION 12 // Integer (1-255)
#define ARC_ANGULAR_TRAVEL_EPSILON 5E-7 // Float (radians)
#define DWELL_TIME_STEP 50 // Integer (1-255) (milliseconds)
// #define STEP_PULSE_DELAY 10 // Step pulse delay in microseconds. Default disabled.
// #define BLOCK_BUFFER_SIZE 18  // Uncomment to override default in planner.h.
// #define SEGMENT_BUFFER_SIZE 6 // Uncomment to override default in stepper.h.
// #define LINE_BUFFER_SIZE 80  // Uncomment to override default in protocol.h

#define HARD_LIMIT_FORCE_STATE_CHECK // Default disabled. Uncomment to enable.

//---------------------SETTINGS ------------------------

// Define EEPROM address indexing for coordinate parameters
#define N_COORDINATE_SYSTEM 6  // Number of supported work coordinate systems (from index 1)
#define SETTING_INDEX_NCOORD N_COORDINATE_SYSTEM+1 // Total number of system stored (from index 0)
// NOTE: Work coordinate indices are (0=G54, 1=G55, ... , 6=G59)




