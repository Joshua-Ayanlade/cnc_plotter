#include "cnc_plotter_TX.h"
#include <IRremote.h>

// Objects
LiquidCrystal_I2C lcd(0X27, 20, 4);
Encoder encoder(CLK, DT, SW);
IRrecv RX(IRpin);
File myFile;
File root;

// IR variables
unsigned long IRvalue;
String cmd = "";
int cmdCount = 0;
int cmdEnd = 0;

// General variables
unsigned long previousMillis = 0;
unsigned int delayTime = 300;
bool acknowledged = false;
bool inputCparameter = false;
bool inputVLparameter = false;
bool inputHLparameter = false;
bool previous = false;
bool next = false;
int counter = 0;
int SAcounter = 0;
int previousCLK;
int maxIncr = 12;
int AmaxIncr = 12;
int SAmaxIncr = 4;
int slideMax = 3;
int slide = 0;

// Mode flags
bool autoMode = false;
bool semiAutoMode = false;
bool manualMode = false;
bool joyStickMode = false;

// Semi-Auto Circle flags
bool fxValue = false;
bool fyValue = false;
bool rValue = false;
bool xcValue = false;
bool ycValue = false;
bool CgcodeGen = false;

// Semi-Auto Line flags
bool x0value = false;
bool y0value = false;
bool Lvalue = false;
bool VLgcodeGen = false;
bool HLgcodeGen = false;

// Manual Mode flags
bool rxStateP = false;
bool rxStateN = false;
bool ryStateP = false;
bool ryStateN = false;
bool butState = false;
int counterX = 0;
int counterY = 0;

// G-code sending variables
String get_ok;
String readString;
bool okToSendGcode = false;
String execFile;

// Directory variables
char *listItems[listItemsTotal];
char tempString[listItemMaxChar];
byte listLength = 0;
char dirx[256] = "/";

// Math variables
float deg[] = { 0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
                65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125,
                130, 135, 140, 145, 150, 155, 160, 165, 170, 175, 180, 185, 190,
                195, 200, 205, 210, 215, 220, 225, 230, 235, 240, 245, 250, 255,
                260, 265, 270, 275, 280, 285, 290, 295, 300, 305, 310, 315, 320,
                325, 330, 335, 340, 345, 350, 355, 360 };
const int arrLen = sizeof(deg) / sizeof(deg[0]);

// Circle parameters
float r;
float xc;
float yc;
float fy;
float fx;

// V-line parameters
float x0 = 12;
float y0 = 12;
float l = 140;

// Coordinate variables
float xx;
float yy;
float *xxArray;
float *yyArray;

//To prevent multiple libraries 
void setupIR(){
  RX.enableIRIn();
}
// --- Function Implementations ---

//*****DIRECTORY FUNCTION******
void scanDir() {
  freeListMemory();  // Start by freeing previously allocated malloc pointers
  File root = SD.open(dirx);
  while (true) {
    File entry = root.openNextFile();
    if (!entry) {
      break;  // no more files
    } else {
      sprintf(tempString, "%s", entry.name());
      listItems[listLength] = (char *)malloc(listItemMaxChar);
      sprintf(listItems[listLength], "%s", tempString);
      listLength++;
      entry.close();
    }
  }
}

void freeListMemory() {
  for (byte i = 0; i <= listLength; i++) {
    free(listItems[i]);
  }
  listLength = 0;
}


//*****AUTO MODE SEND GCODE FUNCTION******
void sendGcode(int c) {
  scanDir();

  myFile = SD.open(listItems[c]);
  if (myFile) {
    Serial1.println("G10 P0 L20 X0 Y0 Z0");  //no limits switches, wakeup GRBL by Zetzeroing.
    while (myFile.available()) {
      while (Serial1.available()) {
        delay(3);
        char c = Serial1.read();
        readString += c;
      }
      if (readString.length() > 0) {
        if (readString.indexOf("ok") >= 0) {
          okToSendGcode = true;
        }
        readString = "";
      }
      if (okToSendGcode == true) {
        String l_line = "";                     //create an empty string
        l_line = myFile.readStringUntil('\n');  //it looks for end of the line in my file
        Serial1.println(l_line);                
        Serial.println(l_line);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(l_line);
        okToSendGcode = false;
      }
    }
    myFile.close();
  }
  myFile.close();
}


