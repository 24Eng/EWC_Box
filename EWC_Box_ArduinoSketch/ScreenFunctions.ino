void newModeScreen(){
  switch(mode){
    case 0:
      startingParametersMode0();
      drawMode0();
      selection1 = 48;
      selection2 = 72;
      return;
    case 1:
      midiNoteOff(0, mainNote, 0);
      // mode1 looks the same as mode0
      startingParametersMode0();
      drawMode0();
      handleEncoders();
      handlePots();
      return;
    case 2:
      // mode2 
      drawMode2();
      midiNoteOff(0, mainNote, 0);
      playNextNote = millis();
      selection2 = 0;
      selection1 = 0;
      handleEncoders();
      handlePots();
      return;
    case 3:
      // mode3 is the percussion mode
      playNextPercussion = millis();
      drawMode3();
      stopMode2();
      handleEncoders();
      handlePots();
      return;
    case 4:
      // mode4 is the drum sampler mode
      playNextPercussion = millis();
      drawMode4();
      handleEncoders();
      handlePots();
      return;
    case 5:
      // mode5 combines percussion and Melody
      playNextPercussion = millis();
      drawMode5();
      startingParametersMode5();
      handleEncoders();
      handlePots();
      return;
    case 6:
      // mode5 combines percussion and Melody
      playNextPercussion = millis();
      drawMode6();
      startingParametersMode6();
      handleEncoders();
      handlePots();
      return;
    default:
      showSTUFFSymbol();
      panic();
      for(int i=0;i<15;i++){
        multipleInstrumentsMode2[i][0] = 0;
      }
      drawScreenBackground();
      return;
  }
}

void drawBeatCircle(int funXCoord, int funYCoord, bool funActive, bool onBeat){
  // The first two inputs are X and Y coordinates.
  // The third input tells the function to draw a white background behind the circle
  // The fourt input tells the function if the circle should be bright or dim
  int funRadius = 4;
  defaultColor = myTFT.rgbTo18b(0, 0, 0);
  ILI9163C_color_18_t funBackgroundColor = myTFT.rgbTo18b(0, 0, 0);
  ILI9163C_color_18_t funFillColor = myTFT.rgbTo18b(100, 0, 100);
  ILI9163C_color_18_t funCenterColor = myTFT.rgbTo18b(255, 0, 0);
  if(funActive){
    funBackgroundColor = myTFT.rgbTo18b(255, 255, 255);
  }
  if(onBeat){
    funFillColor = myTFT.rgbTo18b(255, 255, 0);
  }
  
  myTFT.rectangle(funXCoord-funRadius, funYCoord-funRadius, funXCoord+funRadius, funYCoord+funRadius, false, (color_t)&funBackgroundColor);
  myTFT.circle(funXCoord, funYCoord, funRadius, true, (color_t)&funFillColor);
  myTFT.circle(funXCoord, funYCoord, funRadius-3, true, (color_t)&funCenterColor);
}


void drawChannelBeats(){
  // Draw the active and inactive slots for the currentTrack
  for(int i=0;i<15;i++){
    bool funCurrentNoteUsed = false;
    if(percussionPlayback[currentTrack][i] > 0){
       funCurrentNoteUsed = true;
    }
    updateBeatCircle(i, false, funCurrentNoteUsed);
  }
}

void updateBeatCircle(int funSpot, bool funPlaying, bool funNotePresent){
  // The funSpot integer is the position in the line of circles.
  // The funPlaying boolean controls if the count is on this location.
  // The funNotePresent describes if there is a note stored here.
  funSpot = constrain(funSpot, 0, 15);
  drawBeatCircle(beatCircleXOffset, yMax - 15 - (funSpot*9), funPlaying, funNotePresent);
}



