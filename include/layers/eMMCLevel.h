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
	#pragma mark - constructor & destructor
public:
	eMMCInfo();
	/**
	 @exception
	 no need to free the memory for each pointer such as
	 ParseEntry *eMMC_start_request, *eMMC_request_done;
	 BlockInfo *MappedBlockInfo;
	 */
	eMMCInfo(Node <ParseEntry> * _a, Node <ParseEntry> * _b);
	void init();
	~eMMCInfo();


private:
	List <BlockInfo> *MappedBlockInfo;

	long StartAddress, BlockSz, CntBlocks;
	int CMD;


	#pragma mark - get & set
public:
	List <BlockInfo> *getMappedBlockInfo();
	void pushMappedBlockInfo(BlockInfo *_block);
	
	long getStartAddress();
	void setStartAddress(long _sa);

	long getCntBlocks();
	void setCntBlocks(long _nb);
	long getBlockSz();
	void setBlockSz(long _value);
	long getCMD();
	void setCMD(int __cmd);


#pragma mark - for debug
	void print(ofstream &out);
};

#endif /* defined(__HFSParser__eMMCLevel__) */
