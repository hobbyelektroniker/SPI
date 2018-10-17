/*********************************************
 * Arduino UNO als SPI Master
 * Zeigt eine Textübermittlung zwischen zwei Arduinos
 * 16.10.2018 Der Hobbyelektroniker
 * Vers. 1.0
 * Der Code ist lizenzfrei und darf von jedermann frei verwendet werden
 *********************************************/
 #include <SPI.h>

// SPI - Pins für Arduino UNO
// const int MOSI = 11;
// const int MISO = 12;
// const int SCK = 13;
// const int SS = 10; // wird im Master nicht verwendet

const int megaSlave_select = 7;
String antwort;

void setup() {
  pinMode(megaSlave_select,OUTPUT);
  digitalWrite(megaSlave_select,HIGH);

  SPI.begin ();  // SPI Library initialisieren

  Serial.begin(115200);
}

void askAndReadAnswer(const char txt[]) {
  SPI.beginTransaction(SPISettings(20000, MSBFIRST, SPI_MODE0));
  digitalWrite(megaSlave_select,LOW);
  int i = 0;
  while (txt[i] != 0) { 
    SPI.transfer(txt[i]); // Zeichen senden
    delayMicroseconds(20);
    i++;
  };
  antwort = "";
  char ch;
  SPI.transfer(0); // Fertig, Antwortmodus starten
  delayMicroseconds(80);
  do {
    delayMicroseconds(20);
    ch = SPI.transfer(0); // Antwort anfordern
    antwort += ch;
  } while (ch != 0);
  digitalWrite(megaSlave_select,HIGH);
  SPI.endTransaction(); 
}

void loop() {
  delay(1000);
  Serial.println("Wer bist du?");
  askAndReadAnswer("Wer bist du?");
  Serial.println(antwort);
}