void drawModeTEMPLATE(){
  drawScreenBackground();
  int funXOffset = 10;
  int funYOffset = 10;
  myTFT.setTextCursor(funXOffset,funYOffset);
//  defaultColor = myTFT.rgbTo18b( 255, 255, 255 );
  defaultColor = myTFT.rgbTo18b(255, 0, 0);
  myTFT.print(F("X"));
  
  funYOffset = funYOffset+ 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 255, 0);
  myTFT.print(F("X"));
  
  funYOffset = funYOffset + 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 255, 255);
  myTFT.print(F("X"));
  
  funYOffset = funYOffset + 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 0, 255);
  myTFT.print(F("X"));
  
  
  funXOffset = 60;
  funYOffset = 30;
  
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b( 255, 255, 255 );
  defaultColor = myTFT.rgbTo18b(255, 0, 0);
  myTFT.print(F("         X"));
  
  funYOffset = funYOffset+ 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 255, 0);
  myTFT.print(F("         X"));
  
  funYOffset = funYOffset + 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 255, 255);
  myTFT.print(F("         X"));
  
  funYOffset = funYOffset + 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 0, 255);
  myTFT.print(F("         X"));
}



void drawScreenBackground(){
  myTFT.clearDisplay();
  myTFT.setTextCursor(0,0);
  
  int offsetFactor = 0;
  int barWidth = yMax/4;
  int barHeight = 8;
  int gap = 3;
  
  // By default myTFT assumes that colors are in this form - so let's make one!
  ILI9163C_color_18_t pixelColor; // Name this color "pixel color"
  pixelColor.r = 0;
  pixelColor.g = 0;
  pixelColor.b = 255;
  myTFT.rectangle(0, yMax - (offsetFactor*barWidth), barHeight, yMax - ((offsetFactor+1)*barWidth) + gap, true, (color_t)&pixelColor);
  myTFT.rectangle(xMax-barHeight, yMax - (offsetFactor*barWidth), xMax, yMax - ((offsetFactor+1)*barWidth) + gap, true, (color_t)&pixelColor);
  
  offsetFactor = 1;
  pixelColor.r = 0;
  pixelColor.g = 255;
  pixelColor.b = 255;
  myTFT.rectangle(0, yMax - (offsetFactor*barWidth), barHeight, yMax - ((offsetFactor+1)*barWidth) + gap, true, (color_t)&pixelColor);
  myTFT.rectangle(xMax-barHeight, yMax - (offsetFactor*barWidth), xMax, yMax - ((offsetFactor+1)*barWidth) + gap, true, (color_t)&pixelColor);
  
  offsetFactor = 2;
  pixelColor.r = 0;
  pixelColor.g = 255;
  pixelColor.b = 0;
  myTFT.rectangle(0, yMax - (offsetFactor*barWidth), barHeight, yMax - ((offsetFactor+1)*barWidth) + gap, true, (color_t)&pixelColor);
  myTFT.rectangle(xMax-barHeight, yMax - (offsetFactor*barWidth), xMax, yMax - ((offsetFactor+1)*barWidth) + gap, true, (color_t)&pixelColor);
  
  offsetFactor = 3;
  pixelColor.r = 255;
  pixelColor.g = 0;
  pixelColor.b = 0;
  myTFT.rectangle(0, yMax - (offsetFactor*barWidth), barHeight, yMax - ((offsetFactor+1)*barWidth) + gap, true, (color_t)&pixelColor);
  myTFT.rectangle(xMax-barHeight, yMax - (offsetFactor*barWidth), xMax, yMax - ((offsetFactor+1)*barWidth) + gap, true, (color_t)&pixelColor);
}


void screenTest(){
  myTFT.clearDisplay();
  myTFT.setTextCursor(0,0);            // Sets the cursor relative to the current window, however the current (default) window is equivalent to the whole display. (0,0) is the upper left corner and (myTFT.xExt-1, myTFT.yExt-1) is the lower right
  myTFT.setCurrentWindowColorSequence((color_t)&defaultColor);

  uint16_t hue = HSV_HUE_MIN;
  while(hue <= HSV_HUE_MAX){
    myTFT.setTextCursor(0,0);
    defaultColor = myTFT.hsvTo18b( hue+=30, 255, 255 );
//    Serial.print("\n");
//    Serial.print(hue);
    myTFT.print(F("EWC_Box"));
  }
}

