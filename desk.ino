#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <Keypad.h>


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
const int EXTEND_ACTUATOR_PIN =
const int RETRACT_ACTUATOR_PIN =
const int ACTUATOR_POSITION_PIN =


//lcd
const int LCD_REGISTER_SELECT_PIN = ;
const int LCD_RW_PIN = ;
const int LCD_ENABLE_PIN = ;


// keypad
const int UP_BUTTON_PIN=
const int DOWN_BUTTON_PIN=


#define ROW0_PIN 0
#define ROW1_PIN 0
#define COL0_PIN 0
#define COL1_PIN 0
#define COL2_PIN 0


/** end pins **/




char keys[ROWS][COLS] = {
	{'U', '1', '2'},
	{'D', '3', '4'}
};

byte rowPins[ROWS] = {ROW0_PIN, ROW1_PIN};
byte columnPins[COLS] = {COL0_PIN, COL1_PIN, COL2_PIN};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); 
 
 
int saved_setting_values[] = { 0, 341, 682, 1023};

void setup()
{
	Serial.begin(9600);
	load_all_settings();
	
}

void loop()
{
}

void load_setting(int saved_setting_number)
{
	int value, ever_saved;
	if (saved_setting_number >= NUM_OF_SETTINGS)
		return;
		
	
	value = read_eeprom(saved_setting_number + SAVED_SETTING_START_LOCATION);
	ever_saved = read_eeprom(saved_setting_number + SAVED_SETTING_START_LOCATION);
	if (ever_saved == 255)
		return;
	
	saved_setting_value[saved_setting_number] = value;
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
	value = EEPROM.read(location)
	Serial.print("Read EEPROM location ");
	Serial.print(location, DEC);
	Serial.print(" value was ");
	Serial.println(value, DEC);
}
