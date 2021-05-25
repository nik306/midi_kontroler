const byte numChars = 4;
int receivedChars[numChars];   // an array to store the received data

boolean newData = false;

void setup() {
    Serial.begin(38400);
}

void loop() {
    recv();
    showNewData();
}

void recv() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (rc != endMarker) {
            receivedChars[ndx] = (int)rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            ndx = 0;
            newData = true;
        }
    }
}

void showNewData() {
    if (newData == true) {
        for(int i=0; i<4; i++)
          Serial.println(receivedChars[i]);
        newData = false;
        Serial.println("-----------------");
    }
}
