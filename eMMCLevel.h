//
//  eMMCLevel.h
//  HFSParser
//
//  Created by Dongyun Shin on 3/31/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#ifndef __HFSParser__eMMCLevel__
#define __HFSParser__eMMCLevel__

#include <iostream>

using namespace std;

#include "ParseEntry.h"
#include "Node.h"
#include "List.h"

#include "StartFinishTimeInfo.h"

class BlockInfo;

class eMMCInfo:public StartFinishTimeInfo
{
private:
	List <BlockInfo> *MappedBlockInfo;
	
	long StartAddress, BlockSz, CntBlocks;
	
	int CMD;
public:
	eMMCInfo(): StartFinishTimeInfo()
	{
		init();
	}
	
	/**
	 @exception
	 no need to free the memory for each pointer such as
	 ParseEntry *eMMC_start_request, *eMMC_request_done;
	 BlockInfo *MappedBlockInfo;
	 */
	eMMCInfo(Node <ParseEntry> * _a, Node <ParseEntry> * _b): StartFinishTimeInfo (_a, _b)
	{
		init();
		
		MappedBlockInfo = new List <BlockInfo>();
	}
	
	void init()
	{
		MappedBlockInfo = NULL;
		
		StartAddress = BlockSz = CntBlocks = 0;
		CMD = -1;
	}
	
	~eMMCInfo()
	{
	}

		
	List <BlockInfo> *getMappedBlockInfo();
	
	void pushMappedBlockInfo(BlockInfo *_block);
	
	long getStartAddress();
	
	void setStartAddress(long _sa);
	
	long getCntBlocks();
	
	void setCntBlocks(long _nb);
	
	long getCMD();
	
	void setCMD(int __cmd);
	
	long getBlockSz();

	void setBlockSz(long _value);
	
#pragma mark - for debug
	void print(ofstream &out);
};

#endif /* defined(__HFSParser__eMMCLevel__) */
