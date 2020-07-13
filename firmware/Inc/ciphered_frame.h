#ifndef __CIPHERED_FRAME_H
#define __CIPHERED_FRAME_H

#ifdef __cplusplus
extern "C" {
#endif

#define FRAME_DATA_SIZE (32-sizeof(uint32_t))
#define BLOCKS_PER_FRAME 2

struct ciphered_frame {
  uint32_t counter;
  uint8_t data[FRAME_DATA_SIZE];
};


#ifdef __cplusplus
}
#endif

#endif /* __CIPHERED_FRAME_H */
