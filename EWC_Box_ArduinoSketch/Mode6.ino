void playMode6(){
  checkForTimedChages();
  // This is the percussion-playing part of the mode
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
        midiNoteOn(9, percussionInstruments[i], percussionVelocity);
      }
    }
    playNextPercussion = millis() + mainTempo/4;
    percussionCounter++;
    if((percussionCounter>15) || (percussionCounter<0)){
      percussionCounter = 0;
    }
  }
  
  // This is the melody-playing part of the mode
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
//        int funMelodyNoteVelocity = (melodyVelocity * multipleInstrumentsMode2[i][1]) / 100;
        midiNoteOn(i, multipleInstrumentsMode2[i][0], melodyVelocity);
        
      }
    }
    playNextNote = millis() + mainTempo;
  }
}


void checkForTimedChages(){
  if(millis() >= percussionVelocityTimer){
    updatePercussionVelocity();
    percussionVelocityTimer = millis() + (deltaPercussionVelocityInterval * 1000);
  }
  if(millis() >= percussionProbabilityTimer){
    updatePercussionProbability();
    percussionProbabilityTimer = millis() + (deltaPercussionProbabilityInterval * 1000);
  }
  if(millis() >= percussionTempoTimer){
    updatePercussionTempo();
    percussionTempoTimer = millis() + (deltaPercussionTempoInterval * 1000);
  }
  if(millis() >= percussionInstrumentTimer){
    updatePercussionInstrument();
    percussionInstrumentTimer = millis() + (deltaPercussionInstrumentInterval * 1000);
  }
  if(millis() >= percussionTracksTimer){
    updatePercussionTracks();
    percussionTracksTimer = millis() + (deltaPercussionTracksInterval * 1000);
  }
}

void updatePercussionVelocity(){
  // Increase or decrease the percussion velocity
  percussionVelocity = percussionVelocity + random(-deltaPercussionVelocity, deltaPercussionVelocity+1);
  percussionVelocity = constrain(percussionVelocity, minPercussionVelocity, maxPercussionVelocity);
  if(verboseFeedback){
    Serial.print(F("Perc Vel:\t"));
    Serial.print(percussionVelocity);
    Serial.print("\t");
  }
  
  // Increase or decrease the melody velocity
  melodyVelocity = melodyVelocity + random(-deltaPercussionVelocity, deltaPercussionVelocity+1);
  melodyVelocity = constrain(melodyVelocity, minPercussionVelocity, maxPercussionVelocity);
  if(verboseFeedback){
    Serial.print(F("Mel Vel:\t"));
    Serial.print(melodyVelocity);
    Serial.print("\n");
  }
}
void updatePercussionProbability(){
  // Increase or decrease the percussion probability for new tracks
  percussionProbability = percussionProbability + random(-deltaPercussionProbability, deltaPercussionProbability+1);
  percussionProbability = constrain(percussionProbability, minPercussionProbability, maxPercussionProbability);
  if(verboseFeedback){
    Serial.print(F("Perc Prob:\t"));
    Serial.print(percussionProbability);
    Serial.print("\t");
  }
  // Increase or decrease the probability for melody notes
  melodyProbability = melodyProbability + random(-deltaPercussionProbability, deltaPercussionProbability+1);
  melodyProbability = constrain(melodyProbability, minPercussionProbability, maxPercussionProbability);
  if(verboseFeedback){
    Serial.print(F("Mel Prob:\t"));
    Serial.print(melodyProbability);
    Serial.print("\n");
  }
}
void updatePercussionTempo(){
  // Increase or decrease the tempo
  mainBPM = mainBPM + random(-deltaPercussionTempo, deltaPercussionTempo+1);
  mainBPM = constrain(mainBPM, minBPM, maxBPM);
  mainTempo = calculateTempo(mainBPM);
  if(verboseFeedback){
    Serial.print(F("BPM:\t"));
    Serial.print(mainBPM);
    Serial.print("\n");
  }
}
void updatePercussionInstrument(){
  // Change the instrument playing at the designated track.
  percussionInstruments[deltaInstrumentTracker] = randomPercussionInstruments();
  deltaInstrumentTracker++;
  if(deltaInstrumentTracker>percussionTracks){
    deltaInstrumentTracker = 0;
  }
  // Change an active channel's instrument
  int funPrandomChannel = random(0,melodyTracks+1);
  int funPRandomInstrument = random(0,119);
  midiSetInstrument(funPrandomChannel, funPRandomInstrument);
}
void updatePercussionTracks(){
  // Increase or decrease the number of percussion tracks
  percussionTracks = percussionTracks + random(-deltaPercussionTracks, deltaPercussionTracks+1);
  percussionTracks = constrain(percussionTracks, 0, 7);
  if(verboseFeedback){
    Serial.print(F("Trk:\t"));
    Serial.print(percussionTracks);
    Serial.print("\n");
  }
  // Increase or decrease the number of melody tracks
  melodyTracks = melodyTracks + random(-deltaPercussionTracks, deltaPercussionTracks+1);
  melodyTracks = constrain(melodyTracks, 0, 7);
}

