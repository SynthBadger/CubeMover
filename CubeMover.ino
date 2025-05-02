#include "ESPMax.h"
#include "_espmax.h"
#include "SuctionNozzle.h"
#include <ctype.h>

// === GLOBAL VARIABLES ===
float currentPos[3] = {0, -83, 150};  // Home position
float raisedPos[3] = {0, -83, 213};  //Raised position
const float step = 10.0;               // Movement step
bool pumpState = false;

// Grid for barcode scanning
float grid[9][1][3] = {
  
  { {50, -133, 130}  }, //J
  { {50, -173, 130}  }, //I
  { {50, -218, 130}  }, //H
  
  { {0, -213, 130} }, //A
  { {0, -183, 130} }, //B
  { {0, -133, 130} }, //C

  { {-50, -133, 130} }, //F
  { {-40, -173, 130} }, //E
  { {-50, -213, 130} }, //D


};

// Barcode mappings for different cube colors
String redBarcode    = "TSF::687::239651";
String blueBarcode = "Blue";
String greenBarcode  = "Green";
String targetBarcode = redBarcode;
String dropSection   = "";

void setup() {
  Serial.begin(9600);
  ESPMax_init();
  Nozzle_init();
  set_position(currentPos, 2000);
  Serial.println("Ready. Say commands like:");
  Serial.println("  home");
  Serial.println("  WASD/QE to move");
  Serial.println("  X Y Z coords to go to position");
  Serial.println("  P/V for pump/valve");
  Serial.println("  find <color> cube and drop off in [section|location] <A|B|C>");
}

void loop() {
  if (!Serial.available()) return;
  String input = Serial.readStringUntil('\n');
  input.trim();
  input.toLowerCase();  // case-insensitive parsing

  // === VOICE/NATURAL COMMAND: find <color> cube and optional section ===
  if (input.startsWith("find ") && input.indexOf(" cube") > 5) {
    // Extract color between "find " and " cube"
    int cubePos = input.indexOf(" cube");
    String color = input.substring(5, cubePos);
    color.trim();

    // Extract optional section letter anywhere in input
    String section = "";
    int secIdx = input.indexOf("section ");
    if (secIdx == -1) secIdx = input.indexOf("location ");
    if (secIdx != -1) {
      int start = secIdx + (input.startsWith("section ", secIdx) ? 8 : 9);
      section = input.substring(start);
      section.trim();
      if (section.length() > 1) section = String(section.charAt(0));
      section.toUpperCase();
    }

    // Map color to barcode
    if      (color == "red")    targetBarcode = redBarcode;
    else if (color == "blue") targetBarcode = blueBarcode;
    else if (color == "green")  targetBarcode = greenBarcode;
    else {
      Serial.print("Unknown color: "); Serial.println(color);
      return;
    }

    dropSection = section;  // may be empty
    Serial.print("Finding "); Serial.print(color); Serial.print(" cube");
    if (dropSection.length()) {
      Serial.print(" and dropping off in section "); Serial.print(dropSection);
    }
    Serial.println("...");
    searchAndPick();
    return;
  }

  // === HOME COMMAND ===
  if (input.equalsIgnoreCase("home")) {
    currentPos[0] = 0; currentPos[1] = -83; currentPos[2] = 150;
    set_position(currentPos, 2000);
    Serial.println("Returning to home position: 0, -83, 103");
    delay(3000);
    return;
  }

  // === MOVEMENT KEYS ===
  if (input.length() == 1) {
    char key = input.charAt(0);
    switch (key) {
      case 'w': currentPos[1] -= step; break;
      case 's': currentPos[1] += step; break;
      case 'a': currentPos[0] -= step; break;
      case 'd': currentPos[0] += step; break;
      case 'q': currentPos[2] += step; break;
      case 'e': currentPos[2] -= step; break;
      case 'P':
        pumpState = !pumpState;
        if (pumpState) { Pump_on(); Serial.println("Pump ON"); }
        else           { Pump_off(); Serial.println("Pump OFF"); }
        return;
      case 'V':
        Valve_on(); delay(500); Valve_off();
        Serial.println("Valve pulsed");
        return;
      default:
        Serial.println("Unknown key.");
        return;
    }
    set_position(currentPos, 500);
    Serial.print("Moved to: ");
    Serial.print(currentPos[0]); Serial.print(", ");
    Serial.print(currentPos[1]); Serial.print(", ");
    Serial.println(currentPos[2]);
    return;
  }

  // === XYZ COORDINATE INPUT ===
  char c = input.charAt(0);
  if (isDigit(c) || c == '-' || c == '+') {
    int space1 = input.indexOf(' ');
    int space2 = input.lastIndexOf(' ');
    if (space1 > 0 && space2 > space1) {
      currentPos[0] = input.substring(0, space1).toFloat();
      currentPos[1] = input.substring(space1 + 1, space2).toFloat();
      currentPos[2] = input.substring(space2 + 1).toFloat();
      set_position(currentPos, 1000);
      Serial.print("Moved to: ");
      Serial.print(currentPos[0]); Serial.print(", ");
      Serial.print(currentPos[1]); Serial.print(", ");
      Serial.println(currentPos[2]);
      delay(3000);
    } else {
      Serial.println("Invalid coordinates. Use: X Y Z");
    }
    return;
  }

  // === UNKNOWN COMMAND ===
  Serial.println("Unknown command.");
  
}

