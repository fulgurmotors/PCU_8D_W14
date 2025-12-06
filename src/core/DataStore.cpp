#include "DataStore.h"

DataStore::DataStore() : newDataAvailable(false) {
  memset(&telemetry, 0, sizeof(TelemetryPacket));
}

DataStore &DataStore::getInstance() {
  static DataStore instance;
  return instance;
}

void DataStore::updateTelemetry(const TelemetryPacket &packet) {
  memcpy(&telemetry, &packet, sizeof(TelemetryPacket));
  newDataAvailable = true;
}

const TelemetryPacket &DataStore::getTelemetry() const { return telemetry; }

bool DataStore::isNewDataAvailable() const { return newDataAvailable; }

void DataStore::clearNewDataFlag() { newDataAvailable = false; }
