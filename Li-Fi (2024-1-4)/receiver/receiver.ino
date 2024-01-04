// DE

/* > Information (Teil II) <
  Dieser Code stellt den Teil II des Arduino-Programmier-Projekts, das im Rahmen des
  Informatikunterrichts der elften Klasse im ersten Semester veranlasst wurde, dar.
  
  Teil II macht hierbei den Receiver des mit Licht funktionierenden Texttransmitters aus.
  
  Nicht keine Kommentare sind für das Verständnis dieses Codes zu entbehren, wurden jedoch
  aufgrund der Aufgabenstellung eingefügt; sie beziehen sich ausschließlich auf die Zeilen
  unter oder links von ihnen.
  
  Umlaute und Zeichen, die sich im erweiterten ASCII befinden,
  werden eventuell nicht richtig ausgelesen.

  Die statusLed - eine LED, die die Lichtsignale während der Übertragung des Textes sichtbar
  macht - und ihre dazugehörigen Variablen und Funktion wird in der Vorplanung so nicht genannt
  und ist optional; sie wurde ins Leben gerufen wurde, nachdem eine visuelle Veranschaulichung
  des aktuellen Datenflusses unter Verwendung eines Glasfaserkabels nicht mehr möglich war.

*/



// Bibliotheken für das LCD-Display inkludieren
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---------- Deklarieren der Variablen -----------------------------------------------
int timing = 70; // in Millisekunden, kongruent zum Receiver (als Taktung dienend)

const int button_pin = 1;
const int statusLed_pin = 7;
const int ldr_pin = 0;
const int lightDetection_true = 700;
int ldr_value;
int lightDetection_state;
String binaryInput;
String cleaned_binaryInput;
String convertedText;

// Festlegen der Daten für das LCD-Display
LiquidCrystal_I2C lcd(0x27, 20, 4);


// ---------- Setup und Loop ----------------------------------------------------------

void setup() {
  // Initialisieren der Ein- und Ausgänge
  pinMode(ldr_pin, INPUT);
  pinMode(statusLed_pin, OUTPUT);
  pinMode(button_pin, INPUT_PULLUP);

  Serial.begin(9600); // [optional]
  // Beginnen der Ausgabe zum LCD-Display
  lcd.begin();
  lcd.backlight();
  lcd.clear();

}


// ein Durchgang der loop()-Funktion entspricht dem Empfangen und Löschen einer Nachricht
void loop() {
  // Zurücksetzen des erhaltenden, binärkodierten Inputs
  binaryInput = "";

  // Löschen des Bildschirminhaltes
  lcd.clear();

  // Warten auf das Startsignal des Transmitters
  lcd.setCursor(0, 0);
  lcd.print("[Waiting for signal]");
  while (lightDetection_state_updater(ldr_pin) == 1) {
    statusLed_updater(1);
  }

  statusLed_updater(0);
  // Eingliedern in die Taktung zwischen Transmitter und Receiver
  delay(1.5 * timing);

  // Nachricht erhalten, letzten Byte entfernen und in Text umwandeln
  binaryInput = receive_message();
  cleaned_binaryInput = clean_string(binaryInput);
  convertedText = binary_to_text(cleaned_binaryInput);

  // Text anzeigen
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(convertedText);

  Serial.println(binaryInput); // [optional]
  Serial.println(convertedText); // [optional]
  
  // Warten auf das Drücken des Buttons
  // --> Löschen des Bildschirminhaltes, da die loop()-Funktion von vorn beginnt
  while (analogRead(button_pin) < 1000) {
    delay(10);
  }

}



// :::::::::: Definitionen der eigenen Funktionen :::::::::::::::::::::::::::::::::::::

// Funktion für das Auslesen der potentiellen Anwesenheit von Licht durch den LDR
// ab der festgelegten Schwelle lightDetection_true
bool lightDetection_state_updater(int input_pin) {
  bool state;

  if (analogRead(input_pin) >= lightDetection_true) {
    state = 1;
  } else {
    state = 0;
  }

  return state;

}


String receive_message() {
  // while-Schleife gilt, während das Ende der Nachricht noch nicht erreicht wurde
  while (messageEnd_check() == false) {
    
    // für nach dem Eingliedern in den Übertragungstakt
    if (binaryInput != "") {
      delay(timing);

    }

    // 1 Byte bis Ende der while-Schleife
    /* 
      Das erste Bit eines jeden Bytes wird nicht ausgelesen; da alle Bytes des
      grundlegenden ASCII mit einer '0' beginnen, wird auch jeder vom Receiver empfangende
      Byte mit einer '0' startend dokumentiert. Dies dient einer einfachen Synchronisation
      zwischen Transmitter und Receiver.
      Die sieben letzten Bits eines Bytes werden mit der Funktion lightDetection_state_updater(ldr_pin)
      ausgelesen: Bei einfallendem Licht wird eine '1' an den String binaryInput angehängt,
      bei Dunkelheit eine '0'.
    */
    binaryInput += "0";
    statusLed_updater(0);

    for (int i = 0; i < 7; i++) {
      if (lightDetection_state_updater(ldr_pin) == 1) {
      binaryInput += 1;
      statusLed_updater(1);

      } else {
      binaryInput += 0;
      statusLed_updater(0);
      }
      delay(timing);
    }
    // Leerzeichen für bessere Lesbarkeit (nur relevant für die Ausgabe
    // über den Seriellen Monitor)
    binaryInput += " ";
  }
  statusLed_updater(1);
  return binaryInput;

}


// kontrolliert, ob das Ende der Nachricht erreicht wurde, indem nach den neun
// Zeichen "00001010 " (Line Feed) Ausschau gehalten wird
bool messageEnd_check() {
  bool isEnd;
  
  if (binaryInput.endsWith("00001010 ")) {
    isEnd = true;

  } else {
    isEnd = false;
  }
  return isEnd;

}


// wandelt den binärkodierten Text in eine entsprechende, lesbare Zeichenkette um
String binary_to_text(String binaryString) {
  String textResult = "";

  // Bytes aufteilen
  for (int i = 0; i < binaryString.length(); i += 9) {
    // Extrahieren eines Bytes vom binaryString
    String byteString = binaryString.substring(i, i + 8);

    // binär zu hexadezimal umwandeln
    int byteValue = strtol(byteString.c_str(), NULL, 2);

    // hexadezimal entsprechend dem ASCII in lesbaren Text übertragen
    textResult += char(byteValue);
  }
  return textResult;

}


// entfernt "00001010 " (Line Feed); andernfalls wird ein unerwünschtes Zeichen auf
// dem LCD-Display angezeigt
String clean_string(String string) {
  return string.substring(0, string.length() - 9);

}


// schaltet die statusLed bei einer übergebenen '1' ein und sonst aus
void statusLed_updater(int state) {
  if (state == 1) {
  digitalWrite(statusLed_pin, HIGH);

  } else {
  digitalWrite(statusLed_pin, LOW);
  }

}




