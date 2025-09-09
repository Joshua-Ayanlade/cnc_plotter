#include "cnc_plotter_TX.h"

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  lcd.begin();
  //RX.enableIRIn();    //Use setupIR() to implement this
  setupIR();
  EncoderInterrupt.begin(&encoder);
  pinMode(okPin, INPUT_PULLUP);
  pinMode(prevPin, INPUT_PULLUP);
  pinMode(nextPin, INPUT_PULLUP);
  pinMode(but, INPUT_PULLUP);
  pinMode(rx, INPUT);
  pinMode(ry, INPUT);
  pinMode(4, OUTPUT);

  if (!SD.begin(4)) {
    Serial.println("SD card no detected!");
    return;
  }

  Serial.println("done!");
}

//*****LOOP FUNCTION******
void loop() {
  okButton();
  prevButton();
  nextButton(slideMax);
  previousCLK = digitalRead(CLK);
  homeScreen(slide);

  if (slide == 1 && acknowledged) {
    autoMode = true;
    while (autoMode) {
      acknowledged = false;
      autoScreen(counter);
      okButton();
      rotary(AmaxIncr);
      if (counter == 0 && acknowledged) {
        int c = (counter / 2) + 1;
        sendGcode(c);
        Serial.println("done done");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Executed");
        lcd.setCursor(0, 1);
        lcd.print("Successfully!");
        delay(1000);
        acknowledged = false;
        autoMode = false;
      }

      else if (counter == 2 && acknowledged) {
        int c = (counter / 2) + 1;
        sendGcode(c);
        Serial.println("done done");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Executed");
        lcd.setCursor(0, 1);
        lcd.print("Successfully!");
        delay(1000);
        acknowledged = false;
        autoMode = false;
      }

      else if (counter == 4 && acknowledged) {
        int c = (counter / 2) + 1;
        sendGcode(c);
        Serial.println("done done");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Executed");
        lcd.setCursor(0, 1);
        lcd.print("Successfully!");
        delay(1000);
        acknowledged = false;
        autoMode = false;
      }

      else if (counter == 6 && acknowledged) {
        int c = (counter / 2) + 1;
        sendGcode(c);
        Serial.println("done done");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Executed");
        lcd.setCursor(0, 1);
        lcd.print("Successfully!");
        delay(1000);
        acknowledged = false;
        autoMode = false;
      }

      else if (counter == 8 && acknowledged) {
        int c = (counter / 2) + 1;
        sendGcode(c);
        Serial.println("done done");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Executed");
        lcd.setCursor(0, 1);
        lcd.print("Successfully!");
        delay(1000);
        acknowledged = false;
        autoMode = false;
      }

      else if (counter == 10 && acknowledged) {
        int c = (counter / 2) + 1;
        sendGcode(c);
        Serial.println("done done");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Executed");
        lcd.setCursor(0, 1);
        lcd.print("Successfully!");
        delay(1000);
        acknowledged = false;
        autoMode = false;
      }

      else if (counter == 10 && acknowledged) {
        int c = (counter / 2) + 1;
        sendGcode(c);
        Serial.println("done done");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Executed");
        lcd.setCursor(0, 1);
        lcd.print("Successfully!");
        delay(1000);
        acknowledged = false;
        autoMode = false;
      }
    }
    acknowledged = false;
  }
  //****END OF AUTO MODE LOOP*****

  //****START SEMI-AUTO MODE LOOP*****
  else if (slide == 2 && acknowledged) {
    semiAutoMode = true;
    while (semiAutoMode) {
      acknowledged = false;
      previous = false;
      semiAutoScreen(counter);
      okButton();
      rotary(SAmaxIncr);
      prevButton();
      if (previous) {
        semiAutoMode = false;
      }



      //****SEMI-AUTO CIRCLE LOOP*****
      if (counter == 0 && acknowledged) {
        inputCparameter = true;
        while (inputCparameter) {
          acknowledged = false;
          previous = false;
          okButton();
          prevButton();

          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Press ok to continue");
          delay(150);
          //fxValue = true;   //move down

          if (previous) {
            inputCparameter = false;
          }

          else if (acknowledged) {
            fxValue = true;
            while (fxValue) {
              acknowledged = false;
              previous = false;
              next = false;
              okButton();
              prevButton();
              nextButton(1);
              num();
              fx = cmd.toInt();

              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Set Feedrate mm/min");
              lcd.setCursor(0, 1);
              lcd.print("for X axis");
              lcd.setCursor(0, 3);
              lcd.print(">");
              lcd.print(cmd);
              delay(150);
              Serial.println(cmdCount);

              if (previous) {
                fxValue = false;
              }

              if (next) {
                cmd = "";
              }

              else if (acknowledged) {
                cmd = "";
                Serial.println("setting fyValue");
                fyValue = true;
                while (fyValue) {
                  previous = false;
                  next = false;
                  acknowledged = false;
                  okButton();
                  prevButton();
                  nextButton(1);
                  num();
                  fy = cmd.toInt();
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Set Feedrate mm/min");
                  lcd.setCursor(0, 1);
                  lcd.print("for Y axis");
                  lcd.setCursor(0, 3);
                  lcd.print(">");
                  lcd.print(cmd);
                  delay(150);
                  //Serial.println(fx);

                  if (previous) {
                    fyValue = false;
                  }

                  if (next) {
                    cmd = "";
                  }

                  else if (acknowledged) {
                    cmd = "";
                    Serial.println("setting r Value");
                    rValue = true;
                    while (rValue) {
                      acknowledged = false;
                      previous = false;
                      next = false;
                      okButton();
                      prevButton();
                      nextButton(1);
                      num();
                      r = cmd.toInt();
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Set radius of Circle");
                      lcd.setCursor(0, 1);
                      lcd.print("in mm");
                      lcd.setCursor(0, 3);
                      lcd.print(">");
                      lcd.print(cmd);
                      delay(150);
                      //Serial.println(fx);
                      //Serial.println(fy);

                      if (previous) {
                        rValue = false;
                      }
                      if (next) {
                        cmd = "";
                      }

                      else if (acknowledged) {
                        cmd = "";
                        Serial.println("setting xc Value");
                        xcValue = true;
                        while (xcValue) {
                          acknowledged = false;
                          previous = false;
                          next = false;
                          okButton();
                          prevButton();
                          nextButton(1);
                          num();
                          xc = cmd.toInt();
                          lcd.clear();
                          lcd.setCursor(0, 0);
                          lcd.print("Xc from the Origin");
                          lcd.setCursor(0, 1);
                          lcd.print("in mm");
                          lcd.setCursor(0, 3);
                          lcd.print(">");
                          lcd.print(cmd);
                          delay(150);

                          if (previous) {
                            xcValue = false;
                          }

                          if (next) {
                            cmd = "";
                          }

                          else if (acknowledged) {
                            cmd = "";
                            Serial.println("setting yc Value");
                            ycValue = true;
                            while (ycValue) {
                              acknowledged = false;
                              previous = false;
                              next = false;
                              okButton();
                              prevButton();
                              nextButton(1);
                              num();
                              yc = cmd.toInt();
                              lcd.clear();
                              lcd.setCursor(0, 0);
                              lcd.print("Yc from the Origin");
                              lcd.setCursor(0, 1);
                              lcd.print("in mm");
                              lcd.setCursor(0, 3);
                              lcd.print(">");
                              lcd.print(cmd);
                              delay(150);
                              Serial.println(fx);
                              Serial.println(fy);
                              Serial.println(r);
                              Serial.println(xc);

                              if (previous) {
                                ycValue = false;
                              }

                              if (next) {
                                cmd = "";
                              }

                              else if (acknowledged) {
                                cmd = "";
                                Serial.println("Generating Cgcode");
                                CgcodeGen = true;
                                while (CgcodeGen) {
                                  acknowledged = false;
                                  getCoordX(r, xc);
                                  getCoordY(r, yc);
                                  circleGcode(fx, fy);
                                  lcd.clear();
                                  lcd.setCursor(0, 0);
                                  lcd.print("GCODE generated");
                                  lcd.setCursor(0, 2);
                                  lcd.print("successfully!");
                                  delay(2000);
                                  lcd.clear();
                                  lcd.setCursor(0, 0);
                                  lcd.print("Printing Starts");
                                  lcd.setCursor(0, 2);
                                  lcd.print("shortly!");
                                  delay(2000);
                                  SAsendGcode("circle.txt");
                                  Serial.println("done done!");

                                  Serial.println("Parameters: r: " + String(r) + " xc " + String(xc) + " yc " + String(yc));
                                  Serial.print(" r: " + String(r) + " fx " + String(fx) + " fy " + String(fy));

                                  //****END***

                                  CgcodeGen = false;
                                  ycValue = false;
                                  xcValue = false;
                                  rValue = false;
                                  fyValue = false;
                                  fxValue = false;
                                  inputCparameter = false;
                                  semiAutoMode = false;
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }

      //****SEMI-AUTO VL LOOP*****

      else if (counter == 2 && acknowledged) {
        inputVLparameter = true;
        while (inputVLparameter) {

          acknowledged = false;
          okButton();
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Press ok to continue");
          delay(150);
          //fxValue = true;   //move down
          if (acknowledged) {
            fxValue = true;
            while (fxValue) {
              acknowledged = false;
              okButton();
              num();
              fx = cmd.toInt();
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Set Feedrate mm/min");
              lcd.setCursor(0, 1);
              lcd.print("for X axis");
              lcd.setCursor(0, 3);
              lcd.print(">");
              lcd.print(cmd);
              delay(150);
              Serial.println(cmdCount);

              if (acknowledged) {
                cmd = "";
                Serial.println("setting fyValue");
                fyValue = true;
                while (fyValue) {
                  acknowledged = false;
                  okButton();
                  num();
                  fy = cmd.toInt();
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Set Feedrate mm/min");
                  lcd.setCursor(0, 1);
                  lcd.print("for Y axis");
                  lcd.setCursor(0, 3);
                  lcd.print(">");
                  lcd.print(cmd);
                  delay(150);
                  if (acknowledged) {
                    cmd = "";
                    Serial.println("setting X0 Value");
                    x0value = true;
                    while (x0value) {
                      acknowledged = false;
                      okButton();
                      num();
                      x0 = cmd.toInt();
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Set x0 from the origin");
                      lcd.setCursor(0, 1);
                      lcd.print("in mm");
                      lcd.setCursor(0, 3);
                      lcd.print(">");
                      lcd.print(cmd);
                      delay(150);
                      if (acknowledged) {
                        cmd = "";
                        Serial.println("setting Y0 Value");
                        y0value = true;
                        while (y0value) {
                          acknowledged = false;
                          okButton();
                          num();
                          y0 = cmd.toInt();
                          lcd.clear();
                          lcd.setCursor(0, 0);
                          lcd.print("Set y0 from the origin");
                          lcd.setCursor(0, 1);
                          lcd.print("in mm");
                          lcd.setCursor(0, 3);
                          lcd.print(">");
                          lcd.print(cmd);
                          delay(150);
                          if (acknowledged) {
                            cmd = "";
                            Serial.println("setting length Value");
                            Lvalue = true;
                            while (Lvalue) {
                              acknowledged = false;
                              okButton();
                              num();
                              l = cmd.toInt();
                              lcd.clear();
                              lcd.setCursor(0, 0);
                              lcd.print("Set the length of line");
                              lcd.setCursor(0, 1);
                              lcd.print("in mm");
                              lcd.setCursor(0, 3);
                              lcd.print(">");
                              lcd.print(cmd);
                              delay(150);
                              if (acknowledged) {
                                cmd = "";
                                Serial.println("Generating VLgcode");
                                VLgcodeGen = true;
                                while (VLgcodeGen) {
                                  acknowledged = false;
                                  vLineGcode(x0, y0, l, fx, fy);
                                  lcd.clear();
                                  lcd.setCursor(0, 0);
                                  lcd.print("GCODE generated");
                                  lcd.setCursor(0, 2);
                                  lcd.print("successfully!");
                                  delay(2000);
                                  lcd.clear();
                                  lcd.setCursor(0, 0);
                                  lcd.print("Printing Starts");
                                  lcd.setCursor(0, 2);
                                  lcd.print("shortly!");
                                  delay(2000);
                                  SAsendGcode("vline.txt");
                                  Serial.println("done done!");

                                  Serial.println("Parameters: x0: " + String(x0) + " y0 " + String(y0));
                                  Serial.print(" l: " + String(l) + " fx " + String(fx) + " fy " + String(fy));
                                  //****END***

                                  VLgcodeGen = false;
                                  Lvalue = false;
                                  y0value = false;
                                  x0value = false;
                                  fyValue = false;
                                  fxValue = false;
                                  inputVLparameter = false;
                                  semiAutoMode = false;
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }

      //****SEMI-AUTO HL LOOP*****

      else if (counter == 4 && acknowledged) {
        inputHLparameter = true;
        while (inputHLparameter) {

          acknowledged = false;
          okButton();
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Press ok to continue");
          delay(150);
          //fxValue = true;   //move down
          if (acknowledged) {
            fxValue = true;
            while (fxValue) {
              acknowledged = false;
              okButton();
              num();
              fx = cmd.toInt();
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Set Feedrate mm/min");
              lcd.setCursor(0, 1);
              lcd.print("for X axis");
              lcd.setCursor(0, 3);
              lcd.print(">");
              lcd.print(cmd);
              delay(150);
              Serial.println(cmdCount);

              if (acknowledged) {
                cmd = "";
                Serial.println("setting fyValue");
                fyValue = true;
                while (fyValue) {
                  acknowledged = false;
                  okButton();
                  num();
                  fy = cmd.toInt();
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Set Feedrate mm/min");
                  lcd.setCursor(0, 1);
                  lcd.print("for Y axis");
                  lcd.setCursor(0, 3);
                  lcd.print(">");
                  lcd.print(cmd);
                  delay(150);
                  if (acknowledged) {
                    cmd = "";
                    Serial.println("setting X0 Value");
                    x0value = true;
                    while (x0value) {
                      acknowledged = false;
                      okButton();
                      num();
                      x0 = cmd.toInt();
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Set x0 from the origin");
                      lcd.setCursor(0, 1);
                      lcd.print("in mm");
                      lcd.setCursor(0, 3);
                      lcd.print(">");
                      lcd.print(cmd);
                      delay(150);
                      if (acknowledged) {
                        cmd = "";
                        Serial.println("setting Y0 Value");
                        y0value = true;
                        while (y0value) {
                          acknowledged = false;
                          okButton();
                          num();
                          y0 = cmd.toInt();
                          lcd.clear();
                          lcd.setCursor(0, 0);
                          lcd.print("Set y0 from the origin");
                          lcd.setCursor(0, 1);
                          lcd.print("in mm");
                          lcd.setCursor(0, 3);
                          lcd.print(">");
                          lcd.print(cmd);
                          delay(150);
                          if (acknowledged) {
                            cmd = "";
                            Serial.println("setting length Value");
                            Lvalue = true;
                            while (Lvalue) {
                              acknowledged = false;
                              okButton();
                              num();
                              l = cmd.toInt();
                              lcd.clear();
                              lcd.setCursor(0, 0);
                              lcd.print("Set the length of line");
                              lcd.setCursor(0, 1);
                              lcd.print("in mm");
                              lcd.setCursor(0, 3);
                              lcd.print(">");
                              lcd.print(cmd);
                              delay(150);
                              if (acknowledged) {
                                cmd = "";
                                Serial.println("Generating HLgcode");
                                HLgcodeGen = true;
                                while (HLgcodeGen) {
                                  acknowledged = false;
                                  hLineGcode(x0, y0, l, fx, fy);
                                  lcd.clear();
                                  lcd.setCursor(0, 0);
                                  lcd.print("GCODE generated");
                                  lcd.setCursor(0, 2);
                                  lcd.print("successfully!");
                                  delay(2000);
                                  lcd.clear();
                                  lcd.setCursor(0, 0);
                                  lcd.print("Printing Starts");
                                  lcd.setCursor(0, 2);
                                  lcd.print("shortly!");
                                  delay(2000);
                                  SAsendGcode("hline.txt");
                                  Serial.println("done done!");

                                  Serial.println("Parameters: x0: " + String(x0) + " y0 " + String(y0));
                                  Serial.print(" l: " + String(l) + " fx " + String(fx) + " fy " + String(fy));
                                  //****END***

                                  HLgcodeGen = false;
                                  Lvalue = false;
                                  y0value = false;
                                  x0value = false;
                                  fyValue = false;
                                  fxValue = false;
                                  inputHLparameter = false;
                                  semiAutoMode = false;
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }


  //*******START OF MANUAL MODE LOOP
  else if (slide == 3 && acknowledged) {
    manualMode = true;
    while (manualMode) {
      acknowledged = false;
      previous = false;
      okButton();
      prevButton();
      if (previous) {
        semiAutoMode = false;
      }


      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Press ok to activate");
      lcd.setCursor(0, 1);
      lcd.print("Manual Mode");
      delay(150);
      if (acknowledged) {
        joyStickMode = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Move Slider to desired");
        lcd.setCursor(0, 1);
        lcd.print("Coordinate");
        lcd.setCursor(0, 2);
        lcd.print("and acknowledge to");
        lcd.setCursor(0, 1);
        lcd.print("to set as Origin");
        delay(2000);

        while (joyStickMode) {
          acknowledged = false;
          msendGcode();
        }
      }
    }
  }
}
