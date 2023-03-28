// Include libraries. Sources are listed in the "info" tab
#include "HyperDisplay_KWH018ST01_4WSPI.h" // Click here to get the library: http://librarymanager/All#SparkFun_HyperDisplay_KWH018ST01_4WSPI
                                           // Click here to get the mid-level library: http://librarymanager/All#SparkFun_HyperDisplay_ILI9163C
                                           // Click here to get HyperDisplay (top level): http://librarymanager/All#SparkFun_HyperDisplay

#define PWM_PIN 44
#define CS_PIN 40
#define DC_PIN 41
#define SPI_PORT SPI
#define SPI_SPEED 32000000
KWH018ST01_4WSPI myTFT;           // The KWH018ST01_4WSPI class is used for this breakout, and we will call our object myTFT
ILI9163C_color_18_t defaultColor; // Global objects are used for default colors so that they are always in-scope

#include <Entropy.h>
// The following addresses a problem in version 1.0.5 and earlier of the
// Arduino IDE that prevents randomSeed from working properly.
//   https://github.com/arduino/Arduino/issues/575
#define randomSeed(s) srandom(s)
uint32_t seed_value;
uint8_t random_byte;


// Define the encoder pins
const int encoder0Pin0 = 2;
const int encoder0Pin1 = 22;
const int encoder0Bttn = 23;
const int encoder1Pin0 = 3;
const int encoder1Pin1 = 24;
const int encoder1Bttn = 25;
const int encoder2Pin0 = 18;
const int encoder2Pin1 = 26;
const int encoder2Bttn = 27;
const int encoder3Pin0 = 19;
const int encoder3Pin1 = 28;
const int encoder3Bttn = 29;


// Define the potentiometers
const int pot0 = A0;
const int pot1 = A1;
const int pot2 = A2;
const int pot3 = A3;


// Define the pushbuttons
const int modePushbutton = 32;
const int prandomizePushbutton = 33;


// Define the global variables
int selection0 = 0;
int selection1 = 0;
int selection2 = 0;
int selection3 = 0;
int selection0Previous = 0;
int selection1Previous = 0;
int selection2Previous = 0;
int selection3Previous = 0;
const int selection0Max = 127;
const int selection1Max = 127;
const int selection2Max = 127;
const int selection3Max = 127;
bool changeBit = LOW;
const int debounceTime = 20;
long debounceTimer = 0;
bool modePushbuttonRising = 0;
long modeDebounceTimer = 0;
bool modePushbuttonPreviousState = 0;
bool prandomizePushbuttonRising = 0;
long prandomizeDebounceTimer = 0;
bool prandomizePushbuttonPreviousState = 0;
const int potThreshold = 6;
int pot0PreviousLevel = 0;
int pot1PreviousLevel = 0;
int pot2PreviousLevel = 0;
int pot3PreviousLevel = 0;
int pot0ScreenLevel = 0;
int pot1ScreenLevel = 0;
int pot2ScreenLevel = 0;
int pot3ScreenLevel = 0;
bool changePot = LOW;
int xMax = 128;
int yMax = 160;
int mode = 0;
const int totalModes = 8;
bool serialDetected = false;
int beatCircleXOffset = 35;

// The deltaPercussion**** values are seconds between
// changes in the patterns. First used in mode6()
const int maxPercussionDelta = 30;
int deltaPercussionVelocityInterval = maxPercussionDelta/2;
int deltaPercussionProbabilityInterval = maxPercussionDelta/2;
int deltaPercussionTempoInterval = maxPercussionDelta/2;
int deltaPercussionInstrumentInterval = maxPercussionDelta/2;
int deltaPercussionTracksInterval = maxPercussionDelta/2;

// The ****Timer versions of the above registers contain
// the time, according to millis(), to trigger a change.
long percussionVelocityTimer = 0;
long percussionProbabilityTimer = 0;
long percussionTempoTimer = 0;
long percussionInstrumentTimer = 0;
long percussionTracksTimer = 0;

