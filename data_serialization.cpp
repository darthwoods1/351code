#include "initialData.h"
#include <cmath>
#include <cstring>
#include <vector>


std::vector<uint8_t> serializeData(const data &d) {

  std::vector<uint8_t> buffer;

  // Serialize flag
  buffer.push_back(static_cast<uint8_t>(d.flag));

  // Serialize ID
  uint8_t idBytes[sizeof(int)];
  std::memcpy(idBytes, &d.ID, sizeof(int));
  buffer.insert(buffer.end(), idBytes, idBytes + sizeof(int));

  // Serialize count_
  uint8_t countBytes[sizeof(int)];
  std::memcpy(countBytes, &d.count, sizeof(int));
  buffer.insert(buffer.end(), countBytes, countBytes + sizeof(int));

  // Serialize Volume
  uint32_t volumeBits;
  std::memcpy(&volumeBits, &d.Volume, sizeof(float));
  buffer.insert(buffer.end(), reinterpret_cast<uint8_t *>(&volumeBits),
                reinterpret_cast<uint8_t *>(&volumeBits) + sizeof(uint32_t));

  // Serialize time
  uint32_t timeBits = __builtin_bswap32(d.time);
  buffer.insert(buffer.end(), reinterpret_cast<uint8_t *>(&timeBits),
                reinterpret_cast<uint8_t *>(&timeBits) + sizeof(uint32_t));

  // Serialize lat
  uint64_t latBits;
  std::memcpy(&latBits, &d.lat, sizeof(double));
  latBits = __builtin_bswap64(latBits);
  buffer.insert(buffer.end(), reinterpret_cast<uint8_t *>(&latBits),
                reinterpret_cast<uint8_t *>(&latBits) + sizeof(uint64_t));

  // Serialize lon
  uint64_t lonBits;
  std::memcpy(&lonBits, &d.lon, sizeof(double));
  lonBits = __builtin_bswap64(lonBits);
  buffer.insert(buffer.end(), reinterpret_cast<uint8_t *>(&lonBits),
                reinterpret_cast<uint8_t *>(&lonBits) + sizeof(uint64_t));

  return buffer;
}