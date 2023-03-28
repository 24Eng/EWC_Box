void handleEncoders(){
  sootheEncoders();
  switch(mode){
    case 0:
      encodersInMode0();
      finishHandlingDecoders();
      return;
    case 1:
      encodersInMode1();
      finishHandlingDecoders();
      return;
    case 2:
      encodersInMode2();
      finishHandlingDecoders();
      return;
    case 3:
      encodersInMode3();
      finishHandlingDecoders();
      return;
    case 4:
      encodersInMode4();
      finishHandlingDecoders();
      return;
    case 5:
      encodersInMode5();
      finishHandlingDecoders();
      return;
    case 6:
      encodersInMode6();
      finishHandlingDecoders();
      return;
    default:
      finishHandlingDecoders();
      return;
  }
}

void finishHandlingDecoders(){
  selection0Previous = selection0;
  selection1Previous = selection1;
  selection2Previous = selection2;
  selection3Previous = selection3;
  if(visualDebugging){
    encodersOnScreen();
  }
}

void handlePots(){
  if(verboseFeedback){
    printPotentiometers();
  }
  if(visualDebugging){
    potsOnScreen();
  }
  switch(mode){
    case 0:
      potsInMode0();
      return;
    case 1:
      potsInMode1();
      return;
    case 2:
      potsInMode2();
      return;
    case 3:
      potsInMode3();
      return;
    case 4:
      potsInMode4();
      return;
    case 5:
      potsInMode5();
      return;
    case 6:
      potsInMode6();
      return;
    default:
      
      return;
  }
}

void printPotentiometers(){
  Serial.print("Pot0: ");
  Serial.print(pot0PreviousLevel);
  Serial.print("\t");
  Serial.print("Pot1: ");
  Serial.print(pot1PreviousLevel);
  Serial.print("\t");
  Serial.print("Pot2: ");
  Serial.print(pot2PreviousLevel);
  Serial.print("\t");
  Serial.print("Pot3: ");
  Serial.print(pot3PreviousLevel);
  Serial.print("\n");
}


void printEncoders(){
    Serial.print(selection0);
    Serial.print("\t");
    Serial.print(selection1);
    Serial.print("\t");
    Serial.print(selection2);
    Serial.print("\t");
    Serial.print(selection3);
    Serial.print("\n");
//    Serial2.write(0x090);
}

void potentiometerStates(){
  int pot0Level = analogRead(pot0);
  int pot1Level = analogRead(pot1);
  int pot2Level = analogRead(pot2);
  int pot3Level = analogRead(pot3);
  if((pot0Level > (pot0PreviousLevel + potThreshold)) || (pot0Level < (pot0PreviousLevel - potThreshold))){
    pot0PreviousLevel = pot0Level;
    changePot = HIGH;
  }
  if((pot1Level > (pot1PreviousLevel + potThreshold)) || (pot1Level < (pot1PreviousLevel - potThreshold))){
    pot1PreviousLevel = pot1Level;
    changePot = HIGH;
  }
  if((pot2Level > (pot2PreviousLevel + potThreshold)) || (pot2Level < (pot2PreviousLevel - potThreshold))){
    pot2PreviousLevel = pot2Level;
    changePot = HIGH;
  }
  if((pot3Level > (pot3PreviousLevel + potThreshold)) || (pot3Level < (pot3PreviousLevel - potThreshold))){
    pot3PreviousLevel = pot3Level;
    changePot = HIGH;
  }
}

void buttonStates(){
  bool modeState = digitalRead(modePushbutton);
  bool prandomizeState = digitalRead(prandomizePushbutton);
  modePushbuttonRising = false;
  prandomizePushbuttonRising = false;
  if(!modeState && modePushbuttonPreviousState && (millis() > modeDebounceTimer)){
    modePushbuttonRising = true;
    modeDebounceTimer = millis() + debounceTime*10;
    mode++;
    if(mode > totalModes-1){
      mode = 0;
    }
    if(verboseFeedback){
      Serial.print(F("Mode "));
      Serial.print(mode);
      Serial.print("\n");
    }
    visualDebugging = false;
    newModeScreen();
  }
  modePushbuttonPreviousState = modeState;
  
  if(!prandomizeState && prandomizePushbuttonPreviousState && (millis() > prandomizeDebounceTimer)){
    prandomizePushbuttonRising = true;
    prandomizeDebounceTimer = millis() + debounceTime*6;
    prandomizeButtonAction();
    if(verboseFeedback){
      Serial.print(F("Prandomize\n"));
    }
  }
  prandomizePushbuttonPreviousState = prandomizeState;
}