//********SEMI-AUTO GCODE SENDER FUNCTION*********
void SAsendGcode(String f) {
  myFile = SD.open(f);
  if (myFile) {
    Serial1.println("G10 P0 L20 X0 Y0 Z0");  //no limits switches, wakeup GRBL by Zetzeroing.
    while (myFile.available()) {
      while (Serial1.available()) {
        delay(3);
        char c = Serial1.read();
        readString += c;
      }
      if (readString.length() > 0) {
        if (readString.indexOf("ok") >= 0) {
          okToSendGcode = true;
        }
        readString = "";
      }
      if (okToSendGcode == true) {
        String l_line = "";                     //create an empty string
        l_line = myFile.readStringUntil('\n');  //it looks for end of the line in my file
        Serial1.println(l_line);                //Yes you can send this line
        Serial.println(l_line);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(l_line);
        okToSendGcode = false;
      }
    }
    myFile.close();
  }
  myFile.close();
}


//*****MANUAL MODE GCODE SENDER FUNCTIONS*******
void joyStick() {
  int rxV = analogRead(rx);
  int ryV = analogRead(ry);
  int butV = digitalRead(but);

  if (rxV <= 400) {
    rxStateP = true;
    counterX++;
  }
  if (rxV >= 600) {
    rxStateN = true;
    counterX--;
  }
  if (ryV >= 600) {
    ryStateP = true;
    counterY++;
  }
  if (ryV <= 400) {
    ryStateN = true;
    counterY--;
  }
  if (butV == LOW) {
    butState = true;
  }
  Serial.println("rxState =" + String(rxV));
}


void msendGcode() {
  joyStick();
  while (rxStateP) {
    String l_line = "";
    l_line ="F0";
    Serial1.println(l_line); 
    Serial.println(l_line);

    while (Serial1.available()) {
      delay(3);
      char c = Serial1.read();
      readString += c;
    }

    if (readString.length() > 0) {
      if (readString.indexOf("ok") >= 0) {
        okToSendGcode = true;
      }
      readString = "";
    }
    if (okToSendGcode == true) {
      lcd.clear();
      l_line = "";
      l_line = "G91 X1.0 Y0.0 F4000";
      Serial1.println(l_line);
      Serial.println(l_line);
      lcd.setCursor(0, 0);
      lcd.print(l_line);
      rxStateP = false;
    }
  }

  //rx Negative

  while (rxStateN) {
    String l_line = "";
    l_line ="F0";
    Serial1.println(l_line); 

    while (Serial1.available()) {
      delay(3);
      char c = Serial1.read();
      readString += c;
    }

    if (readString.length() > 0) {
      if (readString.indexOf("ok") >= 0) {
        okToSendGcode = true;
      }
      readString = "";
    }
    if (okToSendGcode == true) {
      lcd.clear();
      l_line = "";
      l_line = "G91 X-1.0 Y0.0 F4000";
      Serial1.println(l_line);
      Serial.println(l_line);
      lcd.setCursor(0, 0);
      lcd.print(l_line);
      rxStateN = false;
    }
  }

  //ry Positive
  while (ryStateP) {
    String l_line = "";
    l_line ="F0";
    Serial1.println(l_line); 

    while (Serial1.available()) {
      delay(3);
      char c = Serial1.read();
      readString += c;
    }

    if (readString.length() > 0) {
      if (readString.indexOf("ok") >= 0) {
        okToSendGcode = true;
      }
      readString = "";
    }
    if (okToSendGcode == true) {
      lcd.clear();
      l_line = "";
      l_line = "G91 X0.0 Y1.0 F4000";
      Serial1.println(l_line);
      Serial.println(l_line);
      lcd.setCursor(0, 0);
      lcd.print(l_line);
      ryStateP = false;
    }
  }


  //ry Negative
  while (ryStateN) {
    String l_line = "";
    l_line ="F0";
    Serial1.println(l_line); 

    while (Serial1.available()) {
      delay(3);
      char c = Serial1.read();
      readString += c;
    }

    if (readString.length() > 0) {
      if (readString.indexOf("ok") >= 0) {
        okToSendGcode = true;
      }
      readString = "";
    }
    if (okToSendGcode == true) {
      lcd.clear();
      l_line = "";
      l_line = "G91 X0.0 Y-1.0 F4000";
      Serial1.println(l_line);
      Serial.println(l_line);
      lcd.setCursor(0, 0);
      lcd.print(l_line);
      ryStateN = false;
    }
  }

  //use to exit manual mode loop
  if (butState == true) {
    lcd.clear();
    String setOrigin = "";
    setOrigin = "G92 X" + String(counterX) + " Y" + String(counterY);
    Serial1.println(setOrigin);
    Serial.println(setOrigin);
    lcd.setCursor(0, 0);
    lcd.print(setOrigin);
    delay(1500);
    butState = false;
    joyStickMode = false;
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("New Origin Coord Set!");
    delay(2000);
    manualMode = false;
  }
}

