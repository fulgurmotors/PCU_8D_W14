#include "SerialProtocol.h"

SerialProtocol::SerialProtocol()
    : currentState(WAIT_HEADER_1), bytesReceived(0) {}

void SerialProtocol::update() {
  while (Serial.available() > 0) {
    uint8_t byte = Serial.read();

    switch (currentState) {
    case WAIT_HEADER_1:
      if (byte == PACKET_HEADER_1) {
        currentState = WAIT_HEADER_2;
      }
      break;

    case WAIT_HEADER_2:
      if (byte == PACKET_HEADER_2) {
        currentState = WAIT_TYPE;
      } else {
        currentState = WAIT_HEADER_1; // Reset
      }
      break;

    case WAIT_TYPE:
      currentType = byte;
      if (currentType == PACKET_TYPE_TELEMETRY) {
        currentState = WAIT_PAYLOAD;
        bytesReceived = 0;
      } else {
        currentState = WAIT_HEADER_1; // Unknown type
      }
      break;

    case WAIT_PAYLOAD:
      payloadBuffer[bytesReceived++] = byte;
      if (bytesReceived >= sizeof(TelemetryPacket)) {
        currentState = WAIT_CHECKSUM;
      }
      break;

    case WAIT_CHECKSUM: {
      uint8_t expectedChecksum =
          calculateChecksum(payloadBuffer, sizeof(TelemetryPacket));
      if (byte == expectedChecksum) {
        processPacket();
      }
      currentState = WAIT_HEADER_1;
    } break;
    }
  }
}

void SerialProtocol::processPacket() {
  if (currentType == PACKET_TYPE_TELEMETRY) {
    TelemetryPacket *packet = (TelemetryPacket *)payloadBuffer;
    DataStore::getInstance().updateTelemetry(*packet);
  }
}

uint8_t SerialProtocol::calculateChecksum(const uint8_t *data, size_t length) {
  uint8_t checksum = 0;
  for (size_t i = 0; i < length; i++) {
    checksum ^= data[i];
  }
  return checksum;
}
