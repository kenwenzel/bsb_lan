char version[] = "0.38";
#define ETHERNET_W5500      // activate for newer Ethernet2-Shields (W5500 instead of W5100) and install Ethernet2 library before compiling

/*
 * 
 * BSB Boiler-System-Bus LAN Interface
 *
 * ATTENION:
 *       There is no waranty that this system will not damage your heating system!
 *
 * Author: Gero Schumacher (gero.schumacher@gmail.com) (up to version 0.16)
 *         Frederik Holst (bsb@code-it.de) (from version 0.17 onwards)
 *         (based on the code and work from many other developers. Many thanks!)
 *
 * see README and HOWTO files for more information
 *
 * Version:
 *       0.1  - 21.01.2015 - initial version
 *       0.5  - 02.02.2015
 *       0.6  - 02.02.2015
 *       0.7  - 06.02.2015
 *       0.8  - 05.03.2015
 *       0.9  - 09.03.2015
 *       0.10  - 15.03.2015
 *       0.11  - 07.04.2015
 *       0.12  - 09.04.2015
 *       0.13  - 31.03.2016
 *       0.14  - 04.04.2016
 *       0.15  - 21.04.2016
 *       0.15a - 25.07.2016
 *       0.16  - 20.11.2016
 *       0.17  - 20.12.2016
 *       0.17a - 20.12.2016
 *       0.18  - 22.12.2016
 *       0.19  - 01.01.2017
 *       0.20  - 27.01.2017
 *       0.21  - 06.02.2017
 *       0.22  - 07.02.2017
 *       0.23  - 12.02.2017
 *       0.24  - 14.02.2017
 *       0.25  - 21.02.20172
 *       0.26  - 27.02.2017
 *       0.27  - 01.03.2017
 *       0.28  - 05.03.2017
 *       0.29  - 07.03.2017
 *       0.30  - 22.03.2017
 *       0.31  - 10.04.2017
 *       0.32  - 18.04.2017
 *       0.33  - 09.05.2017
 *       0.34  - 29.05.2017
 *       0.35  - 25.06.2017
 *       0.36  - 23.08.2017
 *       0.37  - 08.09.2017
 *       0.38  - 11.11.2017
 *
 * Changelog:
 *       version 0.38
 *        - Including two-stage oil furnaces in logging - please note that logging parameters have been adjusted, see BSB_lan_config.h for new values!
 *        - Bugfixing SD-card logging in monitor mode
 *        - Bugfix for setting hour:time parameters via webinterface
 *        - Added Brötje BOB device family (138)
 *        - Added Brötje SOB26 device family (28)
 *        - Added Elco Aquatop 8es device family (85)
 *        - Added Elco Thision 13 Plus device family (203)
 *        - Added Weishaupt WTU 25-G familiy (50)
 *        - New schematics for Arduino/Raspberry board layout
 *       version 0.37
 *        - LPB implementation! More than 450 parameters supported! Switch temporarily between LPB and BSB with the Px command (0=BSB, 1=LPB) or use the setBusType config option to set bus-type at boot-time. Parameter numbers are the same as for BSB.
 *       version 0.36
 *        - bugfix: brought back VT_BIT list of options which were erroneously deleted :(, fixed/freed several memory issues
 *       version 0.35
 *        - new category "Sitherm Pro"; caution: category numbers all move up by one, starting from category "Wärmepumpe" (from 20 to 21) onwards.
 *        - graph display of logging data now comes with crosshair and shows detailed values as tooltip
 *        - improved SD-card output by factor 3 (from 16 to 45 kbps), switching SD-card library from from SD.h to SdFat.h (https://github.com/greiman/SdFat) brings another 10% performance boost
 *        - adjusted paths and directory layout of SdFat to enable compiling from sketch directory.
 *        - new data type vt_sint for signed int data, currently only used in some Sitherm Pro parameters
 *       version 0.34
 *        - Log data can now be displayed as graph
 *        - Webinterface can now display and set vt_bit type parameters in human-readable form
 *        - added KonfigRGx descriptions; caution: various sources used, no guarantee that descriptions match your individual heating system!
 *        - vt_bit is generally read-only in the webinterface. To set, use URL command /S with decimal representation of value
 *        - fixed a bug with vt_seconds_short5, affecting parameters 9500 and 9540.
 *        - fixed bug regarding Fujitsu's device family (from 127 to 170)
 *        - moved libraries from folder libraries to src so they can be included without copying them to the Arduino libraries folder
 *        - modified DallasTemperature.h's include path for OneWire.h
 *       version 0.33
 *        - no more heating system definements anymore due to new autodetect function based on device family (parameter 6225), or set device_id variable to parameter value directly
 *        - two more security options: TRUSTED_IP to limit access to one IP address only, and HTTP authentication with username and password
 *        - Average values are saved on SD-card if present and LOGGER definement is activated
 *        - deactivate logging by setting /L0=0 - this way you can enable LOGGER definement without filling up SD card but still save average values
 *        - new error codes for THISION
 *        - added dump of data payload on website for commands of unknown type, greyed out unsopported parameters
 *        - enable logging of telegrams (log parameter 30000) also in monitor mode (bsb.cpp and bsb.h updated)
 *        - time from heating system is now retreived periodically from broadcast telegrams, further reducing bus activity
 *        - new data type vt_bit for parameters that set individual bits. Display as binary digits, setting still using decimal representation
 *        - new data type vt_temp_short5_us for unsigned one byte temperatures divided by 2 (so far only 887 Vorlaufsoll NormAussentemp)
 *        - new data type vt_percent5 for unsigned one byte temperatures divided by 2 (so far only 885 Pumpe-PWM Minimum)
 *        - new data type vt_seconds_word5 for two byte seconds divided by 2 (so far only 2232, 9500 and 9540)
 *        - new data type vt_seconds_short4 for (signed?) one byte seconds divided by 4 (so far only 2235)
 *        - new data type vt_seconds_short5 for (signed?) one byte seconds divided by 5 (so far only 9500, 9540)
 *        - new data type vt_speed2 for two byte rpm (so far only 7050)
 *        - cleaned up set() function from apparent duplicate cases
 *        - added cases for vt_temp_word, vt_seconds_word5, vt_temp_short, vt_temp_short5, vt_seconds_short4 to set() function
 *       version 0.32
 *        - lots of new parameters suppoerted
 *        - newly designed webinterface allows control over heating system without any additional software or cryptic URL commands. URL commands of course are still available, so no need to change anything when using FHEM etc.
 *        - German webinterface available with definement LANG_DE
 *        - new URL-command /LB=x to log only broadcast messages (x=1) or all bus messages (x=0)
 *        - new URL-command /X to reset the Arduino (need to enable RESET definement in BSB_lan_config.h)
 *        - new logging parameters 20002 and 20003 for hot water loading times and cycles
 *        - moved DS18B20 logging parameters from 20010-20019 to 20200-20299 and DHT22 logging parameters from 20020-20029 to 20100 to 20199
 *        - moved average logging parameter from 20002 to 20004
 *        - set numerous parameters to read-only because that's what they obviously are (K33-36)
 *        - various bugfixes
 *       version 0.31
 *        - increased dumping of logfile by factor 5 / as long as we still have memory left, you can increase logbuflen from 100 to 1000 to increase transfer speed from approx. 16 to 18 kB/s
 *        - adjusted burner activity monitoring based on broadcast messages for Brötje systems
 *        - removed definement PROGNR_5895 because so far, it has only disabled an ENUM definition.
 *        - removed definement PROGNR_6030 because double command ID could be resolved via BROETJE / non-BROETJE definements
 *        - renamed BROETJE_SOB to BROETJE in order to allow for fine-grained distinction between different BROETJE cases (e.g. 6800ff)
 *          This means you have to activate TWO definements when using a Brötje system now: The general BROETJE as well as BROETJE_SOB or BROETJE_BSW.
 *          Have a look at your serial log for parameters 6800 to see which command IDs fit your system and activate one of both accordingly.
 *        - changed 16-Bit addressing of flash memory to 32-Bit to address crashes due to ever growing PROGMEM tables - now we have lots of air to breathe again for new command IDs :)
 *        - removed trailing \0 string from several ENUMs that led to wrong ENUM listings. Please keep in mind not to end ENUMs with a trailing \0 !
 *       version 0.30
 *        - Time library by Paul Stoffregen (https://github.com/PaulStoffregen/Time) is now required and included in the library folder.
 *        - adds logging of raw telegram data to SD card with logging parameter 30000. Logging telegram data is affected by commands /V and /LU
 *        - adds command /LU=x to log only known (x=0) or unknown (x=1) command IDs when logging telegram data
 *        - removed define USE_BROADCAST, broadcast data is now always processed
 *        - new internal functions GetDateTime, TranslateAddr, TranslateType
 *       version 0.29
 *        - adds command /C to display current configuration
 *        - adds command /L to configure logging interval and parameters
 *        - adds option for command /A to set 24h average parameters during runtime
 *        - adds special parameter 20002 for logging /A command (24h averages, only makes sense for long logging intervals)
 *        - bugfixes for logging DS18B20 sensors
 *       version 0.28
 *        - adds special parameters 20000++ for SD card logging of /B, /T and /H commands (see BSB_lan_config.h for examples)
 *        - adds version info to BSB_LAN web interface
 *       version 0.27
 *        - adds date field to log file (requires exact time to be sent by heating system)
 *        - /D0 recreates datalog.txt file with table header
 *        - added "flags" field to command table structure. Currently, only FL_RONLY is supported to make a parameter read-only
 *        - added DEFAULT_FLAG in config. Defaults to NULL, i.e. all fields are read/writeable. 
 *          Setting it to FL_RONLY makes all parameters read-only, e.g. for added level of security. 
 *          Individual parameters can be set to NULL/FL_RONLY to make only these parameters writable/read-only.
 *       version 0.26
 *        - added functionality for logging on micro SD card, using the slot of the w5100 Ethernet shield
 *        - more parameters added (e.g. 8009)
 *       version 0.25
 *        - more FUJITSU parameters added
 *       version 0.24
 *        - updated README with added functions
 *        - added German translations of FAQ and README, courtesy of Ulf Dieckmann
 *       version 0.23
 *        - minor bugfix
 *       version 0.22
 *        - more FUJITSU parameters
 *        - (hopefully) correct implementation of VT_VOLTAGE readings
 *        - minor bugfixes
 *       version 0.21
 *        - added numerous parameters for Fujitsu Wärmepumpe, including new #define FUJITSU directive to activate these parameters due to different parameter numbers
 *        - minor bugfixes
 *       version 0.20
 *        - added more parameters for Feststoffkessel
 *        - minor bugfixes
 *       version 0.19
 *        - added humidity command "H", currently for DHT22 sensors
 *        - added 24h average command "A", define parameters in BSB_lan_config.h
 *        - removed trailing whitespace from menu strings
 *        - fixed command id 0x053D04A2 for THISION heaters
 *        - included Rob Tillaart's DHT library because there are various libraries implementing the protocol and this one is used in the code for its ability to address multiple sensors with one object.
 *        - removed /temp URL parameter as it is a duplicate of /T
 *        - included loop to display DHT22 sensors in IPWE
 *        - making compiling IPWE extensions optional (#define IPWE)
 *       version 0.18
 *        - split off configuration into bsb_lan_config.h
 *        - split off command definitions into bsb_lan_defs.h
 *        - changed GPIO return values from LOW/HIGH to 1/0
 *        - reactivated and updated IPWE (define parameters in config)
 *        - check for protected pins when accessing GPIO (define in config)
 *        - added schematics and PCB files to new subfolder "schematics"
 *       version 0.17a
 *        - minor errors corrected
 *       version 0.17
 *	      - merged v0.16 with FHEM user miwi's changes 
 *       version 0.16
 *        - removed IPWE and EthRly interface
 *        - added GPIO interface
 *        - merged parameters from J.Weber
 *        - resolved duplicate command IDs
 *       version 0.15a
 *        - collated the commands from a Python project and this project,
 *          merged the two versions, corrected obvious errors.
 *          Inserted hypothetical numerical values in ENUM definitions
 *          where Broetje manuals documented only the message texts.
 *        - added information from traces in a Broetje installation with
 *          an ISR-SSR controller and a WOB 25C oil furnace.
 *       version 0.15
 *        - added Solar and Pufferspeicher from Elco Logon B & Logon B MM
 *       version 0.14
 *        - minor bugfixes for Broetje SOB
 *        - extended broadcast handling (experimental)
 *       version 0.13
 *        - change resistor value in receiving path from 4k7 to 1k5
 *        - added values 0x0f and 0x10 to Enum8005
 *        - fixed strings for Zeitprogramme
 *        - added timeout for sending a message (1 second)
 *        - added option T for querying one wire temperature sensors in mixed querys
 *        - added special handling for Broetje SOB
 *        - simplified settings
 *       version 0.12
 *        - added ONEWIRE_SENSORS to ipwe
 *        - fixed parameter decoding for ELCO Thision heating system
 *       version 0.11
 *        - fixed parameter decoding for ELCO Thision heating system
 *       version 0.10
 *        - added more parameters for ELCO Thision heating system
 *       version 0.9
 *        - added more parameters for ELCO Thision heating system
 *        - printTelegramm returns value string for further processing
 *       version 0.8
 *        - added parameters for ELCO Thision heating system
 *        - added IPWE extension
 *        - minor bugfixes
 *       version 0.7
 *        - added bus monitor functionality
 *       version 0.6
 *        - renamed SoftwareSerial to BSBSoftwareSerial
 *        - changed folder structure to enable simple build with arduino sdk
 *       version 0.5
 *        - bugfixes
 *        - added documentation (README)
 *        - added passkey feature
 *        - added R feature (query reset value)
 *        - added E feature (list enum values)
 *        - added setter for almost all value types
 *        - fixed indentation
 *        - added V feature to set verbosity for serial output
 *        - set baudrate to 115200 for serial output
 *        - redirecting favicon request
 *        - added some images of the BSB adapter
 *
 */

#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <SPI.h>
#ifndef ETHERNET_W5500
#include <Ethernet.h>
#else
#include <Ethernet2.h>
#endif
#include <Arduino.h>
#include <util/crc16.h>

#include "src/Time/TimeLib.h"
#include "src/BSB/BSBSoftwareSerial.h"
#include "src/BSB/bsb.h"

#include "BSB_lan_config.h"
#include "BSB_lan_defs.h"
#include "src/d3_js.h"

#ifdef TRUSTED_IP
#ifndef ETHERNET_W5500
#include <utility/w5100.h>
#else
#include <utility/w5500.h>
#endif
#endif

uint8_t len_idx, pl_start;
int device_id;

EthernetClient client;

// char _ipstr[INET6_ADDRSTRLEN];    // addr in format xxx.yyy.zzz.aaa
char _ipstr[20];    // addr in format xxx.yyy.zzz.aaa
byte __remoteIP[4] = {0,0,0,0};   // IP address in bin format  

#ifdef LOGGER
//  #include <SD.h>   // if you run into troubles with SdFat.h, just remove the following two lines and uncomment this line.
  #include "src/SdFat/SdFat.h"
  SdFat SD;
  File Logfile;
#endif

#ifdef ONE_WIRE_BUS
  #include "src/OneWire/OneWire.h"
  #include "src/DallasTemperature/DallasTemperature.h"
  #define TEMPERATURE_PRECISION 9
  // Setup a oneWire instance to communicate with any OneWire devices
  OneWire oneWire(ONE_WIRE_BUS);
  // Pass our oneWire reference to Dallas Temperature.
  DallasTemperature sensors(&oneWire);

  int numSensors;
#endif

#ifdef DHT_BUS
  #include "src/DHT/dht.h"
  int DHT_Pins[] = {DHT_BUS};
  dht DHT;
#endif

char date[20];

static unsigned long lastAvgTime = millis();
static unsigned long lastLogTime = millis();
int numAverages = sizeof(avg_parameters) / sizeof(int);
int anz_ex_gpio = sizeof(exclude_GPIO) / sizeof(byte);
int numLogValues = sizeof(log_parameters) / sizeof(int);
double *avgValues = new double[numAverages];
double *avgValues_Old = new double[numAverages];
double *avgValues_Current = new double[numAverages];
int avgCounter = 1;

unsigned long dev_id = 0;

// variables for handling of broadcast messages
int brenner_stufe = 0;
unsigned long brenner_start   = 0;
unsigned long brenner_start_2   = 0;
unsigned long brenner_duration= 0;
unsigned long brenner_duration_2= 0;
unsigned long brenner_count   = 0;
unsigned long brenner_count_2   = 0;
unsigned long TWW_start   = 0;
unsigned long TWW_duration= 0;
unsigned long TWW_count   = 0;

/* ******************************************************************
 *      ************** Program code starts here **************
 * *************************************************************** */
/* buffer to load PROGMEM values in RAM */
#define BUFLEN 3500
char buffer[BUFLEN];

/* buffer to print output lines*/
#define OUTBUF_LEN  300
char outBuf[OUTBUF_LEN];
byte outBufLen=0;

/**  ****************************************************************
 *  Function: outBufclear()
 *  Does:     Sets ouBufLen = 0 and puts the end-of-string character
 *            into the first buffer position.
 *  Pass parameters:
 *   none
 *  Parameters passed back:
 *   none
 *  Function value returned:
 *   none
 * Global resources used:
 *   char outBuf[]
 * *************************************************************** */
void outBufclear(void){
  outBufLen=0;
  outBuf[0]='\0';
}
/** *****************************************************************
 *  Function: dayofweek()
 *  Does:     Accepts a date as day / month / year and calculates
 *            the day of the week for this day.
 *            1=Monday, .. , 7=Sunday
 *            No sanity checks are performed on the pass parameters.
 * Pass parameters:
 *  uint8  day    1 .. 31
 *  uint8  month  1 .. 12
 *  uint16 year
 * Parameters passed back:
 *  none
 * Function value returned:
 *  day-of-week [1-7]
 * Global resources used:
 *  none
 * *************************************************************** */

int dayofweek(uint8_t day, uint8_t month, uint16_t year)
{
   /** Zeller's congruence for the Gregorian calendar. **/
   /** With 1=Monday, ... 5=Saturday, 7=Sunday         **/
   if (month < 3) {
      month += 12;
      year--;
   }
   return (((13*month+3)/5 + day + year + year/4 - year/100 + year/400) % 7) + 1;
}



/** *****************************************************************
 *  Function: findLine()
 *  Does:     Scans the command table struct for a matching line
 *            number (ProgNr) and returns the command code.
 * Pass parameters:
 *  uint16  line     the requested match (ProgNr)
 *  uint16  startidx starting line (ProgNr) for search .
 *                   Works best if i>0 ;-)
 *  uint32 *cmd      pointer to 32-bit command code variable
 * Parameters passed back:
 *  uint16 *cmd      32-bit command code value filled in
 * Function value returned:
 *  -1        command (ProgNr) not found
 *   i >= 0   success, usable to index the matching table row
 * Global resources used:
 *  none
 * *************************************************************** */
int findLine(uint16_t line
           , uint16_t start_idx  //
           , uint32_t *cmd)      // 32-bit command code
{
  int found;
  int i;
  uint32_t c;
  uint16_t l;

  // search for the line in cmdtbl
  i=start_idx;
  found=0;
  do{
    c=pgm_read_dword_far(pgm_get_far_address(cmdtbl[0].cmd) + i * sizeof(cmdtbl[0]));

//    c=pgm_read_dword(&cmdtbl[i].cmd);  // command code
    if(c==CMD_END) break;
    l=pgm_read_word_far(pgm_get_far_address(cmdtbl[0].line) + i * sizeof(cmdtbl[0]));
//    l=pgm_read_word(&cmdtbl[i].line);  // ProgNr
    if(l==line){
      long devices = pgm_read_dword_far(pgm_get_far_address(cmdtbl[0].devices) + i * sizeof(cmdtbl[0]));
      if ((devices & dev_id) == dev_id) {
        found=1;
        break;
      }
    }
    if(l>line){
      break;
    }
    i++;
  }while(1);

  if(!found){
    return -1;
  }
  if(cmd!=NULL) *cmd=c;
  return i;
}


/** *****************************************************************
 *  Function: freeRam()
 *  Does:     Returns the amount of available RAM
 *
 * *************************************************************** */
int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}


/** *****************************************************************
 *  Function:  SerialPrintHex()
 *  Does:      Sends the hex representation of one byte to the PC
 *             hardware serial interface. Adds a leading zero if
 *             it is a one-digit hex value.
 *  Pass parameters:
 *   byte      the value to convert and send
 * Parameters passed back:
 *   none
 * Function value returned:
 *   none
 * Global resources used:
 *    Serial  instance
 * *************************************************************** */
void SerialPrintHex(byte val) {
  if (val < 16) Serial.print(F("0"));  // add a leading zero to single-digit values
  Serial.print(val, HEX);
}

/** *****************************************************************
 *  Function:  SerialPrintHex32()
 *  Does:      Sends the hex representation of a 32-bit value to the
 *             PC hardware serial interface.  Pad with leading zeroes
 *             to get an eight-character hex representation.
 *  Pass parameters:
 *   uint32 val The value to convert and send
 * Parameters passed back:
 *   none
 * Function value returned:
 *   none
 * Global resources used:
 *    Serial  instance
 * *************************************************************** */