// === SEARCH FUNCTION ===
void searchAndPick() {
  for (int row = 0; row < 9; row++) {
    for (int col = 0; col < 1; col++) {
      float pos[3] = { grid[row][col][0], grid[row][col][1], grid[row][col][2] };
      Serial.print("Moving to scan cell [");
      Serial.print(row); Serial.print(","); Serial.print(col);
      Serial.print("]: ");
      Serial.print(pos[0]); Serial.print(", ");
      Serial.print(pos[1]); Serial.print(", ");
      Serial.println(pos[2]);
      set_position(pos, 800); delay(300);
      String scanned = readBarcode();
      if (scanned.length()) { Serial.print("Scanned: "); Serial.println(scanned); }
      if (scanned == targetBarcode) {
        Serial.println("Match found! Picking up...");
        float shiftedPos[3] = { pos[0], pos[1] - 20, pos[2] };
        set_position(shiftedPos, 500);
        pick_up_cube(shiftedPos);
        return;
      }
    }
  }
  Serial.println("Barcode not found.");
  float home[3] = {0, -83, 150};
  set_position(home, 1000);
  
  Serial.println("Returning to home position.");;
}

// === BARCODE READER ===
String readBarcode() {
  String barcode;
  unsigned long start = millis();
  while (millis() - start < 2000) {
    if (Serial.available()) {
      char c = Serial.read(); barcode += c; if (c == '\n') break;
    }
  }
  barcode.trim(); return barcode;
}

// === PICKUP FUNCTION ===
void pick_up_cube(float* pos) {
  delay(1000);
  float dropHeight = 50;
  float pickupPos[3] = { pos[0], pos[1], pos[2] - dropHeight };
  set_position(pickupPos, 1000); delay(300); Pump_on(); delay(2000);

  set_position(pos, 400); delay(500);
  float raised[3] = {0, -83, 213};
  set_position(raised, 2000); delay(1000);

  float dropOff[3];
  if (dropSection.equalsIgnoreCase("A")) {
    dropOff[0] = 0; dropOff[1] = -243; dropOff[2] = 200;
  } else if (dropSection.equalsIgnoreCase("B")) {
    dropOff[0] = 0; dropOff[1] = -203; dropOff[2] = 200;
  } else if (dropSection.equalsIgnoreCase("C")) {
    dropOff[0] = 0; dropOff[1] = -153; dropOff[2] = 200;
  } else if (dropSection.equalsIgnoreCase("D")) {
    dropOff[0] = -50; dropOff[1] = -233; dropOff[2] = 200;
  } else if (dropSection.equalsIgnoreCase("E")) {
    dropOff[0] = -50; dropOff[1] = -193; dropOff[2] = 200;
  } else if (dropSection.equalsIgnoreCase("F")) {
    dropOff[0] = -50; dropOff[1] = -153; dropOff[2] = 200;
  } else if (dropSection.equalsIgnoreCase("H")) {
    dropOff[0] = 50; dropOff[1] = -233; dropOff[2] = 200;
  } else if (dropSection.equalsIgnoreCase("I")) {
    dropOff[0] = 50; dropOff[1] = -193; dropOff[2] = 200;
  } else if (dropSection.equalsIgnoreCase("J")) {
    dropOff[0] = 50; dropOff[1] = -153; dropOff[2] = 200;
  } else {
    dropOff[0] = currentPos[0]; dropOff[1] = currentPos[1]; dropOff[2] = currentPos[2];
  }

  set_position(dropOff, 1000); delay(1500);
  dropOff[2] = 83;
  set_position(dropOff, 1000); delay(1500);
  Valve_on(); delay(1000); Valve_off(); delay(500); Pump_off(); delay(1000);
  float home[3] = {0, -83, 150};
  set_position(home, 1500); delay(1000);
  Serial.println("Cube released. Returned to home.");
}



// === PUMP OFF ===
void Pump_off() {
  digitalWrite(21, LOW); digitalWrite(19, LOW);
}