//*****OK FUNCTION******
void okButton() {
  int state, currentState;
  if (digitalRead(okPin) == LOW) {
    state = digitalRead(okPin);
    millis();
    if ((millis() - previousMillis) > delayTime) {
      currentState = digitalRead(okPin);
      if (currentState == state) {
        acknowledged = true;
      } else {
        acknowledged = false;
      }
      previousMillis = millis();
    }
  }
}


//*****PREVIOUS FUNCTION******
void prevButton() {
  int state, currentState;
  if (digitalRead(prevPin) == LOW) {
    state = digitalRead(prevPin);
    millis();
    if ((millis() - previousMillis) > delayTime) {
      currentState = digitalRead(prevPin);
      if (currentState == state) {
        previous = true;
        slide--;
        if (slide < 0) {
          slide = 0;
        }
      } else {
        previous = false;
      }
      previousMillis = millis();
    }
  }
}


//*****NEXT FUNCTION******
void nextButton(int slideMax) {
  int state, currentState;
  if (digitalRead(nextPin) == LOW) {
    state = digitalRead(nextPin);
    millis();
    if ((millis() - previousMillis) > delayTime) {
      currentState = digitalRead(nextPin);
      if (currentState == state) {
        next = true;
        slide++;
        if (slide > slideMax) {
          slide = slideMax;
        }
      } else {
        next = false;
      }
      previousMillis = millis();
    }
  }
}


//*****ROTARY ENCODER FUNCTION******
void rotary(int maxIncr) {
  bool pb = encoder.button();
  int delta = encoder.delta();
  counter += delta;
  if (counter > maxIncr) {
    counter = maxIncr;
  }
  if (counter < 0) {
    counter = 0;
  }
  Serial.println(counter);
}


void SArotary(int SAmaxIncr) {
  bool pb = encoder.button();
  int delta = encoder.delta();
  SAcounter += delta * 500;
  if (SAcounter > SAmaxIncr) {
    SAcounter = maxIncr;
  }
  if (SAcounter < 0) {
    SAcounter = 0;
  }
  Serial.println(SAcounter);
}


//*****IR Sensor functions
void decodeIR() {
  if (RX.decode()) {
    IRvalue = RX.decodedIRData.decodedRawData;
    delay(150);
    RX.resume();
  }
}


void num() {
  decodeIR();
  switch (IRvalue) {
    case 0xE619FF00: cmd += "0"; IRvalue = 0; break;
    case 0xBA45FF00: cmd += "1"; IRvalue = 0; break;
    case 0xB946FF00: cmd += "2"; IRvalue = 0; break;
    case 0xB847FF00: cmd += "3"; IRvalue = 0; break;
    case 0xBB44FF00: cmd += "4"; IRvalue = 0; break;
    case 0xBF40FF00: cmd += "5"; IRvalue = 0; break;
    case 0xBC43FF00: cmd += "6"; IRvalue = 0; break;
    case 0xF807FF00: cmd += "7"; IRvalue = 0; break;
    case 0xEA15FF00: cmd += "8"; IRvalue = 0; break;
    case 0xF609FF00: cmd += "9"; IRvalue = 0; break;
    case 0xE31CFF00: IRvalue = 0; return 10; break;
  }
}


