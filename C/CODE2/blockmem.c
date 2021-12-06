/***********************************************************************************************************************************************
 * @file  <blockmem.c>
 * @brief < Implementing a centralized mechanism for working with block memory without using a file system. >
 *
 * 
 * @author <Dmitry Kolosyanko Dmitriy.kolosyanko@gmail.com>
 * @date   <20/05/2017>
 *
 ***********************************************************************************************************************************************/
#include "blockmem.h"

uint32_t blockmem_LastRegDev=-1;
uint32_t blockmem_EvlBlk=0;

uint8_t blockmem_Init(void)
{
	uint32_t i;
	blockmem_EvlBlk=0;
	//Init block Devices
	if((blockmem_BlockDevicesMax==0)||(blockmem_BlockBuffers==0))
	{
		return 1;
	}
	for(i=0;i<blockmem_BlockDevicesMax;i++)
	{
		BlockDeviceTbl[i].SizeBlocks=0;
	}
	for(i=0;i<blockmem_BlockBuffers;i++)
	{
		blockmem_BlockBufferTbl[i].BlockInUse=0x00;
		blockmem_BlockBufferTbl[i].BlockAddr=0xFFFFFFFF;
		blockmem_BlockBufferTbl[i].BlockState=(!blockmem_BufferInUse);
	}
	return 0;
}


uint8_t blockmem_RegistertBlockDevice(uint32_t StartBlock,uint32_t SizeBlocks,uint8_t (*ReadDeviceBlock) (uint32_t BlockAddr ,void* BufferAddr ))
{
	blockmem_LastRegDev++;
	if(blockmem_LastRegDev>blockmem_BlockDevicesMax)
	{
		return 1;
	}
	if(SizeBlocks==0)
	{
		return 2;
	}
	BlockDeviceTbl[blockmem_LastRegDev].StartBlock=StartBlock;
	BlockDeviceTbl[blockmem_LastRegDev].SizeBlocks=SizeBlocks;
	BlockDeviceTbl[blockmem_LastRegDev].ReadDeviceBlock=ReadDeviceBlock;
	return 0;
}

uint8_t blockmem_ReadMemBlock(uint32_t BlockAddr,uint8_t **bufferAddr)
{
	uint32_t i,j,DeviceBlockAddr=-1,CurBuffer=0,CurDevice=-1;
	uint32_t EmptyBlk;

	if(blockmem_LastRegDev==-1)
	{//Ñheck that at least one device is registered
		return 1;
	}
	//SearchBlockDevice
	for(i=0;i<(blockmem_LastRegDev+1);i++)
	{
		if((BlockAddr>=BlockDeviceTbl[i].StartBlock)&&(BlockAddr<(BlockDeviceTbl[i].StartBlock+BlockDeviceTbl[i].SizeBlocks)))
		{
			CurDevice=i;
		}
	}
	if(CurDevice==-1)
	{
		return 2;
	}
	//Check that block already in buffer
	for(i=0;(i<blockmem_BlockBuffers)&&(BlockAddr!=blockmem_BlockBufferTbl[i].BlockAddr);i++);
	if(i==blockmem_BlockBuffers)
	{
		DeviceBlockAddr=(BlockAddr-BlockDeviceTbl[CurDevice].StartBlock);

		//Search Free buffer
		if(blockmem_BlockBufferTbl[blockmem_EvlBlk].BlockInUse==0)
		{
			if(BlockDeviceTbl[CurDevice].ReadDeviceBlock(DeviceBlockAddr,blockmem_BlockBufferTbl[blockmem_EvlBlk].BlockData))
			{
				return 3;
			}
			blockmem_BlockBufferTbl[blockmem_EvlBlk].BlockInUse+=1;
			blockmem_BlockBufferTbl[blockmem_EvlBlk].BlockAddr=BlockAddr;
			*bufferAddr=blockmem_BlockBufferTbl[blockmem_EvlBlk].BlockData;
		}
		else
		{
			//j=blockmem_EvlBlk;

			for(i=0;
			(i<blockmem_BlockBuffers)&&((blockmem_BlockBufferTbl[blockmem_EvlBlk].BlockInUse)!=0)
				;i++, blockmem_EvlBlk=(blockmem_EvlBlk>=(blockmem_BlockBuffers-1))?0:(blockmem_EvlBlk+1));

			if((blockmem_BlockBufferTbl[blockmem_EvlBlk].BlockInUse)!=0)
			{
				return 4;
				//blockmem_EvlBlk=j;
			}


			if(BlockDeviceTbl[CurDevice].ReadDeviceBlock(DeviceBlockAddr,blockmem_BlockBufferTbl[blockmem_EvlBlk].BlockData))
			{
				return 5;
			}
			blockmem_BlockBufferTbl[blockmem_EvlBlk].BlockInUse+=1;
			blockmem_BlockBufferTbl[blockmem_EvlBlk].BlockAddr=BlockAddr;
			*bufferAddr=blockmem_BlockBufferTbl[blockmem_EvlBlk].BlockData;
			blockmem_EvlBlk=(blockmem_EvlBlk>=(blockmem_BlockBuffers-1))?0:(blockmem_EvlBlk+1);
		}
	}
	else
	{//if block already in buffer then return this buffer
		blockmem_BlockBufferTbl[i].BlockInUse+=1;
		*bufferAddr=blockmem_BlockBufferTbl[i].BlockData;
	}
	return 0;
}

//Free Allocated buffer
uint8_t blockmem_FreeBufer(uint32_t BlockAddr)
{
	uint32_t i,CurDevice=-1;

	if(blockmem_LastRegDev==-1)
	{//Ñheck that at least one device is registered
		return 1;
	}
	//SearchBlockDevice
	for(i=0;i<(blockmem_LastRegDev+1);i++)
	{
		if((BlockAddr>=BlockDeviceTbl[i].StartBlock)&&(BlockAddr<(BlockDeviceTbl[i].StartBlock+BlockDeviceTbl[i].SizeBlocks)))
		{
			CurDevice=i;
		}
	}
	if(CurDevice==-1)
	{
		return 2;
	}
	//Search buffer allocated for this block
	for(i=0;(i<blockmem_BlockBuffers)&&(BlockAddr!=blockmem_BlockBufferTbl[i].BlockAddr);i++);
	if(i==blockmem_BlockBuffers)
	{//This block not load in buffer
		return 3;
	}
	if(blockmem_BlockBufferTbl[i].BlockInUse==0)
	{
		return 4;
	}
	//Decrease block usage count
	blockmem_BlockBufferTbl[i].BlockInUse-=1;
	return 0;

}

//Check Is Block Load in buffer
uint8_t blockmem_IsInBuffer(uint32_t BlockAddr)
{
	uint32_t i,CurDevice=-1;

	if(blockmem_LastRegDev==-1)
	{
		return 1;
	}
	//SearchBlockDevice
	for(i=0;i<(blockmem_LastRegDev+1);i++)
	{
		if((BlockAddr>=BlockDeviceTbl[i].StartBlock)&&(BlockAddr<(BlockDeviceTbl[i].StartBlock+BlockDeviceTbl[i].SizeBlocks)))
		{
			CurDevice=i;
		}
	}
	if(CurDevice==-1)
	{
		return 2;
	}
	//Search buffer allocated for this block
	for(i=0;(i<blockmem_BlockBuffers)&&(BlockAddr!=blockmem_BlockBufferTbl[i].BlockAddr);i++);
	if(i==blockmem_BlockBuffers)
	{//This block not load in buffer
		return 3;
	}
	return 0;

}