void encodersOnScreen(){
  int graphWidth = 10;
  int offsetFactor = 2;
  
  // By default myTFT assumes that colors are in this form - so let's make one!
  ILI9163C_color_18_t pixelColor; // Name this color "pixel color"
  pixelColor.r = 0;            // Set the r, g, and b values how you like
  pixelColor.g = 0;
  pixelColor.b = 255;
  defaultColor = myTFT.hsvTo18b(0, 0, 0);
  
//  encoder0ScreenLevel
  myTFT.rectangle(0, yMax - offsetFactor*graphWidth, xMax, yMax - (offsetFactor+1)*graphWidth, true, (color_t)&defaultColor);
  myTFT.rectangle(selection0-graphWidth/2, yMax - offsetFactor*graphWidth, selection0+graphWidth/2, yMax - (offsetFactor+1)*graphWidth, true, (color_t)&pixelColor);
  
//  encoder1ScreenLevel
  pixelColor.r = 0;
  pixelColor.g = 255;
  pixelColor.b = 255;
  offsetFactor = 6;
  myTFT.rectangle(0, yMax - offsetFactor*graphWidth, xMax, yMax - (offsetFactor+1)*graphWidth, true, (color_t)&defaultColor);
  myTFT.rectangle(selection1-graphWidth/2, yMax - offsetFactor*graphWidth, selection1+graphWidth/2, yMax - (offsetFactor+1)*graphWidth, true, (color_t)&pixelColor);
  
//  encoder2ScreenLevel
  pixelColor.r = 0;
  pixelColor.g = 255;
  pixelColor.b = 0;
  offsetFactor = 11;
  myTFT.rectangle(0, yMax - offsetFactor*graphWidth, xMax, yMax - (offsetFactor+1)*graphWidth, true, (color_t)&defaultColor);
  myTFT.rectangle(selection2-graphWidth/2, yMax - offsetFactor*graphWidth, selection2+graphWidth/2, yMax - (offsetFactor+1)*graphWidth, true, (color_t)&pixelColor);
  
//  encoder2ScreenLevel
  pixelColor.r = 255;
  pixelColor.g = 0;
  pixelColor.b = 0;
  offsetFactor = 13;
  myTFT.rectangle(0, yMax - offsetFactor*graphWidth, xMax, yMax - (offsetFactor+1)*graphWidth, true, (color_t)&defaultColor);
  myTFT.rectangle(selection3-graphWidth/2, yMax - offsetFactor*graphWidth, selection3+graphWidth/2, yMax - (offsetFactor+1)*graphWidth, true, (color_t)&pixelColor);
}

void potsOnScreen(){
  int graphWidth = 10;
  int offsetFactor = 0;
  
  // By default myTFT assumes that colors are in this form - so let's make one!
  ILI9163C_color_18_t pixelColor; // Name this color "pixel color"
  pixelColor.r = 0;            // Set the r, g, and b values how you like
  pixelColor.g = 0;
  pixelColor.b = 255;
  defaultColor = myTFT.hsvTo18b(0, 0, 0);
//  pixelColor = myTFT.hsvTo18b(0, 255, 0);
//  pot0ScreenLevel
  int pot0Mapped = map(pot0PreviousLevel, 0,1023,0,127);
  myTFT.rectangle(0, yMax, xMax, yMax-graphWidth, true, (color_t)&defaultColor);
  myTFT.rectangle(0, yMax, pot0Mapped, yMax-graphWidth, true, (color_t)&pixelColor);
  
//  pot1ScreenLevel
  offsetFactor = 4;
  pixelColor.r = 0;
  pixelColor.g = 255;
  pixelColor.b = 255;
  int pot1Mapped = map(pot1PreviousLevel, 0,1023,0,127);
  myTFT.rectangle(0, yMax - offsetFactor*graphWidth, xMax, yMax - (offsetFactor+1)*graphWidth, true, (color_t)&defaultColor);
  myTFT.rectangle(0, yMax - offsetFactor*graphWidth, pot1Mapped, yMax - (offsetFactor+1)*graphWidth, true, (color_t)&pixelColor);
  
//  pot2ScreenLevel
  offsetFactor = 9;
  pixelColor.r = 0;
  pixelColor.g = 255;
  pixelColor.b = 0;
  int pot2Mapped = map(pot2PreviousLevel, 0,1023,0,127);
  myTFT.rectangle(0, yMax - offsetFactor*graphWidth, xMax, yMax - (offsetFactor+1)*graphWidth, true, (color_t)&defaultColor);
  myTFT.rectangle(0, yMax - offsetFactor*graphWidth, pot2Mapped, yMax - (offsetFactor+1)*graphWidth, true, (color_t)&pixelColor);
  
//  pot3ScreenLevel
  offsetFactor = 14;
  pixelColor.r = 255;
  pixelColor.g = 0;
  pixelColor.b = 0;
  int pot3Mapped = map(pot3PreviousLevel, 0,1023,0,127);
  myTFT.rectangle(0, yMax - offsetFactor*graphWidth, xMax, yMax - (offsetFactor+1)*graphWidth, true, (color_t)&defaultColor);
  myTFT.rectangle(0, yMax - offsetFactor*graphWidth, pot3Mapped, yMax - (offsetFactor+1)*graphWidth, true, (color_t)&pixelColor);
}

