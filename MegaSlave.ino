/*********************************************
 * Arduino MEGA als SPI Slave
 * Zeigt eine Textübermittlung zwischen zwei Arduinos
 * 16.10.2018 Der Hobbyelektroniker
 * Vers. 1.0
 * Der Code ist lizenzfrei und darf von jedermann frei verwendet werden
 *********************************************/
 #include <LiquidCrystal.h>

// SPI Pins für Arduino Mega
// const int MISO = 50;
// const int MOSI = 51;
// const int SCK = 52;
// const int SS = 53;

// rs, en, d4, d5, d6, d7
LiquidCrystal lcd(18,19,5,4,3,2);

char frage [100];
char antwort[100];
volatile byte fragePos;
volatile byte antwortPos;
volatile boolean fertig;
volatile boolean antworten;

void setup() {
  // LCD vorbereiten
  lcd.begin(16,2);

  // SPI Slave - Mode vorbereiten
  pinMode(MISO, OUTPUT); // Damit geben wir Antwort
  SPCR = B11000000; // SPI Control Register setzen
  // Bit 7: 1 --> Enable interrupt
  // Bit 6: 1 --> Enable SPI
  // Bit 5: 0 --> MSBFIRST
  // Bit 4: 0 --> Slave - Mode
  // Bit 3,2: 0 --> SPI_MODE0
  // Bit 1,0: 0 --> Geschwindigkeit irrelevant, passt sich Master an
  
  fertig = false;
  antworten = false;
  fragePos = 0;
  lcd.print("Der Mega h\xEFrt.."); // Mit Umlaut: Der Mega hört.. 
}

ISR (SPI_STC_vect)  //SPI Interrupt
{
  char c = SPDR;  // lesen eines Zeichens
  if (c) {
    antworten = false;
    fertig = false;
    frage[fragePos++] = c;  // zum Buffer hinzufügen
  } else { // ist fertig und wartet auf Antwort
    if (!antworten) {
      fertig = true;
      antwortPos = 0;
      antworten = true;
      strcpy( antwort, "Der Mega!" );
    }
    char ch = antwort[antwortPos++];
    SPDR = ch;
    if (ch == 0) antworten = false;
  }
}

void loop() {
  if (fertig) {
    frage[fragePos] = 0;
    lcd.setCursor(0,1); // 2. Zeile
    lcd.print(frage);
    fertig = false;
    fragePos = 0;
  }
}
