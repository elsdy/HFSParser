//
//  eMMCLevel.cpp
//  HFSParser
//
//  Created by Dongyun Shin on 3/31/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#include "eMMCLevel.h"

List <BlockInfo> *eMMCInfo::getMappedBlockInfo()
{
	return MappedBlockInfo;
}
void eMMCInfo::pushMappedBlockInfo(BlockInfo *_block)
{
	Node <BlockInfo> *new_node = new Node <BlockInfo> (_block);
	MappedBlockInfo->push_back(new_node);
}

long eMMCInfo::getStartAddress()
{
	return StartAddress;
}
void eMMCInfo::setStartAddress(long _sa)
{
	StartAddress = _sa;
}

long eMMCInfo::getCntBlocks()
{
	return CntBlocks;
}
void eMMCInfo::setCntBlocks(long _nb)
{
	CntBlocks = _nb;
}
long eMMCInfo::getCMD()
{
	return CMD;
}
void eMMCInfo::setCMD(int __cmd)
{
	CMD = __cmd;
}

long eMMCInfo::getBlockSz()
{
	return BlockSz;
}

void eMMCInfo::setBlockSz(long _value)
{
	BlockSz = _value;
}

#pragma mark - for debug
void eMMCInfo::print(ofstream &out)
{
	if(StartFinishTimeInfo::getStart() != NULL)
		StartFinishTimeInfo::getStart()->printParseEntry(out);
	if(StartFinishTimeInfo::getFinish() != NULL)
		StartFinishTimeInfo::getFinish()->printParseEntry(out);
}

