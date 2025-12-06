#ifndef PROTOCOL_DEFS_H
#define PROTOCOL_DEFS_H

#include <stdint.h>

// Protocol Constants
#define PACKET_HEADER_1 0xFA
#define PACKET_HEADER_2 0xFB

enum PacketType : uint8_t { PACKET_TYPE_TELEMETRY = 0x01 };

#pragma pack(push, 1)
struct TelemetryPacket {
  int8_t gear; // -1=R, 0=N, 1-8
  uint16_t rpm;
  uint16_t speed;

  float brakeBias;
  float fuelTarget;
  float lastLapTime;
  float delta;
  float lastLapFuel;
  float ersLevel;

  uint8_t flag;    // 0=None, 1=Yellow, 2=Red
  uint8_t ersMode; // 0-6
  uint8_t battery; // Percentage
  uint8_t drs;     // 0 or 1

  // Temperatures (using uint8 to save space if 0-255 is enough, assuming
  // degrees C) If higher precision or range needed, use uint16_t
  uint16_t flTyreTemp;
  uint16_t frTyreTemp;
  uint16_t rlTyreTemp;
  uint16_t rrTyreTemp;

  uint16_t flBrakeTemp;
  uint16_t frBrakeTemp;
  uint16_t rlBrakeTemp;
  uint16_t rrBrakeTemp;
};
#pragma pack(pop)

#endif
