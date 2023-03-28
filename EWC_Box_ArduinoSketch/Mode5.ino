void playMode5(){
  if(millis() >= playNextPercussion){
    for(int i=0;i<=percussionTracks;i++){
      if(i == percussionTracks){
        bool funCurrentNoteUsed = false;
        if(percussionPlayback[i][percussionCounter] > 0){
           funCurrentNoteUsed = true;
        }
        int funPreviousNote = percussionCounter - 1;
        if(funPreviousNote<0){
          funPreviousNote = 15;
        }
        bool funPreviousNoteUsed = false;
        if(percussionPlayback[i][funPreviousNote] > 0){
           funPreviousNoteUsed = true;
        }
      }
      if(percussionPlayback[i][percussionCounter] > 0){
        midiNoteOff(9, percussionInstruments[i], 0);
        int funPercussionNoteVelocity = (percussionVelocity * percussionPlayback[i][percussionCounter])/100;
        midiNoteOn(9, percussionInstruments[i], funPercussionNoteVelocity);
      }
    }
    playNextPercussion = millis() + mainTempo/4;
    percussionCounter++;
    if((percussionCounter>15) || (percussionCounter<0)){
      percussionCounter = 0;
    }
  }
  if(millis() >= playNextNote){
    calculateMainNote();
    for(int i=0;i<=melodyTracks;i++){
      if(calculateOddsOfPlaying(multipleInstrumentsMode2[i][2])){
        // Turn off the note is something is playing, then
        // reset the array slot to all zeroes.
        if(multipleInstrumentsMode2[i][0] > 0){
          midiNoteOff(i, multipleInstrumentsMode2[i][0], 0);
        }
        // Figure out the new note by applying the offset
        multipleInstrumentsMode2[i][0] = mainNote + multipleInstrumentsMode2[i][3];
        int funMelodyNoteVelocity = (melodyVelocity * multipleInstrumentsMode2[i][1]) / 100;
        midiNoteOn(i, multipleInstrumentsMode2[i][0], funMelodyNoteVelocity);
      }
      playNextNote = millis() + mainTempo;
    }
  }
}

void potsInMode5(){
  melodyVelocity = pot3PreviousLevel/8;
  updateBluePotValueOnScreen(melodyVelocity);
  
  percussionVelocity = pot2PreviousLevel/8;
  updateGreenPotValueOnScreen(percussionVelocity);
  
  melodyProbability = map(pot1PreviousLevel, 0, 1023, 0, 100);
  updateYellowPotValueOnScreen(melodyProbability);
    
  percussionProbability = map(pot0PreviousLevel, 0, 1023, 0, 100);
  updateRedPotValueOnScreen(percussionProbability);
}

void encodersInMode5(){
  // encoder0 controls the percussion tracks (Red knob)
  if(selection0Previous != selection0){
    percussionTracks = selection0;
    updateRedEncoderValueOnScreen(percussionTracks);
  }
  
  // encoder1 controls the melody channels (Yellow knob)
  if(selection1Previous != selection1){
    midiNoteOff(selection1Previous, multipleInstrumentsMode2[selection1Previous][0], 0);
    melodyTracks = selection1;
    updateYellowEncoderValueOnScreen(melodyTracks);
  }
  
  // encoder2 controls the selected track (Green knob)
  if(selection2Previous != selection2){
    lowerThreshold = selection2;
    updateGreenEncoderValueOnScreen(lowerThreshold);
  }
  
  // encoder3 controls the tempo (Blue knob)
  if(selection3Previous != selection3){
    mainBPM = selection3;
    mainTempo = calculateTempo(mainBPM);
    updateBlueEncoderValueOnScreen(mainBPM);
  }
}

void encoderSoothingInMode5(){
  // Percussion and melody mode
  // The red encoder controls the number of percussion tracks
  if(selection0<0){
    selection0 = 0;
  }
  if(selection0>7){
    selection0 = 7;
  }
  
  // The yellow encoder controls the number of melody channels
  if(selection1<0){
    selection1 = 0;
  }
  if(selection1>7){
    selection1 = 7;
  }
  
  // The green encoder controls the pitch the melody will focus on
  if(selection2<24){
    selection2 = 24;
  }
  if(selection2>103){
    selection2 = 103;
  }
  
  // The blue encoder controls the tempo, as usual
  if(selection3<minTempo){
    // The minTempo can be changed in the declaration, but I set the bottom to 30
    selection3 = minTempo;
  }
  if(selection3>maxTempo){
    // The maxTempo can be changed in the declaration, but I set the cap at 600
    selection3 = maxTempo;
  }
}
  