void SerialPrintHex32(uint32_t val) {
  if (val <= 0x0fffffff) Serial.print(F("0"));
  if (val <= 0x00ffffff) Serial.print(F("0"));
  if (val <= 0x000fffff) Serial.print(F("0"));
  if (val <= 0x0000ffff) Serial.print(F("0"));
  if (val <= 0x00000fff) Serial.print(F("0"));
  if (val <= 0x000000ff) Serial.print(F("0"));
  if (val <= 0x0000000f) Serial.print(F("0"));
  Serial.print(val, HEX);
}


/** *****************************************************************
 *  Function:  SerialPrintData()
 *  Does:      Sends the payload of a telegram in hex to the PC
 *             hardware serial interface in hex.
 *  Pass parameters:
 *   byte *msg bus telegram with magic byte, header, payload and
 *             checksum
 * Parameters passed back:
 *
 * Function value returned:
 *
 * Global resources used:
 *   Serial  instance
 * *************************************************************** */
void SerialPrintData(byte* msg){
  // Calculate pure data length without housekeeping info
  int data_len;
  if (bus_type == 1) {
    data_len=msg[len_idx]-14;     // get packet length, then subtract
  } else {
    data_len=msg[len_idx]-11;     // get packet length, then subtract
  }
  // Start indexing where the payload begins
  for(int i=0;i<data_len;i++){
    SerialPrintHex(msg[pl_start+i]);
    Serial.print(F(" "));
  }
}

/** *****************************************************************
 *  Function:  SerialPrintRAW()
 *  Does:      Sends the telegram content in hex to the PC hardware
 *             serial interface, starting at position null. It stops
 *             when it has sent the requested number of message bytes.
 *             WARNING: this routine does not perform a sanity check
 *             of the length pass parameter.
 *  Pass parameters:
 *   byte *msg pointer to the telegram buffer
 *   byte len  the requested number of bytes to send.
 * Parameters passed back:
 *   none
 * Function value returned:
 *   none
 * Global resources used:
 *   Serial  instance
 * *************************************************************** */
void SerialPrintRAW(byte* msg, byte len){
  for(int i=0;i<len;i++){
    SerialPrintHex(msg[i]);
    Serial.print(F(" "));
  }
}

/** *****************************************************************
 *  Function:  TranslateAddr()
 *  Does:      Returns human-readable device names
 *             selected by a device address.
 *             If the device number is not in the list of known
 *             addresses, return the number itself.
 *
 * Pass parameters:
 *   byte addr the Int8 address value for which we seek the device
 *             name; values GT 127 are mapped to 0 <= addr <= 127
 * Parameters passed back:
 *   none
 * Function value returned:
 *   device string
 * Global resources used:
 *   none
 * *************************************************************** */
char *TranslateAddr(byte addr, char *device){
  switch(addr&0x7F){
    case ADDR_HEIZ: strncpy(device, "HEIZ", 4); break;
    case ADDR_EM1: strncpy(device, "EM1", 4); break;
    case ADDR_EM2: strncpy(device, "EM2", 4); break;
    case ADDR_RGT1: strncpy(device, "RGT1", 4); break;
    case ADDR_RGT2: strncpy(device, "RGT2", 4); break;
    case ADDR_CNTR: strncpy(device, "CNTR", 4); break;
    case ADDR_DISP: strncpy(device, "DISP", 4); break;
    case ADDR_SRVC: strncpy(device, "SRVC", 4); break;
    case ADDR_ALL: strncpy(device, "ALL ", 4); break;
    default: sprintf(device, "%02X", addr); break;
  }
  device[4] = 0;
  return device;
}

/** *****************************************************************
 *  Function:  SerialPrintAddr()
 *  Does:      Sends human-readable device names to the PC hardware
 *             serial interface, selected by a device address.
 *             If the device number is not in the list of known
 *             addresses, send the number itself.
 *
 * Pass parameters:
 *   byte addr the Int8 address value for which we seek the device
 *             name; values GT 127 are mapped to 0 <= addr <= 127
 * Parameters passed back:
 *   none
 * Function value returned:
 *   none
 * Global resources used:
 *   Serial  instance
 * *************************************************************** */
void SerialPrintAddr(byte addr){
  switch(addr&0x7F){
    case ADDR_HEIZ: Serial.print(F("HEIZ")); break;
    case ADDR_EM1: Serial.print(F("EM1")); break;
    case ADDR_EM2: Serial.print(F("EM2")); break;
    case ADDR_RGT1: Serial.print(F("RGT1")); break;
    case ADDR_RGT2: Serial.print(F("RGT2")); break;
    case ADDR_CNTR: Serial.print(F("CNTR")); break;
    case ADDR_SRVC: Serial.print(F("SRVC")); break;
    case ADDR_DISP: Serial.print(F("DISP")); break;
    case ADDR_ALL: Serial.print(F("ALL ")); break;
    default: SerialPrintHex(addr); break;
  }
}

/** *****************************************************************
 *  Function:  TranslateAddr()
 *  Does:      Returns the message type in human-readable form.
 *  Pass parameters:
 *   byte type a number which indicates the type
 * Parameters passed back:
 *   none
 * Function value returned:
 *   message type
 * Global resources used:
 *   none
 * *************************************************************** */
char *TranslateType(byte type, char *mtype){
  switch(type){
    case TYPE_QINF: strncpy(mtype, "QINF", 4); break;
    case TYPE_INF: strncpy(mtype, "INF", 4); break;
    case TYPE_SET: strncpy(mtype, "SET", 4); break;
    case TYPE_ACK: strncpy(mtype, "ACK", 4); break;
    case TYPE_NACK: strncpy(mtype, "NACK", 4); break;
    case TYPE_QUR: strncpy(mtype, "QUR", 4); break;
    case TYPE_ANS: strncpy(mtype, "ANS", 4); break;
    case TYPE_QRV: strncpy(mtype, "QRV", 4); break;
    case TYPE_ARV: strncpy(mtype, "ARV", 4); break;
    case TYPE_ERR: strncpy(mtype, "ERR", 4); break;
    // If no match found: print the hex value
    default: sprintf(mtype, "%02X", type); break;
  } // endswitch
  mtype[4] = 0;
  return mtype;
}

/** *****************************************************************
 *  Function:  SerialPrintType()
 *  Does:      Prints the message type in human-readable form.
 *  Pass parameters:
 *   byte type a number which indicates the type
 * Parameters passed back:
 *   none
 * Function value returned:
 *   none
 * Global resources used:
 *   Serial    the hardware serial interface to a PC
 * *************************************************************** */
void SerialPrintType(byte type){
  switch(type){
    case TYPE_QINF: Serial.print(F("QINF")); break;
    case TYPE_INF: Serial.print(F("INF")); break;
    case TYPE_SET: Serial.print(F("SET")); break;
    case TYPE_ACK: Serial.print(F("ACK")); break;
    case TYPE_NACK: Serial.print(F("NACK")); break;
    case TYPE_QUR: Serial.print(F("QUR")); break;
    case TYPE_ANS: Serial.print(F("ANS")); break;
    case TYPE_QRV: Serial.print(F("QRV")); break;
    case TYPE_ARV: Serial.print(F("ARV")); break;
    case TYPE_ERR:
    Serial.print(F("ERR"));
    //outBufLen+=sprintf(outBuf+outBufLen,"ERR");
    break;
    // If no match found: print the hex value
    default: SerialPrintHex(type); break;
  } // endswitch
} // --- SerialPrintType() ---

/** *****************************************************************
 *  Function:
 *  Does:
 *  Pass parameters:
 *
 * Parameters passed back:
 *
 * Function value returned:
 *
 * Global resources used:
 *
 * *************************************************************** */
void printLineNumber(uint32_t val) {
    char *p=outBuf+outBufLen;
    outBufLen+=sprintf(outBuf+outBufLen,"%4ld",val);
    Serial.print(p);
}

/** *****************************************************************
 *  Function:
 *  Does:
 *  Pass parameters:
 *
 * Parameters passed back:
 *
 * Function value returned:
 *
 * Global resources used:
 *
 * *************************************************************** */
void printBIT(byte *msg,byte data_len){
  char *p=outBuf+outBufLen;
  if(data_len == 2){
    if(msg[pl_start]==0){
      for (int i=7;i>=0;i--) {
        outBufLen+=sprintf(outBuf+outBufLen,"%d",msg[pl_start+1] >> i & 1);
      }
    } else {
      outBufLen+=sprintf(outBuf+outBufLen,"---");
    }
    Serial.print(p);
  }else{
    Serial.print(F("BYTE len error len!=2: "));
    SerialPrintData(msg);
    outBufLen+=sprintf(outBuf+outBufLen,"decoding error");
  }
}

/** *****************************************************************
 *  Function:
 *  Does:
 *  Pass parameters:
 *
 * Parameters passed back:
 *
 * Function value returned:
 *
 * Global resources used:
 *
 * *************************************************************** */
void printBYTE(byte *msg,byte data_len,const char *postfix){
  char *p=outBuf+outBufLen;
  if(data_len == 2){
    if(msg[pl_start]==0){
      outBufLen+=sprintf(outBuf+outBufLen,"%d",msg[pl_start+1]);
    } else {
      outBufLen+=sprintf(outBuf+outBufLen,"---");
    }
    if(postfix!=NULL){
      outBufLen+=sprintf(outBuf+outBufLen," %s",postfix);
    }
    Serial.print(p);
  }else{
    Serial.print(F("BYTE len error len!=2: "));
    SerialPrintData(msg);
    outBufLen+=sprintf(outBuf+outBufLen,"decoding error");
  }
}

/** *****************************************************************
 *  Function:
 *  Does:
 *  Pass parameters:
 *
 * Parameters passed back:
 *
 * Function value returned:
 *
 * Global resources used:
 *
 * *************************************************************** */
void printWORD(byte *msg,byte data_len, long multiplier, const char *postfix){
  long lval;
  char *p=outBuf+outBufLen;

  if(data_len == 3){
    if(msg[pl_start]==0){
      lval=((long(msg[pl_start+1])<<8)+long(msg[pl_start+2])) * multiplier;
      outBufLen+=sprintf(outBuf+outBufLen,"%ld",lval);
    } else {
      outBufLen+=sprintf(outBuf+outBufLen,"---");
    }
    if(postfix!=NULL){
      outBufLen+=sprintf(outBuf+outBufLen," %s",postfix);
    }
    Serial.print(p);
  }else{
    Serial.print(F("WORD len error len!=3: "));
    SerialPrintData(msg);
    outBufLen+=sprintf(outBuf+outBufLen,"decoding error");
  }
}

/** *****************************************************************
 *  Function:
 *  Does:
 *  Pass parameters:
 *
 * Parameters passed back:
 *
 * Function value returned:
 *
 * Global resources used:
 *
 * *************************************************************** */
void printSINT(byte *msg,byte data_len, long multiplier, const char *postfix){
  int16_t lval;
  char *p=outBuf+outBufLen;

  if(data_len == 3){
    if(msg[pl_start]==0){
      lval=(((int16_t)(msg[pl_start+1])<<8) + (int16_t)(msg[pl_start+2])) * multiplier;
      outBufLen+=sprintf(outBuf+outBufLen,"%d",lval);
    } else {
      outBufLen+=sprintf(outBuf+outBufLen,"---");
    }
    if(postfix!=NULL){
      outBufLen+=sprintf(outBuf+outBufLen," %s",postfix);
    }
    Serial.print(p);
  }else{
    Serial.print(F("WORD len error len!=3: "));
    SerialPrintData(msg);
    outBufLen+=sprintf(outBuf+outBufLen,"decoding error");
  }
}

/** *****************************************************************
 *  Function:
 *  Does:
 *  Pass parameters:
 *
 * Parameters passed back:
 *
 * Function value returned:
 *
 * Global resources used:
 *
 * *************************************************************** */
void printDWORD(byte *msg,byte data_len,long divider, const char *postfix){
  long lval;
  char *p=outBuf+outBufLen;

  if(data_len == 5){
    if(msg[pl_start]==0){
      lval=((long(msg[pl_start+1])<<24)+(long(msg[pl_start+2])<<16)+(long(msg[pl_start+3])<<8)+long(msg[pl_start+4]))/divider;
      outBufLen+=sprintf(outBuf+outBufLen,"%ld",lval);
    } else {
      outBufLen+=sprintf(outBuf+outBufLen,"---");
    }
    if(postfix!=NULL){
      outBufLen+=sprintf(outBuf+outBufLen," %s",postfix);
    }
    Serial.print(p);
  }else{
    Serial.print(F("DWORD len error len!=5: "));
    SerialPrintData(msg);
    outBufLen+=sprintf(outBuf+outBufLen,"decoding error");
  }
}

/** *****************************************************************
 *  Function:
 *  Does:
 *  Pass parameters:
 *
 * Parameters passed back:
 *
 * Function value returned:
 *
 * Global resources used:
 *
 * *************************************************************** */
void _printFIXPOINT(double dval, int precision){
  int a,b,i;
  if(dval<0){
    outBufLen+=sprintf(outBuf+outBufLen,"-");
    dval*=-1.0;
  }
  double rval=10.0;
  for(i=0;i<precision;i++) rval*=10.0;
  dval+=5.0/rval;
  a=(int)(dval);
  rval/=10.0;
  b=(int)(dval*rval - a*rval);
  if(precision==0){
    outBufLen+=sprintf(outBuf+outBufLen,"%d",a);
  }else{
    char formatstr[8]="%d.%01d";
    formatstr[5]='0'+precision;
    outBufLen+=sprintf(outBuf+outBufLen,formatstr,a,b);
  }
}

/** *****************************************************************
 *  Function:
 *  Does:
 *  Pass parameters:
 *
 * Parameters passed back:
 *
 * Function value returned:
 *
 * Global resources used:
 *
 * *************************************************************** */
void printFIXPOINT(byte *msg,byte data_len,double divider,int precision,const char *postfix){
  double dval;
  char *p=outBuf+outBufLen;

  if(data_len == 3){
    if(msg[pl_start]==0){
      dval=double((msg[pl_start+1] << 8) + msg[pl_start+2]) / divider;
      _printFIXPOINT(dval,precision);
    } else {
      outBufLen+=sprintf(outBuf+outBufLen,"---");
    }
    if(postfix!=NULL){
      outBufLen+=sprintf(outBuf+outBufLen," %s",postfix);
    }
    Serial.print(p);
  }else{
    Serial.print(F("FIXPOINT len !=3: "));
    SerialPrintData(msg);
    outBufLen+=sprintf(outBuf+outBufLen,"decoding error");
  }
}

/** *****************************************************************
 *  Function:
 *  Does:
 *  Pass parameters:
 *
 * Parameters passed back:
 *
 * Function value returned:
 *
 * Global resources used:
 *
 * *************************************************************** */
void printFIXPOINT_DWORD(byte *msg,byte data_len,double divider,int precision,const char *postfix){
  double dval;
  char *p=outBuf+outBufLen;

  if(data_len == 5){
    if(msg[pl_start]==0){
      dval=double((long(msg[pl_start+1])<<24)+(long(msg[pl_start+2])<<16)+(long(msg[pl_start+3])<<8)+long(msg[pl_start+4])) / divider;
      _printFIXPOINT(dval,precision);
    } else {
      outBufLen+=sprintf(outBuf+outBufLen,"---");
    }
    if(postfix!=NULL){
      outBufLen+=sprintf(outBuf+outBufLen," %s",postfix);
    }
    Serial.print(p);
  }else{
    Serial.print(F("FIXPOINT_DWORD len !=5: "));
    SerialPrintData(msg);
    outBufLen+=sprintf(outBuf+outBufLen,"decoding error");
  }
}

/** *****************************************************************
 *  Function:
 *  Does:
 *  Pass parameters:
 *
 * Parameters passed back:
 *
 * Function value returned:
 *
 * Global resources used:
 *
 * *************************************************************** */
void printFIXPOINT_BYTE(byte *msg,byte data_len,double divider,int precision,const char *postfix){
  double dval;
  char *p=outBuf+outBufLen;

  if(data_len == 2){
    if(msg[pl_start]==0){
      dval=double((signed char)msg[pl_start+1]) / divider;
      _printFIXPOINT(dval,precision);
    } else {
      outBufLen+=sprintf(outBuf+outBufLen,"---");
    }
    if(postfix!=NULL){
      outBufLen+=sprintf(outBuf+outBufLen," %s",postfix);
    }
    Serial.print(p);
  }else{
    Serial.print(F("FIXPOINT_BYTE len !=2: "));
    SerialPrintData(msg);
    outBufLen+=sprintf(outBuf+outBufLen,"decoding error");
  }
}

/** *****************************************************************
 *  Function:
 *  Does:
 *  Pass parameters:
 *
 * Parameters passed back:
 *
 * Function value returned:
 *
 * Global resources used:
 *
 * *************************************************************** */
void printFIXPOINT_BYTE_US(byte *msg,byte data_len,double divider,int precision,const char *postfix){
  double dval;
  char *p=outBuf+outBufLen;

  if(data_len == 2){
    if(msg[pl_start]==0){
      dval=double(msg[pl_start+1]) / divider;
      _printFIXPOINT(dval,precision);
    } else {
      outBufLen+=sprintf(outBuf+outBufLen,"---");
    }
    if(postfix!=NULL){
      outBufLen+=sprintf(outBuf+outBufLen," %s",postfix);
    }
    Serial.print(p);
  }else{
    Serial.print(F("FIXPOINT_BYTE len !=2: "));
    SerialPrintData(msg);
    outBufLen+=sprintf(outBuf+outBufLen,"decoding error");
  }
}

/** *****************************************************************
 *  Function:
 *  Does:
 *  Pass parameters:
 *
 * Parameters passed back:
 *
 * Function value returned:
 *
 * Global resources used:
 *
 * *************************************************************** */
void printCHOICE(byte *msg,byte data_len,const char *val0,const char *val1){
  char *p=outBuf+outBufLen;

  if(data_len == 2){
    if(msg[pl_start]==0){
      if(msg[pl_start+1]==0){
        outBufLen+=sprintf(outBuf+outBufLen,"%d - %s",msg[pl_start+1],val0);
      }else{
        outBufLen+=sprintf(outBuf+outBufLen,"%d - %s",msg[pl_start+1],val1);
      }
    }else{
      outBufLen+=sprintf(outBuf+outBufLen,"---");
    }
    Serial.print(p);
  }else{
    Serial.print(F("CHOICE len !=2: "));
    SerialPrintData(msg);
    outBufLen+=sprintf(outBuf+outBufLen,"decoding error");
  }
}

/** *****************************************************************
 *  Function:
 *  Does:
 *  Pass parameters:
 *
 * Parameters passed back:
 *
 * Function value returned:
 *
 * Global resources used:
 *
 * *************************************************************** */
void printENUM(char* enumstr,uint16_t enumstr_len,uint16_t search_val, int print_val){
  uint16_t val;
  char *p=outBuf+outBufLen;

  if(enumstr!=NULL){
    uint16_t c=0;
    while(c<enumstr_len){
      if((byte)enumstr[c+1]!=' '){
        val=uint16_t(((uint8_t*)enumstr)[c]) << 8 | uint16_t(((uint8_t*)enumstr)[c+1]);
        c++;
      }else{
        val=uint16_t(((uint8_t*)enumstr)[c]);
      }
      //skip leading space
      c+=2;
      if(val==search_val){
       // enum value found
       break;
      }
      while(enumstr[c]!=0) c++;
      c++;
    }
    if(c<enumstr_len){
      if(print_val){
        outBufLen+=sprintf(outBuf+outBufLen,"%d - %s",val,&enumstr[c]);
      }else{
        outBufLen+=sprintf(outBuf+outBufLen,"%s",&enumstr[c]);
      }
    }else{
      outBufLen+=sprintf(outBuf+outBufLen,"%d - not found",search_val);
    }
    Serial.print(p);
  }
}

/** *****************************************************************
 *  Function:
 *  Does:
 *  Pass parameters:
 *
 * Parameters passed back:
 *
 * Function value returned:
 *
 * Global resources used:
 *
 * *************************************************************** */
void printDateTime(byte *msg,byte data_len){
  char *p=outBuf+outBufLen;

  if(data_len == 9){
    if(msg[pl_start]==0){
      outBufLen+=sprintf(outBuf+outBufLen,"%02d.%02d.%d %02d:%02d:%02d",msg[pl_start+3],msg[pl_start+2],msg[pl_start+1]+1900,msg[pl_start+5],msg[pl_start+6],msg[pl_start+7]);
    } else {
      outBufLen+=sprintf(outBuf+outBufLen,"---");
    }
    Serial.print(p);
  }else{
    Serial.print(F(" VT_DATETIME len !=9: "));
    SerialPrintData(msg);
    outBufLen+=sprintf(outBuf+outBufLen,"decoding error");
  }
}

/** *****************************************************************
 *  Function:
 *  Does:
 *  Pass parameters:
 *
 * Parameters passed back:
 *
 * Function value returned:
 *
 * Global resources used:
 *
 * *************************************************************** */
