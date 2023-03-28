void playMode0(){
  if(millis() >= playNextNote){
    midiNoteOff(0, mainNote, 0);
    if(freePlayMode){
      calculateMainNote();
      midiNoteOn(0, mainNote, mainVelocity);
      playNextNote = 2147483647;
    }else{
      if(calculateOddsOfPlaying(percentChanceToPlay)){
        calculateMainNote();
        midiNoteOn(0, mainNote, mainVelocity);
      }
      playNextNote = millis() + mainTempo;
    }
  }
}

void prandomizeInMode0(){
  playNextNote = millis();
  freePlayMode = true;
}

void potsInMode0(){
  mainVelocity = pot3PreviousLevel/8;
  updateBluePotValueOnScreen(mainVelocity);
  percentChanceToPlay = map(pot2PreviousLevel, 0, 1023, 0, 100);
  updateGreenPotValueOnScreen(percentChanceToPlay);
}

void encodersInMode0(){
  // Recognize that encoder3 controls the tempo (Blue knob)
  mainBPM = selection3;
  mainTempo = calculateTempo(mainBPM);
  if(mainBPM == 30){
    updateBlueEncoderValueOnScreen(0);
    freePlayMode = true;
  }else{
    updateBlueEncoderValueOnScreen(mainBPM);
    if(freePlayMode){
      playNextNote = millis();
      freePlayMode = false;
    }
  }
  
  // Recognize that encoder2 controls the upper pitch range (Green knob)
  upperThreshold = selection2;
  updateGreenEncoderValueOnScreen(upperThreshold);
  // Recognize that encoder1 controls the lower pitch range (Yellow knob)
  lowerThreshold = selection1;
  updateYellowEncoderValueOnScreen(lowerThreshold);
  
  // Change instruments according to selection0 (Red knob)
  mainInstrument = selection0;
  midiSetInstrument(0, mainInstrument);
  updateInstrumentOnScreen(mainInstrument);
}

void encoderSoothingInMode0(){
  // The instrument selections should wrap around, so when
  // the value drops below 0, it shoots to 127, and when it
  // exceeds 127, it drops back to 0.
  if(selection0<0){
    selection0 = selection0Max;
  }
  if(selection0>selection0Max){
    selection0 = 0;
  }
  
  // The lowerThreshold should not drop below zero or go higher than upperThreshold
  if(selection1<0){
    selection1 = 0;
  }
  if(selection1>upperThreshold){
    selection1 = upperThreshold;
  }
  // The upperThreshold should not rise above 127 or go lower than lowerThreshold
  if(selection2<lowerThreshold){
    selection2 = lowerThreshold;
  }
  if(selection2>127){
    selection2 = 127;
  }
  
  if(selection3<minTempo){
    // The minTempo can be changed in the declaration, but I set the bottom to 30
    selection3 = minTempo;
  }
  if(selection3>maxTempo){
    // The maxTempo can be changed in the declaration, but I set the cap at 600
    selection3 = maxTempo;
  }
}

void startingParametersMode0(){
  selection0 = 0;
  selection1 = 48;
  selection2 = 72;
  selection3 = 120;
  
  updateRedEncoderValueOnScreen(selection0);
  updateYellowEncoderValueOnScreen(selection1);
  updateGreenEncoderValueOnScreen(selection2);
  updateBlueEncoderValueOnScreen(selection3);
}

void drawMode0(){
  drawScreenBackground();
  int funXOffset = 10;
  int funYOffset = 10;
  myTFT.setTextCursor(funXOffset,funYOffset);
//  defaultColor = myTFT.rgbTo18b( 255, 255, 255 );
  defaultColor = myTFT.rgbTo18b(255, 0, 0);
  myTFT.print(F("Velocity"));
  
  funYOffset = funYOffset+ 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 255, 0);
  myTFT.print(F("% to play"));
  
  funYOffset = funYOffset + 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 255, 255);
  myTFT.print(" ");
  
  funYOffset = funYOffset + 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 0, 255);
  myTFT.print(" ");
  
  
  funXOffset = 60;
  funYOffset = 30;
  
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b( 255, 255, 255 );
  defaultColor = myTFT.rgbTo18b(255, 0, 0);
  myTFT.print(F("     Tempo"));
  
  funYOffset = funYOffset+ 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 255, 0);
  myTFT.print(F("  Hi Pitch"));
  
  funYOffset = funYOffset + 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 255, 255);
  myTFT.print(F(" Low Pitch"));
  
  funYOffset = funYOffset + 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 0, 255);
  myTFT.print(F("Instrument"));
}
