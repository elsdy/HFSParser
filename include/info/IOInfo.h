//
//  IOInfo.h
//  HFSParser
//
//  Created by Dongyun Shin on 3/21/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#ifndef __HFSParser__IOInfo__
#define __HFSParser__IOInfo__

#include <iostream>

#include "ParseEntry.h"
#include "Node.h"

#include "BlockLevel.h"
#include "eMMCLevel.h"

#include "Interrupt.h"
#include "Scheduler.h"
#include "List.h"

class BlockInfo;

using namespace std;

class IOInfo: public StartFinishTimeInfo
{
public:
	IOInfo();
	IOInfo( Node<ParseEntry> * _io_start, Node<ParseEntry> *_io_end, int _Fd, long _Size, long _Pos, string _filename);
	IOInfo( Node<ParseEntry> * _io_start, Node<ParseEntry> *_io_end);
	~IOInfo();

private:
	int Fd;
	long Size;
	long Pos;
	string FileName;
	string R_W_type;
	
	List <InterruptInfo> *Interrupt;

	double TotalInterruptHandlingTime;

	/// a list includes interrupt occured during this IO handling
	List <SoftInterruptInfo> *SoftInterrupt;

	double TotalSoftInterruptHandlingTime;

	double PureIoHandlingTime;
	List <StartFinishTimeInfo> *PureIoHandlingParticle;

	BlockInfo *MappedBlockIO;
	eMMCInfo *MappedEmmcIO;

	/// a list includes schedule occured during this IO handling
	List <ScheduleInfo> *Schedule;
	eMMCInfo *eMMCOperation;
	
	void init();
	
public:
	int getFd();
	void setFd(int _Fd);
	
	long getSize();
	void setSize(long _Size);
	
	long getPos();
	void setPos(long _Pos);
	
	string getFileName();
	void setFileName(string _FileName);
	
	string getR_W_type();
	void setR_W_type(string _R_W_type);
	int identify_R_W_type( Node<ParseEntry> * _io_start);

	List <InterruptInfo> *getInterrupt();
	
	double getTotalInterruptHandlingTime();
	void setTotalInterruptHandlingTime(double _value);
	void addTotalInterruptHandlingTime(double _value);

	List <SoftInterruptInfo> *getSoftInterrupt();
	double getTotalSoftInterruptHandlingTime();
	void setTotalSoftInterruptHandlingTime(double _value);
	void addTotalSoftInterruptHandlingTime(double _value);

	List <ScheduleInfo> *getSchedule();

	double getPureIoHandlingTime();
	void setPureIoHandlingTime(double _value);
	void addPureIoHandlingTime(double _value);
	void printPureIoHandling(ofstream & _out);
	
	List <StartFinishTimeInfo> *getPureIoHandlingParticle();

	BlockInfo *getMappedBlockIO();
	void setMappedBlockIO(BlockInfo *_block_io);
};

#endif /* defined(__HFSParser__IOInfo__) */