void printDate(byte *msg,byte data_len){
  char *p=outBuf+outBufLen;

  if(data_len == 9){
    if(msg[pl_start]==0){
      outBufLen+=sprintf(outBuf+outBufLen,"%02d.%02d",msg[pl_start+3],msg[pl_start+2]);
    } else {
      outBufLen+=sprintf(outBuf+outBufLen,"---");
    }
    Serial.print(p);
  }else{
    Serial.print(F(" VT_DATE len !=9: "));
    SerialPrintData(msg);
    outBufLen+=sprintf(outBuf+outBufLen,"decoding error");
  }
}

/** *****************************************************************
 *  Function:
 *  Does:
 *  Pass parameters:
 *
 * Parameters passed back:
 *
 * Function value returned:
 *
 * Global resources used:
 *
 * *************************************************************** */
void printTimeProg(byte *msg,byte data_len){
  char *p=outBuf+outBufLen;

  if(data_len == 12){
    outBufLen+=sprintf(outBuf+outBufLen,"1. ");
    if(msg[pl_start]<24){
      outBufLen+=sprintf(outBuf+outBufLen,"%02d:%02d - %02d:%02d",msg[pl_start],msg[pl_start+1],msg[pl_start+2],msg[pl_start+3]);
    }else{
      outBufLen+=sprintf(outBuf+outBufLen,"--:-- - --:--");
    }
    outBufLen+=sprintf(outBuf+outBufLen," 2. ");
    if(msg[pl_start+4]<24){
      outBufLen+=sprintf(outBuf+outBufLen,"%02d:%02d - %02d:%02d",msg[pl_start+4],msg[pl_start+5],msg[pl_start+6],msg[pl_start+7]);
    }else{
      outBufLen+=sprintf(outBuf+outBufLen,"--:-- - --:--");
    }
    outBufLen+=sprintf(outBuf+outBufLen," 3. ");
    if(msg[pl_start+8]<24){
      outBufLen+=sprintf(outBuf+outBufLen,"%02d:%02d - %02d:%02d",msg[pl_start+8],msg[pl_start+9],msg[pl_start+10],msg[pl_start+11]);
    }else{
      outBufLen+=sprintf(outBuf+outBufLen,"--:-- - --:--");
    }
    Serial.print(p);
  }else{
    Serial.print(F(" VT_TIMEPROG len !=12: "));
    SerialPrintData(msg);
    outBufLen+=sprintf(outBuf+outBufLen,"decoding error");
  }
}

/** *****************************************************************
 *  Function:
 *  Does:
 *  Pass parameters:
 *
 * Parameters passed back:
 *
 * Function value returned:
 *
 * Global resources used:
 *
 * *************************************************************** */
void printTime(byte *msg,byte data_len){
  char *p=outBuf+outBufLen;

  if(data_len == 3){
    if(msg[pl_start]==0){
      outBufLen+=sprintf(outBuf+outBufLen,"%02d:%02d",msg[pl_start+1],msg[pl_start+2]);
    } else {
      outBufLen+=sprintf(outBuf+outBufLen,"--:--");
    }
    Serial.print(p);
  }else{
    Serial.print(F("VT_HOUR_MINUTES len !=3: "));
    SerialPrintData(msg);
    outBufLen+=sprintf(outBuf+outBufLen,"decoding error");
  }
}

/** *****************************************************************
 *  Function:
 *  Does:
 *  Pass parameters:
 *   byte * msg
 *   byte len
 *  Parameters passed back:
 *   byte *msg    unchanged
 *  Function value returned:
 *   none
 *  Global resources used:
 *    outBuf
 * *************************************************************** */
void printLPBAddr(byte *msg,byte data_len){
  char *p=outBuf+outBufLen;

  if(data_len == 2){
    if(msg[pl_start]==0){   // payload Int8 value
    outBufLen+=sprintf(outBuf+outBufLen,"%02d.%02d",msg[pl_start+1]>>4,(msg[pl_start+1] & 0x0f)+1);
  }else{
    outBufLen+=sprintf(outBuf+outBufLen,"---");
  }
  Serial.print(p);
  }else{
    Serial.print(F(" VT_LPBADDR len !=2: "));
    SerialPrintData(msg);
    outBufLen+=sprintf(outBuf+outBufLen,"decoding error");
  }
}

/** *****************************************************************
 *  Function:  printTelegram()
 *  Does:      Send the decoded telegram content to the hardware
 *             serial interface.
 *  Pass parameters:
 *   char* msg    the BS bus telegram
 *  Parameters passed back:
 *   char * pvalstr
 *  Function value returned:
 *   char*
 *  Global resources used:
 *   Serial    hardware serial interface to a PC
 *   outBuf[]
 * *************************************************************** */
char *printTelegram(byte* msg) {
  char gradC[]="&deg;C";      // special non-ASCII characters
  char perc[]="&#037;";
  char *pvalstr=NULL;

  outBufclear();

/*
#if !DEBUG
  // suppress DE telegrams
  if(msg[0]==0xDE) return;

  // suppress QUERY telegrams
  if(msg[4]==TYPE_QUR) return;
#endif
*/
  // source
  SerialPrintAddr(msg[1+(bus_type*2)]); // source address
  Serial.print(F("->"));
  SerialPrintAddr(msg[2]); // destination address
  Serial.print(F(" "));
  // msg[3] contains the message length, not handled here
  SerialPrintType(msg[4+(bus_type*4)]); // message type, human readable
  Serial.print(F(" "));

  uint32_t cmd;
  if (bus_type == 1) {
    if(msg[8]==TYPE_QUR || msg[8]==TYPE_SET){ //QUERY and SET: byte 5 and 6 are in reversed order
      cmd=(uint32_t)msg[10]<<24 | (uint32_t)msg[9]<<16 | (uint32_t)msg[11] << 8 | (uint32_t)msg[12];
    }else{
      cmd=(uint32_t)msg[9]<<24 | (uint32_t)msg[10]<<16 | (uint32_t)msg[11] << 8 | (uint32_t)msg[12];
    }
  } else {
    if(msg[4]==TYPE_QUR || msg[4]==TYPE_SET){ //QUERY and SET: byte 5 and 6 are in reversed order
      cmd=(uint32_t)msg[6]<<24 | (uint32_t)msg[5]<<16 | (uint32_t)msg[7] << 8 | (uint32_t)msg[8];
    }else{
      cmd=(uint32_t)msg[5]<<24 | (uint32_t)msg[6]<<16 | (uint32_t)msg[7] << 8 | (uint32_t)msg[8];
    }
  }
  // search for the command code in cmdtbl
  int i=0;        // begin with line 0
  boolean known=0;
  uint32_t c;     // command code
  c=pgm_read_dword_far(pgm_get_far_address(cmdtbl[0].cmd) + i * sizeof(cmdtbl[0]));
//  c=pgm_read_dword(&cmdtbl[i].cmd);    // extract the command code from line i
  while(c!=CMD_END){
    if(c == cmd){
      long devices = pgm_read_dword_far(pgm_get_far_address(cmdtbl[0].devices) + i * sizeof(cmdtbl[0]));
      if ((devices & dev_id) == dev_id) {
        known=1;
        break;
      }
    }
    i++;
    c=pgm_read_dword_far(pgm_get_far_address(cmdtbl[0].cmd) + i * sizeof(cmdtbl[0]));
//    c=pgm_read_dword(&cmdtbl[i].cmd);
  }
  if(!known){                          // no hex code match
    // Entry in command table is "UNKNOWN" (0x00000000)
    Serial.print(F("     "));
    SerialPrintHex32(cmd);             // print what we have got
    Serial.print(F(" "));
  }else{
    // Entry in command table is a documented command code
    uint16_t line=pgm_read_word_far(pgm_get_far_address(cmdtbl[0].line) + i * sizeof(cmdtbl[0]));
//    uint16_t line=pgm_read_word(&cmdtbl[i].line);
    printLineNumber(line);             // the ProgNr
    Serial.print(F(" "));
    outBufLen+=sprintf(outBuf+outBufLen," ");

    // print category
    int cat=pgm_read_byte_far(pgm_get_far_address(cmdtbl[0].category) + i * sizeof(cmdtbl[0]));
//    int cat=pgm_read_byte(&cmdtbl[i].category);
    int len=sizeof(ENUM_CAT);
    memcpy_PF(buffer, pgm_get_far_address(ENUM_CAT), len);
//    memcpy_P(buffer, &ENUM_CAT,len);
    buffer[len]=0;
    printENUM(buffer,len,cat,0);
    Serial.print(F(" - "));
    outBufLen+=sprintf(outBuf+outBufLen," - ");

    // print menue text
    strcpy_PF(buffer, pgm_read_word_far(pgm_get_far_address(cmdtbl[0].desc) + i * sizeof(cmdtbl[0])));
//    strcpy_P(buffer, (char*)pgm_read_word(&(cmdtbl[i].desc)));
    char *p=outBuf+outBufLen;
    outBufLen+=sprintf(outBuf+outBufLen," %s: ",buffer);
    Serial.print(p);
  }
  // decode parameter
  int data_len;
  if (bus_type == 1) {
    data_len=msg[len_idx]-14;     // get packet length, then subtract
  } else {
    data_len=msg[len_idx]-11;     // get packet length, then subtract
  }
  if(data_len < 0){
    Serial.print(F("len ERROR "));
    Serial.print(msg[len_idx]);
  }else{
    if(data_len > 0){
      if(known){
        if(msg[4+(bus_type*4)]==TYPE_ERR){
          char *p=outBuf+outBufLen;
//          outBufLen+=sprintf(outBuf+outBufLen,"error %d",msg[9]); For truncated error message LPB bus systems
//          if((msg[9]==0x07 && bus_type==0) || (msg[9]==0x05 && bus_type==1)){
          outBufLen+=sprintf(outBuf+outBufLen,"error %d",msg[pl_start]);
          if(msg[pl_start]==0x07){
            outBufLen+=sprintf(outBuf+outBufLen," (parameter not supported)");
          }
          Serial.print(p);
        }else{
          pvalstr=outBuf+outBufLen;
          switch(pgm_read_byte_far(pgm_get_far_address(cmdtbl[0].type) + i * sizeof(cmdtbl[0]))) {
//          switch(pgm_read_byte(&cmdtbl[i].type)){
            case VT_DATETIME: // special
              printDateTime(msg,data_len);
              break;
            case VT_SUMMERPERIOD:
            case VT_VACATIONPROG:
              printDate(msg,data_len);
              break;
            case VT_TIMEPROG:
              printTimeProg(msg,data_len);
              break;
            case VT_SECONDS_WORD: //u16 s
              printWORD(msg,data_len,1,"s");
              break;
            case VT_SECONDS_WORD5: // u16  - Wert als Temperatur interpretiert (RAW / 2)
              printFIXPOINT(msg,data_len,2.0,1,"s");
              break;
            case VT_SECONDS_SHORT: //u8 s
              printBYTE(msg,data_len,"s");
              break;
            case VT_SECONDS_SHORT4: // s8 / 4 (signed)
              printFIXPOINT_BYTE(msg,data_len,4.0,1,"s");
              break;
            case VT_SECONDS_SHORT5: // s8 / 5 (signed)
              printFIXPOINT_BYTE(msg,data_len,5.0,1,"s");
              break;
            case VT_MINUTES_SHORT: //u8 min
              printBYTE(msg,data_len,"min");
              break;
            case VT_MINUTES_WORD: //u16 min
              printWORD(msg,data_len,1,"min");
              break;
            case VT_MINUTES: // u32 min
              printDWORD(msg,data_len,60,"min");
              break;
            case VT_HOURS_SHORT: // u8 h
              printBYTE(msg,data_len,"h");
              break;
            case VT_HOURS_WORD: // u16 h
              printWORD(msg,data_len,1,"h");
              break;
            case VT_HOURS: // u32 h
              printDWORD(msg,data_len,3600,"h");
              break;
            case VT_HOUR_MINUTES: // u8:u8
              printTime(msg,data_len);
              break;
            case VT_TEMP: // s16 / 64.0 - Wert als Temperatur interpretiert (RAW / 64)
              printFIXPOINT(msg,data_len,64.0,1,gradC);
              break;
            case VT_TEMP_WORD: // s16  - Wert als Temperatur interpretiert (RAW )
              printFIXPOINT(msg,data_len,1.0,1,gradC);
              break;
            case VT_TEMP_SHORT: // s8
              printFIXPOINT_BYTE(msg,data_len,1.0,0,gradC);
              break;
            case VT_TEMP_SHORT5: // s8 / 2 (signed)
              printFIXPOINT_BYTE(msg,data_len,2.0,1,gradC);
              break;
            case VT_TEMP_SHORT5_US: // s8 / 2 (unsigned)
              printFIXPOINT_BYTE_US(msg,data_len,2.0,1,gradC);
              break;
            case VT_PRESSURE_WORD: // u16 / 10.0 bar
              printFIXPOINT(msg,data_len,10.0,1,"bar");
              break;
            case VT_PRESSURE: // u8 / 10.0 bar
              printFIXPOINT_BYTE(msg,data_len,10.0,1,"bar");
              break;
            case VT_POWER: // u32 / 10.0 kW
              printFIXPOINT_DWORD(msg,data_len,10.0,1,"kW");
              break;
            case VT_POWER_WORD: // u16 / 10.0 kW
              printFIXPOINT(msg,data_len,10.0,1,"kW");
              break;
            case VT_CURRENT: // u16 / 100 uA
              printFIXPOINT(msg,data_len,100.0,2,"uA");
              break;
            case VT_PROPVAL: // u16 / 16
              printFIXPOINT(msg,data_len,16.0,2,"");
              break;
            case VT_GRADIENT: // u16
              printWORD(msg,data_len,1,"min/K");
              break;
            case VT_SPEED: // u16
              printFIXPOINT(msg,data_len,0.02,0,"rpm");
              break;
            case VT_SPEED2: // u16
              printFIXPOINT(msg,data_len,1,0,"rpm");
              break;
            case VT_FP02: // u16 / 50.0 - Wert als Festkommazahl mit 2/100 Schritten interpretiert (RAW / 50)
              printFIXPOINT(msg,data_len,50.0,2,NULL);
              break;
            case VT_FP1: // s16 / 10.0 Wert als Festkommazahl mit 1/10 Schritten interpretiert (RAW / 10)
              printFIXPOINT(msg,data_len,10.0,1,NULL);
              break;
            case VT_BIT: // u8
              printBIT(msg,data_len);
              break;
            case VT_BYTE: // u8
              printBYTE(msg,data_len,NULL);
              break;
            case VT_ONOFF:
              printCHOICE(msg,data_len,"Aus","Ein");
              break;
            case VT_YESNO:
              printCHOICE(msg,data_len,"Nein","Ja");
              break;
            case VT_CLOSEDOPEN:
              printCHOICE(msg,data_len,"Offen","Geschlossen");
              break;
            case VT_MANUAUTO:  //FUJITSU
              printCHOICE(msg,data_len,"Automatisch","Manuell");
              break;
            case VT_BLOCKEDREL:  //FUJITSU
              printCHOICE(msg,data_len,"Gesperrt","Freigegeben");
              break;
            case VT_DAYS: // u8 Tage
              printBYTE(msg,data_len,"Tage");
              break;
            case VT_DAYS_WORD: // u16 Tage
              printWORD(msg,data_len,1,"Tage");
              break;
            case VT_MONTHS: // u8 Monate
              printBYTE(msg,data_len,"Monate");
              break;
            case VT_WEEKDAY: // enum
              if(data_len == 2){
                if(msg[pl_start]==0){
                  int len=sizeof(ENUM_WEEKDAY);
//                  memcpy_PF(buffer, pgm_get_far_address(ENUM_WEEKDAY), len);
                  memcpy_P(buffer, &ENUM_WEEKDAY,len);
                  buffer[len]=0;
                  printENUM(buffer,len,msg[pl_start+1],0);
                }else{
                  Serial.print(F("---"));
                  outBufLen+=sprintf(outBuf+outBufLen,"---");
                }
              }else{
                Serial.print(F(" VT_WEEKDAY !=2: "));
                SerialPrintData(msg);
                outBufLen+=sprintf(outBuf+outBufLen,"decoding error");
              }
              break;
            case VT_ENUM: // enum
              if((data_len == 2 && (dev_id & DEV_FJ_WSK) != dev_id) || (data_len == 3 && (dev_id & DEV_FJ_WSK) == dev_id)){
                if((msg[pl_start]==0 && data_len==2) || (msg[pl_start]==0 && msg[pl_start+1]==0 && data_len==3)){
                  if(pgm_read_word_far(pgm_get_far_address(cmdtbl[0].enumstr) + i * sizeof(cmdtbl[0]))!=0) {
                    int len=pgm_read_word_far(pgm_get_far_address(cmdtbl[0].enumstr_len) + i * sizeof(cmdtbl[0]));
                    memcpy_PF(buffer, pgm_read_word_far(pgm_get_far_address(cmdtbl[0].enumstr) + i * sizeof(cmdtbl[0])),len);
                    buffer[len]=0;

                    if (data_len == 2) {
                      printENUM(buffer,len,msg[pl_start+1],1);
                    } else {                            // Fujitsu: data_len == 3
                      printENUM(buffer,len,msg[pl_start+2],1);
                    }
                  }else{
                    Serial.print(F("no enum str "));
                    SerialPrintData(msg);
                    outBufLen+=sprintf(outBuf+outBufLen,"no enum str");
                  }
                }else{
                  Serial.print(F("---"));
                  outBufLen+=sprintf(outBuf+outBufLen,"---");
                }
              }
              else{
                Serial.print(F(" VT_ENUM len !=2 && len != 3: "));
                SerialPrintData(msg);
                outBufLen+=sprintf(outBuf+outBufLen,"decoding error");
              }
              break;
            case VT_PERCENT: // u8 %
              printBYTE(msg,data_len,perc);
              break;
            case VT_PERCENT5: // u8 %
              printFIXPOINT_BYTE(msg,data_len,2.0,1,perc);
              break;
            case VT_PERCENT_WORD: // u16 / 2 %
              printFIXPOINT(msg,data_len,2.0,1,perc);
              break;
            case VT_DWORD: // s32
              printDWORD(msg,data_len,1,NULL);
              break;
            case VT_STRING: // string
              if(data_len > 0){
                if(msg[pl_start]!=0){
                  msg[pl_start + data_len]='\0'; // write terminating zero
                  Serial.print((char*)&msg[pl_start]);
                  outBufLen+=sprintf(outBuf+outBufLen,"%s",(char*)&msg[pl_start]);
                } else {
                  Serial.print(F("-"));
                  outBufLen+=sprintf(outBuf+outBufLen,"-");
                }
              }else{
                Serial.print(F(" VT_STRING len ==0: "));
                SerialPrintData(msg);
                outBufLen+=sprintf(outBuf+outBufLen,"decoding error");
              }
              break;
            case VT_ERRORCODE: //  s16
              if(data_len == 3){
                if(msg[pl_start]==0){
                  long lval;
                  lval=(long(msg[pl_start+1])<<8)+long(msg[pl_start+2]);
                  int len=sizeof(ENUM_ERROR);
//                  memcpy_PF(buffer, pgm_get_far_address(ENUM_ERROR), len);
                  memcpy_P(buffer, &ENUM_ERROR,len);
                  buffer[len]=0;
                  printENUM(buffer,len,lval,1);
                } else {
                  Serial.print(F("---"));
                  outBufLen+=sprintf(outBuf+outBufLen,"---");
                }
              }else{
                Serial.print(F(" VT_ERRORCODE len ==0: "));
                SerialPrintData(msg);
                outBufLen+=sprintf(outBuf+outBufLen,"decoding error");
              }
              break;
            case VT_UINT: //  s16
              printWORD(msg,data_len,1,NULL);
              break;
            case VT_UINT5: //  s16 * 5
              printWORD(msg,data_len,5,NULL);
              break;
            case VT_SINT: //  s16
              printSINT(msg,data_len,1,NULL);
              break;
            case VT_VOLTAGE: // u16 - 0.0 -> 00 00 //FUJITSU
              printFIXPOINT_BYTE(msg,data_len,10.0,1,"Volt");
//              printBYTE(msg,data_len,"Volt");
              break;
            case VT_VOLTAGEONOFF:
              printCHOICE(msg,data_len,"0 Volt","230 Volt");
              break;
            case VT_LPBADDR: //decoding unklar 00 f0 -> 15.01
              printLPBAddr(msg,data_len);
              break;
            case VT_UNKNOWN:
            default:
              SerialPrintData(msg);
              outBufLen+=sprintf(outBuf+outBufLen,"unknown type");
              break;
          }
        }
      }else{
        SerialPrintData(msg);
//        Serial.println();
//        SerialPrintRAW(msg,msg[len_idx]+bus_type);
        outBufLen+=sprintf(outBuf+outBufLen,"unknown command");
      }
    }
  }
  Serial.println();
  if(verbose){
    SerialPrintRAW(msg,msg[len_idx]+bus_type);
    Serial.println();
  }
  return pvalstr;
}

