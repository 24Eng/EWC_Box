void playMode3(){
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
    if(verboseFeedback && !digitalRead(prandomizePushbutton)){
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
    playNextPercussion = millis() + mainTempo/4;
    percussionCounter++;
    // Increment the counter and loop back when it passes 15.
    if((percussionCounter>15) || (percussionCounter<0)){
      percussionCounter = 0;
    }
  }
}

void potsInMode3(){
  mainVelocity = pot3PreviousLevel/8;
  updateBluePotValueOnScreen(mainVelocity);
  
  percussionProbability = map(pot2PreviousLevel, 0, 1023, 0, 100);
  updateGreenPotValueOnScreen(percussionProbability);
}

void encodersInMode3(){
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
  
  // Change percussion instruments according to selection0 (Red knob)
  if(selection0Previous != selection0){
    percussionInstruments[currentTrack] = selection0;
  }
  updatePercussionInstrumentOnScreen(percussionInstruments[currentTrack]);
}

void encoderSoothingInMode3(){
  // Percussion mode
  // The percussion selections should wrap around, so when
  // the value drops below 27, it shoots to 87, and when it
  // exceeds 87, it drops back to 27.
  if(selection0<27){
    selection0 = 87;
  }
  if(selection0>87){
    selection0 = 27;
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


void prandomizeInMode3(){
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
  // Go through all the active tracks, up to 16 of them and
  // assign a random instrument from the MIDI list, 27-87 then
  // determine if a note will occur at one of the sixteenn 
  for(int i=0;i<=currentTrack;i++){
    percussionInstruments[i] = randomPercussionInstruments();
    for(int k=0;k<16;k++){
      if(calculateOddsOfPlaying(percussionProbability)){
        int velocityCap = max(funMinimumVelocity, mainVelocity);
        percussionPlayback[i][k] = random(funMinimumVelocity, velocityCap);
      }
    }
  }
  drawChannelBeats();
  updatePercussionInstrumentOnScreen(percussionInstruments[currentTrack]);
}

int randomPercussionInstruments(){
  // Generate a prandom number between 27 and 87, but
  // if the number is on the undesiralbe list, then
  // generate a number between 35 and 48, which are 
  // drums that usually do well in beat loops.
  int funRawInstrument = random(27,88);
  if((funRawInstrument == 27) || (funRawInstrument == 29) || (funRawInstrument == 30)|| (funRawInstrument == 49) || (funRawInstrument == 51) || (funRawInstrument == 57) || (funRawInstrument == 59) || (funRawInstrument == 71)|| (funRawInstrument == 72)|| (funRawInstrument == 78)|| (funRawInstrument == 79)){
    funRawInstrument = random(35,49);
  }
  return funRawInstrument;
}
      

void drawMode3(){
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