// The following values determine how much to tweak the
// running value when automatically changing an arrangement.
int deltaPercussionVelocity = 5;
int deltaPercussionProbability = 5;
int deltaPercussionTempo = 5;
int deltaPercussionInstrument = 1;
int deltaPercussionTracks = 1;

const int minPercussionVelocity = 60;
const int maxPercussionVelocity = 127;
const int minPercussionProbability = 5;
const int maxPercussionProbability = 60;
const int minBPM = 60;
const int maxBPM = 180;
int deltaInstrumentTracker = 0;

// MIDI constants from Adafruit code
#define MIDI_NOTE_ON  0x90
#define MIDI_NOTE_OFF 0x80
#define MIDI_CHAN_MSG 0xB0
#define MIDI_CHAN_BANK 0x00
#define MIDI_CHAN_VOLUME 0x07
#define MIDI_CHAN_PROGRAM 0xC0

// Create some useful variables
int currentNote = 60;
int maxTempo = 600;
int minTempo = 30;
int activeChannels = 0;
int runningChannels = 0;
int lowerThreshold = 48;
int upperThreshold = 72;
long playNextNote = 0;
long playNextPercussion = 0;
int percentChanceToPlay = 100;
int mainInstrument = 0;
int instrumentChannels[16];
int currentChannel = 0;
int currentNoteOffset = 0;
int currentTrack = 0;
int percussionProbability = 0;
int percussionInstruments[8];
// percussionPlayback is eight tracks, with sixteen possible locations
// where a note can happen.
int percussionPlayback[8][16];
int percussionCounter = 0;
bool freePlayMode = false;
int prandomEight[8];
int percussionVelocity = 0;
int percussionTracks = 0;
int melodyVelocity = 0;
int melodyProbability = 0;
int melodyTracks = 0;

// Copy over some variables from old code
int noteDelta = 0;
int mainNote = 60;
int mainVelocity = 70;
int mainTempo = 500;
int mainBPM = 120;
int mainIntensity = 0;
int oldMainIntensity = 0;
long nextNoteIteration = 0;
int mainChannel = 0;
long timeThisCycle;
int minIntensity = 0;
int maxIntensity = 299;
boolean updateIntensity = false;
int intensityBreaks = 6;
int intensityBreakpoint = (maxIntensity / intensityBreaks) + 1;
int BGMSet = 0;
int totalBGMs = 1;  //Zero justified

// We only need one scale and we don't have any chords.
const int minorPentatonicScale[] = {-12, -9, -8, -5, -2, 0, 3, 4, 7, 10, 12};


// We are creating an array to contain all the notes which need to play and
// it will contain a slot for each channel, 0-15, although channel 0x9 (percussion), may
// need to be used in a different way.
// Note, Time on, Time off, Velocity, Expired
// 0000, 1111111, 22222222, 33333333, 4444444
long primaryNotes[16][5][2];
boolean evenOddNotePlaying = false;

// Each time a channel gets prandom values assigned to it, they
// get stored here. The instrument is assigned another way, but
// that will get prandomized another way.
// Velocity is on this list, but that value is set by the 
// potentiometer.
// Pitch offset, Time offset, Play frequency, Duration, Velocity, Instrument
// 000000000000, 11111111111, 22222222222222, 33333333, 44444444, 5555555555
int channelTweaks[16][6];

/*
*/
// The multipleInstrumentsMode2[][] array contains sixteen slots, one for
// each channel, and three partitions each to contain modifiers.
// note, Velocity, probability, offset
// 0000, 11111111, 22222222222, 333333
int multipleInstrumentsMode2[16][4];

// Stuff for reading from the SD card
// The array has four sections for the four moods. 
// Each mood has thirty fields for the instruments and behaviors of that mood, but
// not all the fields need to be populated.
const int moodVariables = 50;
int moods[4][moodVariables];