void clearScreen(){
  myTFT.clearDisplay();
  myTFT.setTextCursor(0,0);
  myTFT.setCurrentWindowColorSequence((color_t)&defaultColor);
}

void showSTUFFSymbol(){
  clearScreen();
  int increment = 120;
  int funCounter = 0;
  defaultColor = myTFT.hsvTo18b(funCounter, 255, 255);
  myTFT.print(F(" S       SSSS        "));
  defaultColor = myTFT.hsvTo18b(funCounter += increment, 255, 255);
  myTFT.print(F("  S     S    S       "));
  defaultColor = myTFT.hsvTo18b(funCounter += increment, 255, 255);
  myTFT.print(F("   S   S      S      "));
  defaultColor = myTFT.hsvTo18b(funCounter += increment, 255, 255);
  myTFT.print(F("    S  S       S     "));
  defaultColor = myTFT.hsvTo18b(funCounter += increment, 255, 255);
  myTFT.print(F("     S S             "));
  defaultColor = myTFT.hsvTo18b(funCounter += increment, 255, 255);
  myTFT.print(F("      S              "));
  defaultColor = myTFT.hsvTo18b(funCounter += increment, 255, 255);
  myTFT.print(F("       S             "));
  defaultColor = myTFT.hsvTo18b(funCounter += increment, 255, 255);
  myTFT.print(F("        S   S        "));
  defaultColor = myTFT.hsvTo18b(funCounter += increment, 255, 255);
  myTFT.print(F("         S S         "));
  defaultColor = myTFT.hsvTo18b(funCounter += increment, 255, 255);
  myTFT.print(F("          S          "));
  defaultColor = myTFT.hsvTo18b(funCounter += increment, 255, 255);
  myTFT.print(F("         S S         "));
  defaultColor = myTFT.hsvTo18b(funCounter += increment, 255, 255);
  myTFT.print(F("        S   S        "));
  defaultColor = myTFT.hsvTo18b(funCounter += increment, 255, 255);
  myTFT.print(F("             S       "));
  defaultColor = myTFT.hsvTo18b(funCounter += increment, 255, 255);
  myTFT.print(F("            S S      "));
  defaultColor = myTFT.hsvTo18b(funCounter += increment, 255, 255);
  myTFT.print(F("    S       S  S     "));
  defaultColor = myTFT.hsvTo18b(funCounter += increment, 255, 255);
  myTFT.print(F("     S      S   S    "));
  defaultColor = myTFT.hsvTo18b(funCounter += increment, 255, 255);
  myTFT.print(F("      S    S     S   "));
  defaultColor = myTFT.hsvTo18b(funCounter += increment, 255, 255);
  myTFT.print(F("       SSSS       S  "));
  myTFT.print(F("                     "));
  defaultColor = myTFT.rgbTo18b(255, 0, 0);
  myTFT.print(F("  24 Hour Engineer   "));
}