//*****HOMESCREEN FUNCTION******
void homeScreen(int slide) {
  switch (slide) {
    case 0:
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("CNC PLOTTER");
      delay(250);
      break;
    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("AUTOMATIC MODE");
      lcd.setCursor(0, 2);
      lcd.print("Press Ok to Select");
      lcd.setCursor(0, 3);
      lcd.print("File");
      delay(250);
      break;


    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("SEMI-AUTO MOODE");
      lcd.setCursor(0, 2);
      lcd.print("Press Ok to Enter");
      lcd.setCursor(0, 3);
      lcd.print("Plotting Parameters");
      delay(250);
      break;

    case 3:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("MANUAL MODE");
      lcd.setCursor(0, 2);
      lcd.print("Press Ok to activate");
      lcd.setCursor(0, 3);
      lcd.print("manual mode");
      delay(250);
      break;
  }
}


void semiAutoScreen(int count) {
  switch (count) {
    case 0:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(">");
      lcd.print("CIRCLE");
      lcd.setCursor(1, 1);
      lcd.print("VERTICAL LINE");
      lcd.setCursor(1, 2);
      lcd.print("HORIZONTAL LINE");
      delay(250);
      break;

    case 2:
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("CIRCLE");
      lcd.setCursor(0, 1);
      lcd.print(">");
      lcd.print("VERTICAL LINE");
      lcd.setCursor(1, 2);
      lcd.print("HORIZONTAL LINE");
      delay(250);
      break;

    case 4:
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("CIRCLE");
      lcd.setCursor(1, 1);
      lcd.print("VERTICAL LINE");
      lcd.setCursor(0, 2);
      lcd.print(">");
      lcd.print("HORIZONTAL LINE");
      delay(250);
      break;
  }
}


void autoScreen(int count) {
  scanDir();
  switch (count) {
    case 0:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(">");
      lcd.print(String(listItems[1]));
      lcd.setCursor(1, 1);
      lcd.print(String(listItems[2]));
      lcd.setCursor(1, 2);
      lcd.print(String(listItems[3]));
      lcd.setCursor(1, 3);
      lcd.print(String(listItems[4]));
      delay(250);
      break;
    case 2:
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print(String(listItems[1]));
      lcd.setCursor(0, 1);
      lcd.print(">");
      lcd.print(String(listItems[2]));
      lcd.setCursor(1, 2);
      lcd.print(String(listItems[3]));
      lcd.setCursor(1, 3);
      lcd.print(String(listItems[4]));
      delay(250);
      break;

    case 4:
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print(String(listItems[1]));
      lcd.setCursor(1, 1);

      lcd.print(String(listItems[2]));
      lcd.setCursor(0, 2);
      lcd.print(">");
      lcd.print(String(listItems[3]));
      lcd.setCursor(1, 3);
      lcd.print(String(listItems[4]));
      delay(250);
      break;



    case 6:
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print(String(listItems[1]));
      lcd.setCursor(1, 1);

      lcd.print(String(listItems[2]));
      lcd.setCursor(1, 2);
      lcd.print(String(listItems[3]));
      lcd.setCursor(0, 3);
      lcd.print(">");
      lcd.print(String(listItems[4]));
      delay(250);
      break;

    case 8:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(">");
      lcd.print(String(listItems[5]));
      lcd.setCursor(1, 1);
      lcd.print(String(listItems[6]));
      lcd.setCursor(1, 2);
      lcd.print(String(listItems[7]));
      lcd.setCursor(1, 3);
      lcd.print(String(listItems[8]));
      delay(250);
      break;

    case 10:
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print(String(listItems[5]));
      lcd.setCursor(0, 1);
      lcd.print(">");
      lcd.print(String(listItems[6]));
      lcd.setCursor(1, 2);
      lcd.print(String(listItems[7]));
      lcd.setCursor(1, 3);
      lcd.print(String(listItems[8]));
      delay(250);
      break;

    case 12:
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print(String(listItems[5]));
      lcd.setCursor(1, 1);

      lcd.print(String(listItems[6]));
      lcd.setCursor(0, 2);
      lcd.print(">");
      lcd.print(String(listItems[7]));
      lcd.setCursor(1, 3);
      lcd.print(String(listItems[8]));
      delay(250);
      break;
  }
}