const char percussion_027[] PROGMEM = "          High Q";
const char percussion_028[] PROGMEM = "            Slap";
const char percussion_029[] PROGMEM = "    Scratch Push";
const char percussion_030[] PROGMEM = "    Scratch Pull";
const char percussion_031[] PROGMEM = "          Sticks";
const char percussion_032[] PROGMEM = "    Square Click";
const char percussion_033[] PROGMEM = " Metronome Click";
const char percussion_034[] PROGMEM = "  Metronome Bell";
const char percussion_035[] PROGMEM = "     Bass Drum 2";
const char percussion_036[] PROGMEM = "     Bass Drum 1";
const char percussion_037[] PROGMEM = "      Side Stick";
const char percussion_038[] PROGMEM = "    Snare Drum 1";
const char percussion_039[] PROGMEM = "       Hand Clap";
const char percussion_040[] PROGMEM = "    Snare Drum 2";
const char percussion_041[] PROGMEM = "       Low Tom 2";
const char percussion_042[] PROGMEM = "   Closed Hi-hat";
const char percussion_043[] PROGMEM = "       Low Tom 1";
const char percussion_044[] PROGMEM = "    Pedal Hi-hat";
const char percussion_045[] PROGMEM = "       Mid Tom 2";
const char percussion_046[] PROGMEM = "     Open Hi-hat";
const char percussion_047[] PROGMEM = "       Mid Tom 1";
const char percussion_048[] PROGMEM = "      High Tom 2";
const char percussion_049[] PROGMEM = "  Crash Cymbal 1";
const char percussion_050[] PROGMEM = "      High Tom 1";
const char percussion_051[] PROGMEM = "   Ride Cymbal 1";
const char percussion_052[] PROGMEM = "  Chinese Cymbal";
const char percussion_053[] PROGMEM = "       Ride Bell";
const char percussion_054[] PROGMEM = "      Tambourine";
const char percussion_055[] PROGMEM = "   Splash Cymbal";
const char percussion_056[] PROGMEM = "         Cowbell";
const char percussion_057[] PROGMEM = "  Crash Cymbal 2";
const char percussion_058[] PROGMEM = "      Vibra Slap";
const char percussion_059[] PROGMEM = "   Ride Cymbal 2";
const char percussion_060[] PROGMEM = "      High Bongo";
const char percussion_061[] PROGMEM = "       Low Bongo";
const char percussion_062[] PROGMEM = " Mute High Conga";
const char percussion_063[] PROGMEM = " Open High Conga";
const char percussion_064[] PROGMEM = "       Low Conga";
const char percussion_065[] PROGMEM = "    High Timbale";
const char percussion_066[] PROGMEM = "     Low Timbale";
const char percussion_067[] PROGMEM = "      High Agogo";
const char percussion_068[] PROGMEM = "       Low Agogo";
const char percussion_069[] PROGMEM = "          Cabasa";
const char percussion_070[] PROGMEM = "         Maracas";
const char percussion_071[] PROGMEM = "   Short Whistle";
const char percussion_072[] PROGMEM = "    Long Whistle";
const char percussion_073[] PROGMEM = "     Short Guiro";
const char percussion_074[] PROGMEM = "      Long Guiro";
const char percussion_075[] PROGMEM = "          Claves";
const char percussion_076[] PROGMEM = " High Wood Block";
const char percussion_077[] PROGMEM = "  Low Wood Block";
const char percussion_078[] PROGMEM = "      Mute Cuica";
const char percussion_079[] PROGMEM = "      Open Cuica";
const char percussion_080[] PROGMEM = "   Mute Triangle";
const char percussion_081[] PROGMEM = "   Open Triangle";
const char percussion_082[] PROGMEM = "          Shaker";
const char percussion_083[] PROGMEM = "     Jingle Bell";
const char percussion_084[] PROGMEM = "        Belltree";
const char percussion_085[] PROGMEM = "       Castanets";
const char percussion_086[] PROGMEM = "      Mute Surdo";
const char percussion_087[] PROGMEM = "      Open Surdo";