void updateBluePotValueOnScreen(int funInfo){
  if(potentiometerFeedbackOnScreen){
    int funXCoord = 9;
    int funYCoord = 25;
    int funRed = 0;
    int funGrn = 0;
    int funBlu = 255;
    // Draw a black rectangle over the area we want to update
    defaultColor = myTFT.hsvTo18b(0, 0, 0);
    myTFT.rectangle(funXCoord, funYCoord, 40, funYCoord+15, true, (color_t)&defaultColor);
    // Write text over the black rectangle
    myTFT.setTextCursor(funXCoord+1,funYCoord);
    defaultColor = myTFT.rgbTo18b(funBlu, funGrn, funRed);
    myTFT.print(funInfo);
  }
}

void updateGreenPotValueOnScreen(int funInfo){
  if(potentiometerFeedbackOnScreen){
    int funXCoord = 9;
    int funYCoord = 25+40;
    int funRed = 0;
    int funGrn = 255;
    int funBlu = 0;
    // Draw a black rectangle over the area we want to update
    defaultColor = myTFT.hsvTo18b(0, 0, 0);
    myTFT.rectangle(funXCoord, funYCoord, 40, funYCoord+15, true, (color_t)&defaultColor);
    // Write text over the black rectangle
    myTFT.setTextCursor(funXCoord+1,funYCoord);
    defaultColor = myTFT.rgbTo18b(funBlu, funGrn, funRed);
    myTFT.print(funInfo);
  }
}

void updateYellowPotValueOnScreen(int funInfo){
  if(potentiometerFeedbackOnScreen){
    int funXCoord = 9;
    int funYCoord = 25+40+40;
    int funRed = 255;
    int funGrn = 255;
    int funBlu = 0;
    // Draw a black rectangle over the area we want to update
    defaultColor = myTFT.hsvTo18b(0, 0, 0);
    myTFT.rectangle(funXCoord, funYCoord, 40, funYCoord+15, true, (color_t)&defaultColor);
    // Write text over the black rectangle
    myTFT.setTextCursor(funXCoord+1,funYCoord);
    defaultColor = myTFT.rgbTo18b(funBlu, funGrn, funRed);
    myTFT.print(funInfo);
  }
}

void updateRedPotValueOnScreen(int funInfo){
  if(potentiometerFeedbackOnScreen){
    int funXCoord = 9;
    int funYCoord = 25+40+40+40;
    int funRed = 255;
    int funGrn = 0;
    int funBlu = 0;
    // Draw a black rectangle over the area we want to update
    defaultColor = myTFT.hsvTo18b(0, 0, 0);
    myTFT.rectangle(funXCoord, funYCoord, 40, funYCoord+15, true, (color_t)&defaultColor);
    // Write text over the black rectangle
    myTFT.setTextCursor(funXCoord+1,funYCoord);
    defaultColor = myTFT.rgbTo18b(funBlu, funGrn, funRed);
    myTFT.print(funInfo);
  }
}

void updateBlueEncoderValueOnScreen(int funInfo){
  int funXCoord = xMax-40;
  int funYCoord = 15;
  int funRed = 0;
  int funGrn = 0;
  int funBlu = 255;
  // Draw a black rectangle over the area we want to update
  defaultColor = myTFT.hsvTo18b(0, 0, 0);
//  defaultColor = myTFT.rgbTo18b(funBlu, funGrn, funRed);
  myTFT.rectangle(funXCoord, funYCoord, funXCoord+25, funYCoord+10, true, (color_t)&defaultColor);
  // Write text over the black rectangle
  myTFT.setTextCursor(funXCoord+1,funYCoord);
  defaultColor = myTFT.rgbTo18b(funBlu, funGrn, funRed);
  myTFT.print(funInfo);
}

