//
//  BlockLevel.cpp
//  HFSParser
//
//  Created by Dongyun Shin on 3/31/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#include "BlockLevel.h"

unsigned long long BlockInfo::getStartAddr_BioRemap()
{
	return StartAddr_BioRemap;
}

void BlockInfo::setStartAddr_BioRemap(unsigned long long _StartAddr_BioRemap)
{
	StartAddr_BioRemap = _StartAddr_BioRemap;
}

unsigned long long BlockInfo::getSize_BioRemap()
{
	return Size_BioRemap;
}
void BlockInfo::setSize_BioRemap(unsigned long long _Size_BioRemap)
{
	Size_BioRemap = _Size_BioRemap;
}
ParseEntry * BlockInfo::getBlock_bio_remap()
{
	return Block_bio_remap;
}
void BlockInfo::setBlock_bio_remap(ParseEntry * _Block_bio_remap)
{
	Block_bio_remap = _Block_bio_remap;
}

unsigned long long BlockInfo::getStartAddr_BioQueue()
{
	return StartAddr_BioQueue;
}
void BlockInfo::setStartAddr_BioQueue(unsigned long long _StartAddr_BioQueue)
{
	StartAddr_BioQueue = _StartAddr_BioQueue;
}
unsigned long long BlockInfo::getSize_BioQueue()
{
	return Size_BioQueue;
}
void BlockInfo::setSize_BioQueue(unsigned long long _Size_BioQueue)
{
	Size_BioQueue = _Size_BioQueue;
}
ParseEntry * BlockInfo::getBlock_bio_queue()
{
	return Block_bio_queue;
}
void BlockInfo::setBlock_bio_queue(ParseEntry * _Block_bio_queue)
{
	Block_bio_queue = _Block_bio_queue;
}

unsigned long long BlockInfo::getStartAddr_GetRq()
{
	return StartAddr_GetRq;
}
void BlockInfo::setStartAddr_GetRq(unsigned long long _StartAddr_GetRq)
{
	StartAddr_GetRq = _StartAddr_GetRq;
}
unsigned long long BlockInfo::getSIze_GetRq()
{
	return SIze_GetRq;
}
void BlockInfo::setSIze_GetRq(unsigned long long _SIze_GetRq)
{
	SIze_GetRq = _SIze_GetRq;
}
ParseEntry * BlockInfo::getBlock_getrq()
{
	return Block_getrq;
}
void BlockInfo::setBlock_getrq(ParseEntry * _Block_getrq)
{
	Block_getrq = _Block_getrq;
}

unsigned long long BlockInfo::getStartAddr_BioBackMerge()
{
	return StartAddr_BioBackMerge;
}
void BlockInfo::setStartAddr_BioBackMerge(unsigned long long _StartAddr_BioBackMerge)
{
	StartAddr_BioBackMerge = _StartAddr_BioBackMerge;
}
unsigned long long BlockInfo::getSize_BioBackMerge()
{
	return Size_BioBackMerge;
}
void BlockInfo::setSize_BioBackMerge(unsigned long long _Size_BioBackMerge)
{
	Size_BioBackMerge = _Size_BioBackMerge;
}
ParseEntry *BlockInfo::getBlock_bio_backmerge()
{
	return Block_bio_backmerge;
}
void BlockInfo::setBlock_bio_backmerge(ParseEntry *_Block_bio_backmerge)
{
	Block_bio_backmerge = _Block_bio_backmerge;
}

unsigned long long BlockInfo::getStartAddr_BioFrontMerge()
{
	return StartAddr_BioFrontMerge;
}
void BlockInfo::setStartAddr_BioFrontMerge(unsigned long long _StartAddr_BioFrontMerge)
{
	StartAddr_BioFrontMerge = _StartAddr_BioFrontMerge;
}
unsigned long long BlockInfo::getSize_BioFrontMerge()
{
	return Size_BioFrontMerge;
}
void BlockInfo::setSize_BioFrontMerge(unsigned long long _Size_BioFrontMerge)
{
	Size_BioFrontMerge = _Size_BioFrontMerge;
}
ParseEntry *BlockInfo::getBlock_bio_frontmerge()
{
	return Block_bio_frontmerge;
}
void BlockInfo::setBlock_bio_frontmerge(ParseEntry *_Block_bio_frontmerge)
{
	Block_bio_frontmerge = _Block_bio_frontmerge;
}