const char *const MIDIPercussionList[61] PROGMEM = {
    percussion_027,
    percussion_028,
    percussion_029,
    percussion_030,
    percussion_031,
    percussion_032,
    percussion_033,
    percussion_034,
    percussion_035,
    percussion_036,
    percussion_037,
    percussion_038,
    percussion_039,
    percussion_040,
    percussion_041,
    percussion_042,
    percussion_043,
    percussion_044,
    percussion_045,
    percussion_046,
    percussion_047,
    percussion_048,
    percussion_049,
    percussion_050,
    percussion_051,
    percussion_052,
    percussion_053,
    percussion_054,
    percussion_055,
    percussion_056,
    percussion_057,
    percussion_058,
    percussion_059,
    percussion_060,
    percussion_061,
    percussion_062,
    percussion_063,
    percussion_064,
    percussion_065,
    percussion_066,
    percussion_067,
    percussion_068,
    percussion_069,
    percussion_070,
    percussion_071,
    percussion_072,
    percussion_073,
    percussion_074,
    percussion_075,
    percussion_076,
    percussion_077,
    percussion_078,
    percussion_079,
    percussion_080,
    percussion_081,
    percussion_082,
    percussion_083,
    percussion_084,
    percussion_085,
    percussion_086,
    percussion_087
};


