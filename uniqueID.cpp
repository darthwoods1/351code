#include "initialData.h"


int makeID(void) {

  uint32_t uniqueId[3];
  uniqueId[0] = *(uint32_t *)(0x1FFF7A10);
  uniqueId[1] = *(uint32_t *)(0x1FFF7A14);
  uniqueId[2] = *(uint32_t *)(0x1FFF7A18);

  // printf("Device ID: %08X%08X%08X\n", uniqueId[0], uniqueId[1], uniqueId[2]);
  return uniqueId[2];
}