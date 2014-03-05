#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <Keypad.h>
#include "Actuator.h"
#include "LcdAdapter.h"


struct TransitionOccurred {
	char key;
	KeyState fromState;
	KeyState toState;
};
/** begin non-pin consts **/
//settings
const int NUM_OF_SETTINGS = 4;
const int SAVED_SETTING_START_LOCATION = 0;
const int HAS_SETTING_EVER_BEEN_SAVED_LOCATION = 4;

//lcd
const int NUM_OF_LCD_CHARS = 16;
const int NUM_OF_LCD_LINES = 2;
const int MARQUEE_TIME = 5000;

//keypad
const int KEYPAD_HOLD_TIME = 500;
const int KEYPAD_ROWS = 2;
const int KEYPAD_COLS = 3;

/** end non-pin consts **/


/** begin pins **/



//actuator
const int EXTEND_ACTUATOR_PIN = 0;
const int RETRACT_ACTUATOR_PIN =  1;
//analog
const int POSITION_ACTUATOR_PIN =  0;


//lcd
const int LCD_REGISTER_SELECT_PIN = 2;
const int LCD_RW_PIN = 3;
const int LCD_ENABLE_PIN = 4;
const int LCD_D4 =5;
const int LCD_D5=6;
const int LCD_D6 =7 ;
const int LCD_D7=8;

// keypad


const int ROW0_PIN=9;
const int ROW1_PIN=10;
const int COL0_PIN=11;
const int COL1_PIN= 12;
const int COL2_PIN =13;


/** end pins **/

const int ROWS=2;
const int COLS=3;


char keys[ROWS][COLS] = {
	{'U', '1', '2'},
	{'D', '3', '4'}
};

byte row_pins[ROWS] = {ROW0_PIN, ROW1_PIN};
byte col_pins[COLS] = {COL0_PIN, COL1_PIN, COL2_PIN};
Keypad keypad = Keypad( makeKeymap(keys), row_pins, col_pins, ROWS, COLS ); 

Actuator actuator = Actuator(EXTEND_ACTUATOR_PIN, RETRACT_ACTUATOR_PIN, POSITION_ACTUATOR_PIN);
LcdAdapter lcd = LcdAdapter(LCD_REGISTER_SELECT_PIN, LCD_RW_PIN, LCD_ENABLE_PIN, LCD_D4, LCD_D5, LCD_D6, LCD_D7, MARQUEE_TIME);

int minimum_height_in_mm = 0;
int maximium_height_in_mm = 0;
 
bool didTransitionOccur = false;
TransitionOccurred currentTransition;
 
 
int saved_setting_values_defaults[] = { 0, 341, 682, 1023};




void setup()
{
	Serial.begin(9600);
	setupPins();
	load_all_settings();
	keypad.addTransitionListener(&handle_transition);
	lcd.begin(NUM_OF_LCD_CHARS, NUM_OF_LCD_LINES);
}



void loop()
{
	//just run us
	//need to run 
	keypad.getKeys();
	lcd.handleEvents();
	
	lcd.print( getPrettyActuatorValue());
	//if we need to handle a keyPress, we do it here
	
	if (didTransitionOccur)
	{
		switch (currentTransition.key)
		{
			case 'U':
				    actuator.goUp();
					break;			
			case 'D':
					actuator.goDown();
					break;
			default:
					//we we've moved from PRESSED to HOLD or RELEASE
					if(currentTransition.fromState == PRESSED && currentTransition.toState == HOLD)
					{
						//we save!
					    save_setting(getByteFromDigit(currentTransition.key));
						lcd.marquee("Saving " + getPrettyActuatorValue() + " to save space " + currentTransition.key);
					}
					else if(currentTransition.fromState == PRESSED && currentTransition.toState == RELEASED)
					{
						//we go to!
						int value = load_setting(getByteFromDigit(currentTransition.key));
						if (value >= 0)
						{
							actuator.goTo(value);
						}
					}
		}//keypress handling
	}
	//clear our transition state
	clearTransitionState();
}

int load_setting(byte saved_setting_number)
{
	int value, ever_saved;
	if (saved_setting_number >= NUM_OF_SETTINGS)
		return -1;
		
	
	value = read_eeprom(savedSettingLocation(saved_setting_number));
	ever_saved = read_eeprom(hasEverBeenSavedLocation(saved_setting_number));
	if (ever_saved == 255)
		return saved_setting_values_defaults[saved_setting_number];
	
	return value;
}



void save_setting(byte saved_setting_number)
{
  if (saved_setting_number >= NUM_OF_SETTINGS){
	return;
  }
  
  write_eeprom(savedSettingLocation(saved_setting_number), actuator.getCurrentPosition());
  write_eeprom(hasEverBeenSavedLocation(saved_setting_number), 1);
}

void load_all_settings()
{
	for(int i = 0; i < NUM_OF_SETTINGS; i++)
	{
		load_setting(i);
	}
}


int read_eeprom(int location)
{
	int value;
	value = EEPROM.read(location);
	Serial.print("Read EEPROM location ");
	Serial.print(location, DEC);
	Serial.print(" value was ");
	Serial.println(value, DEC);
}

void write_eeprom(int location, int value)
{
	EEPROM.write(location, value);
	Serial.print("Wrote EEPROM location ");
	Serial.print(location, DEC);
	Serial.print(" with value ");
	Serial.println(value, DEC);
}
	
byte getByteFromDigit(char digit)
{
	return digit - '0';
}


int savedSettingLocation(byte savedSettingNumber)
{
	return (savedSettingNumber + SAVED_SETTING_START_LOCATION) * sizeof(int);
}

int hasEverBeenSavedLocation(byte savedSettingNumber)
{
	return (savedSettingNumber + HAS_SETTING_EVER_BEEN_SAVED_LOCATION) *sizeof(int);
}


//someday will get you the value in inches or millimeters. Currently just gives you the number
String getPrettyActuatorValue()
{
	return String(actuator.getCurrentPosition());
}

void handle_transition(char key, KeyState from, KeyState to)
{
	currentTransition.key = key;
	currentTransition.fromState = from;
	currentTransition.toState = to;
	didTransitionOccur = true;
}

void clearTransitionState()
{
	didTransitionOccur = false;
}

void setupPins()
{
	pinMode(EXTEND_ACTUATOR_PIN, OUTPUT);
	pinMode(RETRACT_ACTUATOR_PIN,OUTPUT);
	pinMode(LCD_REGISTER_SELECT_PIN,OUTPUT);
	pinMode(LCD_RW_PIN,OUTPUT);
	pinMode(LCD_ENABLE_PIN,OUTPUT);
	pinMode(LCD_D4,OUTPUT);
	pinMode(LCD_D5,OUTPUT);
	pinMode(LCD_D6,OUTPUT);
	pinMode(LCD_D7,OUTPUT);
	
	pinMode(ROW0_PIN,INPUT);
	pinMode(ROW1_PIN, INPUT);
	pinMode(COL0_PIN, INPUT);
	pinMode(COL1_PIN, INPUT);
	pinMode(COL2_PIN, INPUT);
}