const char instrument_000[] PROGMEM = "Acou Grand Piano";
const char instrument_001[] PROGMEM = " Brt Acou Piano ";
const char instrument_002[] PROGMEM = "Elec Grand Piano";
const char instrument_003[] PROGMEM = "Honky-tonk Piano";
const char instrument_004[] PROGMEM = "Electric Piano 1";
const char instrument_005[] PROGMEM = "Electric Piano 2";
const char instrument_006[] PROGMEM = "  Harpsichord   ";
const char instrument_007[] PROGMEM = "    Clavinet    ";
const char instrument_008[] PROGMEM = "    Celesta     ";
const char instrument_009[] PROGMEM = "  Glockenspiel  ";
const char instrument_010[] PROGMEM = "   Music Box    ";
const char instrument_011[] PROGMEM = "   Vibraphone   ";
const char instrument_012[] PROGMEM = "    Marimba     ";
const char instrument_013[] PROGMEM = "   Xylophone    ";
const char instrument_014[] PROGMEM = " Tubular Bells  ";
const char instrument_015[] PROGMEM = "    Dulcimer    ";
const char instrument_016[] PROGMEM = " Drawbar Organ  ";
const char instrument_017[] PROGMEM = "Percussive Organ";
const char instrument_018[] PROGMEM = "   Rock Organ   ";
const char instrument_019[] PROGMEM = "  Church Organ  ";
const char instrument_020[] PROGMEM = "   Reed Organ   ";
const char instrument_021[] PROGMEM = "   Accordion    ";
const char instrument_022[] PROGMEM = "   Harmonica    ";
const char instrument_023[] PROGMEM = "Tango Accordion ";
const char instrument_024[] PROGMEM = "Ac Guitar(Nylon)";
const char instrument_025[] PROGMEM = "Ac Guitar(Steel)";
const char instrument_026[] PROGMEM = "Elc Guitar(Jazz)";
const char instrument_027[] PROGMEM = "Ec Guitar(Clean)";
const char instrument_028[] PROGMEM = "Ec Guitar(Muted)";
const char instrument_029[] PROGMEM = "Overdrive Guitar";
const char instrument_030[] PROGMEM = "  Dist Guitar   ";
const char instrument_031[] PROGMEM = "Guitar Harmonics";
const char instrument_032[] PROGMEM = " Acoustic Bass  ";
const char instrument_033[] PROGMEM = "Ec Bass (Finger)";
const char instrument_034[] PROGMEM = " Ec Bass (Pick) ";
const char instrument_035[] PROGMEM = " Fretless Bass  ";
const char instrument_036[] PROGMEM = "  Slap Bass 1   ";
const char instrument_037[] PROGMEM = "  Slap Bass 2   ";
const char instrument_038[] PROGMEM = "  Synth Bass 1  ";
const char instrument_039[] PROGMEM = "  Synth Bass 2  ";
const char instrument_040[] PROGMEM = "     Violin     ";
const char instrument_041[] PROGMEM = "     Viola      ";
const char instrument_042[] PROGMEM = "     Cello      ";
const char instrument_043[] PROGMEM = "   Contrabass   ";
const char instrument_044[] PROGMEM = "Tremolo Strings ";
const char instrument_045[] PROGMEM = "Pizzicato Stngs ";
const char instrument_046[] PROGMEM = "Orchestral Harp ";
const char instrument_047[] PROGMEM = "    Timpani     ";
const char instrument_048[] PROGMEM = "String Ensemble1";
const char instrument_049[] PROGMEM = "String Ensemble2";
const char instrument_050[] PROGMEM = "Synth Strings 1 ";
const char instrument_051[] PROGMEM = "Synth Strings 2 ";
const char instrument_052[] PROGMEM = "   Choir Aahs   ";
const char instrument_053[] PROGMEM = "   Voice Oohs   ";
const char instrument_054[] PROGMEM = "  Synth Choir   ";
const char instrument_055[] PROGMEM = " Orchestra Hit  ";
const char instrument_056[] PROGMEM = "    Trumpet     ";
const char instrument_057[] PROGMEM = "    Trombone    ";
const char instrument_058[] PROGMEM = "      Tuba      ";
const char instrument_059[] PROGMEM = " Muted Trumpet  ";
const char instrument_060[] PROGMEM = "  French Horn   ";
const char instrument_061[] PROGMEM = " Brass Section  ";
const char instrument_062[] PROGMEM = " Synth Brass 1  ";
const char instrument_063[] PROGMEM = " Synth Brass 2  ";
const char instrument_064[] PROGMEM = "  Soprano Sax   ";
const char instrument_065[] PROGMEM = "    Alto Sax    ";
const char instrument_066[] PROGMEM = "   Tenor Sax    ";
const char instrument_067[] PROGMEM = "  Baritone Sax  ";
const char instrument_068[] PROGMEM = "      Oboe      ";
const char instrument_069[] PROGMEM = "  English Horn  ";
const char instrument_070[] PROGMEM = "    Bassoon     ";
const char instrument_071[] PROGMEM = "    Clarinet    ";
const char instrument_072[] PROGMEM = "    Piccolo     ";
const char instrument_073[] PROGMEM = "     Flute      ";
const char instrument_074[] PROGMEM = "    Recorder    ";
const char instrument_075[] PROGMEM = "   Pan Flute    ";
const char instrument_076[] PROGMEM = "  Blown bottle  ";
const char instrument_077[] PROGMEM = "   Shakuhachi   ";
const char instrument_078[] PROGMEM = "    Whistle     ";
const char instrument_079[] PROGMEM = "    Ocarina     ";
const char instrument_080[] PROGMEM = "Lead 1 (Square) ";
const char instrument_081[] PROGMEM = "Lead 2(Sawtooth)";
const char instrument_082[] PROGMEM = "Lead 3(Calliope)";
const char instrument_083[] PROGMEM = " Lead 4 (Chiff) ";
const char instrument_084[] PROGMEM = "Lead 5 (Charang)";
const char instrument_085[] PROGMEM = " Lead 6 (Voice) ";
const char instrument_086[] PROGMEM = "Lead 7 (Fifths) ";
const char instrument_087[] PROGMEM = "Lead8(Bass+Lead)";
const char instrument_088[] PROGMEM = "Pad 1 (New Age) ";
const char instrument_089[] PROGMEM = "  Pad 2 (Warm)  ";
const char instrument_090[] PROGMEM = "Pad 3(Polysynth)";
const char instrument_091[] PROGMEM = " Pad 4 (Choir)  ";
const char instrument_092[] PROGMEM = " Pad 5 (Bowed)  ";
const char instrument_093[] PROGMEM = "Pad 6 (Metallic)";
const char instrument_094[] PROGMEM = "  Pad 7 (Halo)  ";
const char instrument_095[] PROGMEM = " Pad 8 (Sweep)  ";
const char instrument_096[] PROGMEM = "  FX 1 (Rain)   ";
const char instrument_097[] PROGMEM = "FX 2(Soundtrack)";
const char instrument_098[] PROGMEM = " FX 3 (Crystal) ";
const char instrument_099[] PROGMEM = "FX 4(Atmosphere)";
const char instrument_100[] PROGMEM = "FX 5(Brightness)";
const char instrument_101[] PROGMEM = " FX 6 (Goblins) ";
const char instrument_102[] PROGMEM = " FX 7 (Echoes)  ";
const char instrument_103[] PROGMEM = " FX 8 (Sci-fi)  ";
const char instrument_104[] PROGMEM = "     Sitar      ";
const char instrument_105[] PROGMEM = "     Banjo      ";
const char instrument_106[] PROGMEM = "    Shamisen    ";
const char instrument_107[] PROGMEM = "      Koto      ";
const char instrument_108[] PROGMEM = "    Kalimba     ";
const char instrument_109[] PROGMEM = "    Bagpipe     ";
const char instrument_110[] PROGMEM = "     Fiddle     ";
const char instrument_111[] PROGMEM = "     Shanai     ";
const char instrument_112[] PROGMEM = "  Tinkle Bell   ";
const char instrument_113[] PROGMEM = "     Agogo      ";
const char instrument_114[] PROGMEM = "  Steel Drums   ";
const char instrument_115[] PROGMEM = "   Woodblock    ";
const char instrument_116[] PROGMEM = "   Taiko Drum   ";
const char instrument_117[] PROGMEM = "  Melodic Tom   ";
const char instrument_118[] PROGMEM = "   Synth Drum   ";
const char instrument_119[] PROGMEM = " Reverse Cymbal ";
const char instrument_120[] PROGMEM = "  Guitar Fret   ";
const char instrument_121[] PROGMEM = "  Breath Noise  ";
const char instrument_122[] PROGMEM = "    Seashore    ";
const char instrument_123[] PROGMEM = "   Bird Tweet   ";
const char instrument_124[] PROGMEM = " Telephone Ring ";
const char instrument_125[] PROGMEM = "   Helicopter   ";
const char instrument_126[] PROGMEM = "    Applause    ";
const char instrument_127[] PROGMEM = "    Gunshot     ";

