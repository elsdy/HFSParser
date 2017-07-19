//
//  BlockLevel.h
//  HFSParser
//
//  Created by Dongyun Shin on 3/31/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#ifndef __HFSParser__BlockLevel__
#define __HFSParser__BlockLevel__

#include <iostream>
#include <string>

#include "ParseEntry.h"
#include "List.h"
#include "Node.h"
#include "IOInfo.h"
#include "eMMCLevel.h"

class IOInfo;

class Parser;

class eMMCInfo;

class BlockInfo
{
#pragma mark - constructor & destructor
public:
	BlockInfo();
	~BlockInfo();


private:
	unsigned long long StartAddr_BioRemap;
	unsigned long long Size_BioRemap;
	ParseEntry * Block_bio_remap;
	
	unsigned long long StartAddr_BioQueue;
	unsigned long long Size_BioQueue;
	ParseEntry * Block_bio_queue;
	
	unsigned long long StartAddr_GetRq;
	unsigned long long SIze_GetRq;
	ParseEntry * Block_getrq;
	
	unsigned long long StartAddr_BioBackMerge;
	unsigned long long Size_BioBackMerge;
	ParseEntry * Block_bio_backmerge;
	
	unsigned long long StartAddr_BioFrontMerge;
	unsigned long long Size_BioFrontMerge;
	ParseEntry * Block_bio_frontmerge;
	
	unsigned long long StartAddr_RqInsert;
	unsigned long long Size_RqInsert;
	ParseEntry * Block_rq_insert;
	
	unsigned long long StartAddr_RqIssue;
	unsigned long long Size_RqIssue;
	ParseEntry * Block_rq_issue;
	
	unsigned long long StartAddr_RqComplete;
	unsigned long long Size_RqComplete;
	ParseEntry * Block_rq_complete;
	
	ParseEntry * Block_plug, * Block_unplug;
	
	string R_W;
	
	double TimeFromIssueToComplete; // block_rq_complete - block_rq_issue
	double TimeFromRemapToIssue; //block_rq_issue - block_rq_insert
	
	double PureTimeFromIssueToComplete;
	double PureTimeFromRemapToIssue;
	
	Node <IOInfo> * MappedIO;

	eMMCInfo * MappedEmmc;


#pragma mark - basic mathods
public:
	unsigned long long getStartAddr_BioRemap();
	void setStartAddr_BioRemap(unsigned long long _StartAddr_BioRemap);
	unsigned long long getSize_BioRemap();
	void setSize_BioRemap(unsigned long long _Size_BioRemap);
	ParseEntry *  getBlock_bio_remap();
	void setBlock_bio_remap(ParseEntry *  _Block_bio_remap);

	
	unsigned long long getStartAddr_BioQueue();
	void setStartAddr_BioQueue(unsigned long long _StartAddr_BioQueue);
	unsigned long long getSize_BioQueue();
	void setSize_BioQueue(unsigned long long _Size_BioQueue);
	ParseEntry *  getBlock_bio_queue();
	void setBlock_bio_queue(ParseEntry *  _Block_bio_queue);


	unsigned long long getStartAddr_GetRq();
	void setStartAddr_GetRq(unsigned long long _StartAddr_GetRq);
	unsigned long long getSIze_GetRq();
	void setSIze_GetRq(unsigned long long _SIze_GetRq);
	ParseEntry *  getBlock_getrq();
	void setBlock_getrq(ParseEntry *  _Block_getrq);

	
	unsigned long long getStartAddr_BioBackMerge();
	void setStartAddr_BioBackMerge(unsigned long long _StartAddr_BioBackMerge);
	unsigned long long getSize_BioBackMerge();
	void setSize_BioBackMerge(unsigned long long _Size_BioBackMerge);
	ParseEntry * getBlock_bio_backmerge();
	void setBlock_bio_backmerge(ParseEntry * _Block_bio_backmerge);

	
	unsigned long long getStartAddr_BioFrontMerge();
	void setStartAddr_BioFrontMerge(unsigned long long _StartAddr_BioFrontMerge);
	unsigned long long getSize_BioFrontMerge();
	void setSize_BioFrontMerge(unsigned long long _Size_BioFrontMerge);
	ParseEntry * getBlock_bio_frontmerge();
	void setBlock_bio_frontmerge(ParseEntry * _Block_bio_frontmerge);


	unsigned long long getStartAddr_RqInsert();
	void setStartAddr_RqInsert(unsigned long long _StartAddr_RqInsert);
	unsigned long long getSize_RqInsert();
	void setSize_RqInsert(unsigned long long _Size_RqInsert);
	ParseEntry * getBlock_rq_insert();
	void setBlock_rq_insert(ParseEntry * _Block_rq_insert);


	unsigned long long getStartAddr_RqIssue();
	void setStartAddr_RqIssue(unsigned long long _StartAddr_RqIssue);
	unsigned long long getSize_RqIssue();
	void setSize_RqIssue(unsigned long long _Size_RqIssue);
	ParseEntry * getBlock_rq_issue();
	void setBlock_rq_issue(ParseEntry * _Block_rq_issue);


	unsigned long long getStartAddr_RqComplete();
	void setStartAddr_RqComplete(unsigned long long _StartAddr_RqComplete);
	unsigned long long getSize_RqComplete();
	void setSize_RqComplete(unsigned long long _Size_RqComplete);
	ParseEntry * getBlock_rq_complete();
	void setBlock_rq_complete(ParseEntry * _Block_rq_complete);


	string getR_W();
	void setR_W(string _R_W);

	
	double getTimeFromIssueToComplete();
	void setTimeFromIssueToComplete(double _TimeFromIssueToComplete);
	double getTimeFromRemapToIssue();
	void setTimeFromRemapToIssue( double _TimeFromRemapToIssue);

	
	double getPureTimeFromIssueToComplete();
	void setPureTimeFromIssueToComplete(double _PureTimeFromIssueToComplete);
	double getPureTimeFromRemapToIssue();
	void setPureTimeFromRemapToIssue( double _PureTimeFromRemapToIssue);


	ParseEntry * getBlock_plug();
	void setBlock_plug(ParseEntry * _Block_plug);
	ParseEntry * getBlock_unplug();
	void setBlock_unplug(ParseEntry * _Block_unplug);

	Node <IOInfo> * getMappedIO();
	void setMappedIO( Node <IOInfo> * _MappedIO);

	eMMCInfo * getMappedEmmc();
	void setMappedEmmc( eMMCInfo * _MappedEmmc);


int setBlockEventParseEntry(ParseEntry * remap, ParseEntry * queue, ParseEntry * getrq, ParseEntry * backmerge, ParseEntry * frontmerge, ParseEntry * issue, ParseEntry * complete);


#pragma mark - for debug
	void print(ofstream &out);
	void print();
};

#endif /*  defined(__HFSParser__BlockLevel__) */
