#ifndef SERIAL_PROTOCOL_H
#define SERIAL_PROTOCOL_H

#include "DataStore.h"
#include "ProtocolDefs.h"
#include <Arduino.h>

class SerialProtocol {
public:
  SerialProtocol();
  void update();

private:
  enum State {
    WAIT_HEADER_1,
    WAIT_HEADER_2,
    WAIT_TYPE,
    WAIT_PAYLOAD,
    WAIT_CHECKSUM
  };

  State currentState;
  uint8_t currentType;
  uint8_t payloadBuffer[sizeof(TelemetryPacket)];
  uint16_t bytesReceived;

  void processPacket();
  uint8_t calculateChecksum(const uint8_t *data, size_t length);
};

#endif