unsigned long long BlockInfo::getStartAddr_RqIssue()
{
	return StartAddr_RqIssue;
}
void BlockInfo::setStartAddr_RqIssue(unsigned long long _StartAddr_RqIssue)
{
	StartAddr_RqIssue = _StartAddr_RqIssue;
}
unsigned long long BlockInfo::getSize_RqIssue()
{
	return Size_RqIssue;
}
void BlockInfo::setSize_RqIssue(unsigned long long _Size_RqIssue)
{
	Size_RqIssue = _Size_RqIssue;
}
ParseEntry *BlockInfo::getBlock_rq_issue()
{
	return Block_rq_issue;
}
void BlockInfo::setBlock_rq_issue(ParseEntry *_Block_rq_issue)
{
	Block_rq_issue = _Block_rq_issue;
}

unsigned long long BlockInfo::getStartAddr_RqComplete()
{
	return StartAddr_RqComplete;
}
void BlockInfo::setStartAddr_RqComplete(unsigned long long _StartAddr_RqComplete)
{
	StartAddr_RqComplete = _StartAddr_RqComplete;
}
unsigned long long BlockInfo::getSize_RqComplete()
{
	return Size_RqComplete;
}
void BlockInfo::setSize_RqComplete(unsigned long long _Size_RqComplete)
{
	Size_RqComplete = _Size_RqComplete;
}
ParseEntry *BlockInfo::getBlock_rq_complete()
{
	return Block_rq_complete;
}
void BlockInfo::setBlock_rq_complete(ParseEntry *_Block_rq_complete)
{
	Block_rq_complete = _Block_rq_complete;
}

string BlockInfo::getR_W()
{
	return R_W;
}
void BlockInfo::setR_W(string _R_W)
{
	R_W = _R_W;
}

double BlockInfo::getTimeFromIssueToComplete()
{
	return TimeFromIssueToComplete;
}
void BlockInfo::setTimeFromIssueToComplete(double _TimeFromIssueToComplete)
{
	TimeFromIssueToComplete = _TimeFromIssueToComplete;
}
double BlockInfo::getTimeFromRemapToIssue()
{
	return TimeFromRemapToIssue;
}
void BlockInfo::setTimeFromRemapToIssue( double _TimeFromRemapToIssue)
{
	TimeFromRemapToIssue = _TimeFromRemapToIssue;
}

double BlockInfo::getPureTimeFromIssueToComplete()
{
	return PureTimeFromIssueToComplete;
}
void BlockInfo::setPureTimeFromIssueToComplete(double _PureTimeFromIssueToComplete)
{
	PureTimeFromIssueToComplete = _PureTimeFromIssueToComplete;
}
double BlockInfo::getPureTimeFromRemapToIssue()
{
	return PureTimeFromRemapToIssue;
}
void BlockInfo::setPureTimeFromRemapToIssue( double _PureTimeFromRemapToIssue)
{
	PureTimeFromRemapToIssue = _PureTimeFromRemapToIssue;
}

ParseEntry *BlockInfo::getBlock_plug()
{
	return Block_plug;
}
void BlockInfo::setBlock_plug(ParseEntry *_Block_plug)
{
	Block_plug = _Block_plug;
}
ParseEntry *BlockInfo::getBlock_unplug()
{
	return Block_unplug;
}
void BlockInfo::setBlock_unplug(ParseEntry *_Block_unplug)
{
	Block_unplug = _Block_unplug;
}

Node <IOInfo> *BlockInfo::getMappedIO()
{
	return MappedIO;
}
void BlockInfo::setMappedIO( Node <IOInfo> *_MappedIO)
{
	MappedIO = _MappedIO;
}

eMMCInfo *BlockInfo::getMappedEmmc()
{
	return MappedEmmc;
}
void BlockInfo::setMappedEmmc( eMMCInfo *_MappedEmmc)
{
	MappedEmmc = _MappedEmmc;
}

