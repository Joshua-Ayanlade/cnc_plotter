#ifndef CNC_PLOTTER_TX_H
#define CNC_PLOTTER_TX_H

#include <Arduino.h>
#include <SD.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>
#include <math.h>

class IRrecv;  //To allow to create IRrecv object (extern) to be accessible outside this file

// Pin Definitions
#define DT 8
#define CLK 9
#define okPin 24
#define nextPin 25
#define prevPin 26
#define SW 7
#define IRpin 2
#define but 10
#define rx A0
#define ry A1

// Directory Definitions
#define listItemsTotal 100   //maximum number of file names to store
#define listItemMaxChar 100  //maximum number of characters per file name, including null terminator


// --- Extern Variable Declarations ---

// Objects
extern LiquidCrystal_I2C lcd;
extern Encoder encoder;
extern IRrecv RX;
extern File myFile;  
extern File root;

// IR variables
extern unsigned long IRvalue;
extern String cmd;
extern int cmdCount;
extern int cmdEnd;


// General variables
extern unsigned long previousMillis;
extern unsigned int delayTime;
extern bool acknowledged;
extern bool inputCparameter;
extern bool inputVLparameter;
extern bool inputHLparameter;
extern bool previous;
extern bool next;
extern int counter;
extern int SAcounter;
extern int previousCLK;
extern int maxIncr;
extern int AmaxIncr;
extern int SAmaxIncr;
extern int slideMax;
extern int slide;


// Mode flags
extern bool autoMode;
extern bool semiAutoMode;
extern bool manualMode;
extern bool joyStickMode;

// Semi-Auto Circle flags
extern bool fxValue;
extern bool fyValue;
extern bool rValue;
extern bool xcValue;
extern bool ycValue;
extern bool CgcodeGen;

// Semi-Auto Line flags
extern bool x0value;
extern bool y0value;
extern bool Lvalue;
extern bool VLgcodeGen;
extern bool HLgcodeGen;

// Manual Mode flags
extern bool rxStateP;
extern bool rxStateN;
extern bool ryStateP;
extern bool ryStateN;
extern bool butState;
extern int counterX;
extern int counterY;

// G-code sending variables
extern String get_ok;
extern String readString;
extern bool okToSendGcode;
extern String execFile;

// Directory variables
extern char *listItems[listItemsTotal];
extern char tempString[listItemMaxChar];
extern byte listLength;
extern char dirx[256];

// Math variables
extern float deg[];
extern const int arrLen;

// Circle parameters
extern float r;
extern float xc;
extern float yc;
extern float fy;
extern float fx;

// V-line parameters
extern float x0;
extern float y0;
extern float l;

// Coordinate variables
extern float xx;
extern float yy;
extern float *xxArray;
extern float *yyArray;


// --- Function Prototypes ---
void setupIR();

// File system functions
void scanDir();
void freeListMemory();

// G-code sending functions
void sendGcode(int c);
void SAsendGcode(String f);
void msendGcode();
void joyStick();

// Button and Input functions
void okButton();
void prevButton();
void nextButton(int slideMax);
void rotary(int maxIncr);
void SArotary(int SAmaxIncr);

// IR remote functions
void decodeIR();
void num();

// LCD Screen functions
void homeScreen(int slide);
void semiAutoScreen(int count);
void autoScreen(int count);

// G-code generation functions (Placeholders)
float *getCoordX(float r, float xc);
float *getCoordY(float r, float yc);
void circleGcode(float fx, float fy);
void vLineGcode(float x0, float y0, float l, float fx, float fy);
void hLineGcode(float x0, float y0, float l, float fx, float fy);


#endif