void printPStr(uint32_t outstr, uint16_t outstr_len) {
  int htmlbuflen = 100;
  byte htmllineBuf[htmlbuflen];
  int i = 0;
  for (unsigned int x=0;x<outstr_len-1;x++) {
    htmllineBuf[i] = pgm_read_byte_far(outstr+x);
    i++;
    if (i==htmlbuflen) {
      i=0;
      client.write(htmllineBuf, htmlbuflen);
    }
  }
  //final packet
  if (i > 0) client.write(htmllineBuf, i);
}

/** *****************************************************************
 *  Function:  webPrintHeader()
 *  Does:      Sets up the HTML code to start a web page
 *  Pass parameters:
 *   none
 *  Parameters passed back:
 *   none
 *  Function value returned:
 *   none
 *  Global resources used:
 *   client object
 * *************************************************************** */
void webPrintHeader(void){

  printPStr(pgm_get_far_address(header_html), sizeof(header_html));

#ifdef PASSKEY
  client.print(PASSKEY);
  client.print(F("/"));
#endif
  client.println(F("'>BSB-LAN Web</A></h1></center>"));
  client.print(F("<table width=80% align=center><tr bgcolor=#f0f0f0><td width=20% align=center><a href='/"));
#ifdef PASSKEY
  client.print(PASSKEY);
  client.print(F("/"));
#endif
#ifdef LANG_DE
  client.print(F("K'>Heizungsfunktionen"));
#else
  client.print(F("K'>Heater functions"));
#endif

  client.print(F("</a></td><td width=20% align=center>"));

#ifndef ONE_WIRE_BUS
#ifdef LANG_DE
  client.print(F("<font color=#000000>DS18B20-Sensoren</font>"));
#else
  client.print(F("<font color=#000000>DS18B20 sensors</font>"));
#endif
#else
  client.print(F("<a href='/"));
#ifdef PASSKEY
  client.print(PASSKEY);
  client.print(F("/"));
#endif
#ifdef LANG_DE
  client.print(F("T'>DS18B20-Sensoren</a>"));
#else
  client.print(F("T'>DS18B20 sensors</a>"));
#endif
#endif

  client.print(F("</td><td width=20% align=center>"));

#ifndef DHT_BUS
#ifdef LANG_DE
  client.print(F("<font color=#000000>DHT22-Sensoren</font>"));
#else
  client.print(F("<font color=#000000>DHT22 sensors</font>"));
#endif
#else
  client.print(F("<a href='/"));
#ifdef PASSKEY
  client.print(PASSKEY);
  client.print(F("/"));
#endif
#ifdef LANG_DE
  client.print(F("H'>DHT22-Sensoren</a>"));
#else
  client.print(F("H'>DHT22 sensors</a>"));
#endif
#endif

  client.print(F("</td><td width=20% align=center>"));

#ifndef LOGGER
#ifdef LANG_DE
  client.print(F("<font color=#000000>Ausgabe Logdatei</font>"));
#else
  client.print(F("<font color=#000000>Dump log file</font>"));
#endif
#else
  client.print(F("<a href='/"));
#ifdef PASSKEY
  client.print(PASSKEY);
  client.print(F("/"));
#endif
#ifdef LANG_DE
  client.print(F("DG'>Anzeige Logdatei</a>"));
#else
  client.print(F("DG'>Display logfile</a>"));
#endif
#endif

  client.print(F("</td></tr><tr bgcolor=#f0f0f0><td width=20% align=center>"));

  client.print(F("<a href='/"));
#ifdef PASSKEY
  client.print(PASSKEY);
  client.print(F("/"));
#endif
#ifdef LANG_DE
  client.print(F("C'>Konfiguration"));
#else
  client.print(F("C'>Configuration"));
#endif

  client.print(F("</a></td><td width=20% align=center><a href='/"));

#ifdef PASSKEY
  client.print(PASSKEY);
  client.print(F("/"));
#endif
#ifdef LANG_DE
  client.print(F("O'>URL-Befehle"));
#else
  client.print(F("O'>URL commands"));
#endif

  client.print(F("</a></td><td width=20% align=center>"));

  client.print(F("<a href='http://github.com/fredlcore/bsb_lan/blob/master/HOWTO"));
#ifdef LANG_DE
  client.print(F("_de"));
#endif
  client.print(F(".md' target='new'>HowTo</a></td><td width=20% align=center><a href='http://github.com/fredlcore/bsb_lan/blob/master/FAQ"));
#ifdef LANG_DE
  client.print(F("_de"));
#endif
  client.println(F(".md' target='_new'>FAQ</a></td>"));
//  client.println(F("<td width=20% align=center><a href='http://github.com/fredlcore/bsb_lan' target='new'>GitHub Repo</a></td>"));
  client.println(F("</tr></table><p></p><table align=center width=80%><tr><td>"));

} // --- webPrintHeader() ---

/** *****************************************************************
 *  Function:  webPrintFooter()
 *  Does:      Sets up the closing HTML code of a web page.
 *  Pass parameters:
 *   none
 *  Parameters passed back:
 *   none
 *  Function value returned:
 *   none
 *  Global resources used:
 *   client object
 * *************************************************************** */
void webPrintFooter(void){
  client.println(F("</td></tr></table>"));
  client.println(F("</body>"));
  client.println(F("</html>"));
  client.println();

} // --- webPrintFooter() ---

/** *****************************************************************
 *  Function:  webPrintSite()
 *  Does:      Sets up HTML code to display a help page.
 *  Pass parameters:
 *   none
 *  Parameters passed back:
 *   none
 *  Function value returned:
 *   none
 *  Global resources used:
 *   client object
 * *************************************************************** */
void webPrintSite() {
  webPrintHeader();

  client.println(F("<p>"));
  client.print(F("BSB-LAN Web, Version "));
  client.print(version);
#ifdef LANG_DE
  client.println(F("<p><b>Heizungsfunktionen:</b> Hiermit können Sie die Funktionen Ihres Heizungssystems abfragen bzw. steuern. Die einzelnen Parameter sind in entsprechende Kategorien aufgeteilt, die Sie anklicken können."));
  client.println(F("<p><b>Konfiguration anzeigen:</b> Hier sehen Sie, mit welchen Werten BSB-LAN konfiguriert ist. Eine Änderung dieser Parameter ist über die erweiterten URL-Befehle möglich."));
  client.println(F("<p><b>Erweiterte URL-Befehle:</b> Zeigt Ihnen eine Übersicht der Befehle an, die Sie über die direkte Eingabe in der Adresszeile des Browsers absenden können. Diese Befehle sind auch für die Anbindung an Hausautomations-Systeme wie FHEM nötig."));
#else
  client.println(F("<p><b>Heater functions:</b> Allows you to query or set parameters of your heating system, sorted in different clickable categories."));
  client.println(F("<p><b>Show configuration:</b> Displays a list of configuration options. You can change most of these by using the extended URL commands."));
  client.println(F("<p><b>Advanced URL commands:</b> Displays a list of extended commands which you can access by directly entering them in the browser's address line. These commands are also necessary to link up BSB-LAN to any home automation system such as FHEM."));
#endif
  webPrintFooter();
} // --- webPrintSite() ---



#define MAX_PARAM_LEN 22

/**  *****************************************************************
 *  Function: set()
 *  Does:     This routine does all the work to set up a SET
 *            command for various heater parameters.  As such,
 *            it uses a BIG switch statement to differentiate
 *            between the various parameter types and formats
 *            them according to the rules of the heater mfgr.
 * Pass parameters:
 *  uint16  line     the line number (ProgNr)
 *  char  * val      the value to set
 *  bool setcmd      True:  builds a SET msg;
 *                   False: builds an INF msg
 * Parameters passed back:
 *
 * Function value returned:
 *  0         failure (incomplete input data, ..)
 *
 * Global resources used:
 *  Serial instance
 *  bus    instance
 * *************************************************************** */
int set(uint16_t line      // the ProgNr of the heater parameter
      , const char *val          // the value to set
      , bool setcmd)       // true: SET msg; false: INF msg
{
  byte msg[33];            // we know the maximum length
  byte tx_msg[33];
  int i;
  uint32_t c;              // command code
  uint8_t param[MAX_PARAM_LEN]; // 33 -9 - 2
  uint8_t param_len;

  // Search the command table from the start for a matching line nbr.
  i=findLine(line,0,&c);   // find the ProgNr and get the command code
  if(i<0) return 0;        // no match

  // Get the parameter type from the table row[i]
  switch(pgm_read_byte_far(pgm_get_far_address(cmdtbl[0].type) + i * sizeof(cmdtbl[0]))) {
//  switch(pgm_read_byte(&cmdtbl[i].type)){
    // ---------------------------------------------
    // 8-bit unsigned integer representation
    // Months or minutes
    // No input values sanity check
    case VT_MONTHS: //(Wartungsintervall)
    case VT_MINUTES_SHORT: // ( Fehler - Alarm)
    case VT_PERCENT:
      {
      if(val[0]!='\0'){
        uint8_t t=atoi(val);
        param[0]=0x06;  //enable
        param[1]=t;
      }else{
        param[0]=0x05;  // disable
        param[1]=0x00;
      }
      param_len=2;
      }
      break;

    // ---------------------------------------------
    // 8-bit unsigned integer representation
    // All enumeration (list) types
    // No input values sanity check
    case VT_ENUM:          // enumeration types
    case VT_ONOFF: // 1 = On                      // TODO: Check if 1=on - my Thision says on = 255
    case VT_CLOSEDOPEN: // 1 = geschlossen
    case VT_YESNO: // 1 = Ja
    case VT_WEEKDAY: // (1=Mo..7=So)
    case VT_DAYS: // (Legionellenfkt. Periodisch)
    case VT_HOURS_SHORT: // (Zeitkonstante Gebäude)
    case VT_BIT:
    case VT_BYTE:
    case VT_TEMP_SHORT:
      {
      uint8_t t=atoi(val);
      param[0]=0x01;  //enable
      param[1]= t;
      param_len=2;
      }
      break;

    // ---------------------------------------------
    // 16-bit unsigned integer representation
    // Example: Brennerstarts Intervall/Brennerstarts seit Wartung
    // No input values sanity check
    case VT_UINT:
    case VT_SINT:
    case VT_HOURS_WORD: // (Brennerstunden Intervall - nur durch 100 teilbare Werte)
    case VT_MINUTES_WORD: // (Legionellenfunktion Verweildauer)
      {
      if(val[0]!='\0'){
        uint16_t t=atoi(val);
        param[0]=0x06;  //enable
        param[1]=(t >> 8);
        param[2]= t & 0xff;
      }else{
        param[0]=0x05;  // disable
        param[1]=0x00;
        param[2]=0x00;
      }
      param_len=3;
      }
      break;

    // ---------------------------------------------
    // 16-bit unsigned integer representation
    // Temperature values
    case VT_TEMP_WORD:
      {
      uint16_t t=atoi(val);     // TODO: Isn't VT_TEMP_WORD a signed number?
      if(val[0]!='\0'){
        param[0]=0x01;
        param[1]=(t >> 8);
        param[2]= t & 0xff;
      }                         // TODO: Do we need a disable section here as well?
      param_len=3;
      }
      break;

    // ---------------------------------------------
    // 32-bit unsigned integer representation
    case VT_DWORD:
      {
      if(val[0]!='\0'){
        uint32_t t=atoi(val);
        param[0]=0x06;  //enable
        param[1]=(t >> 24) & 0xff;
        param[2]=(t >> 16) & 0xff;
        param[3]=(t >> 8) & 0xff;
        param[4]= t & 0xff;
      }else{
        param[0]=0x05;  // disable
        param[1]=0x00;
        param[2]=0x00;
        param[3]=0x00;
        param[4]=0x00;
      }
      param_len=5;
      }
      break;

    // ---------------------------------------------
    // 8-bit unsigned integer representation
    // No input values sanity check
    case VT_HOUR_MINUTES: //TODO test it
      {
      if(val[0]!='\0'){
        uint8_t h=atoi(val);
        uint8_t m=0;
        while(*val!='\0' && *val!=':' && *val!='.') val++;
        if(*val==':' || *val=='.'){
          val++;
          m=atoi(val);
        }
        param[0]=0x06;  //enable
        param[1]= h;
        param[2]= m;
      }else{
        param[0]=0x05;  // disable
        param[1]=0x00;
        param[2]=0x00;
      }
      param_len=3;
      }
      break;

    // ---------------------------------------------
    // Example: (Telefon Kundendienst)
    case VT_STRING:
      {
      strncpy((char *)param,val,MAX_PARAM_LEN);
      param[MAX_PARAM_LEN-1]='\0';
      param_len=strlen((char *)param)+1;
      }
      break;

    // ---------------------------------------------
    // 16-bit unsigned integer representation
    // Temperature values, mult=64
    case VT_TEMP:
      {
      uint16_t t=atof(val)*64.0;
      if(setcmd){
        param[0]=0x01;
        param[1]=(t >> 8);
        param[2]= t & 0xff;
      }else{ // INF message type (e.g. for room temperature)
        param[0]=(t >> 8);
        param[1]= t & 0xff;
        param[2]=0x00;
      }
      param_len=3;
      }
      break;
    case VT_TEMP_SHORT5_US:
    case VT_TEMP_SHORT5:
    case VT_PERCENT5:
      {
      if(val[0]!='\0'){
        uint8_t t=atof(val);
        param[0]=0x01;  //enable
        param[1]=t*2;
      }
      param_len=2;
      }
      break;

    // ---------------------------------------------
    // 2-byte floating point representation
    // Example: Kennlinie Steilheit, mult=50
    // No input values sanity check
    case VT_FP02:
      {
      uint16_t t=atof(val)*50.0;
      param[0]=0x01;
      param[1]=(t >> 8);
      param[2]= t & 0xff;
      param_len=3;
      }
      break;
    // ---------------------------------------------
    // 8-bit unsigned integer representation
    // Example: pressure value, mult=10
    // No input values sanity check
    case VT_PRESSURE:
      {
      uint8_t t=atof(val)*10.0;
      param[0]=0x01;
      param[1]= t;
      param_len=2;
      }
      break;

    // ---------------------------------------------
    // 32-bit unsigned integer representation
    // Minutes, mult=60
    // Example: HK1 - Einschaltoptimierung
    // No input values sanity check
    case VT_MINUTES:
      {
      uint32_t t=atoi(val)*60;
      param[0]=0x01;
      param[1]=(t >> 24) & 0xff;
      param[2]=(t >> 16) & 0xff;
      param[3]=(t >> 8) & 0xff;
      param[4]= t & 0xff;
      param_len=5;
      }
      break;

    case VT_SECONDS_WORD5:
      {
      uint16_t t=atoi(val)*2;
      if(val[0]!='\0'){
        param[0]=0x01;
        param[1]=(t >> 8);
        param[2]= t & 0xff;
      }                         // TODO: Do we need a disable section here as well?
      param_len=3;
      }
      break;

    case VT_SECONDS_SHORT4:
      {
      if(val[0]!='\0'){
        uint8_t t=atof(val);
        param[0]=0x01;  //enable
        param[1]=t*4;
      }
      param_len=2;
      }
      break;

    case VT_SECONDS_SHORT5:
      {
      if(val[0]!='\0'){
        uint8_t t=atof(val);
        param[0]=0x01;  //enable
        param[1]=t*5;
      }
      param_len=2;
      }
      break;

    // ---------------------------------------------
    // Schedule data
    case VT_DATETIME: // TODO do we have to send INF or SET command?
      {
      //S0=dd.mm.yyyy_mm:hh:ss
      // date and time are transmitted as INF message by the display unit
      // DISP->ALL  INF    0 Uhrzeit und Datum -  Datum/Zeit: 30.01.2015 23:17:00
      // DC 8A 7F 14 02 05 00 00 6C 00 73 01 1E 05 17 11 00 00 A1 AB
      int d,m,y,min,hour,sec;
      // The caller MUST provide six values for an event
      if(6!=sscanf(val,"%d.%d.%d_%d:%d:%d",&d,&m,&y,&hour,&min,&sec))
        return 0;

      // Send to the PC hardware serial interface (DEBUG)
      Serial.print(F("date time: "));
      Serial.print(d);
      Serial.print(F("."));
      Serial.print(m);
      Serial.print(F("."));
      Serial.print(y);
      Serial.print(F(" "));
      Serial.print(hour);
      Serial.print(F(":"));
      Serial.print(min);
      Serial.print(F(":"));
      Serial.println(sec);

      // Set up the command payload
      //outBufLen+=sprintf(outBuf+outBufLen,"%02d.%02d.%d %02d:%02d:%02d",msg[12],msg[11],msg[10]+1900,msg[14],msg[15],msg[16]);
      param[0]=0x01; //???
      param[1]=y-1900;
      param[2]=m;
      param[3]=d;
      param[4]=dayofweek(d,m,y);
      param[6]=hour;
      param[7]=min;
      param[8]=sec;
      param_len=9;
      }
      break;
    // ---------------------------------------------
    // Schedule up to three ON-OFF blocks per day; at least one ON-OFF
    // block must be defined. Begin and end times are given hour minute.
    case VT_TIMEPROG: // TODO test it
      {
      //S502=05:00-22:00|xx:xx-xx:xx|xx:xx-xx:xx
      //DISP->HEIZ SET  502 Zeitprogramm Heizkreis 1 -  Mi: 1. 05:00 - 22:00 2. --:-- - --:-- 3. --:-- - --:--
      //DC 8A 00 17 03 3D 05 0A 8E 05 00 16 00 80 00 00 00 80 00 00 00 08 98
      // Default values if not requested otherwise
      int h1s=0x80,m1s=0x00,h2s=0x80,m2s=0x00,h3s=0x80,m3s=0x00;
      int h1e=0x80,m1e=0x00,h2e=0x80,m2e=0x00,h3e=0x80,m3e=0x00;
      int ret;
      ret=sscanf(val,"%d:%d-%d:%d|%d:%d-%d:%d|%d:%d-%d:%d",&h1s,&m1s,&h1e,&m1e,&h2s,&m2s,&h2e,&m2e,&h3s,&m3s,&h3e,&m3e);
      // we need at least the first period
      if(ret<4)      // BEGIN hour/minute and END hour/minute
        return 0;
      param[0]=h1s;     // minimum definition
      param[1]=m1s;
      param[2]=h1e;
      param[3]=m1e;

      param[4]=h2s;     // use default values if undefined
      param[5]=m2s;
      param[6]=h2e;
      param[7]=m2e;

      param[8]=h3s;     // use default values if undefined
      param[9]=m3s;
      param[10]=h3e;
      param[11]=m3e;
      param_len=12;
      }
      break;
    // ---------------------------------------------
    // Define day/month BEGIN and END dates for vacation periods
    case VT_VACATIONPROG:
      //DISP->HEIZ SET      3D0509C6 06 00 02 0A 00 00 00 00 17
      //outBufLen+=sprintf(outBuf+outBufLen,"%02d.%02d",msg[12],msg[11]);
      param[1]=0;
      param[2]=0;
      param[3]=0;
      param[4]=0;
      param[5]=0;
      param[6]=0;
      param[7]=0;
      param[8]=0x17; //?
      param_len=9;
      if(val[0]!='\0'){
          int d,m;
          if(2!=sscanf(val,"%d.%d.",&d,&m))
            return 0;      // incomplete input data
          param[0]=0x06;   // flag = enabled
          param[2]=m;
          param[3]=d;
      }else{
          param[0]=0x05;   // flag = disabled
      }
      break;
    // ---------------------------------------------
    case VT_SUMMERPERIOD: // TODO do we have to send INF or SET command?
      {
    // Sommerzeit scheint im DISP gehandelt zu werden
    // Bei Anzeige werden keine Werte abgefragt. Bei Änderung wird ein INF geschickt.
    // Sommerzeit Beginn 25.3. DISP->ALL  INF      0500009E 00 FF 03 19 FF FF FF FF 16
    // Sommerzeit Ende 25.11. DISP->ALL  INF      0500009D 00 FF 0B 19 FF FF FF FF 16
      int d,m;
      if(2!=sscanf(val,"%d.%d",&d,&m))
        return 0;
      param[0]=0;
      param[1]=0xff;
      param[2]=m;
      param[3]=d;
      param[4]=0xff;
      param[5]=0xff;
      param[6]=0xff;
      param[7]=0xff;
      param[8]=0x17; //?
      param_len=9;
      }
      break;
    // ---------------------------------------------
    case VT_HOURS: // (read only)
    case VT_VOLTAGE: // read only (Ein-/Ausgangstest)
    case VT_LPBADDR: // read only (LPB-System - Aussentemperaturlieferant)
    case VT_PRESSURE_WORD: // read only (Diagnose Verbraucher)
    case VT_FP1: // read only (SW-Version)
    case VT_ERRORCODE: // read only
    case VT_UNKNOWN:
    default:
      return 0;
    break;
  } // endswitch

  // Check for readonly parameter
  if(pgm_read_byte_far(pgm_get_far_address(cmdtbl[0].flags) + i * sizeof(cmdtbl[0])) == 1) {
//  if (pgm_read_byte(&cmdtbl[i].flags) == 1) {
    Serial.println(F("Parameter is readonly!"));
    return 2;   // return value for trying to set a readonly parameter
  }

  // Send a message to PC hardware serial port
  Serial.print(F("setting line: "));
  Serial.print(line);
  Serial.print(F(" val: "));
  SerialPrintRAW(param,param_len);
  Serial.println();

  uint8_t t=setcmd?TYPE_SET:TYPE_INF;

  // Send telegram to the bus
  if(!bus.Send(t           // message type
             , c           // command code
             , msg         // receive buffer
             , tx_msg      // xmit buffer
             , param       // payload
             , param_len   // payload length
             , setcmd))    // wait_for_reply
  {
    Serial.println(F("set failed"));
    return 0;
  }

  // Decode the xmit telegram and send it to the PC serial interface
  if(verbose) {
    printTelegram(tx_msg);
#ifdef LOGGER
    LogTelegram(tx_msg);
#endif
  }

  // no answer for TYPE_INF
  if(t!=TYPE_SET) return 1;

  // Decode the rcv telegram and send it to the PC serial interface
  printTelegram(msg);
#ifdef LOGGER
  LogTelegram(msg);
#endif

  // Expect an acknowledgement to our SET telegram
  if(msg[4+(bus_type*4)]!=TYPE_ACK){      // msg type at 4 (BSB) or 8 (LPB)
    Serial.println(F("set failed NACK"));
    return 0;
  }

  return 1;
} // --- set() ---