const char *const MIDIList[128] PROGMEM = {
  instrument_000,
  instrument_001,
  instrument_002,
  instrument_003,
  instrument_004,
  instrument_005,
  instrument_006,
  instrument_007,
  instrument_008,
  instrument_009,
  instrument_010,
  instrument_011,
  instrument_012,
  instrument_013,
  instrument_014,
  instrument_015,
  instrument_016,
  instrument_017,
  instrument_018,
  instrument_019,
  instrument_020,
  instrument_021,
  instrument_022,
  instrument_023,
  instrument_024,
  instrument_025,
  instrument_026,
  instrument_027,
  instrument_028,
  instrument_029,
  instrument_030,
  instrument_031,
  instrument_032,
  instrument_033,
  instrument_034,
  instrument_035,
  instrument_036,
  instrument_037,
  instrument_038,
  instrument_039,
  instrument_040,
  instrument_041,
  instrument_042,
  instrument_043,
  instrument_044,
  instrument_045,
  instrument_046,
  instrument_047,
  instrument_048,
  instrument_049,
  instrument_050,
  instrument_051,
  instrument_052,
  instrument_053,
  instrument_054,
  instrument_055,
  instrument_056,
  instrument_057,
  instrument_058,
  instrument_059,
  instrument_060,
  instrument_061,
  instrument_062,
  instrument_063,
  instrument_064,
  instrument_065,
  instrument_066,
  instrument_067,
  instrument_068,
  instrument_069,
  instrument_070,
  instrument_071,
  instrument_072,
  instrument_073,
  instrument_074,
  instrument_075,
  instrument_076,
  instrument_077,
  instrument_078,
  instrument_079,
  instrument_080,
  instrument_081,
  instrument_082,
  instrument_083,
  instrument_084,
  instrument_085,
  instrument_086,
  instrument_087,
  instrument_088,
  instrument_089,
  instrument_090,
  instrument_091,
  instrument_092,
  instrument_093,
  instrument_094,
  instrument_095,
  instrument_096,
  instrument_097,
  instrument_098,
  instrument_099,
  instrument_100,
  instrument_101,
  instrument_102,
  instrument_103,
  instrument_104,
  instrument_105,
  instrument_106,
  instrument_107,
  instrument_108,
  instrument_109,
  instrument_110,
  instrument_111,
  instrument_112,
  instrument_113,
  instrument_114,
  instrument_115,
  instrument_116,
  instrument_117,
  instrument_118,
  instrument_119,
  instrument_120,
  instrument_121,
  instrument_122,
  instrument_123,
  instrument_124,
  instrument_125,
  instrument_126,
  instrument_127
};
char buffer[16];