void updateGreenEncoderValueOnScreen(int funInfo){
  int funXCoord = xMax-40;
  int funYCoord = 15+40;
  int funRed = 0;
  int funGrn = 255;
  int funBlu = 0;
  // Draw a black rectangle over the area we want to update
  defaultColor = myTFT.hsvTo18b(0, 0, 0);
//  defaultColor = myTFT.rgbTo18b(funBlu, funGrn, funRed);
  myTFT.rectangle(funXCoord, funYCoord, funXCoord+25, funYCoord+10, true, (color_t)&defaultColor);
  // Write text over the black rectangle
  myTFT.setTextCursor(funXCoord+1,funYCoord);
  defaultColor = myTFT.rgbTo18b(funBlu, funGrn, funRed);
  myTFT.print(funInfo);
}

void updateYellowEncoderValueOnScreen(int funInfo){
  int funXCoord = xMax-40;
  int funYCoord = 15+40+40;
  int funRed = 255;
  int funGrn = 255;
  int funBlu = 0;
  // Draw a black rectangle over the area we want to update
  defaultColor = myTFT.hsvTo18b(0, 0, 0);
//  defaultColor = myTFT.rgbTo18b(funBlu, funGrn, funRed);
  myTFT.rectangle(funXCoord, funYCoord, funXCoord+25, funYCoord+10, true, (color_t)&defaultColor);
  // Write text over the black rectangle
  myTFT.setTextCursor(funXCoord+1,funYCoord);
  defaultColor = myTFT.rgbTo18b(funBlu, funGrn, funRed);
  myTFT.print(funInfo);
}

void updateRedEncoderValueOnScreen(int funInfo){
  int funXCoord = xMax-40;
  int funYCoord = 15+40+40+40;
  int funRed = 255;
  int funGrn = 0;
  int funBlu = 0;
  // Draw a black rectangle over the area we want to update
  defaultColor = myTFT.hsvTo18b(0, 0, 0);
//  defaultColor = myTFT.rgbTo18b(funBlu, funGrn, funRed);
  myTFT.rectangle(funXCoord, funYCoord, funXCoord+25, funYCoord+10, true, (color_t)&defaultColor);
  // Write text over the black rectangle
  myTFT.setTextCursor(funXCoord+1,funYCoord);
  defaultColor = myTFT.rgbTo18b(funBlu, funGrn, funRed);
  myTFT.print(funInfo);
}

void updateInstrumentOnScreen(int funInstrument){
  int funXCoord = xMax-105;
//  int funYCoord = 15+40+40+40;
//  int funXCoord = 60;
  int funYCoord = 150;
  int funRed = 255;
  int funGrn = 0;
  int funBlu = 0;
  // Draw a black rectangle over the area we want to update
  defaultColor = myTFT.hsvTo18b(0, 0, 0);
  myTFT.rectangle(funXCoord, funYCoord, funXCoord+95, funYCoord+10, true, (color_t)&defaultColor);
  // Write text over the black rectangle
  myTFT.setTextCursor(funXCoord+1,funYCoord);
  defaultColor = myTFT.rgbTo18b(funBlu, funGrn, funRed);
  strcpy_P(buffer, (char *)pgm_read_word(&(MIDIList[funInstrument])));  // Necessary casts and dereferencing, just copy.
  myTFT.print(buffer);
}

void updatePercussionInstrumentOnScreen(int funInstrument){
  funInstrument = constrain(funInstrument, 27, 88);
  int funXCoord = xMax-105;
//  int funYCoord = 15+40+40+40;
//  int funXCoord = 60;
  int funYCoord = 150;
  int funRed = 255;
  int funGrn = 0;
  int funBlu = 0;
  // Draw a black rectangle over the area we want to update
  defaultColor = myTFT.hsvTo18b(0, 0, 0);
  myTFT.rectangle(funXCoord, funYCoord, funXCoord+95, funYCoord+10, true, (color_t)&defaultColor);
  // Write text over the black rectangle
  myTFT.setTextCursor(funXCoord+1,funYCoord);
  defaultColor = myTFT.rgbTo18b(funBlu, funGrn, funRed);
  strcpy_P(buffer, (char *)pgm_read_word(&(MIDIPercussionList[funInstrument - 27])));  // Necessary casts and dereferencing, just copy.
  myTFT.print(buffer);
}