/** *****************************************************************
 *  Function:  query()
 *  Does:      Retrieves parameters from the heater controller.
 *             Addresses the controller parameters by line (ProgNr).
 *             The query() function can interrogate a whole range
 *             of ProgNrs, delimited by line_start and line_end
 *             inclusive.
 *  Pass parameters:
 *   uint16 linestart  begin to retrieve at this RogNr
 *   uint16 lineend    stop at this ProgNr
 *   bool   noprint    True:  do not display results in the web client
 *                     False: display results in the web client
 * Parameters passed back:
 *   none
 * Function value returned:
 *   result string
 * Global resources used:
 *   char outBuf[]
 *   Serial instance
 *   bus    instance
 *   client instance
 * *************************************************************** */
char* query(uint16_t line_start  // begin at this line (ProgNr)
          , uint16_t line_end    // end with this line (ProgNr)
          , boolean no_print)    // display in web client?
{
  byte msg[33];      // response buffer
  byte tx_msg[33];   // xmit buffer
  uint32_t c;        // command code
  uint16_t line;     // ProgNr
  int i=0;
  int idx=0;
  int retry;
  int formnr=0;
  char *pvalstr=NULL;

  if (!no_print) {         // display in web client?
//    client.println(F("<p><form><table>")); // yes, begin HTML paragraph
  }
  for(line=line_start;line<=line_end;line++){
    outBufclear();
    i=findLine(line,idx,&c);

    if(i>=0){
      idx=i;
      //Serial.println(F("found"));
      if(c!=CMD_UNKNOWN){     // send only valid command codes
        retry=QUERY_RETRIES;
        while(retry){
          if(bus.Send(TYPE_QUR, c, msg, tx_msg)){

            // Decode the xmit telegram and send it to the PC serial interface
            if(verbose) {
              printTelegram(tx_msg);
#ifdef LOGGER
              LogTelegram(tx_msg);
#endif
            }

            // Decode the rcv telegram and send it to the PC serial interface
            pvalstr=printTelegram(msg);
#ifdef LOGGER
            LogTelegram(msg);
#endif
            break;   // success, break out of while loop
          }else{
            Serial.println(F("query failed"));
            retry--;          // decrement number of attempts
          }
        } // endwhile, maximum number of retries reached
        if(retry==0) {
          if (bus_type == 1 && msg[8] == TYPE_ERR) {
            outBufLen+=sprintf(outBuf+outBufLen,"error %d",msg[9]);
          } else {
            outBufLen+=sprintf(outBuf+outBufLen,"%d query failed",line);
          }
        }
      }else{
        //Serial.println(F("unknown command"));
        //if(line_start==line_end) outBufLen+=sprintf(outBuf+outBufLen,"%d unknown command",line);
      } // endelse, valid / invalid command codes
    }else{
      //Serial.println(F("line not found"));
      //if(line_start==line_end) outBufLen+=sprintf(outBuf+outBufLen,"%d line not found",line);
    } // endelse, line (ProgNr) found / not found
    if(outBufLen>0){
      if (!no_print) {  // display result in web client
        formnr++;
        if (msg[4+(bus_type*4)] == TYPE_ERR) {
          client.println(F("<tr style='color: #7f7f7f'><td>"));
        } else {
          client.println(F("<tr><td>"));
        }
        client.println(outBuf);

        uint8_t flags = pgm_read_byte_far(pgm_get_far_address(cmdtbl[0].flags) + i * sizeof(cmdtbl[0]));
        uint8_t type = pgm_read_byte_far(pgm_get_far_address(cmdtbl[0].type) + i * sizeof(cmdtbl[0]));
        uint16_t enumstr_len = pgm_read_word_far(pgm_get_far_address(cmdtbl[0].enumstr_len) + i * sizeof(cmdtbl[0]));
        uint32_t enumstr = pgm_read_word_far(pgm_get_far_address(cmdtbl[0].enumstr) + i * sizeof(cmdtbl[0]));

        // dump data payload for unknown types
        if (type == VT_UNKNOWN && msg[4+(bus_type*4)] != TYPE_ERR) {
          int data_len;
          if (bus_type == 1) {
            data_len=msg[len_idx]-14;     // get packet length, then subtract
          } else {
            data_len=msg[len_idx]-11;     // get packet length, then subtract
          }
          for (i=0;i<=data_len-1;i++) {
            if (msg[pl_start+i] < 16) client.print(F("0"));  // add a leading zero to single-digit values
            client.print(msg[pl_start+i], HEX);
          }
        }

        client.println(F("</td><td>"));
        if ((flags != FL_RONLY || (flags==FL_RONLY && type == VT_BIT)) && msg[4] != TYPE_ERR && type != VT_UNKNOWN) {
          if(type == VT_ENUM || type == VT_BIT) {

            client.print(F("<select "));
            if (type == VT_BIT) {
              client.print(F("multiple "));
            }
            client.print(F("id='value"));
            client.print(formnr);
            client.println(F("'>"));
            memcpy_PF(buffer, enumstr, enumstr_len);
            buffer[enumstr_len]=0;
            uint16_t val;
            uint16_t c=0;
            uint8_t bitmask=0;
            while(c<enumstr_len){
              if(buffer[c+1]!=' ' || buffer[c+2]==' '){         // ENUMs must not contain two consecutive spaces! Necessary because VT_BIT bitmask may be 0x20 which equals space
                val=uint16_t(((uint8_t*)buffer)[c]) << 8 | uint16_t(((uint8_t*)buffer)[c+1]);
                if (type == VT_BIT) {
                  bitmask = val & 0xff;
                  val = val >> 8 & 0xff;
                }
                c++;
              }else{
                val=uint16_t(((uint8_t*)buffer)[c]);
              }
              //skip leading space
              c+=2;

              sprintf(outBuf,"%s",&buffer[c]);
              client.print(F("<option value='"));
              client.print(val);
              if ( (type == VT_ENUM && strtod(pvalstr,NULL) == val) || (type == VT_BIT && (msg[10] & bitmask) == (val & bitmask)) ) {
                client.print(F("' SELECTED>"));
              } else {
                client.print(F("'>"));
              }
              client.print(outBuf);
              client.println(F("</option>"));

              while(buffer[c]!=0) c++;
              c++;
            }
            client.print(F("</select></td><td>"));
            if (type != VT_BIT) {
              client.print(F("<input type=button value='Set' onclick=\"set"));
              if (type == VT_BIT) {
                client.print(F("bit"));
              }
              client.print(F("("));
              client.print(line);
              client.print(F(","));
              client.print(formnr);
              client.print(F(")\">"));
            }
          } else {
            client.print(F("<input type=text id='value"));
            client.print(formnr);
            client.print(F("' VALUE='"));

            char* colon_pos = strchr(pvalstr,':');
            if (colon_pos>0) {
              *colon_pos = '.';
            }

            client.print(strtod(pvalstr,NULL));
            client.print(F("'></td><td><input type=button value='Set' onclick=\"set("));
            client.print(line);
            client.print(F(","));
            client.print(formnr);
            client.print(F(")\">"));
          }
        }
        client.println(F("</td></tr>"));
      }
    } // endif, outBufLen > 0
  } // endfor, for each valid line (ProgNr) command within selected range

  // TODO: check at least for data length (only used for temperature values)
  /*
  int data_len=msg[3]-11;
  if(data_len==3){
    returnval = printFIXPOINT(msg,data_len,64.0,1,"");
  }
  */
  return pvalstr;
} // --- query() ---

/** *****************************************************************
 *  Function:  GetDateTime()
 *  Does:      Returns human-readable date/time string
 *
 * Pass parameters:
 *   date buffer
 * Parameters passed back:
 *   none
 * Function value returned:
 *   date/time string
 * Global resources used:
 *   none
 * *************************************************************** */
char *GetDateTime(char date[]){
  sprintf(date,"%02d.%02d.%d %02d:%02d:%02d",day(),month(),year(),hour(),minute(),second());
  date[20] = 0;
  return date;
}

/** *****************************************************************
 *  Function:  SetDevId()
 *  Does:      Sets device_id and dev_id
 *
 * Pass parameters:
 *   none
 * Parameters passed back:
 *   none
 * Function value returned:
 *   none
 * Global resources used:
 *   none
 * *************************************************************** */

void SetDevId() {
  uint8_t family=0;
  boolean known=0;

  if (bus_type == 1) {
    device_id = 1;
  } else {
    if (fixed_device_id < 1) {
      dev_id = 0;
      device_id = strtod(query(6225,6225,1),NULL);
    } else {
      device_id = fixed_device_id;
    }
  }
  int i=0;
  family=pgm_read_byte_far(pgm_get_far_address(dev_tbl[0].dev_family) + i * sizeof(dev_tbl[0]));
  while(family!=DEV_NONE){
    if(family == device_id){
      known=1;
      break;
    }
    i++;
    family=pgm_read_byte_far(pgm_get_far_address(dev_tbl[0].dev_family) + i * sizeof(dev_tbl[0]));
  }
  if (!known){
    dev_id=DEV_ALL;
  } else {
    dev_id=pgm_read_dword_far(pgm_get_far_address(dev_tbl[0].dev_bit_id) + i * sizeof(dev_tbl[0]));
  }
  Serial.print(F("Device family: "));
  Serial.println(device_id);
  Serial.print(F("Device ID: "));
  Serial.println(dev_id);
}

/** *****************************************************************
 *  Function:  SetDateTime()
 *  Does:      Sets date/time based on heating system's clock
 *
 * Pass parameters:
 *   none
 * Parameters passed back:
 *   none
 * Function value returned:
 *   none
 * Global resources used:
 *   TimeLib
 * *************************************************************** */
void SetDateTime(){
  byte rx_msg[33];      // response buffer
  byte tx_msg[33];   // xmit buffer
  uint32_t c;        // command code

  findLine(0,0,&c);
  if(c!=CMD_UNKNOWN){     // send only valid command codes
    if(bus.Send(TYPE_QUR, c, rx_msg, tx_msg)){
      if (bus_type == 1) {
        setTime(rx_msg[18], rx_msg[19], rx_msg[20], rx_msg[16], rx_msg[15], rx_msg[14]+1900);
      } else {
        setTime(rx_msg[14], rx_msg[15], rx_msg[16], rx_msg[12], rx_msg[11], rx_msg[10]+1900);
      }
    }
  }
}

/** *****************************************************************
 *  Function:  LogTelegram()
 *  Does:      Logs the telegram content in hex to the SD card,
 *             starting at position null. It stops
 *             when it has sent the requested number of message bytes.
 *  Pass parameters:
 *   byte *msg pointer to the telegram buffer
 * Parameters passed back:
 *   none
 * Function value returned:
 *   none
 * Global resources used:
 *   log_parameters
 * *************************************************************** */
#ifdef LOGGER
void LogTelegram(byte* msg){
  File dataFile;
  char device[5];
  uint32_t cmd;
  int i=0;        // begin with line 0
  boolean known=0;
  uint32_t c;     // command code
  uint8_t type=0;
  uint8_t cmd_type=0;
  double divisor=1;
  uint8_t precision=0;
  int data_len;
  double dval;

  if (log_parameters[0] == 30000) {

    if(msg[4+(bus_type*4)]==TYPE_QUR || msg[4+(bus_type*4)]==TYPE_SET){ //QUERY and SET: byte 5 and 6 are in reversed order
      cmd=(uint32_t)msg[6+(bus_type*4)]<<24 | (uint32_t)msg[5+(bus_type*4)]<<16 | (uint32_t)msg[7+(bus_type*4)] << 8 | (uint32_t)msg[8+(bus_type*4)];
    }else{
      cmd=(uint32_t)msg[5+(bus_type*4)]<<24 | (uint32_t)msg[6+(bus_type*4)]<<16 | (uint32_t)msg[7+(bus_type*4)] << 8 | (uint32_t)msg[8+(bus_type*4)];
    }
    // search for the command code in cmdtbl
    c=pgm_read_dword_far(pgm_get_far_address(cmdtbl[0].cmd) + i * sizeof(cmdtbl[0]));
//    c=pgm_read_dword(&cmdtbl[i].cmd);    // extract the command code from line i
    while(c!=CMD_END){
      if(c == cmd){
        long devices = pgm_read_dword_far(pgm_get_far_address(cmdtbl[0].devices) + i * sizeof(cmdtbl[0]));
        if ((devices & dev_id) == dev_id) {
          known=1;
          break;
        }
      }
      i++;
      c=pgm_read_dword_far(pgm_get_far_address(cmdtbl[0].cmd) + i * sizeof(cmdtbl[0]));
//      c=pgm_read_dword(&cmdtbl[i].cmd);
    }

    if (log_unknown_only == 0 || (log_unknown_only == 1 && known == 0)) {
      if (log_bc_only == 0 || (log_bc_only == 1 && ((msg[2]==ADDR_ALL && bus_type==0) || (msg[2]>=0xF0 && bus_type==1)))) {
        dataFile = SD.open("datalog.txt", FILE_WRITE);
        if (dataFile) {
          dataFile.print(millis());
          dataFile.print(F(";"));
          dataFile.print(GetDateTime(date));
          dataFile.print(F(";"));

          if(!known){                          // no hex code match
          // Entry in command table is "UNKNOWN" (0x00000000)
            dataFile.print(F("UNKNOWN"));
          }else{
            // Entry in command table is a documented command code
            uint16_t line=pgm_read_word_far(pgm_get_far_address(cmdtbl[0].line) + i * sizeof(cmdtbl[0]));
            cmd_type=pgm_read_byte_far(pgm_get_far_address(cmdtbl[0].type) + i * sizeof(cmdtbl[0]));
//            uint16_t line=pgm_read_word(&cmdtbl[i].line);
            dataFile.print(line);             // the ProgNr
          }
          dataFile.print(F(";"));
          dataFile.print(TranslateAddr(msg[1+(bus_type*2)], device));
          dataFile.print(F("->"));
          dataFile.print(TranslateAddr(msg[2], device));
          dataFile.print(F(" "));
          dataFile.print(TranslateType(msg[4+(bus_type*4)], device));
          dataFile.print(F(";"));

          for(int i=0;i<msg[len_idx]+bus_type;i++){
            if (i > 0) {
              dataFile.print(F(" "));
            }
            if (msg[i] < 16) dataFile.print(F("0"));  // add a leading zero to single-digit values
            dataFile.print(msg[i], HEX);
          }

          // additionally log data payload in addition to raw messages when data payload is max. 32 Bit

          if ((msg[4+(bus_type*4)] == TYPE_INF || msg[4+(bus_type*4)] == TYPE_SET || msg[4+(bus_type*4)] == TYPE_ANS) && msg[len_idx] < 17+bus_type) {
            dataFile.print(F(";"));
            i=0;
            while(type!=VT_UNKNOWN){
              if(type == cmd_type){
                known=1;
                break;
              }
              i++;
              type=pgm_read_byte_far(pgm_get_far_address(divtbl[0].type) + i * sizeof(divtbl[0]));
            }
            if (bus_type == 1) {
              data_len=msg[1]-14;
            } else {
              data_len=msg[3]-11;
            }
            dval = 0;
            divisor=pgm_read_float_far(pgm_get_far_address(divtbl[0].divisor) + i * sizeof(divtbl[0]));
            precision=pgm_read_byte_far(pgm_get_far_address(divtbl[0].precision) + i * sizeof(divtbl[0]));
            for (i=0;i<data_len-1+bus_type;i++) {
              if (bus_type == 1) {
                dval = dval + long(msg[10+i-(msg[4]==TYPE_INF)]<<((data_len-2-i)*8));
              } else {
                dval = dval + long(msg[14+i-(msg[8]==TYPE_INF)]<<((data_len-2-i)*8));
              }
            }
            dval = dval / divisor;
/*
            if (precision==0) {
              dataFile.print(int(round(dval)));
            } else {
              char formatstr[6]="%0.0f";
              formatstr[3]=precision;
              dataFile.print(printf("%0.1f",dval));
            }
*/
            int a,b,i;
            if(dval<0){
              dval*=-1.0;
            }
            double rval=10.0;
            for(i=0;i<precision;i++) rval*=10.0;
            dval+=5.0/rval;
            a=(int)(dval);
            rval/=10.0;
            b=(int)(dval*rval - a*rval);
            if(precision==0){
              dataFile.print(a);
            }else{
//              char formatstr[8]="%d.%01d";
//              formatstr[5]='0'+precision;
              dataFile.print(a);
              dataFile.print(",");
              dataFile.print(b);
            }

          }
          dataFile.println();
          dataFile.close();
        }
      }
    }
  }
}
#endif

#ifdef DHT_BUS

/** *****************************************************************
 *  Function:  dht22()
 *  Does:      Reads a DHT22 sensor
 *  Pass parameters:
 *   none
 *  Parameters passed back:
 *   none
 *  Function value returned:
 *   none
 *  Global resources used:
 *    DHT_Pins  array with pins to use
 *    outBuf[]
 *    Serial   the hardware serial interface to a PC
 *    DHT  class which handles sensors
 *    client object
 * *************************************************************** */
void dht22(void) {
  int i;
  Serial.println(F("start request values"));
  int numDHTSensors = sizeof(DHT_Pins) / sizeof(int);
  Serial.print(F("DHT22 sensors: "));
  Serial.println(numDHTSensors);
    outBufclear();
    for(i=0;i<numDHTSensors;i++){

    int chk = DHT.read22(DHT_Pins[i]);
    switch (chk) {
      case DHTLIB_OK:  
      Serial.print(F("OK,\t")); 
      break;
      case DHTLIB_ERROR_CHECKSUM: 
      Serial.print(F("Checksum error,\t")); 
      break;
      case DHTLIB_ERROR_TIMEOUT: 
      Serial.print(F("Time out error,\t")); 
      break;
      default: 
      Serial.print(F("Unknown error,\t")); 
      break;
    }

    double hum = DHT.humidity;
    double temp = DHT.temperature;
    Serial.println(F("end request values"));
    Serial.print(F("temp["));
    Serial.print(i);
    Serial.print(F("]: "));
    Serial.print(temp);
    Serial.print(F(", hum["));
    Serial.print(i);
    Serial.print(F("]: "));
    Serial.println(hum);
    if (hum > 0 && hum < 101) {
      outBufLen+=sprintf(outBuf+outBufLen,"<tr><td>temp[%d]: ",i);
      _printFIXPOINT(temp,2);
      outBufLen+=sprintf(outBuf+outBufLen,"</td></tr><tr><td>");
      outBufLen+=sprintf(outBuf+outBufLen,"hum[%d]: ",i);
      _printFIXPOINT(hum,2);
      outBufLen+=sprintf(outBuf+outBufLen,"</td></tr>");
    }
  }
  client.println(outBuf);  
}
#endif  //ifdef DHT_BUS

#ifdef ONE_WIRE_BUS

/** *****************************************************************
 *  Function:  ds18b20()
 *  Does:      Reads a DS18B20 temperature sensor
 *  Pass parameters:
 *   none
 *  Parameters passed back:
 *   none
 *  Function value returned:
 *   none
 *  Global resources used:
 *    int numSensors
 *    outBuf[]
 *    Serial   the hardware serial interface to a PC
 *    sensors  class which handles sensors
 *    client object
 * *************************************************************** */
void ds18b20(void) {
  int i;
  //webPrintHeader();
  Serial.println(F("start requestTemperatures"));
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println(F("end requestTemperatures"));
  Serial.println(F("start getTempCByIndex"));
  outBufclear();
  for(i=0;i<numSensors;i++){
    float t=sensors.getTempCByIndex(i);
    Serial.print(F("temp["));
    Serial.print(i);
    Serial.print(F("]: "));
    Serial.print(t);
    Serial.println();
    outBufLen+=sprintf(outBuf+outBufLen,"temp[%d]: ",i);
    _printFIXPOINT(t,2);
    outBufLen+=sprintf(outBuf+outBufLen,"<br>");
  }
  client.println(outBuf);
  //webPrintFooter();
} // --- ds18b20() ---
#endif   // ifdef ONE_WIRE_BUS