//******SEMI-AUTO FUNCTIONS*******
float *getCoordX(float r, float xc) {
  static float x[arrLen] = {};
  for (int i = 0; i < arrLen; i++) {
    x[i] = (r * cos((deg[i] * PI) / 180)) + xc;
  }
  return x;
}

float *getCoordY(float r, float yc) {
  static float y[arrLen] = {};
  for (int i = 0; i < arrLen; i++) {
    y[i] = (r * sin((deg[i] * PI) / 180)) + yc;
  }
  return y;
}

void circleGcode(float fx, float fy) {
  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1)
      ;
  }
  if (SD.exists("circle.txt")) {
    SD.remove("circle.txt");
  }
  Serial.println("initialization done.");


  myFile = SD.open("circle.txt", FILE_WRITE);

  if (myFile) {

    xxArray = getCoordX(r, xc);
    yyArray = getCoordY(r, yc);
    myFile.println("M3");
    myFile.println("G90");
    myFile.println("G21");
    myFile.println("G1 F" + String(fy, 4));
    myFile.println(String("G1") + String("X") + String(xxArray[0], 4) + String(" ") + String("Y") + String(yyArray[0], 4));
    myFile.println("M5 S90");
    myFile.println("G4 P0.20000000298023224");
    myFile.println("G1 F" + String(fx, 4));


    for (int i = 1; i < arrLen; i++) {
      xx = *(xxArray + i);
      yy = *(yyArray + i);


      myFile.println(String("G1 ") + String("X") + String(xx, 4) + String(" ") + String("Y") + String(yy, 4));

      delay(10);
    }
    myFile.println("M3");
    myFile.println("G4 P0.20000000298023224");
    myFile.println("G1 F" + String(fy, 4));
    myFile.println("G1 X0 Y0");

    // close the file:
    myFile.close();
    Serial.println("done.");

  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening circle.txt");
  }
}

void vLineGcode(float x0, float y0, float l, float fx, float fy) {
  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1)
      ;
  }
  if (SD.exists("vline.txt")) {
    SD.remove("vline.txt");
  }
  Serial.println("initialization done.");


  myFile = SD.open("vline.txt", FILE_WRITE);

  if (myFile) {

    myFile.println("M3");
    myFile.println("G90");
    myFile.println("G21");
    myFile.println("G1 F" + String(fy, 4));
    myFile.println(String("G1") + " " + String("X") + String(x0, 4) + String(" ") + String("Y") + String(y0, 4));
    myFile.println("M5 S90");
    myFile.println("G4 P0.20000000298023224");
    myFile.println("G1 F" + String(fx, 4));

    myFile.println(String("G1") + " " + String("X") + String(x0, 4) + String(" ") + String("Y") + String(l + y0, 4));

    myFile.println("M3");
    myFile.println("G4 P0.20000000298023224");
    myFile.println("G1 F" + String(fy, 4));
    myFile.println("G1 X0 Y0");

    // close the file:
    myFile.close();
    Serial.println("done.");

  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening circle.txt");
  }

  // re-open the file for reading:
  //SAsendGcode("vline.txt");
}

void hLineGcode(float x0, float y0, float l, float fx, float fy) {
  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1)
      ;
  }
  if (SD.exists("hline.txt")) {
    SD.remove("hline.txt");
  }
  Serial.println("initialization done.");


  myFile = SD.open("hline.txt", FILE_WRITE);

  if (myFile) {

    myFile.println("M3");
    myFile.println("G90");
    myFile.println("G21");
    myFile.println("G1 F" + String(fy, 4));
    myFile.println(String("G1") + " " + String("X") + String(x0, 4) + String(" ") + String("Y") + String(y0, 4));
    myFile.println("M5 S90");
    myFile.println("G4 P0.20000000298023224");
    myFile.println("G1 F" + String(fx, 4));

    myFile.println(String("G1") + " " + String("X") + String(l + x0, 4) + String(" ") + String("Y") + String(y0, 4));

    myFile.println("M3");
    myFile.println("G4 P0.20000000298023224");
    myFile.println("G1 F" + String(fy, 4));
    myFile.println("G1 X0 Y0");

    // close the file:
    myFile.close();
    Serial.println("done.");

  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening circle.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("hline.txt");
  if (myFile) {
    Serial.println("hline.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening circle.txt");
  }
}