void potsInMode6(){
  deltaPercussionVelocityInterval = map(pot3PreviousLevel, 0, 1023, 1, maxPercussionDelta);
  percussionVelocityTimer = millis() + (deltaPercussionVelocityInterval * 1000);
  updateBluePotValueOnScreen(deltaPercussionVelocityInterval);
  
  deltaPercussionProbabilityInterval = map(pot2PreviousLevel, 0, 1023, 1, maxPercussionDelta);
  percussionProbabilityTimer = millis() + (deltaPercussionProbabilityInterval * 1000);
  updateGreenPotValueOnScreen(deltaPercussionProbabilityInterval);
}

void encodersInMode6(){
  // encoder0 is not used in mode6()
  
  // encoder1 controls the delay between track incrementation/decrementation (Yellow knob)
  if(selection1Previous != selection1){
    deltaPercussionTracksInterval = selection1;
    updateYellowEncoderValueOnScreen(deltaPercussionTracksInterval);
  }
  
  // encoder2 controls the delay between instrument changes (Green knob)
  if(selection2Previous != selection2){
    deltaPercussionInstrumentInterval = selection2;
    updateGreenEncoderValueOnScreen(deltaPercussionInstrumentInterval);
  }
  
  // encoder3 controls the delay between tempo changes (Blue knob)
  if(selection3Previous != selection3){
    deltaPercussionTempoInterval = selection3;
    updateBlueEncoderValueOnScreen(deltaPercussionTempoInterval);
  }
}

void encoderSoothingInMode6(){
  // Percussion progression
  // The red encoder is not used in mode6() 
  if(selection0<0){
    selection0 = 0;
  }
  if(selection0>1){
    selection0 = 1;
  }
  
  // The yellow encoder controls the delay between track incrementation/decrementation
  if(selection1<0){
    selection1 = 0;
  }
  if(selection1>maxPercussionDelta){
    selection1 = maxPercussionDelta;
  }
  
  // The green encoder controls the delay between instrument changes
  if(selection2<0){
    selection2 = 0;
  }
  if(selection2>maxPercussionDelta){
    selection2 = maxPercussionDelta;
  }
  
  // The blue encoder controls the delay between tempo changes
  if(selection3<0){
    selection3 = 0;
  }
  if(selection3>maxPercussionDelta){
    selection3 = maxPercussionDelta;
  }
}
  

void prandomizeInMode6(){
  playNextPercussion = millis();
  playNextNote = millis();
  // Set the playback variables to prandom values
  percussionVelocity = random(minPercussionVelocity, maxPercussionVelocity+1);
  percussionProbability = random(minPercussionProbability, maxPercussionProbability+1);
  percussionTracks = random(0, 8);
  mainBPM = random(minBPM, maxBPM+1);
  mainTempo = calculateTempo(mainBPM);
  
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
  if(verboseFeedback){
    Serial.print(F("Velocity:\t"));
    Serial.print(percussionVelocity);
    Serial.print(F("\tProbability:\t"));
    Serial.print(percussionProbability);
    Serial.print(F("\tTracks:\t"));
    Serial.print(percussionTracks);
    Serial.print(F("\tBPM:\t"));
    Serial.print(mainBPM);
    Serial.print("\n");
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
    
    multipleInstrumentsMode2[i][1] = melodyVelocity;
    
    multipleInstrumentsMode2[i][2] = melodyProbability;
    
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

void startingParametersMode6(){
  selection0 = 0;
  selection1 = maxPercussionDelta/2;
  selection2 = maxPercussionDelta/2;
  selection3 = maxPercussionDelta/2;
  percussionCounter = 0;
  
  updateYellowEncoderValueOnScreen(selection1);
  updateGreenEncoderValueOnScreen(selection2);
  updateBlueEncoderValueOnScreen(selection3);
}

void drawMode6(){
  drawScreenBackground();
  int funXOffset = 10;
  int funYOffset = 10;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(255, 0, 0);
  myTFT.print(F("Velocity  "));
  
  funYOffset = funYOffset+ 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 255, 0);
  myTFT.print(F("%       "));
  
  funYOffset = funYOffset + 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 255, 255);
  myTFT.print("-");
  
  funYOffset = funYOffset + 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 0, 255);
  myTFT.print("-");
  
  funXOffset = 60;
  funYOffset = 30;
  
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b( 255, 255, 255 );
  defaultColor = myTFT.rgbTo18b(255, 0, 0);
  myTFT.print(F("     Tempo"));
  
  funYOffset = funYOffset + 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 255, 0);
  myTFT.print(F("      Inst"));
  
  funYOffset = funYOffset + 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 255, 255);
  myTFT.print(F("    Tracks"));
  
  funYOffset = funYOffset + 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 0, 255);
  myTFT.print(F("         -"));
}