bool verboseFeedback = false;
bool visualDebugging = false;
bool potentiometerFeedbackOnScreen = false;
int incomingByte = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////
/////////2/4///H/O/U/R///E/N/G/I/N/E/E/R//////////////////2/4///H/O/U/R///E/N/G/I/N/E/E/R/////////
//////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  // Use pins at inputs or outputs
  pinMode(encoder0Pin0, INPUT_PULLUP);
  pinMode(encoder0Pin1, INPUT_PULLUP);
  pinMode(encoder0Bttn, INPUT_PULLUP);
  pinMode(encoder1Pin0, INPUT_PULLUP);
  pinMode(encoder1Pin1, INPUT_PULLUP);
  pinMode(encoder1Bttn, INPUT_PULLUP);
  pinMode(encoder2Pin0, INPUT_PULLUP);
  pinMode(encoder2Pin1, INPUT_PULLUP);
  pinMode(encoder2Bttn, INPUT_PULLUP);
  pinMode(encoder3Pin0, INPUT_PULLUP);
  pinMode(encoder3Pin1, INPUT_PULLUP);
  pinMode(encoder3Bttn, INPUT_PULLUP);
  pinMode(modePushbutton, INPUT_PULLUP);
  pinMode(prandomizePushbutton, INPUT_PULLUP);
  pinMode(pot0, INPUT);
  pinMode(pot1, INPUT);
  pinMode(pot2, INPUT);
  pinMode(pot3, INPUT);
  
  
//  selection3 = mainBPM;
//  mainTempo = calculateTempo(mainBPM);
//  selection2 = upperThreshold;
//  selection1 = lowerThreshold;
  
  // Perform screen's startup sequence
  delay(100);
  myTFT.begin(DC_PIN, CS_PIN, PWM_PIN, SPI_PORT, SPI_SPEED);
  myTFT.clearDisplay();
  myTFT.setTextCursor(0,0);            // Sets the cursor relative to the current window, however the current (default) window is equivalent to the whole display. (0,0) is the upper left corner and (myTFT.xExt-1, myTFT.yExt-1) is the lower right
  myTFT.setCurrentWindowColorSequence((color_t)&defaultColor);
  newModeScreen();
  
  Serial.begin(115200);
  Serial2.begin(31250);
//  Serial.print(F("\nWelcome to Electronic Wind Chimes by Brian McEvoy\n"));
  
  midiSetInstrument(0, 4);
  channelTweaks[0][0] = 0;
  channelTweaks[0][1] = 0;
  channelTweaks[0][2] = 100;
  channelTweaks[0][3] = 10;
  channelTweaks[0][4] = mainVelocity;
  
  Entropy.initialize();
  seed_value = Entropy.random();
  // By using the Entropy library to seed the normal pseudo-random number generator which
  // ensures that the standard libraries random number generator will provide different starting
  // values each time the sketch is run.
  randomSeed(seed_value);
  
  attachInterrupt(digitalPinToInterrupt(encoder0Pin0), encoder0Triggered, FALLING);
  attachInterrupt(digitalPinToInterrupt(encoder1Pin0), encoder1Triggered, FALLING);
  attachInterrupt(digitalPinToInterrupt(encoder2Pin0), encoder2Triggered, FALLING);
  attachInterrupt(digitalPinToInterrupt(encoder3Pin0), encoder3Triggered, FALLING);
}

void loop() {
  buttonStates();
  potentiometerStates();
  readSerial();
  
  if(changeBit){
    handleEncoders();
    changeBit = LOW;
  }
  
  if(changePot){
    handlePots();
    changePot = LOW;
  }
  modeBehavior();
}
