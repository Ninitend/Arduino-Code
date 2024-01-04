// DE

/* > Information (Teil I) <
  Dieser Code stellt den Teil I des Arduino-Programmier-Projekts, das im Rahmen des
  Informatikunterrichts der elften Klasse im ersten Semester veranlasst wurde, dar.

  Teil I macht hierbei den Transmitter des mit Licht funktionierenden Texttransmitters aus.

  Nicht keine Kommentare sind für das Verständnis dieses Codes zu entbehren, wurden jedoch
  aufgrund der Aufgabenstellung eingefügt; sie beziehen sich ausschließlich auf die Zeilen
  unter oder links von ihnen.
  
  Umlaute und Zeichen, die sich im erweiterten ASCII befinden,
  werden nicht unterstützt.

*/



// ---------- Deklarieren der Variablen -----------------------------------------------
int timing = 70;  // in Millisekunden, kongruent zum Receiver (als Taktung dienend)


const int mainLed_pin = 17; // Variable für die LED zum Senden
String inputText; // String für die Texteingabe des Benutzers
String binaryText; // String für die binärkodierte Version der Benutzereingabe


// ---------- Setup und Loop ----------------------------------------------------------

void setup() {
  Serial.begin(9600); // [optional]
  
  // Initialisieren der mainLed
  pinMode(mainLed_pin, OUTPUT);

}


void loop() {
  // Warten auf Benutzereingabe
  while (Serial.available() == 0) {
    digitalWrite(mainLed_pin, HIGH);
  }

  // Speichern der Benutzereingabe im String "inputText"
  inputText = Serial.readString();
  // Ausschalten der mainLed --> Startsignal für den Receiver
  digitalWrite(mainLed_pin, LOW);

  binaryText = text_to_binary(inputText);
  
  // Übergabe jenes Binärcodes an diese Funktion zum Ausgeben per LED
  blinkBinary(binaryText);

  // optionales Ausgeben der Benutzereingabe in Binärform (endend mit "00001010 " (Line Feed))
  Serial.println(binaryText);

}



// :::::::::: Definitionen der eigenen Funktionen :::::::::::::::::::::::::::::::::::::

// Umwandeln der Benutzereingabe in Binärcode
String text_to_binary(String text) {
  String binaryResult = "";

  // for-Schleife iteriert entsprechend der inputText- bzw. text-Länge
  for (int i = 0; i < text.length(); i++) {
    // Umwandeln jedes Zeichens in Hexadezimalform (nach dem ASCII)
    int hexValue = text.charAt(i);

    // Anhängen der binärkodierten Form an das Ergebnis der Funktion text_to_binary(String text)
    // mit dem Umwandeln durch die Funktion hex_to_binary(hexValue)
    binaryResult += hex_to_binary(hexValue) + " ";
  }
  
  return binaryResult;

}


// quasi eine Unterfunktion für text_to_binary(String text)
String hex_to_binary(int hexValue) {
  String binary = "";

  // Laufvariable wird pro Durchgang um 1 dekrementiert
  for (int i = 7; i >= 0; i--) {
    // ternärer Operator "?" lässt bei true von "(hexValue & (1 << i))"
    // eine '1' und sonst eine '0' an den String binary anhängen
    binary += (hexValue & (1 << i)) ? '1' : '0';
  }

  return binary;

}


void blinkBinary(String binaryString) {
  // for-Schleife wiederholt sich für jedes Zeichen in binaryString
  for (int i = 0; i < binaryString.length(); i++) {
    // aktuelles Zeichen im char-Datentyp
    char bit = binaryString.charAt(i);

    // für eine '1' in binaryString wird die 
    if (bit == '1') {
      digitalWrite(mainLed_pin, HIGH);
      delay(timing);
    
    } else if (bit == '0') {
      digitalWrite(mainLed_pin, LOW);
      delay(timing);
    
    } else if (bit == ' ') { // [optional und nicht notwendig]
      // [Leerzeichen zwischen Bytes]

    }
  }
  
  // vorzeitiges Einschalten der mainLed
  digitalWrite(mainLed_pin, HIGH); // [optional]

}