#ifdef IPWE

/*************************** IPWE Extension **************************************/
/** *****************************************************************
 *  Function:  Ipwe()
 *  Does:      Sets up HTML code to display a table with sensor readings.
 *             Queries several controller parameters addressed by their
 *             line (ProgNr) and the LED0 output pin.
 *  Pass parameters:
 *   none
 *  Parameters passed back:
 *   none
 *  Function value returned:
 *   none
 *  Global resources used:
 *    numSensors
 *    client object
 *    led0   output pin 3
 * *************************************************************** */
void Ipwe() {
  webPrintHeader();
  int i;
  int counter = 0;
  int numIPWESensors = sizeof(ipwe_parameters) / sizeof(int);
  Serial.print(F("IPWE sensors: "));
  Serial.println(numIPWESensors);
  double ipwe_sensors[numIPWESensors];
  for (i=0; i < numIPWESensors; i++) {
    ipwe_sensors[i] = strtod(query(ipwe_parameters[i],ipwe_parameters[i],1),NULL);
  }

  client.print(F("<html><body><form><table border=1><tbody><tr><td>Sensortyp</td><td>Adresse</td><td>Beschreibung</td><td>Temperatur</td><td>Luftfeuchtigkeit</td><td>Windgeschwindigkeit</td><td>Regenmenge</td></tr>"));
  for (i=0; i < numIPWESensors; i++) {
    counter++;
    client.print(F("<tr><td>T<br></td><td>"));
    client.print(counter);
    client.print(F("<br></td><td>"));
    client.print(lookup_descr(ipwe_parameters[i]));
    client.print(F("<br></td><td>"));
    client.print(ipwe_sensors[i]);
    client.print(F("<br></td><td>0<br></td><td>0<br></td><td>0<br></td></tr>"));
  }

  for (int i=0; i<numAverages; i++) {
    if (avg_parameters[i] > 0) {
      counter++;
      client.print(F("<tr><td>T<br></td><td>"));
      client.print(counter);
      client.print(F("<br></td><td>"));
      client.print(F("Avg"));
      client.print(lookup_descr(avg_parameters[i]));            
      client.print(F("<br></td><td>"));
      double rounded = round(avgValues[i]*10);
      client.println(rounded/10);
// TODO: extract and display unit text from cmdtbl.type
      client.print(F("<br></td><td>0<br></td><td>0<br></td><td>0<br></td></tr>"));
    }
  }

#ifdef ONE_WIRE_BUS
  // output of one wire sensors
  sensors.requestTemperatures();
  DeviceAddress device_address;
  char device_ascii[17];
  for(i=0;i<numSensors;i++){
    counter++;
    double t=sensors.getTempCByIndex(i);
    sensors.getAddress(device_address, i);
    sprintf(device_ascii, "%02x%02x%02x%02x%02x%02x%02x%02x",device_address[0],device_address[1],device_address[2],device_address[3],device_address[4],device_address[5],device_address[6],device_address[7]);

    client.print(F("<tr><td>T<br></td><td>"));
    client.print(counter);
    client.print(F("<br></td><td>"));
    client.print(device_ascii);
    client.print(F("<br></td><td>"));
    client.print(t);
    client.print(F("<br></td><td>0<br></td><td>0<br></td><td>0<br></td></tr>"));
  }
#endif

#ifdef DHT_BUS
  // output of DHT sensors
  int numDHTSensors = sizeof(DHT_Pins) / sizeof(int);
  for(i=0;i<numDHTSensors;i++){
    DHT.read22(DHT_Pins[i]);
    
    double hum = DHT.humidity;
    double temp = DHT.temperature;
    if (hum > 0 && hum < 101) {
      counter++;
      client.print(F("<tr><td>T<br></td><td>"));
      client.print(counter);
      client.print(F("<br></td><td>"));
      client.print(F("DHT sensor "));
      client.print(i+1);
      client.print(F(" temperature"));
      client.print(F("<br></td><td>"));
      client.print(temp);
      client.print(F("<br></td><td>0<br></td><td>0<br></td><td>0<br></td></tr>"));
      counter++;
      client.print(F("<tr><td>F<br></td><td>"));
      client.print(counter);
      client.print(F("<br></td><td>"));
      client.print(F("DHT sensor "));
      client.print(i+1);
      client.print(F(" humidity"));
      client.print(F("<br></td><td>0<br></td><td>"));
      client.print(hum);
      client.print(F("<br></td><td>0<br></td><td>0<br></td></tr>"));
    }
  }
#endif

  client.print(F("</tbody></table></form>"));
  webPrintFooter();
} 

#endif    // --- Ipwe() ---

char *lookup_descr(uint16_t line) {
  int i=findLine(line,0,NULL);
  if (i<0) {                    // Not found (for this heating system)?
    strcpy_P(buffer, STR99999); // Unknown command has line no. 10999
  } else {
    strcpy_PF(buffer, pgm_read_word_far(pgm_get_far_address(cmdtbl[0].desc) + i * sizeof(cmdtbl[0])));
//  strcpy_P(buffer, (char*)pgm_read_word(&(cmdtbl[i].desc)));
  }
  return buffer;
}

/** *****************************************************************
 *  Function:
 *  Does:
 *  Pass parameters:
 *   none
 * Parameters passed back:
 *   none
 * Function value returned:
 *   none
 * Global resources used:
 *   char   outBuf[]
 *   bus    instance
 *   Serial instance
 *   client instance
 *   server instance
 * *************************************************************** */
