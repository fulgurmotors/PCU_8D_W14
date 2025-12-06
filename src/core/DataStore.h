#ifndef DATA_STORE_H
#define DATA_STORE_H

#include "ProtocolDefs.h"
#include <string.h>

class DataStore {
public:
  static DataStore &getInstance();

  // Setters
  void updateTelemetry(const TelemetryPacket &packet);

  // Getters
  const TelemetryPacket &getTelemetry() const;
  bool isNewDataAvailable() const;
  void clearNewDataFlag();

private:
  DataStore();
  DataStore(const DataStore &) = delete;
  DataStore &operator=(const DataStore &) = delete;

  TelemetryPacket telemetry;
  bool newDataAvailable;
};

#endif
