#ifndef _idtLoader_h_
#define _idtLoader_h_

void load_idt();

#pragma pack(push)
#pragma pack (1)

/**
 * Interrupt descriptor
 */
typedef struct {
  uint16_t offset_l;
  uint16_t segment_selector;
  uint8_t zero_1;
  uint8_t access;
  uint16_t offset_m;
  uint32_t offset_h;
  uint32_t zero_2;
} int_desc_t;

#pragma pack(pop)

#endif
