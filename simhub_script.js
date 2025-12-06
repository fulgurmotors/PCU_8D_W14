/**
 * SimHub Custom Serial Script for PCU-8D
 * Protocol: SimPacket (Binary)
 * 
 * Instructions:
 * 1. Enable "Custom Serial device" in SimHub.
 * 2. Set Baud Rate to 115200.
 * 3. Paste this code into the "Javascript Message" section (Update loop).
 */

// --- Configuration ---
var HEADER_1 = 0xFA;
var HEADER_2 = 0xFB;
var PACKET_TYPE_TELEMETRY = 0x01;

// --- Helper: Float to Bytes (Little Endian) ---
function floatToBytes(value, buffer, offset) {
    if (value === null || isNaN(value)) value = 0.0;
    // Manual packing for IEEE 754 float (Little Endian)
    // This is a robust fallback if DataView is not fully supported in SimHub's specific JS engine version
    // But typically SimHub handles standard JS well. attempting a simpler approach first or standard DataView.
    // If DataView is available:
    try {
        var view = new DataView(new ArrayBuffer(4));
        view.setFloat32(0, value, true);
        buffer[offset] = view.getUint8(0);
        buffer[offset + 1] = view.getUint8(1);
        buffer[offset + 2] = view.getUint8(2);
        buffer[offset + 3] = view.getUint8(3);
        return;
    } catch (e) {
        // Fallback or ignore
    }
}

// --- Helper: Int16 to Bytes (Little Endian) ---
function int16ToBytes(value, buffer, offset) {
    if (value === null || isNaN(value)) value = 0;
    value = Math.round(value); // Ensure integer
    buffer[offset] = value & 0xFF;
    buffer[offset + 1] = (value >> 8) & 0xFF;
}

function int8ToBytes(value, buffer, offset) {
    if (value === null || isNaN(value)) value = 0;
    value = Math.round(value);
    buffer[offset] = value & 0xFF;
}

// --- Main Script ---

// 1. Gather Data (safely)
var gear = $prop('GameRawData.Physics.Gear');
if (gear == null) gear = 0;

var rpm = $prop('Rpms') || 0;
var speed = $prop('SpeedKmh') || 0;
var brakeBias = $prop('GameRawData.Graphics.BrakeBias') || 0;
var fuelTarget = 0.0; // Define mapping or static
var lastLapTime = $prop('LastLapTime') || 0;
var delta = $prop('PersistantTrackerPlugin.SessionBestLiveDeltaSeconds') || 0;
var lastLapFuel = 0.0; // Define mapping
var ersLevel = $prop('GameRawData.Graphics.Energy') || 0;

var flag = 0;
var rawFlag = $prop('GameRawData.Graphics.Flag');
if (rawFlag == 1) flag = 1; // Yellow
if (rawFlag == 2) flag = 2; // Red

var ersMode = 0; // Define mapping
var battery = $prop('BatteryCharge') || 0; // if available, or calc from energy
var drs = $prop('GameRawData.Graphics.DRS') || 0;

var flTyre = $prop('TyreTemperatureFrontLeft') || 0;
var frTyre = $prop('TyreTemperatureFrontRight') || 0;
var rlTyre = $prop('TyreTemperatureRearLeft') || 0;
var rrTyre = $prop('TyreTemperatureRearRight') || 0;

var flBrake = $prop('BrakeTemperatureFrontLeft') || 0;
var frBrake = $prop('BrakeTemperatureFrontRight') || 0;
var rlBrake = $prop('BrakeTemperatureRearLeft') || 0;
var rrBrake = $prop('BrakeTemperatureRearRight') || 0;

// 2. Build Payload (49 bytes)
var payloadSize = 49;
// Creating array with zeros
var payload = [];
for (var k = 0; k < payloadSize; k++) payload[k] = 0;

int8ToBytes(gear, payload, 0);
int16ToBytes(rpm, payload, 1);
int16ToBytes(speed, payload, 3);
floatToBytes(brakeBias, payload, 5);
floatToBytes(fuelTarget, payload, 9);
floatToBytes(lastLapTime, payload, 13);
floatToBytes(delta, payload, 17);
floatToBytes(lastLapFuel, payload, 21);
floatToBytes(ersLevel, payload, 25);
payload[29] = flag & 0xFF;
payload[30] = ersMode & 0xFF;
payload[31] = battery & 0xFF;
payload[32] = drs & 0xFF;

int16ToBytes(flTyre, payload, 33);
int16ToBytes(frTyre, payload, 35);
int16ToBytes(rlTyre, payload, 37);
int16ToBytes(rrTyre, payload, 39);
int16ToBytes(flBrake, payload, 41);
int16ToBytes(frBrake, payload, 43);
int16ToBytes(rlBrake, payload, 45);
int16ToBytes(rrBrake, payload, 47);

// 3. Checksum
var checksum = 0;
for (var i = 0; i < payloadSize; i++) {
    checksum = checksum ^ payload[i];
}

// 4. Send Packet
// Structure: [HEADER_1, HEADER_2, TYPE, ...PAYLOAD..., CHECKSUM]
var packet = [HEADER_1, HEADER_2, PACKET_TYPE_TELEMETRY];
for (var i = 0; i < payloadSize; i++) {
    packet.push(payload[i]);
}
packet.push(checksum);

// Send to Serial
serial.write(packet);