void prandomizeButtonAction(){
  switch(mode){
    case 0:
      prandomizeInMode0();
      return;
    case 1:
      prandomizeInMode1();
      return;
    case 2:
      prandomizeInMode2();
      return;
    case 3:
      prandomizeInMode3();
      return;
    case 4:
      prandomizeInMode4();
      return;
    case 5:
      prandomizeInMode5();
      return;
    case 6:
      prandomizeInMode6();
      return;
    default:
      if(verboseFeedback){
        Serial.print("PRandomize not used here\n");
      }
      return;
  }
}

void encoder0Triggered(){
  if((debounceTimer + debounceTime) > millis()){
    return;
  }else{
    debounceTimer = millis();
  }
  int encoderButton = digitalRead(encoder0Bttn);
  if(digitalRead(encoder0Pin1)){
    if(!encoderButton){
      selection0 = selection0 - 5;
    }else{
      selection0--;
    }
  }else{
    if(!encoderButton){
      selection0 = selection0 + 5;
    }else{
      selection0++;
    }
  }
  changeBit = HIGH;
}

void encoder1Triggered(){
  if((debounceTimer + debounceTime) > millis()){
    return;
  }else{
    debounceTimer = millis();
  }
  int encoderButton = digitalRead(encoder1Bttn);
  if(digitalRead(encoder1Pin1)){
    if(!encoderButton){
      selection1 = selection1 - 5;
    }else{
      selection1--;
    }
  }else{
    if(!encoderButton){
      selection1 = selection1 + 5;
    }else{
      selection1++;
    }
  }
  changeBit = HIGH;
}

void encoder2Triggered(){
  if((debounceTimer + debounceTime) > millis()){
    return;
  }else{
    debounceTimer = millis();
  }
  int encoderButton = digitalRead(encoder2Bttn);
  if(digitalRead(encoder2Pin1)){
    if(!encoderButton){
      selection2 = selection2 - 5;
    }else{
      selection2--;
    }
  }else{
    if(!encoderButton){
      selection2 = selection2 + 5;
    }else{
      selection2++;
    }
  }
  changeBit = HIGH;
}

void encoder3Triggered(){
  if((debounceTimer + debounceTime) > millis()){
    return;
  }else{
    debounceTimer = millis();
  }
  int encoderButton = digitalRead(encoder3Bttn);
  if(digitalRead(encoder3Pin1)){
    if(!encoderButton){
      selection3 = selection3 - 5;
    }else{
      selection3--;
    }
  }else{
    if(!encoderButton){
      selection3 = selection3 + 5;
    }else{
      selection3++;
    }
  }
  changeBit = HIGH;
}

void sootheEncoders(){
  switch(mode){
    case 0:
      encoderSoothingInMode0();
      return;
    case 1:
      encoderSoothingInMode1();
      return;
    case 2:
      encoderSoothingInMode2();
      return;
    case 3:
      encoderSoothingInMode3();
      return;
    case 4:
      encoderSoothingInMode4();
      return;
    case 5:
      encoderSoothingInMode5();
      return;
    case 6:
      encoderSoothingInMode6();
      return;
    default:
      if(selection0<0){
        selection0 = selection0Max;
      }
      if(selection0>selection0Max){
        selection0 = 0;
      }
      
      if(selection1<0){
        selection1 = selection1Max;
      }
      if(selection1>selection1Max){
        selection1 = 0;
      }
      
      if(selection2<0){
        selection2 = selection2Max;
      }
      if(selection2>selection2Max){
        selection2 = 0;
      }
      
      if(selection3<0){
        selection3 = selection3Max;
      }
      if(selection3>selection3Max){
        selection3 = 0;
      }
      return;
  }
}

void modeBehavior(){
  switch(mode){
    case 0:
      playMode0();
      return;
    case 1:
      playMode1();
      return;
    case 2:
      playMode2();
      return;
    case 3:
      playMode3();
      return;
    case 4:
      playMode4();
      return;
    case 5:
      playMode5();
      return;
    case 6:
      playMode6();
      return;
    default:
      
      return;
  }
}
