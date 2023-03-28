void readSerial(){
  if (Serial.available() > 0) {
    if (!serialDetected){
      Serial.print(F("Serial detected\n"));
      serialDetected = true;
    }
    serialDetected = true;
    incomingByte = Serial.read();
    switch (incomingByte){
      case 48:
        // Recognize a "0"
        mode = 0;
        Serial.print("Mode: ");
        Serial.print(mode);
        Serial.print("\n");
        newModeScreen();
        break;
      case 49:
        // Recognize a "1"
        mode = 1;
        Serial.print("Mode: ");
        Serial.print(mode);
        Serial.print("\n");
        newModeScreen();
        break;
      case 50:
        // Recognize a "2"
        mode = 2;
        Serial.print("Mode: ");
        Serial.print(mode);
        Serial.print("\n");
        newModeScreen();
        break;
      case 51:
        // Recognize a "3"
        mode = 3;
        Serial.print("Mode: ");
        Serial.print(mode);
        Serial.print("\n");
        newModeScreen();
        break;
      case 52:
        // Recognize a "4"
        mode = 4;
        Serial.print("Mode: ");
        Serial.print(mode);
        Serial.print("\n");
        newModeScreen();
        break;
      case 53:
        // Recognize a "5"
        mode = 5;
        Serial.print("Mode: ");
        Serial.print(mode);
        Serial.print("\n");
        newModeScreen();
        break;
      case 54:
        // Recognize a "6"
        mode = 6;
        Serial.print("Mode: ");
        Serial.print(mode);
        Serial.print("\n");
        newModeScreen();
        break;
      case 55:
        // Recognize a "7"
        mode = 7;
        Serial.print("Mode: ");
        Serial.print(mode);
        Serial.print("\n");
        newModeScreen();
        break;
      case 56:
        // Recognize a "8"
        mode = 8;
        Serial.print("Mode: ");
        Serial.print(mode);
        Serial.print("\n");
        newModeScreen();
        break;
      case 57:
        // Recognize a "9"
        mode = 9;
        Serial.print("Mode: ");
        Serial.print(mode);
        Serial.print("\n");
        newModeScreen();
        break;
      case 70:
        // Recognize a "F"
        potentiometerFeedbackOnScreen = !potentiometerFeedbackOnScreen;
        newModeScreen();
        break;
      case 102:
        // Recognize a "f"
        potentiometerFeedbackOnScreen = !potentiometerFeedbackOnScreen;
        newModeScreen();
        break;
      case 80:
        // Recognize a "P"
        panic();
        break;
      case 112:
        // Recognize a "p"
        panic();
        break;
      case 82:
        // Recognize a "R"
        prandomizeButtonAction();
        break;
      case 114:
        // Recognize a "r"
        prandomizeButtonAction();
        break;
      case 83:
        // Recognize a "S"
        Serial.print(F("STUFF Symbol"));
        Serial.print("\n");
        showSTUFFSymbol();
        break;
      case 115:
        // Recognize a "s"
        Serial.print(F("STUFF Symbol"));
        Serial.print("\n");
        showSTUFFSymbol();
        break;
      case 116:
        // Recognize a "t"
        screenTest();
        break;
      case 118:
        // Recognize a "v"
        Serial.print(F("Showing visual feedback"));
        Serial.print("\n");
        myTFT.clearDisplay();
        visualDebugging = true;
        encodersOnScreen();
        potsOnScreen();
        break;
      case 86:
        // Recognize a "V"
        Serial.print(F("Showing visual feedback"));
        Serial.print("\n");
        myTFT.clearDisplay();
        visualDebugging = true;
        encodersOnScreen();
        potsOnScreen();
        break;
      case 91:
        // Recognize a "["
        Serial.print(F("Printing minimal feedback"));
        Serial.print("\n");
        verboseFeedback = false;
//        for (int i=0;i<4;i++){
//          updateMoodOnScreen(i);
//        }
        break;
      case 93:
        // Recognize a "]"
        Serial.print(F("Printing verbose feedback\n"));
        Serial.print(F("Visual Feedback: "));
        Serial.print(visualDebugging);
        Serial.print(F("\nVerbose Feedback: "));
        Serial.print(visualDebugging);
        Serial.print(F("\nPotentiometer Feedback: "));
        Serial.print(potentiometerFeedbackOnScreen);
        Serial.print(F("\nFree play mode: "));
        Serial.print(freePlayMode);
        Serial.print("\n");
        verboseFeedback = true;
        break;
      case 63:
        // Recognize a "?"
        Serial.print(F("\n\t\tAvailable commands\n"));
        Serial.print(F("?\tDisplay this help menu\n"));
        Serial.print(F("[\tSend minimal feedback\n"));
        Serial.print(F("]\tSend verbose feedback\n"));
        Serial.print(F("F\tShow potentiometer values on screen\n"));
        Serial.print(F("P\tPanic. Stop all notes\n"));
        Serial.print(F("R\tPrandomize\n"));
        Serial.print(F("S\tSTUFF Symbol\n"));
        Serial.print(F("V\tShow encoders and pots on screen\n"));
        Serial.print(F("0-9\tDirect access to any mode\n"));
        break;
      default:
        if ((incomingByte == 10) || (incomingByte == 13)){
          break;
        }else{
          Serial.print(F("Your command \""));
          Serial.write(incomingByte);
          Serial.print(F("\" was not a recognized command. Type ? for help.\n"));
          Serial.print(incomingByte, DEC);
          Serial.write("\n");
        }
        break;
    }
  }
}
