#ifndef _BLOCKMEM_H_
#define _BLOCKMEM_H_

#include "base.h"

#define blockmem_BlockSize 1024
#define blockmem_BlockBuffers 5
#define blockmem_BlockDevicesMax 1
#define blockmem_FreeBuffer 0xFFFFFFFF

#define blockmem_BufferInUse 0x01

extern uint32_t blockmem_LastRegDev;

struct blockmem_blockbuffer{
	uint32_t BlockAddr;
	uint8_t  BlockInUse;
	uint8_t  BlockState;
	uint8_t  BlockData[blockmem_BlockSize];
}blockmem_BlockBufferTbl[blockmem_BlockBuffers];

struct blockmem_blockdevice{
	uint32_t StartBlock;
	uint32_t SizeBlocks;
	uint8_t (*ReadDeviceBlock) (uint32_t BlockAddr ,void* BufferAddr );
}BlockDeviceTbl[blockmem_BlockDevicesMax];

extern uint8_t blockmem_Init(void);
extern uint8_t blockmem_RegistertBlockDevice(uint32_t StartBlock,uint32_t SizeBlocks,uint8_t (*ReadDeviceBlock) (uint32_t BlockAddr ,void* BufferAddr ));
extern uint8_t blockmem_ReadMemBlock(uint32_t BlockAddr,uint8_t **bufferAddr);
extern uint8_t blockmem_ReadMemBlock1(uint32_t BlockAddr,uint8_t **bufferAddr);
extern uint8_t blockmem_FreeBufer(uint32_t BlockAddr);
extern uint8_t blockmem_IsInBuffer(uint32_t BlockAddr);

#endif//_BLOCKMEM_H_
