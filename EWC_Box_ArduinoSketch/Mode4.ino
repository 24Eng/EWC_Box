void playMode4(){
  if(millis() >= playNextPercussion){
    for(int i=0;i<=currentTrack;i++){
      if(i == currentTrack){
        // When the loop comes around to the currentChannel
        // update the relevant circles on the screen.
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
        updateBeatCircle(percussionCounter, true, funCurrentNoteUsed);
        updateBeatCircle(funPreviousNote, false, funPreviousNoteUsed);
      }
      // If there is an active note in the slot, stop any 
      // notes playing and play the new one at the prescribed velocity.
      if(percussionPlayback[i][percussionCounter] > 0){
        midiNoteOff(9, percussionInstruments[i], 0);
        midiNoteOn(9, percussionInstruments[i], percussionPlayback[i][percussionCounter]);
      }
    }
    playNextPercussion = millis() + mainTempo/4;
    percussionCounter++;
    // Increment the counter and loop back when it passes 15.
    if((percussionCounter>15) || (percussionCounter<0)){
      percussionCounter = 0;
    }
  }
}

void potsInMode4(){
  mainVelocity = pot3PreviousLevel/8;
  updateBluePotValueOnScreen(mainVelocity);
  percussionProbability = map(pot2PreviousLevel, 0, 1023, 0, 100);
  updateGreenPotValueOnScreen(percussionProbability);
}

void encodersInMode4(){
  // Recognize that encoder3 controls the tempo (Blue knob)
  mainBPM = selection3;
  mainTempo = calculateTempo(mainBPM);
  updateBlueEncoderValueOnScreen(mainBPM);
  // Recognize that encoder2 controls the selected track (Green knob)
  currentTrack = selection2;
  updateGreenEncoderValueOnScreen(currentTrack);
  if(selection2Previous != selection2){
    drawChannelBeats();
    selection0 = percussionInstruments[currentTrack];
  }
}

void eightShuffled(){
  // Reset the prandomEight[] array
  for(int i=0;i<8;i++){
    prandomEight[i] = 10;
  }
  
  for(int i=0;i<8;i++){
    bool funPrandomized = false;
    while(!funPrandomized){
      int funPrandomAssignment = random(0, 8);
      if(prandomEight[funPrandomAssignment] == 10){
        prandomEight[funPrandomAssignment] = i;
        funPrandomized = true;
      }
    }
  }
  if(verboseFeedback){
    Serial.print("\nPrandom order (0-7):\t\t");
    for(int i=0;i<8;i++){
      Serial.print(prandomEight[i]);
      Serial.print("\t");
    }
    Serial.print("\n\n");
  }
}

void encoderSoothingInMode4(){
  // Drum sampler mode
  // Encoder0 is not used
  if(selection0<0){
    selection0 = selection0Max;
  }
  if(selection0>selection1Max){
    selection0 = 0;
  }
  
  if(selection1<0){
    selection1 = selection1Max;
  }
  if(selection1>selection1Max){
    selection1 = 0;
  }
  
  // The track selector should not drop below 0, or exceed 7
  if(selection2<0){
    selection2 = 0;
  }
  if(selection2>7){
    selection2 = 7;
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
  

void prandomizeInMode4(){
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
  
  // Go through all 8 tracks and assign a random instrument from
  // 35 to 42 by adding 35 to prandomEight[x] after it has been prandomized.
  eightShuffled();
  for(int i=0;i<8;i++){
    percussionInstruments[i] = prandomEight[i] + 35;
  }
  
  // One difference between mode4() and mode3() is that this mode will
  // arrange a track for all eight instruments, even if they are higher
  // than the current track. Using this method, the user can select how
  // many instruments are playing, even if they PRandomize with few
  // tracks selected.
  for(int i=0;i<8;i++){
    for(int k=0;k<16;k++){
      if(calculateOddsOfPlaying(percussionProbability)){
        int velocityCap = max(funMinimumVelocity, mainVelocity);
        percussionPlayback[i][k] = random(funMinimumVelocity, velocityCap);
      }
    }
  }
  if(verboseFeedback){
    for(int i=0;i<8;i++){
      Serial.print("Channel ");
      Serial.print(i);
      for(int k=0;k<16;k++){
        Serial.print("\t");
        Serial.print(percussionPlayback[i][k]);
      }
      Serial.print("\n");
    }
    Serial.print("\n\n");
  }
  drawChannelBeats();
}


void drawMode4(){
  drawScreenBackground();
  int funXOffset = 10;
  int funYOffset = 10;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(255, 0, 0);
  myTFT.print(F("Vel"));
  
  funYOffset = funYOffset+ 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 255, 0);
  myTFT.print(F("%"));
  
  funYOffset = funYOffset + 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 255, 255);
  myTFT.print("");
  
  funYOffset = funYOffset + 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 0, 255);
  myTFT.print("");
  
  
  funXOffset = 60;
  funYOffset = 30;
  
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b( 255, 255, 255 );
  defaultColor = myTFT.rgbTo18b(255, 0, 0);
  myTFT.print(F("     Tempo"));
  
  funYOffset = funYOffset+ 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 255, 0);
  myTFT.print(F("     Track"));
  
  funYOffset = funYOffset + 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 255, 255);
  myTFT.print(F(""));
  
  funYOffset = funYOffset + 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 0, 255);
  myTFT.print(F(""));
}
