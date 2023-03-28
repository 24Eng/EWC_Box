void playMode1(){
  if(millis() >= playNextNote){
    if(freePlayMode){
      midiNoteOff(0, mainNote, 0);
      calculateMainNote();
      midiNoteOn(0, mainNote, mainVelocity);
      playNextNote = 2147483647;
    }else{
      if(calculateOddsOfPlaying(percentChanceToPlay)){
        midiNoteOff(0, mainNote, 0);
        calculateMainNoteRandomly();
        midiNoteOn(0, mainNote, mainVelocity);
      }
      playNextNote = millis() + mainTempo;
    }
  }
}

void prandomizeInMode1(){
  playNextNote = millis();
  freePlayMode = true;
}

void potsInMode1(){
  mainVelocity = pot3PreviousLevel/8;
  updateBluePotValueOnScreen(mainVelocity);
  percentChanceToPlay = map(pot2PreviousLevel, 0, 1023, 0, 100);
  updateGreenPotValueOnScreen(percentChanceToPlay);
}


void encodersInMode1(){
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


void encoderSoothingInMode1(){
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
  if(selection3>180){
    // The maxTempo must cap at 100 so the RNG can keep up
    selection3 = 180;
  }
}