unsigned long long BlockInfo::getStartAddr_RqInsert()
{
	return StartAddr_RqInsert;
}

void BlockInfo::setStartAddr_RqInsert(unsigned long long _StartAddr_RqInsert)
{
	StartAddr_RqInsert = _StartAddr_RqInsert;
}

unsigned long long BlockInfo::getSize_RqInsert()
{
	return Size_RqInsert;
}

void BlockInfo::setSize_RqInsert(unsigned long long _Size_RqInsert)
{
	Size_RqInsert = _Size_RqInsert;
}

ParseEntry *BlockInfo::getBlock_rq_insert()
{
	return Block_rq_insert;
}
void BlockInfo::setBlock_rq_insert(ParseEntry *_Block_rq_insert)
{
	Block_rq_insert = _Block_rq_insert;
}

int BlockInfo::setBlockEventParseEntry(ParseEntry *remap, ParseEntry *queue, ParseEntry *getrq, ParseEntry *backmerge, ParseEntry *frontmerge, ParseEntry *issue, ParseEntry *complete)
{
	int ret = 0;
	
	if(remap != NULL)
	{
		Block_bio_remap = remap;
		remap->getisCollectedInWhichList().setValue(BLOCK_IO);
		ret++;
	}
	if(queue != NULL)
	{
		Block_bio_queue = queue;
		queue->getisCollectedInWhichList().setValue(BLOCK_IO);
		ret++;
	}
	if(getrq != NULL)
	{
		Block_getrq = getrq;
		getrq->getisCollectedInWhichList().setValue(BLOCK_IO);
		ret++;
	}
	if(backmerge != NULL)
	{
		Block_bio_backmerge = backmerge;
		backmerge->getisCollectedInWhichList().setValue(BLOCK_IO);
		ret++;
	}
	if(frontmerge != NULL)
	{
		Block_bio_frontmerge = frontmerge;
		frontmerge->getisCollectedInWhichList().setValue(BLOCK_IO);
		ret++;
	}
	if(issue != NULL)
	{
		Block_rq_issue = issue;
		issue->getisCollectedInWhichList().setValue(BLOCK_IO);
		ret++;
	}
	if(complete != NULL)
	{
		Block_rq_complete = complete;
		complete->getisCollectedInWhichList().setValue(BLOCK_IO);
		ret++;
	}
	return ret;
}


void BlockInfo::print(ofstream &out)
{
	if(Block_bio_remap)
		Block_bio_remap->printParseEntry(out);
	if(Block_bio_queue)
		Block_bio_queue->printParseEntry(out);
	if(Block_getrq)
		Block_getrq->printParseEntry(out);
	if(Block_bio_backmerge)
		Block_bio_backmerge->printParseEntry(out);
	if(Block_bio_frontmerge)
		Block_bio_frontmerge->printParseEntry(out);
	if(Block_rq_issue)
		Block_rq_issue->printParseEntry(out);
	if(Block_rq_complete)
		Block_rq_complete->printParseEntry(out);
	if(MappedEmmc)
	{
		out << "==== mapped Emmc Event ====" << endl;
		MappedEmmc->print(out);
	}
	else
	{
		out << "==== no mapped Emmc Event ====" << endl;
	}
}

void BlockInfo::print()
{
	if(Block_bio_remap)
		Block_bio_remap->printParseEntry();
	if(Block_bio_queue)
		Block_bio_queue->printParseEntry();
	if(Block_getrq)
		Block_getrq->printParseEntry();
	if(Block_bio_backmerge)
		Block_bio_backmerge->printParseEntry();
	if(Block_bio_frontmerge)
		Block_bio_frontmerge->printParseEntry();
	if(Block_rq_issue)
		Block_rq_issue->printParseEntry();
	if(Block_rq_complete)
		Block_rq_complete->printParseEntry();
	if(MappedEmmc)
	{
		cout << "==== mapped Emmc Event ====" << endl;
		MappedEmmc->StartFinishTimeInfo::print();
	}
	else
	{
		cout << "==== no mapped Emmc Event ====" << endl;
	}
}
