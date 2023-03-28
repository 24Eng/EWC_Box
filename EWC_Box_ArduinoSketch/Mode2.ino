void playMode2(){
  if(millis() >= playNextNote){
    calculateMainNote();
    for(int i=0;i<16;i++){
      if(calculateOddsOfPlaying(multipleInstrumentsMode2[i][2])){
        // Turn off the note is something is playing, then
        // reset the array slot to all zeroes.
        if(multipleInstrumentsMode2[i][0] > 0){
          midiNoteOff(i, multipleInstrumentsMode2[i][0], 0);
        }
        // Figure out the new note by applying the offset
        multipleInstrumentsMode2[i][0] = mainNote + multipleInstrumentsMode2[i][3];
        midiNoteOn(i, multipleInstrumentsMode2[i][0], multipleInstrumentsMode2[i][1]);
      }
      playNextNote = millis() + mainTempo;
    }
  }
}

void stopMode2(){
  for(int i=0;i<16;i++){
    if(multipleInstrumentsMode2[i][0] > 0){
      midiNoteOff(i, multipleInstrumentsMode2[i][0], 0);
    }
  }
}

void potsInMode2(){
  multipleInstrumentsMode2[currentChannel][1] = pot3PreviousLevel/8;
  updateBluePotValueOnScreen(mainVelocity);
  multipleInstrumentsMode2[currentChannel][2] = map(pot2PreviousLevel, 0, 1023, 0, 100);
  updateGreenPotValueOnScreen(percentChanceToPlay);
  
  // Recognize that pot1 controls the upper pitch range (Yellow knob)
  upperThreshold = pot1PreviousLevel/8;
  
  // Recognize that pot0 controls the lower pitch range (Yellow knob)
  lowerThreshold = pot0PreviousLevel/8;
  if(upperThreshold < lowerThreshold){
    upperThreshold = lowerThreshold;
  }
  updateYellowPotValueOnScreen(upperThreshold);
  updateRedPotValueOnScreen(lowerThreshold);
}

void encodersInMode2(){
  // Recognize that encoder3 controls the tempo (Blue knob)
  mainBPM = selection3;
  mainTempo = calculateTempo(mainBPM);
  updateBlueEncoderValueOnScreen(mainBPM);
  // Recognize that encoder2 controls the selected channel (Green knob)
  if(selection2Previous != selection2){
    currentChannel = selection2;
    updateGreenEncoderValueOnScreen(currentChannel);
    updateInstrumentOnScreen(instrumentChannels[currentChannel]);
    selection0 = instrumentChannels[currentChannel];
    selection1 = multipleInstrumentsMode2[currentChannel][3];
  }
  
  // Recognize that encoder1 controls the note offset (Yellow knob)
  multipleInstrumentsMode2[currentChannel][3] = selection1;
  updateYellowEncoderValueOnScreen(multipleInstrumentsMode2[currentChannel][3]);
  
  // Change instruments according to selection0 (Red knob)
  if(selection0Previous != selection0){
    mainInstrument = selection0;
    instrumentChannels[currentChannel] = selection0;
    midiSetInstrument(currentChannel, instrumentChannels[currentChannel]);
    updateInstrumentOnScreen(instrumentChannels[currentChannel]);
  }
  
}

void encoderSoothingInMode2(){
  // The instrument selections should wrap around, so when
  // the value drops below 0, it shoots to 127, and when it
  // exceeds 127, it drops back to 0.
  if(selection0<0){
    selection0 = selection0Max;
  }
  if(selection0>selection0Max){
    selection0 = 0;
  }
  
  // The currentNoteOffset should range from -24 to 24, or 
  // two octaves lower or higher.
  if(selection1<(-24)){
    selection1 = (-24);
  }
  if(selection1>24){
    selection1 = 24;
  }
  
  // The channel selector should not drop below 0, or above 15
  // Channel 09 is percussion and I may need a way to skip over that.
  if(selection2<0){
    selection2 = 0;
  }
  if(selection2>15){
    selection2 = 15;
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



void prandomizeInMode2(){
  // Turn off any channels that might have stored notes
  for(int i=0;i<16;i++){
    if(multipleInstrumentsMode2[i][0] > 0){
      midiNoteOff(i, multipleInstrumentsMode2[i][0], 0);
    }
    multipleInstrumentsMode2[i][0] = 0;
  }
  // Look at the active channel, this is how many instruments should be prandomized.
  // Assign them instruments from the approved list.
  // Note        [0] should equal the current mainNote
  // Velocity    [1] should be prandomized from 30 to 127
  // Probability [2] should be prandomized from 5 to 100
  // Offset      [3] should be prandomized from (-2 to 2)*12
  for(int i=0;i<16;i++){
    if((i<=currentChannel) && i!= 9){
      if(multipleInstrumentsMode2[i][0] > 0){
        midiNoteOff(i, multipleInstrumentsMode2[i][0], 0);
      }
      multipleInstrumentsMode2[i][0] = mainNote;
      multipleInstrumentsMode2[i][1] = random(30, 128);
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
    }else{
      multipleInstrumentsMode2[i][0] = 0;
      multipleInstrumentsMode2[i][1] = 0;
      multipleInstrumentsMode2[i][2] = 0;
      multipleInstrumentsMode2[i][3] = 0;
    }
  }
  
  if(verboseFeedback){
      Serial.print(F("Channel Instruments\n"));
  }
  for(int i=0;i<16;i++){
    int funPRandomInstrument = random(0,119);
    if((i<=currentChannel) && i!= 9){
      midiSetInstrument(i,funPRandomInstrument);
      instrumentChannels[i] = funPRandomInstrument;
    }else{
      instrumentChannels[i] = 0;
    }
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
  updateInstrumentOnScreen(instrumentChannels[currentChannel]);
  updateYellowEncoderValueOnScreen(multipleInstrumentsMode2[currentChannel][3]);
}

void drawMode2(){
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
  myTFT.print("Hi Pitch");
  
  funYOffset = funYOffset + 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 0, 255);
  myTFT.print("Low Pitch");
  
  
  funXOffset = 60;
  funYOffset = 30;
  
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b( 255, 255, 255 );
  defaultColor = myTFT.rgbTo18b(255, 0, 0);
  myTFT.print(F("     Tempo"));
  
  funYOffset = funYOffset+ 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 255, 0);
  myTFT.print(F("   Channel"));
  
  funYOffset = funYOffset + 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 255, 255);
  myTFT.print(F("    Offset"));
  
  funYOffset = funYOffset + 40;
  myTFT.setTextCursor(funXOffset,funYOffset);
  defaultColor = myTFT.rgbTo18b(0, 0, 255);
  myTFT.print(F("Instrument"));
}