void prandomizeInMode5(){
  int funMinimumVelocity = 60;
  // PRandomize will assign prandom notes and velocities to 
  // the selected tracks. Velocity will serve as the cap for 
  // volume, and intensity will calculate how many notes are active.
  
  // Turn any active notes off
  for(int i=0;i<8;i++){
    for(int k=0;k<16;k++){
      percussionPlayback[i][k] = 0;
    }
  }
  // Go through all 8 percussion tracks and
  // assign a random instrument from the MIDI list, 27-87 then
  // determine if a note will occur at points on the sixteen beats.
  for(int i=0;i<8;i++){
    percussionInstruments[i] = randomPercussionInstruments();
    for(int k=0;k<16;k++){
      if(calculateOddsOfPlaying(percussionProbability)){
        percussionPlayback[i][k] = random(30, 101);
      }
    }
  }
  
  
  // Pick the parameters for melody notes
  // Turn off any channels that might have stored notes
  for(int i=0;i<8;i++){
    if(multipleInstrumentsMode2[i][0] > 0){
      midiNoteOff(i, multipleInstrumentsMode2[i][0], 0);
    }
    multipleInstrumentsMode2[i][0] = 0;
  }
  // Assign each channel instruments from the approved list.
  // Note        [0] should equal the current mainNote
  // Velocity    [1] should be prandomized from 30 to 100
  // Probability [2] should be prandomized from 5 to 100
  // Offset      [3] should be prandomized from (-2 to 2)*12
  for(int i=0;i<8;i++){
    if(multipleInstrumentsMode2[i][0] > 0){
      midiNoteOff(i, multipleInstrumentsMode2[i][0], 0);
    }
    multipleInstrumentsMode2[i][0] = mainNote;
    
    int velocityCap = max(funMinimumVelocity, melodyVelocity);
    multipleInstrumentsMode2[i][1] = random(funMinimumVelocity, 101);
    
    multipleInstrumentsMode2[i][2] = random(5,101);
    
    int funOffset = random(0,5);
    funOffset = funOffset - 2;
    funOffset = funOffset * 12;
    multipleInstrumentsMode2[i][3] = funOffset;
    if(verboseFeedback){
      Serial.print(F("Instrument "));
      Serial.print(i);
      Serial.print(F("\tNote: "));
      Serial.print(mainNote);
      Serial.print(F("\tVelocity: "));
      Serial.print(multipleInstrumentsMode2[i][1]);
      Serial.print(F("\tProbability: "));
      Serial.print(multipleInstrumentsMode2[i][2]);
      Serial.print(F("\tOffset: "));
      Serial.print(multipleInstrumentsMode2[i][3]);
      Serial.print("\n");
    }
  }
  
  if(verboseFeedback){
    Serial.print(F("Channel Instruments\n"));
  }
  for(int i=0;i<8;i++){
    int funPRandomInstrument = random(0,119);
    midiSetInstrument(i,funPRandomInstrument);
    instrumentChannels[i] = funPRandomInstrument;
    if(verboseFeedback){
      Serial.print(F("Channel "));
      Serial.print(i);
      Serial.print(F("\tInstrument "));
      Serial.print(funPRandomInstrument);
      Serial.print("\n\n");
    }
  }
  if(verboseFeedback){
    Serial.print("\n");
  }
}

void startingParametersMode5(){
  selection0 = 0;
  selection1 = 0;
  selection2 = 60;
  selection3 = 120;
  
  updateRedEncoderValueOnScreen(selection0);
  updateYellowEncoderValueOnScreen(selection1);
  updateGreenEncoderValueOnScreen(selection2);
  updateBlueEncoderValueOnScreen(selection3);
}

void drawMode5(){
  drawScreenBackground();
  int funXOffset = 10;
  int funYOffset = 10;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(255, 0, 0);
  myTFT.print(F("Melody Vel"));
  
  funYOffset = funYOffset+ 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 255, 0);
  myTFT.print(F("Perc Vel"));
  
  funYOffset = funYOffset + 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 255, 255);
  myTFT.print("Melody %");
  
  funYOffset = funYOffset + 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 0, 255);
  myTFT.print("Perc %");
  
  
  funXOffset = 60;
  funYOffset = 30;
  
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b( 255, 255, 255 );
  defaultColor = myTFT.rgbTo18b(255, 0, 0);
  myTFT.print(F("     Tempo"));
  
  funYOffset = funYOffset + 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 255, 0);
  myTFT.print(F("     Pitch"));
  
  funYOffset = funYOffset + 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 255, 255);
  myTFT.print(F(" Melody Tk"));
  
  funYOffset = funYOffset + 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 0, 255);
  myTFT.print(F("   Perc Tk"));
}