void loop() {
  byte  msg[33];                       // response buffer
  byte  tx_msg[33];                    // xmit buffer
  char c;
  const byte MaxArrayElement=252;
  char  cLineBuffer[MaxArrayElement];  //
  byte  bPlaceInBuffer;                // index into buffer

  // Monitor the bus and send incoming data to the PC hardware serial
  // interface.
  // Separate telegrams after a pause of more than one character time.
  if(monitor){
    boolean busmsg=bus.Monitor(msg);
#ifdef LOGGER
    if (busmsg==true) {
      LogTelegram(msg);
    }
#endif
  }else{
    // Listen for incoming messages, identified them by their magic byte.
    // Method GetMessage() validates length byte and CRC.
    if (bus.GetMessage(msg)) { // message was syntactically correct
       // Decode the rcv telegram and send it to the PC serial interface
      if(verbose) {
        printTelegram(msg);
#ifdef LOGGER
        LogTelegram(msg);
#endif
      }
      // Is this a broadcast message?
      if(((msg[2]==ADDR_ALL && bus_type==0) || (msg[2]>=0xF0 && bus_type==1)) && msg[4+(bus_type*4)]==TYPE_INF){ // handle broadcast messages
      // Decode the rcv telegram and send it to the PC serial interface
        if (!verbose) {        // don't log twice if in verbose mode, but log broadcast messages also in non-verbose mode
          printTelegram(msg);
#ifdef LOGGER
          LogTelegram(msg);
#endif
        }

        // Filter Brenner Status messages (attention: partially undocumented enum values)

        uint32_t cmd;
        cmd=(uint32_t)msg[5+(bus_type*4)]<<24 | (uint32_t)msg[6+(bus_type*4)]<<16 | (uint32_t)msg[7+(bus_type*4)] << 8 | (uint32_t)msg[8+(bus_type*4)];
        if(cmd==0x0500006C) {   // set Time from BC, same CommandID for BSB and LPB
          setTime(msg[pl_start+5], msg[pl_start+6], msg[pl_start+7], msg[pl_start+3], msg[pl_start+2], msg[pl_start+1]+1900);
        }

        if(cmd==0x31000212) {    // TWW Status Elco / Brötje SOB
          Serial.print(F("INF: TWW-Status: "));
          Serial.println(msg[11]);      // assumed info byte

          if( (msg[11]==0x4D && (dev_id & ~DEV_EL_THI) == dev_id) || (msg[11]==0xCD && (dev_id & DEV_EL_THI) == dev_id)) {  // TWW Ladung on BROETJE_SOB and THISION
            if(TWW_start==0){        // has not been timed
              TWW_start=millis();   // keep current timestamp
              TWW_count++;          // increment number of starts
            }
          }else{             // TWW Ladung aus
            if(TWW_start!=0){        // start has been timed
              unsigned long TWW_end;
              TWW_end=millis();      // timestamp the end
              if(TWW_end >= TWW_start){
                TWW_duration+=(TWW_end-TWW_start)/1000;
              }else{ // overflow
                TWW_duration+=(0xffffffff-TWW_start+TWW_end)/1000;
              }
                TWW_start=0;
            } // endif, a previous start has been timed
          } // endif, TWW is off
        } // endif, Status TWW command code

        if(cmd==0x05000213) {     // Brennerstatus; CommandID 0x053d0f66 was suggested at some point as well, but so far has not been identified in one of the heating systems
          unsigned long brenner_end;
          boolean reset_brenner_timer = 0;
          Serial.print(F("INF: Brennerstatus: "));
          Serial.println(msg[pl_start]);      // first payload byte

          if(msg[pl_start]==0x04) {       // Stufe 1
            if(brenner_start==0){        // has not been timed
              brenner_start=millis();   // keep current timestamp
              brenner_count++;          // increment number of starts
            }
            if (brenner_stufe == 2) {
              reset_brenner_timer = 1;
            }
            brenner_stufe=1;
          }
          if(msg[pl_start]==0x14) {       // Stufe 2 (only oil furnace)
            if(brenner_start_2==0){        // has not been timed
              brenner_start_2=millis();   // keep current timestamp
              brenner_count_2++;          // increment number of starts
            }
            if (brenner_stufe == 1) {
              reset_brenner_timer = 1;
            }
            brenner_stufe=2;
          }
          if (reset_brenner_timer == 1) {   // Stufenwechsel bei mehrstufigem Ölbrenner
            brenner_end=millis();      // timestamp the end
            if (brenner_stufe == 2) {    // Stufe jetzt 2, war also vorher 1
              if(brenner_end >= brenner_start){
                brenner_duration+=(brenner_end-brenner_start)/1000;
              }else{ // overflow
                brenner_duration+=(0xffffffff-brenner_start+brenner_end)/1000;
              }
              brenner_start=0;              
            }
            if (brenner_stufe == 1) {   // Stufe jetzt 1, war also vorher 2
              if(brenner_end >= brenner_start_2){
                brenner_duration_2+=(brenner_end-brenner_start_2)/1000;
              }else{ // overflow
                brenner_duration_2+=(0xffffffff-brenner_start_2+brenner_end)/1000;
              }
              brenner_start_2=0;
            }
            reset_brenner_timer = 0;
          }
          if (msg[pl_start]==0x00) {    // brenner off
            brenner_end=millis();      // timestamp the end
            if(brenner_start!=0){        // start has been timed
              if(brenner_end >= brenner_start){
                brenner_duration+=(brenner_end-brenner_start)/1000;
              }else{ // overflow
                brenner_duration+=(0xffffffff-brenner_start+brenner_end)/1000;
              }
              brenner_start=0;
            } // endif, a previous start has been timed
            if(brenner_start_2!=0){        // start has been timed
              if(brenner_end >= brenner_start_2){
                brenner_duration_2+=(brenner_end-brenner_start_2)/1000;
              }else{ // overflow
                brenner_duration_2+=(0xffffffff-brenner_start_2+brenner_end)/1000;
              }
              brenner_start_2=0;
            } // endif, a previous start has been timed
          } // endif, brenner is off
        } // endif, Status Brenner command code
      } // endif, broadcasts
    } // endif, GetMessage() returned True

   // At this point drop possible GetMessage() failures silently

  } // endelse, NOT in monitor mode

  // Listen for incoming clients
  client = server.available();
  if (client) {

#ifdef TRUSTED_IP
    int so = client.getSocketNumber();
    uint8_t remoteIP[4];
    W5100.readSnDIPR(so, remoteIP);       // change to W5500 here for W5500 type ethernet shields
    if (remoteIP[3] != TRUSTED_IP) {      // reject clients from unauthorized IP addresses; replace != with > to block access from all IPs greater than TRUSTED_IP segment
      Serial.print(F("Rejected access from "));
      for (int i=0; i<4; i++) {
        Serial.print(remoteIP[i]);
        if (i<3) {
          Serial.print(F("."));
        }
      }
      Serial.println(F("."));
      client.stop();
    }
#endif

   // Read characters from client and assemble them in cLineBuffer
    bPlaceInBuffer=0;            // index into cLineBuffer
    while (client.connected()) {
      if (client.available()) {
        c = client.read();       // read one character
        Serial.print(c);         // and send it to hardware UART

        if ((c!='\n') && (c!='\r') && (bPlaceInBuffer<MaxArrayElement)){
          cLineBuffer[bPlaceInBuffer++]=c;
          continue;
        }
        // Got an EOL character
        Serial.println();

        // perform HTTP-Authentification by reading the remaining client data and look for credentials
#ifdef USER_PASS_B64
        char linebuf[80];
        uint8_t charcount=0;
        boolean authenticated=false;
        memset(linebuf,0,sizeof(linebuf));
        boolean currentLineIsBlank = false;
        while (client.connected()) {
          if (client.available()) {
            char c = client.read();
            linebuf[charcount]=c;
            if (charcount<sizeof(linebuf)-1) charcount++;
            if (c == '\n' && currentLineIsBlank) {
              break;
            }
            if (c == '\n') {
              // you're starting a new line
              currentLineIsBlank = true;
              if (strstr(linebuf,"Authorization: Basic")!=0 && strstr(linebuf,USER_PASS_B64)!=0) {
                authenticated=true;
              }
              memset(linebuf,0,sizeof(linebuf));
              charcount=0;
            } else if (c != '\r') {
              // you've gotten a character on the current line
              currentLineIsBlank = false;
            }            
          }
        }
        // if no credentials found in HTTP header, send 401 Authorization Required
        if (!authenticated) {
          client.println(F("HTTP/1.1 401 Authorization Required"));
          client.println(F("WWW-Authenticate: Basic realm=\"Secure Area\""));
          client.println(F("Content-Type: text/html"));
          client.println(F("Connnection: close"));
          client.println();
          client.println(F("<!DOCTYPE HTML>"));
          client.println(F("<HTML>  <HEAD>   <TITLE>Error</TITLE>"));
          client.println(F(" </HEAD> <BODY><H1>401 Unauthorized.</H1></BODY> </HTML>"));
          client.stop();
        }
        // otherwise continue like normal
#endif

        // Flush any remaining bytes from the client buffer
        client.flush();
        // GET / HTTP/1.1 (anforderung website)
        // GET /710 HTTP/1.0 (befehlseingabe)
        String urlString = String(cLineBuffer);
        urlString = urlString.substring(urlString.indexOf('/'), urlString.indexOf(' ', urlString.indexOf('/')));
                Serial.println(urlString);
        urlString.toCharArray(cLineBuffer, MaxArrayElement);

// IPWE START
#ifdef IPWE
        if (urlString == "/ipwe.cgi") {
          Ipwe();
          break;
        }
#endif
// IPWE END

        // Set up a pointer to cLineBuffer
        char *p=cLineBuffer;
#ifdef PASSKEY
        // check for valid passkey
        p=strchr(cLineBuffer+1,'/');
        if(p==NULL){    // no match
          break;
        }
        *p='\0';     // mark end of string
        if(strcmp(cLineBuffer+1, PASSKEY)){
          Serial.println(F("no matching passkey"));
          webPrintHeader();
          webPrintFooter();
          break;
        }
        *p='/';
#endif
        // simply print the website
        if(!strcmp(p,"/")){
          webPrintSite();
          break;
        }

        // Answer to unknown requests
        if(!isdigit(p[1]) && strchr("ABCDEGHIKLMOPRSTVXY",p[1])==NULL){
          webPrintHeader();
          webPrintFooter();
          break;
        }
        // setting verbosity level
        if(p[1]=='V'){
          p+=2;
          verbose=atoi(p);
          webPrintHeader();
          if(verbose>0){
#ifdef LANG_DE
            client.println(F("Verbositäts-Modus aktiviert!<BR>"));
#else
            client.println(F("Verbose mode activated!<BR>"));
#endif
          }else{
#ifdef LANG_DE
            client.println(F("Verbositäts-Modus deaktiviert!<BR>"));
#else
            client.println(F("Verbose mode deactivated!<BR>"));
#endif
          }
#ifdef LANG_DE
          client.println(F("Der Verbositäts-Modus betrifft sowohl die serielle Konsole des Arduino Mega2560 als auch (optional) das Loggen der Bus-Daten auf die microSD-Karte, so dass die Karte u.U. sehr schnell voll wird! Die html-Ausgabe bleibt unverändert."));
#else
          client.println(F("Verbose output affects both the serial console of the mega2560 as well as (optional) logging bus data to SD card, so this can fill up your card pretty fast! The html output is kept unchanged."));
#endif
          webPrintFooter();
          break;
        }
        // switching monitor on/off
        if(p[1]=='M'){
          p+=2;               // hopefully finds a digit there ...
          monitor=atoi(p);    // .. to convert
          webPrintHeader();
          if(monitor>0){
#ifdef LANG_DE
            client.println(F("Serieller Monitor aktiviert!"));
#else
            client.println(F("Serial monitor activated!"));
#endif
          }else{
#ifdef LANG_DE
            client.println(F("Serieller Monitor deaktiviert!"));
#else
            client.println(F("Serial monitor deactivated!"));
#endif
          }
#ifdef LANG_DE
	  client.println(F("Nur die serielle Ausgabe ist betroffen."));
#else
	  client.println(F("Only serial output is affected."));
#endif
          webPrintFooter();
          break;
        }

        // Send a SET command or an information message
        // char * cLineBuffer has the following structure:
        // p[1]          'S' | 'I'
        // p[3]          ProgNr (digits, any (!) length)
        // p[3+length]   '='
        // p[3+length+1] Value, any (!) length
        // There is only the buffer size which limits to the
        // permissible lengths, no sanity checks
        if ( p[1]=='S'        // SET command
          || p[1]=='I')       // INF information message
        {
          int line;
          bool setcmd= (p[1]=='S'); // True if SET command

          p+=2;               // third position in cLineBuffer
          if(!isdigit(*p)){   // now we check for digits - nice
            webPrintHeader();
#ifdef LANG_DE
            client.println(F("FEHLER: Ungültiger Parameter!"));
#else
            client.println(F("ERROR: invalid parameter line!"));
#endif
            webPrintFooter();
            break;
          }
          line=atoi(p);       // convert until non-digit char is found
          p=strchr(p,'=');    // search for '=' sign
          if(p==NULL){        // no match
            webPrintHeader();
#ifdef LANG_DE
            client.println(F("FEHLER: Ungültiger Wert!"));
#else
            client.println(F("ERROR: invalid parameter value!"));
#endif
            webPrintFooter();
            break;
          }
          p++;                   // position pointer past the '=' sign
          Serial.print(F("set ProgNr "));
          Serial.print(line);    // the ProgNr
          Serial.print(F(" = "));
          Serial.println(p);     // the value

          // Now send it out to the bus
          int setresult = set(line,p,setcmd);
          if(setresult!=1){
            webPrintHeader();
#ifdef LANG_DE
            client.println(F("FEHLER: Setzen fehlgeschlagen!"));
#else
            client.println(F("ERROR: set failed!"));
#endif
            if (setresult == 2) {
#ifdef LANG_DE
              client.println(F(" - Parameter ist nur lesbar"));
#else
              client.println(F(" - parameter is readonly"));
#endif
            }
            webPrintFooter();
            break;
          }
          if(setcmd){            // was this a SET command?
            webPrintHeader();
            // Query controller for this value
            query(line,line,0);  // read value back
            webPrintFooter();
          }else{
            webPrintHeader();
            webPrintFooter();
          }
          break;
        }
        // list categories
        if(p[1]=='K' && !isdigit(p[2])){
          //list categories
          webPrintHeader();
          int len=sizeof(ENUM_CAT);
          memcpy_PF(buffer, pgm_get_far_address(ENUM_CAT), len);
//          memcpy_P(buffer, &ENUM_CAT,len);
          buffer[len]=0;
          client.print(F("<a href='/"));
          #ifdef PASSKEY
            client.print(PASSKEY);
            client.print(F("/"));
          #endif
          #ifdef LANG_DE
          client.println(F("B'>Brennerstatistik</A><BR>"));
          #else
          client.println(F("B'>Heater statistics</A><BR>"));
          #endif
          client.print(F("<a href='/"));
          #ifdef PASSKEY
            client.print(PASSKEY);
            client.print(F("/"));
          #endif
          #ifdef LANG_DE
          client.println(F("A'>24h Durchschnittswerte</A><BR><BR>"));
          #else
          client.println(F("A'>24h averages</A><BR><BR>"));
          #endif
          for(int cat=0;cat<CAT_UNKNOWN;cat++){
            outBufclear();
            printENUM(buffer,len,cat,1);
            Serial.println();
            client.print(F("<A HREF='K"));
            client.print(cat);
            client.print(F("'>"));
            client.println(outBuf);
            client.println(F("</A><br>"));
          }
          webPrintFooter();
          break;
        }
        // list enum values
        if(p[1]=='E'){
          webPrintHeader();
          uint16_t line = atoi(&p[2]);
          int i=findLine(line,0,NULL);
          if(i>=0){
            // check type
              if(pgm_read_byte_far(pgm_get_far_address(cmdtbl[0].type) + i * sizeof(cmdtbl[0]))==VT_ENUM) {
//            if(pgm_read_byte(&cmdtbl[i].type)==VT_ENUM){
              uint16_t enumstr_len=pgm_read_word_far(pgm_get_far_address(cmdtbl[0].enumstr_len) + i * sizeof(cmdtbl[0]));
//              uint16_t enumstr_len=pgm_read_word(&cmdtbl[i].enumstr_len);
              memcpy_PF(buffer, pgm_read_word_far(pgm_get_far_address(cmdtbl[0].enumstr) + i * sizeof(cmdtbl[0])),enumstr_len);
//              memcpy_P(buffer, (char*)pgm_read_word(&(cmdtbl[i].enumstr)),enumstr_len);
              buffer[enumstr_len]=0;

              uint16_t val;
              uint16_t c=0;
              while(c<enumstr_len){
                if(buffer[c+1]!=' '){
                  val=uint16_t(((uint8_t*)buffer)[c]) << 8 | uint16_t(((uint8_t*)buffer)[c+1]);
                  c++;
                }else{
                  val=uint16_t(((uint8_t*)buffer)[c]);
                }
                //skip leading space
                c+=2;

                sprintf(outBuf,"%d - %s",val,&buffer[c]);
                client.println(outBuf);
                client.println(F("<br>"));

                while(buffer[c]!=0) c++;
                c++;
              }
            }else{
#ifdef LANG_DE
              client.println(F("FEHLER: Falscher Typ!"));
#else
              client.println(F("ERROR: wrong type!"));
#endif
            }
          }else{
#ifdef LANG_DE
            client.println(F("FEHLER: Zeile nicht gefunden!"));
#else
	          client.println(F("ERROR: line not found!"));
#endif
          }
          webPrintFooter();
          break;
        }
        // query reset value
        if(p[1]=='R'){
          uint32_t c;
          webPrintHeader();
          uint16_t line = atoi(&p[2]);
          int i=findLine(line,0,&c);
          if(i<0){
#ifdef LANG_DE
            client.println(F("FEHLER: Zeile nicht gefunden!"));
#else
            client.println(F("ERROR: line not found!"));
#endif
          }else{
            if(!bus.Send(TYPE_QRV, c, msg, tx_msg)){
              Serial.println(F("set failed"));  // to PC hardware serial I/F
#ifdef LANG_DE
              client.println(F("FEHLER: Setzen fehlgeschlagen!"));
#else
              client.println(F("ERROR: set failed!"));
#endif
            }else{

              // Decode the xmit telegram and send it to the PC serial interface
              if(verbose) {
                printTelegram(tx_msg);
#ifdef LOGGER
                LogTelegram(tx_msg);
#endif
              }

              // Decode the rcv telegram and send it to the PC serial interface
              printTelegram(msg);   // send to hardware serial interface
#ifdef LOGGER
              LogTelegram(msg);
#endif
              if(outBufLen>0){
                client.println(outBuf);
                client.println(F("<br>"));
              }
            }
          }
          webPrintFooter();
          break;
        }

        if(p[1]=='Y'){
#ifdef DEBUG
          webPrintHeader();
          uint8_t type = strtol(&p[2],NULL,16);
          uint32_t c = (uint32_t)strtoul(&p[5],NULL,16);
          if(!bus.Send(type, c, msg, tx_msg)){
            Serial.println(F("bus send failed"));  // to PC hardware serial I/F
          }else{
            // Decode the xmit telegram and send it to the PC serial interface
            printTelegram(tx_msg);
#ifdef LOGGER
            LogTelegram(tx_msg);
#endif
          }
          // Decode the rcv telegram and send it to the PC serial interface
          printTelegram(msg);   // send to hardware serial interface
#ifdef LOGGER
          LogTelegram(msg);
#endif
          if(outBufLen>0){
            client.println(outBuf);
            client.println(F("<br>"));
          }
          for (int i=0;i<tx_msg[len_idx]+bus_type;i++) {
            if (tx_msg[i] < 16) client.print(F("0"));  // add a leading zero to single-digit values
            client.print(tx_msg[i], HEX);
            client.print(F(" "));
          }
          client.println(F("<br>"));
          for (int i=0;i<msg[len_idx]+bus_type;i++) {
            if (msg[i] < 16) client.print(F("0"));  // add a leading zero to single-digit values
            client.print(msg[i], HEX);
            client.print(F(" "));
          }
          webPrintFooter();
#endif
          break;
        }
        
        if(p[1]=='O') {   // display URL command list
          webPrintHeader();

          printPStr(pgm_get_far_address(url_command_html), sizeof(url_command_html));

#ifndef ONE_WIRE_BUS
#ifdef LANG_DE
          client.println(F(" <tr bgcolor=#f0f0f0><td valign=top></td><td>Aktiviere das Definement <code>#define ONE_WIRE_BUS</code> in BSB_lan_config.h für den folgenden Befehl:</td></tr>"));
          client.println(F(" <tr bgcolor=#f0f0f0><td valign=top>/T</td><td>Abfrage von angeschlossenen DS18B20 Temperatursensoren (optional).</td></tr>"));
#else
          client.println(F(" <tr bgcolor=#f0f0f0><td valign=top></td><td>Activate definement <code>#define ONE_WIRE_BUS</code> in BSB_lan_config.h for the following command:</td></tr>"));
          client.println(F(" <tr bgcolor=#f0f0f0><td valign=top>/T</td><td>Query values of connected ds18b20 temperature sensors (optional).</td></tr>"));
#endif  
#else
#ifdef LANG_DE
          client.println(F(" <tr><td valign=top>/T</td><td>Abfrage von angeschlossenen DS18B20 Temperatursensoren (optional).</td></tr>"));
#else
          client.println(F(" <tr><td valign=top>/T</td><td>Query values of connected ds18b20 temperature sensors (optional).</td></tr>"));
#endif
#endif

#ifndef DHT_BUS
#ifdef LANG_DE
          client.println(F("<tr bgcolor=#f0f0f0><td valign=top></td><td>Aktiviere das Definement <code>#define DHT_BUS</code> in BSB_lan_config.h für den folgenden Befehl:</td></tr>"));
          client.println(F("<tr bgcolor=#f0f0f0><td valign=top>/H</td><td>Abfrage von DHT22 Feuchtigkeits-/Temperatursensoren (optional).</td></tr>"));
#else
          client.println(F("<tr bgcolor=#f0f0f0><td valign=top></td><td>Activate definement <code>#define DHT_BUS</code> in BSB_lan_config.h for the following command:</td></tr>"));
          client.println(F("<tr bgcolor=#f0f0f0><td valign=top>/H</td><td>Query values of connected DHT22 humidity/temperature sensors (optional).</td></tr>"));
#endif
#else
#ifdef LANG_DE
          client.println(F("<tr><td valign=top>/H</td><td>Abfrage von DHT22 Feuchtigkeits-/Temperatursensoren (optional).</td></tr>"));
#else
          client.println(F("<tr><td valign=top>/H</td><td>Query values of connected DHT22 humidity/temperature sensors (optional).</td></tr>"));
#endif
#endif

#ifndef LOGGER
#ifdef LANG_DE
          client.println(F("<tr bgcolor=#f0f0f0><td valign=top></td><td>Aktiviere das Definement <code>#define LOGGER</code> in BSB_lan_config.h für die folgenden Befehle:</td></tr>"));
          client.println(F("<tr bgcolor=#f0f0f0><td valign=top>/D</td><td>Darstellung des Logfiles datalog.txt auf der microSD-Karte.</td></tr>"));
          client.println(F("<tr bgcolor=#f0f0f0><td valign=top>/D0</td><td>Löschen bzw. Zurücksetzen des Logfiles datalog.txt auf der microSD-Karte.</td></tr>"));
          client.println(F("<tr bgcolor=#f0f0f0><td valign=top>/L=x,y,z</td><td>Setzt das Logging-Intervall auf x Sekunden und (optional) die Logging-Parameter auf y und z (bis zu 20 Parameter). Um das Loggen zu deaktivieren, kann L=0,0 genutzt werden.</td></tr>"));
          client.println(F("<tr bgcolor=#f0f0f0><td valign=top>/LU=x</td><td>Wenn Bus-Telegramme geloggt werden (Logging-Parameter 30000 als einzigen Parameter setzen!), logge nur unbekannte commandIDs (x=1) oder alle Telegramme (x=0).</td></tr>"));
          client.println(F("<tr bgcolor=#f0f0f0><td valign=top>/LU=x</td><td>Wenn Bus-Telegramme geloggt werden (Logging-Parameter 30000 als einzigen Parameter setzen!), logge nur Broadcast-Telegramme (x=1) oder alle Telegramme (x=0).</td></tr>"));
#else
          client.println(F("<tr bgcolor=#f0f0f0><td valign=top></td><td>Activate definement <code>#define LOGGER</code> in BSB_lan_config.h for the following commands:</td></tr>"));
          client.println(F("<tr bgcolor=#f0f0f0><td valign=top>/D</td><td>Dump logged data from datalog.txt on micro SD card.</td></tr>"));
          client.println(F("<tr bgcolor=#f0f0f0><td valign=top>/D0</td><td>Delete datalog.txt on micro SD card.</td></tr>"));
          client.println(F("<tr bgcolor=#f0f0f0><td valign=top>/L=x,y,z</td><td>Set logging interval to x seconds and (optionally) sets logging parameters to y and z (up to 20). To deactivate logging, you can use L=0,0.</td></tr>"));
          client.println(F("<tr bgcolor=#f0f0f0><td valign=top>/LU=x</td><td>When logging bus telegrams (logging parameter 30000 only), log only unknown command IDs (x=1) or all (x=0) telegrams.</td></tr>"));
          client.println(F("<tr bgcolor=#f0f0f0><td valign=top>/LU=x</td><td>When logging bus telegrams (logging parameter 30000 only), log only broadcast telegrams (x=1) or all (x=0) telegrams.</td></tr>"));
#endif  
#else
#ifdef LANG_DE
          client.println(F("<tr><td valign=top>/D</td><td>Darstellung des Logfiles datalog.txt auf der microSD-Karte.</td></tr>"));
          client.println(F("<tr><td valign=top>/D0</td><td>Löschen bzw. Zurücksetzen des Logfiles datalog.txt auf der microSD-Karte.</td></tr>"));
          client.println(F("<tr><td valign=top>/L=x,y,z</td><td>Setzt das Logging-Intervall auf x Sekunden und (optional) die Logging-Parameter auf y und z (bis zu 20 Parameter). Um das Loggen zu deaktivieren, kann L=0,0 genutzt werden.</td></tr>"));
          client.println(F("<tr><td valign=top>/LU=x</td><td>Wenn Bus-Telegramme geloggt werden (Logging-Parameter 30000 als einzigen Parameter setzen!), logge nur unbekannte commandIDs (x=1) oder alle Telegramme (x=0).</td></tr>"));
          client.println(F("<tr><td valign=top>/LU=x</td><td>Wenn Bus-Telegramme geloggt werden (Logging-Parameter 30000 als einzigen Parameter setzen!), logge nur Broadcast-Telegramme (x=1) oder alle Telegramme (x=0).</td></tr>"));
#else
          client.println(F("<tr><td valign=top>/D</td><td>Dump logged data from datalog.txt on micro SD card.</td></tr>"));
          client.println(F("<tr><td valign=top>/D0</td><td>Delete datalog.txt on micro SD card.</td></tr>"));
          client.println(F("<tr><td valign=top>/L=x,y,z</td><td>Set logging interval to x seconds and (optionally) sets logging parameters to y and z (up to 20). To deactivate logging, you can use L=0,0.</td></tr>"));
          client.println(F("<tr><td valign=top>/LU=x</td><td>When logging bus telegrams (logging parameter 30000 only), log only unknown command IDs (x=1) or all (x=0) telegrams.</td></tr>"));
          client.println(F("<tr><td valign=top>/LB=x</td><td>When logging bus telegrams (logging parameter 30000 only), log only broadcast telegrams (x=1) or all (x=0) telegrams.</td></tr>"));
#endif
#endif

#ifndef RESET
#ifdef LANG_DE
          client.println(F(" <tr bgcolor=#f0f0f0><td valign=top></td><td>Aktiviere das Definement <code>#define RESET</code> in BSB_lan_config.h für den folgenden Befehl:</td></tr>"));
          client.println(F(" <tr bgcolor=#f0f0f0><td valign=top>/X</td><td>Reset des Arduino durchführen.</td></tr>"));
#else
          client.println(F(" <tr bgcolor=#f0f0f0><td valign=top></td><td>Activate definement <code>#define RESET</code> in BSB_lan_config.h for the following command:</td></tr>"));
          client.println(F(" <tr bgcolor=#f0f0f0><td valign=top>/X</td><td>Execute a reset of the Arduino.</td></tr>"));
#endif  
#else
#ifdef LANG_DE
          client.println(F(" <tr><td valign=top>/X</td><td>Reset des Arduino durchführen.</td></tr>"));
#else
          client.println(F(" <tr><td valign=top>/X</td><td>Execute a reset of the Arduino.</td></tr>"));
#endif
#endif

          client.print(F("</table>"));

#ifdef LANG_DE
          client.println(F("Mehrere Abfragen können miteinander verkettet werden, z.B. <code>/K0/710/8000-8999/T</code></p>"));
#else
          client.println(F("Multiple queries are possible, e.g. <code>/K0/710/8000-8999/T</code></p>"));
#endif
          webPrintFooter();
          break;
        }
          
#ifdef LOGGER            
        if(p[1]=='D'){ // access datalog file
          if (p[2]=='0') {  // remove datalog file
            webPrintHeader();
            SD.remove("datalog.txt");
            File dataFile = SD.open("datalog.txt", FILE_WRITE);
            if (dataFile) {
              dataFile.println(F("Milliseconds;Date;Parameter;Description;Value"));
              dataFile.close();
#ifdef LANG_DE
              client.println(F("Datei datalog.txt entfernt und neu generiert."));
#else
              client.println(F("File datalog.txt removed and recreated."));
#endif
              Serial.println(F("File datalog.txt removed and recreated."));
            } else {
              client.println(F("Failed to create datalog.txt."));
            }
            webPrintFooter();
          } else if (p[2]=='G') {
            webPrintHeader();
      	    client.println(F("<A HREF='D'>Download Data</A><div align=center></div>"));

            printPStr(pgm_get_far_address(graph_html), sizeof(graph_html));

            webPrintFooter();
          } else {  // dump datalog file
            client.println(F("HTTP/1.1 200 OK"));
            client.println(F("Content-Type: text/plain"));
            client.println();

            File dataFile = SD.open("datalog.txt");
            // if the file is available, read from it:
            if (dataFile) {

              unsigned long startdump = millis();
              int logbuflen = 512;
              byte loglineBuf[logbuflen];
              int chars_read = 0;

              chars_read = dataFile.read(&loglineBuf , logbuflen);
              while (chars_read == logbuflen) {
                client.write(loglineBuf,logbuflen);
                chars_read = dataFile.read(&loglineBuf , logbuflen);
              }
              //final packet
              if (chars_read > 0) client.write(loglineBuf, chars_read);
              dataFile.close();

              Serial.print(F("Duration: "));
              Serial.println(millis()-startdump);
            } else {
              client.println(F("Failed to open datalog.txt"));
            }
          }
          break;
        }
#endif
        if (p[1]=='C'){ // dump configuration
          webPrintHeader();
#ifdef LANG_DE
          client.println(F("Konfiguration<BR><BR>"));
//          client.println(F("Heizungstyp-spezifische Konfiguration:"));
#else
          client.println(F("Configuration<BR><BR>"));
//          client.println(F("Special heating system configuration:"));
#endif
/*
          #ifdef THISION 
          client.println(F("Thision "));
          #endif
          #ifdef FUJITSU
          client.println(F("Fujitsu "));
          #endif
          #ifdef BROETJE
          client.println(F("Brötje "));
          #endif
          #ifdef BROETJE_SOB
          client.println(F("Brötje SOB "));
          #endif
          #ifdef BROETJE_BSW
          client.println(F("Brötje BSW "));
          #endif
          client.println(F("<BR>"));
*/

//          client.println(F("BSB pins: "));
//          client.println(bus);
//          client.println(F("<BR><BR>"));

          client.print(F("RAM: "));
          client.print(freeRam());
          client.println(F("Bytes <BR>"));
          client.print(F("Bus-System: "));
          if (bus_type == 1) {
            client.print(F("LPB"));
          } else {
            client.print(F("BSB"));
          }
          client.println(F("<BR>"));
#ifdef LANG_DE
          client.print(F("Monitor Modus: "));
#else
          client.print(F("Monitor mode: "));
#endif
          client.println(monitor);
          client.print(F("<BR>"));
#ifdef LANG_DE
          client.print(F("Verbositäts-Level: "));
#else
          client.print(F("Verbosity level: "));
#endif
          client.print(verbose);
          client.println(F("<BR>"));
          
          #ifdef ONE_WIRE_BUS
          client.println(F("1-Wire bus pins: "));
          client.println(ONE_WIRE_BUS);
          client.println(F("<BR>"));
          #endif

          #ifdef DHT_BUS
          client.println(F("DHT22 bus pins: "));
          client.println(DHT_BUS);
          client.println(F("<BR>"));
          #endif

          client.println(F("Excluded GPIO pins: "));
          for (int i=0; i<anz_ex_gpio; i++) {
            client.print (exclude_GPIO[i]);
            client.print(F(" "));
          }
          client.println(F("<BR>"));
          
          client.println(F("MAC address: "));
          for (int i=0; i<=5; i++) {
            if (mac[i] < 10) {
              client.print(F("0"));
            }
            client.print(mac[i], HEX);
            client.print(F(" "));
          }
          client.println(F("<BR>"));

/*
          client.println(F("IP address: "));
          client.println(ip);
          client.println(F("<BR>"));
*/

#ifdef LANG_DE
          client.println(F("Berechnung von 24h-Mittelwerten für die folgenden Parameter: <BR>"));
#else
          client.println(F("Calculating 24h averages for the following parameters: <BR>"));
#endif
          for (int i=0; i<numAverages; i++) {
            if (avg_parameters[i] > 0) {
              client.print (avg_parameters[i]);
              client.print(F(" - "));
              client.print(lookup_descr(avg_parameters[i])); 
              client.println(F("<BR>"));
            }
          }
          client.println(F("<BR>"));

          #ifdef LOGGER
#ifdef LANG_DE
          client.println(F("Loggen der folgenden Parameter alle "));
#else
          client.println(F("Logging the following parameters every "));
#endif
          client.print(log_interval);
#ifdef LANG_DE
          client.println(F(" Sekunden: <BR>"));
#else
          client.println(F(" seconds: <BR>"));
#endif
          for (int i=0; i<numLogValues; i++) {
            if (log_parameters[i] > 0) {
              client.print (log_parameters[i]);
              client.print(F(" - "));
              if (log_parameters[i] < 20000) {
                client.print(lookup_descr(log_parameters[i])); 
              } else {
                if (log_parameters[i] == 20000) {
                  client.print(F("Brennerlaufzeit"));
                }
                if (log_parameters[i] == 20001) {
                  client.print(F("Brennertakte"));
                }
                if (log_parameters[i] == 20002) {
                  client.print(F("TWW-Laufzeit"));
                }
                if (log_parameters[i] == 20003) {
                  client.print(F("TWW-Takte"));
                }
                if (log_parameters[i] == 20004) {
#ifdef LANG_DE
                  client.println(F("24h-Mittelwerte (s.o.)"));
#else
                  client.println(F("24h averages (see above)"));
#endif
                }
                if (log_parameters[i] >= 20100 && log_parameters[i] < 20200) {
                  client.print(F("DHT22-Sensor"));
                }
                if (log_parameters[i] >= 20200 && log_parameters[i] < 20300) {
                  client.print(F("1-Wire-Sensor"));
                }
                if (log_parameters[i] == 30000) {
#ifdef LANG_DE
                  client.println(F("BSB-Datentelegramme (unabhängig vom Intervall)<BR>"));
                  client.println(F("Nur unbekannte Bus-Telegramme loggen: "));
#else
                  client.print(F("BSB-Data telegrams (irrespective of interval)<BR>"));
                  client.print(F("Logging unknown bus telegrams only: "));
#endif
                  if (log_unknown_only) {
#ifdef LANG_DE
                    client.println(F("Ja<BR>"));
#else
                    client.println(F("yes<BR>"));
#endif
                  } else {
#ifdef LANG_DE
                    client.println(F("Nein<BR>"));
#else
                    client.println(F("no<BR>"));
#endif
                  }
#ifdef LANG_DE
                  client.println(F("Nur Broadcast Bus-Telegramme loggen: "));
#else
                  client.print(F("Logging broadcast bus telegrams only: "));
#endif
                  if (log_bc_only) {
#ifdef LANG_DE
                    client.println(F("Ja<BR>"));
#else
                    client.println(F("yes<BR>"));
#endif
                  } else {
#ifdef LANG_DE
                    client.println(F("Nein<BR>"));
#else
                    client.println(F("no<BR>"));
#endif
                  }
                }
              }
              client.println(F("<BR>"));
            }
          }
          #endif

          client.println(F("<BR>"));
          webPrintFooter();
          break;
        }
        if (p[1]=='L' && p[2]=='B' && p[3]=='='){
          if (p[4]=='1') {
            log_bc_only=1;
          } else {
            log_bc_only=0;
          }
          webPrintHeader();
#ifdef LANG_DE
          client.print(F("Nur Broadcast Bus-Telegramme loggen: "));
#else
          client.print(F("Logging broadcast bus telegrams only: "));
#endif
          if (log_bc_only) {
#ifdef LANG_DE
            client.println(F("Ja<BR>"));
#else
            client.println(F("yes<BR>"));
#endif
          } else {
#ifdef LANG_DE
            client.println(F("Nein<BR>"));
#else
            client.println(F("no<BR>"));
#endif
          }
          webPrintFooter();
          break;
        }
        if (p[1]=='L' && p[2]=='U' && p[3]=='='){
          if (p[4]=='1') {
            log_unknown_only=1;
          } else {
            log_unknown_only=0;
          }
          webPrintHeader();
#ifdef LANG_DE
          client.print(F("Nur unbekannte Bus-Telegramme loggen: "));
#else
          client.print(F("Logging unknown bus telegrams only: "));
#endif
          if (log_unknown_only) {
#ifdef LANG_DE
            client.println(F("Ja<BR>"));
#else
            client.println(F("yes<BR>"));
#endif
          } else {
#ifdef LANG_DE
            client.println(F("Nein<BR>"));
#else
            client.println(F("no<BR>"));
#endif
          }
          webPrintFooter();
          break;
        }
        if (p[1]=='L' && p[2]=='='){ // logging configuration: L=<interval>,<parameter 1>,<parameter 2>,...,<parameter20>
          webPrintHeader();
          char* log_token = strtok(p,"=,");  // drop everything before "="
          log_token = strtok(NULL, "=,");   // first token: interval
          if (log_token != 0) {
            log_interval = atoi(log_token);
//            if (log_interval < 10) {log_interval = 10;}
            lastLogTime = millis();
#ifdef LANG_DE
            client.print(F("Neues Log-Intervall: "));
            client.print(log_interval);
            client.println(F(" Sekunden<BR>"));
#else
            client.print(F("New logging interval: "));
            client.print(log_interval);
            client.println(F(" seconds<BR>"));
#endif
          }
          log_token = strtok(NULL,"=,");    // subsequent tokens: logging parameters
          int token_counter = 0;
          if (log_token != 0) {
            for (int i=0;i<numLogValues;i++) {
              log_parameters[i] = 0;
            }
#ifdef LANG_DE
            client.println(F("Neue Log-Parameter: "));
#else
            client.println(F("New logging parameters: "));
#endif
          }
          while (log_token!=0) {
            int log_parameter = atoi(log_token);
            if (token_counter < numLogValues) {
              log_parameters[token_counter] = log_parameter; 
              client.print(log_parameters[token_counter]);
              client.println(F(" "));
              token_counter++;
            }
            log_token = strtok(NULL,"=,");
          }
          webPrintFooter();
          break;
        }
        if (p[1]=='P') {
          webPrintHeader();
          client.print(F("Bus-System: "));
          if (p[2]=='1') {
            bus_type=bus.setBusType(BUS_LPB);
            len_idx = 1;
            pl_start = 13;
            client.println(F("LPB"));
          } else {
            bus_type=bus.setBusType(BUS_BSB);
            len_idx = 3;
            pl_start = 9;
            client.println(F("BSB"));
          }
          SetDevId();
          webPrintFooter();
          break;
        }
        if (p[1]=='X'){           // Reset Arduino
#ifdef RESET
          webPrintHeader();
          client.println(F("Reset..."));
          webPrintFooter();
          client.stop();
#ifdef LOGGER
          File dataFile = SD.open("datalog.txt", FILE_WRITE);
          if (dataFile) {
            dataFile.close();
          }
#endif
          asm volatile ("  jmp 0");
          while (1==1) {}
#endif
          break;
        }
        // print queries
        webPrintHeader();
        char* range;
        char* line_start;
        char* line_end;
        int start=-1;
        int end=-1;
        range = strtok(p,"/");
        while(range!=0){
          if(range[0]=='T'){
#ifdef ONE_WIRE_BUS
            ds18b20();
#endif
          }else if(range[0]=='H'){ // handle humidity command
#ifdef DHT_BUS
            dht22();
#endif
/*
#ifdef LANG_DE
            client.println(F("<a href='javascript:history.back()'>Zur&uuml;ck</a>"));
#else
            client.println(F("<a href='javascript:history.back()'>Back</a>"));
#endif
*/
          }else if(range[0]=='A') { // handle average command
            if (range[1]=='=') {
              char* avg_token = strtok(range,"=,");  // drop everything before "="
              avg_token = strtok(NULL,"=,");    // subsequent tokens: average parameters
              int token_counter = 0;
              if (avg_token != 0) {
                for (int i=0;i<numAverages;i++) {
                  avg_parameters[i] = 0;
                  avgValues[i] = 0;
                  avgValues_Old[i] = -9999;
                  avgValues_Current[i] = 0;
                }
                avgCounter = 1;
#ifdef LOGGER
                SD.remove("averages.txt");
#endif
#ifdef LANG_DE
                client.println(F("Neue Durchschnitts-Parameter: "));
#else
                client.println(F("New average parameters: "));
#endif
              }
              while (avg_token!=0) {
                int avg_parameter = atoi(avg_token);
                if (token_counter < numAverages) {
                  avg_parameters[token_counter] = avg_parameter; 
                  client.print(avg_parameters[token_counter]);
                  client.println(F(" "));
                  token_counter++;
                }
                avg_token = strtok(NULL,"=,");
              }
            } else {
              for (int i=0; i<numAverages; i++) {
                if (avg_parameters[i] > 0) {
                  client.print(F("<tr><td>"));
                  client.print(avg_parameters[i]);
                  client.print(F(" Avg"));
                  client.print(lookup_descr(avg_parameters[i]));            
                  client.print(F(": "));
                  double rounded = round(avgValues[i]*10);
                  client.println(rounded/10);
// TODO: extract and display unit text from cmdtbl.type
                  client.println(F("</td></tr>"));
                }
              }
/*
#ifdef LANG_DE
              client.println(F("<a href='javascript:history.back()'>Zur&uuml;ck</a>"));
#else
              client.println(F("<a href='javascript:history.back()'>Back</a>"));
#endif
*/
            }
          }else if(range[0]=='G'){ // handle gpio command
            uint8_t val;
            uint8_t pin;
            bool error = false;
            p=range+1;
            if(!isdigit(*p)){   // now we check for digits
#ifdef LANG_DE
              client.println(F("FEHLER: Ungültiger Parameter!"));
#else
              client.println(F("ERROR: invalid parameter line!"));
#endif
              break;
            }
            pin=(uint8_t)atoi(p);       // convert until non-digit char is found
            for (int i=0; i < anz_ex_gpio; i++) {
              if (pin==exclude_GPIO[i]) {
                error = true;
              }
            }
            if (error==true) {
#ifdef LANG_DE
              client.println(F("FEHLER: Geschützter GPIO-Pin!"));
#else
              client.println(F("ERROR: protected GPIO pin!"));
#endif
              break;
            }
            p=strchr(p,'=');    // search for '=' sign
            if(p==NULL){        // no match -> query value
              val=digitalRead(pin);
            }else{ // set value
              p++;
              if(!strncasecmp(p,"on",2) || !strncasecmp(p,"high",2) || *p=='1'){
                val=HIGH;
              }else{
                val=LOW;
              }
              digitalWrite(pin, val);
              pinMode(pin, OUTPUT); // TODO: does this case a problem if already set as output?
            }
            client.print(F("GPIO"));
            client.print(pin);
            client.print(F(": "));
            client.print(val!=LOW?F("1"):F("0"));
          }else if(range[0]=='B'){
            if(range[1]=='0'){ // reset furnace duration
#ifdef LANG_DE
              client.println(F("Heizkessel- und TWW-Laufzeit auf null gesetzt.<br>"));
#else
              client.println(F("Furnace and hot water duration is set to zero.<br>"));
#endif
              brenner_duration=0;
              brenner_count=0;
              brenner_duration_2=0;
              brenner_count_2=0;
              TWW_duration=0;
              TWW_count=0;
            }else{
              // query brenner duration
              client.print(F("<tr><td>Brenner Laufzeit Stufe 1: "));
              client.print(brenner_duration);
              client.println(F("</td></tr><tr><td>"));
              client.print(F("Brenner Takte Stufe 1: "));
              client.print(brenner_count);
              client.println(F("</td></tr>"));
              client.print(F("<tr><td>Brenner Laufzeit Stufe 2: "));
              client.print(brenner_duration_2);
              client.println(F("</td></tr><tr><td>"));
              client.print(F("Brenner Takte Stufe 2: "));
              client.print(brenner_count_2);
              client.println(F("</td></tr>"));
              client.print(F("<tr><td>TWW Laufzeit: "));
              client.print(TWW_duration);
              client.println(F("</td></tr><tr><td>"));
              client.print(F("TWW Takte: "));
              client.print(TWW_count);
              client.println(F("</td></tr>"));
/*
#ifdef LANG_DE
              client.println(F("<a href='javascript:history.back()'>Zur&uuml;ck</a>"));
#else
              client.println(F("<a href='javascript:history.back()'>Back</a>"));
#endif
*/

            }
          }else{
            if(range[0]=='K'){
              uint8_t cat,search_cat;
              uint16_t line;
              int i;
              uint32_t c;
              i=0;
              start=-1;
              end=-1;
              search_cat=atoi(&range[1]);
              c=pgm_read_dword_far(pgm_get_far_address(cmdtbl[0].cmd) + i * sizeof(cmdtbl[0]));
//              c=pgm_read_dword(&cmdtbl[i].cmd);
              while(c!=CMD_END){
                cat=pgm_read_byte_far(pgm_get_far_address(cmdtbl[0].category) + i * sizeof(cmdtbl[0]));
//                cat=pgm_read_byte(&cmdtbl[i].category);
                if(cat==search_cat){
                  if(start<0){
                    line=pgm_read_word_far(pgm_get_far_address(cmdtbl[0].line) + i * sizeof(cmdtbl[0]));
//                    line=pgm_read_word(&cmdtbl[i].line);
                    start=line;
                  }
                }else{
                  if(start>=0){
                    line=pgm_read_word_far(pgm_get_far_address(cmdtbl[0].line) + (i-1) * sizeof(cmdtbl[0]));
//                    line=pgm_read_word(&cmdtbl[i-1].line);
                    end=line;
                    break;
                  }
                }
                i++;
                c=pgm_read_dword_far(pgm_get_far_address(cmdtbl[0].cmd) + i * sizeof(cmdtbl[0]));
//                c=pgm_read_dword(&cmdtbl[i].cmd);
              }
              if(end<start){
                end=start;
              }
            }else{
              // split range
              line_start=range;
              line_end=strchr(range,'-');
              if(line_end==NULL){
                line_end=line_start;
              }else{
                *line_end='\0';
                line_end++;
              }
              start=atoi(line_start);
              end=atoi(line_end);
            }
            query(start,end,0);
          }

          range = strtok(NULL,"/");
        } // endwhile
/*
#ifdef LANG_DE
        client.println(F("</table></form><a href='javascript:history.back()'>Zur&uuml;ck</a></p>"));   // finish HTML paragraph
#else
        client.println(F("</table></form><a href='javascript:history.back()'>Back</a></p>"));   // finish HTML paragraph
#endif
*/
        webPrintFooter();
        break;
      } // endif, client available
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  } // endif, client

#ifdef LOGGER

  if ((millis() - lastLogTime >= (log_interval * 1000)) && log_interval > 0) {
//    SetDateTime(); // receive inital date/time from heating system
    double log_values[numLogValues];
    for (int i=0; i < numLogValues; i++) {
      if (log_parameters[i] > 0) {
        log_values[i] = strtod(query(log_parameters[i],log_parameters[i],1),NULL);
      }
    }
    File dataFile = SD.open("datalog.txt", FILE_WRITE);

    if (dataFile) {
      for (int i=0; i < numLogValues; i++) {
        if (log_parameters[i] > 0 && log_parameters[i] != 20002 && log_parameters[i] != 30000) {
          dataFile.print(millis());
          dataFile.print(F(";"));
          dataFile.print(GetDateTime(date)); // get current time from heating system
          dataFile.print(F(";"));
          dataFile.print(log_parameters[i]);
          dataFile.print(F(";"));
        }
        if (log_parameters[i] > 0 && log_parameters[i] < 20000) {
          dataFile.print(lookup_descr(log_parameters[i]));
          dataFile.print(F(";"));
          dataFile.println(log_values[i]);
        } else {
          if (log_parameters[i] == 20000) {
            dataFile.print(F("Brennerlaufzeit Stufe 1"));
            dataFile.print(F(";"));
            dataFile.println(brenner_duration);
          }
          if (log_parameters[i] == 20001) {
            dataFile.print(F("Brennertakte Stufe 1"));
            dataFile.print(F(";"));
            dataFile.println(brenner_count);
          }
          if (log_parameters[i] == 20002) {
            dataFile.print(F("Brennerlaufzeit Stufe 2"));
            dataFile.print(F(";"));
            dataFile.println(brenner_duration_2);
          }
          if (log_parameters[i] == 20003) {
            dataFile.print(F("Brennertakte Stufe 2"));
            dataFile.print(F(";"));
            dataFile.println(brenner_count_2);
          }
          if (log_parameters[i] == 20004) {
            dataFile.print(F("TWW-Laufzeit"));
            dataFile.print(F(";"));
            dataFile.println(TWW_duration);
          }
          if (log_parameters[i] == 20005) {
            dataFile.print(F("TWW-Takte"));
            dataFile.print(F(";"));
            dataFile.println(TWW_count);
          }
          if (log_parameters[i] == 20006) {
            for (int i=0; i<numAverages; i++) {
              if (avg_parameters[i] > 0) {
                dataFile.print(millis());
                dataFile.print(F(";"));
                dataFile.print(GetDateTime(date)); // get current time from heating system
                dataFile.print(F(";"));
                dataFile.print(avg_parameters[i]);
                dataFile.print(F(";"));
                dataFile.print(F("Avg_"));
                dataFile.print(lookup_descr(avg_parameters[i]));            
                dataFile.print(F(";"));
                double rounded = round(avgValues[i]*10);
                dataFile.println(rounded/10);
// TODO: extract and display unit text from cmdtbl.type
              }
            }
          }
#ifdef DHT_BUS
          if (log_parameters[i] >= 20100 && log_parameters[i] < 20200) {
            int log_sensor = log_parameters[i] - 20100;
            int chk = DHT.read22(DHT_Pins[log_sensor]);
            Serial.println(chk);
            double hum = DHT.humidity;
            double temp = DHT.temperature;
            if (hum > 0 && hum < 101) {
              dataFile.print(F("Temperature "));
              dataFile.print(log_sensor);
              dataFile.print(F(";"));
              dataFile.println(temp);

              dataFile.print(millis());
              dataFile.print(F(";"));
              dataFile.print(GetDateTime(date)); // get current time from heating system
              dataFile.print(F(";"));
              dataFile.print(log_parameters[i]);
              dataFile.print(F(";"));
              dataFile.print(F("Humidity "));
              dataFile.print(log_sensor);
              dataFile.print(F(";"));
              dataFile.println(hum);
             }
          }
#endif
#ifdef ONE_WIRE_BUS
          if (log_parameters[i] >= 20200 && log_parameters[i] < 20300) {
            int log_sensor = log_parameters[i] - 20200;
            sensors.requestTemperatures(); // Send the command to get temperatures
            float t=sensors.getTempCByIndex(log_sensor);
            dataFile.print(F("Temperature "));
            dataFile.print(log_sensor);
            dataFile.print(F(";"));
            dataFile.println(t);
          }
#endif
        }
      } 
      dataFile.close();
   } else {
    // if the file isn't open, pop up an error:
#ifdef LANG_DE
      client.println(F("Fehler beim Öffnen der Datei datalog.txt!"));
#else
      client.println(F("Error opening datalog.txt!"));
       #endif
      Serial.println(F("Error opening datalog.txt!"));
    }
    lastLogTime += log_interval * 1000;
  }
#endif

// Calculate 24h averages
  if (millis() - lastAvgTime >= 60000) {
    if (avgCounter == 1441) {
      for (int i=0; i<numAverages; i++) {
        avgValues_Old[i] = avgValues[i];
        avgValues_Current[i] = 0;
      }
      avgCounter = 1;
    }
    for (int i=0; i<numAverages; i++) {
      if (avg_parameters[i] > 0) {
        double reading = strtod(query(avg_parameters[i],avg_parameters[i],1),NULL);
        if (isnan(reading)) {} else {
          avgValues_Current[i] = (avgValues_Current[i] * (avgCounter-1) + reading) / avgCounter;
          if (avgValues_Old[i] == -9999) {
            avgValues[i] = avgValues_Current[i];
          } else {
            avgValues[i] = ((avgValues_Old[i]*(1440-avgCounter))+(avgValues_Current[i]*avgCounter)) / 1440;
          }
        }
      }
    }

    avgCounter++;
    lastAvgTime += 60000;

#ifdef LOGGER

// write averages to SD card to protect from power off

    if (SD.exists("averages.txt")) {
      SD.remove("averages.txt");
    }
    File avgfile = SD.open("averages.txt", FILE_WRITE);
    if (avgfile) {
      for (int i=0; i<numAverages; i++) {
        avgfile.println(avgValues[i]);
        avgfile.println(avgValues_Old[i]);
        avgfile.println(avgValues_Current[i]);
      }
      avgfile.println(avgCounter);
      avgfile.close();
    }

#endif

// while we are here, update date/time as well...
//    SetDateTime();      
  }
// end calculate averages

} // --- loop () ---

/** *****************************************************************
 *  Function: setup()
 *  Does:     Sets up the Arduino including its Ethernet shield.
 * Pass parameters:
 *  none
 * Parameters passed back:
 *  none
 * Function value returned:
 *  none
 * Global resources used:
 *  numSensors
 *  server instance
 *  sensors instance
 *  Serial instance
 *  Ethernet instance
 * *************************************************************** */
void setup() {

  if (bus_type == 0) {
    len_idx = 3;
    pl_start = 9;
  } else {
    len_idx = 1;
    pl_start = 13;
  }

  // The computer hardware serial interface #0:
  //   115,800 bps, 8 data bits, no parity
  Serial.begin(115200, SERIAL_8N1); // hardware serial interface #0
  Serial.println(F("READY"));
  Serial.print(F("free RAM:"));
  Serial.println(freeRam());

#ifdef LOGGER
  // disable w5100 while setting up SD
  pinMode(10,OUTPUT);
  digitalWrite(10,HIGH);
  Serial.print(F("Starting SD.."));
  if(!SD.begin(4)) Serial.println(F("failed"));
  else Serial.println(F("ok"));

#else
  // enable w5100 SPI
  pinMode(10,OUTPUT);
  digitalWrite(10,LOW);

  // disable SD Card
  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);
#endif

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
#ifdef LOGGER
  digitalWrite(10,HIGH);
#endif
  server.begin();

#ifdef ONE_WIRE_BUS
  // check ds18b20 sensors
  sensors.begin();
  numSensors=sensors.getDeviceCount();
  Serial.print(F("numSensors: "));
  Serial.println(numSensors);
#endif

// initialize average calculation

  for (int i=0; i<numAverages; i++) {
    avgValues[i] = 0;
    avgValues_Old[i] = -9999;
    avgValues_Current[i] = 0;
  }

#ifdef LOGGER

// restore average 

  if (SD.exists("averages.txt")) {
    File avgfile = SD.open("averages.txt", FILE_READ);
    if (avgfile) {
      char c;
      char num[10];
      int x;
      for (int i=0; i<numAverages; i++) {
        c = avgfile.read();
        x = 0;
        while (avgfile.available() && c != '\n') {
          num[x] = c;
          x++;
          c = avgfile.read();
        }
        num[x]='\0';
        avgValues[i] = atof(num);

        c = avgfile.read();
        x = 0;
        while (avgfile.available() && c != '\n') {
          num[x] = c;
          x++;
          c = avgfile.read();
        }
        num[x]='\0';
        avgValues_Old[i] = atof(num);

        c = avgfile.read();
        x = 0;
        while (avgfile.available() && c != '\n') {
          num[x] = c;
          x++;
          c = avgfile.read();
        }
        num[x]='\0';
        avgValues_Current[i] = atof(num);
      }

      c = avgfile.read();
      x = 0;
      while (avgfile.available() && c != '\n') {
        num[x] = c;
        x++;
        c = avgfile.read();
      }
      num[x]='\0';
      avgCounter = atoi(num);
    }
    avgfile.close();
  }

#endif

// receive inital date/time from heating system
  SetDateTime();
  
// receive device_id (Gerätefamilie) from heating system
  SetDevId();
}

