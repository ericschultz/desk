#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <Keypad.h>
#include "Actuator.h"


/** begin non-pin consts **/
//settings
const int NUM_OF_SETTINGS = 4;
const int SAVED_SETTING_START_LOCATION = 0;
const int HAS_SETTING_EVER_BEEN_SAVED_LOCATION = 4;

//lcd
const int NUM_OF_LCD_CHARS = 16;
const int NUM_OF_LCD_LINES = 2;

//keypad
const int KEYPAD_HOLD_TIME = 500;
const int KEYPAD_ROWS = 2;
const int KEYPAD_COLS = 3;

/** end non-pin consts **/


/** begin pins **/



//actuator
const int EXTEND_ACTUATOR_PIN = 0;
const int RETRACT_ACTUATOR_PIN =  0;
const int POSITION_ACTUATOR_PIN =  0;


//lcd
const int LCD_REGISTER_SELECT_PIN = 0;
const int LCD_RW_PIN = 0;
const int LCD_ENABLE_PIN = 0;


// keypad
const int UP_BUTTON_PIN= 0;
const int DOWN_BUTTON_PIN= 0;

const int ROW0_PIN=0;
const int ROW1_PIN=0;
const int COL0_PIN=0;
const int COL1_PIN= 0;
const int COL2_PIN =0;


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

int minimum_height_in_mm = 0;
int maximium_height_in_mm = 0;
 
 
int saved_setting_values[] = { 0, 341, 682, 1023};

void setup()
{
	Serial.begin(9600);
	load_all_settings();
	keypad.addTransitionListener(&handle_transition);
	
}

void loop()
{
	//just run us
	keypad.getKeys();
	//if we need to handle a keyPress
}

void load_setting(byte saved_setting_number)
{
	int value, ever_saved;
	if (saved_setting_number >= NUM_OF_SETTINGS)
		return;
		
	
	value = read_eeprom(savedSettingLocation(saved_setting_number));
	ever_saved = read_eeprom(hasEverBeenSavedLocation(saved_setting_number));
	if (ever_saved == 255)
		return;
	
	saved_setting_values[saved_setting_number] = value;
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
	

int savedSettingLocation(byte savedSettingNumber)
{
	return (savedSettingNumber + SAVED_SETTING_START_LOCATION) * sizeof(int);
}

int hasEverBeenSavedLocation(byte savedSettingNumber)
{
	return (savedSettingNumber + HAS_SETTING_EVER_BEEN_SAVED_LOCATION) *sizeof(int);
}

void handle_transition(char key, KeyState from, KeyState to)
{
}




