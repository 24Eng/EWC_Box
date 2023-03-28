// Ensure that the note about to be played is within the limits.
int maintainNoteLimits(int funStartingPoint, int funRequestedChange, int funLowerLimit, int funUpperLimit){
  int safelyRangedChange;
  boolean unaltered = true;
  int requestedNote = funStartingPoint + minorPentatonicScale[funRequestedChange];
  // Assess if the requested note needs to be raised to keep it above the lower threshold.
  if (requestedNote < funLowerLimit){
    // If the requested change is already greater than six, the limits were probably
    // changed recently so just keep going to a higher note. Else, add six to make the change positive.
    if (funRequestedChange > 6){
      safelyRangedChange = funRequestedChange;
      unaltered = false;
    }else{
      safelyRangedChange = 6 + funRequestedChange;
      unaltered = false;
    }
  }
  // Assess if the requested note needs to be lowered to keep it below the upper threshold.
  if ((requestedNote > funUpperLimit) && unaltered){
    // If the requested change is already less than six, the limits were probably
    // changed recently so just keep going to a lower note. Else, subtract six to make the change negative.
    if (funRequestedChange < 6){
      safelyRangedChange = funRequestedChange;
    }else{
      safelyRangedChange = funRequestedChange - 6;
    }
  }
  // For an unknown reason, the next print statement must be intact for
  // the code to run properly.
  // DO NOT DELETE THIS PRINTLN STATEMENT.
  // But sometimes deleting it works too. YMMV
//  Serial.print("");
//  Serial.println(safelyRangedChange);
  // For an unknown reason, the previous print statement must be intact for
  // the code to run properly.
  // DO NOT DELETE THIS PRINTLN STATEMENT
  // But sometimes deleting it works too. YMMV
  safelyRangedChange = constrain(safelyRangedChange, 0, 11);
  return safelyRangedChange;
}


void calculateMainNote(){
  // Generate the next note.
  noteDelta = random(0, 12);
  // Check to see that the note will be within the prescribed range.
  noteDelta = maintainNoteLimits(mainNote, noteDelta, lowerThreshold, upperThreshold);
  // Change the mainNote variable according to the selected scale.
  mainNote = mainNote + minorPentatonicScale[noteDelta];
  nextNoteIteration = timeThisCycle + (mainTempo);
}

void calculateMainNoteRandomly(){
  // Generate the next note.
  noteDelta = Entropy.random(0,12);
//  noteDelta = (Entropy.random(WDT_RETURN_BYTE) / 21);
//  noteDelta = constrain(noteDelta, 0, 11);
  if(verboseFeedback){
    Serial.print("Note delta: ");
    Serial.print(noteDelta);
    Serial.print("\n");
  }
  // Check to see that the note will be within the prescribed range.
  noteDelta = maintainNoteLimits(mainNote, noteDelta, lowerThreshold, upperThreshold);
  // Change the mainNote variable according to the selected scale.
  mainNote = mainNote + minorPentatonicScale[noteDelta];
  nextNoteIteration = timeThisCycle + (mainTempo);
}

void panic(){
  Serial2.write(0xFF);
  for (int i = 0; i < 16; i++){
    for (int k = 0; k < 127; k++){
      midiNoteOff(i, k, 0);
    }
    midiSetChannelVolume(i, 127);
  }
}


void stopAllNotes(){
  for (int e=0; e <= 1; e++){
    for (int i = 0; i < 16; i++){
      for (int k = 0; k < 127; k++){
        midiNoteOff(i, k, 0);
      }
      primaryNotes[i][4][e] = 0;
    }
  }
}

void stopActiveNotes(){
  for (int e=0; e <= 1; e++){
    for (int i = 0; i <= activeChannels; i++){
      for (int k = 0; k < 127; k++){
        midiNoteOff(i, k, 0);
      }
      primaryNotes[i][4][e] = 0;
    }
  }
}

int calculateTempo(int funBPM){
  // The mainTempo is the number of milliseconds between notes whereas BPM is beats-per-minute but more
  // accurately the notes-per-minute in this context.
  // Protocol is to assign or change the mainBPM, send it to this rountine, and return the mainTempo value and
  // and print the BPM if request by verboseFeedback.
  if(false){
    Serial.print("BPM: ");
    Serial.print(funBPM);
    Serial.print("\t");
  }
  return (60000 / funBPM);
}


bool calculateOddsOfPlaying(int funPercentage){
  int funDieRoll = random(0, 100);
  if(funDieRoll <= funPercentage){
    return true;
  }
  return false;
}
