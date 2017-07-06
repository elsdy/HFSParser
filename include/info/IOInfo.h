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

class BlockInfo;

using namespace std;

class IOInfo: public StartFinishTimeInfo
{
private:
	int Fd;
	long Size;
	long Pos;
	
	List <StartFinishTimeInfo> *PureIoHandlingParticle;
	double PureIoHandlingTime;
	
	string FileName;
	string R_W_type;
		
	BlockInfo *MappedBlockIO;
	eMMCInfo *MappedEmmcIO;
	
	/// a list includes interrupt occured during this IO handling
	List <InterruptInfo> *Interrupt;
	double TotalInterruptHandlingTime;
	List <SoftInterruptInfo> *SoftInterrupt;
	double TotalSoftInterruptHandlingTime;
	/// a list includes schedule occured during this IO handling
	List <ScheduleInfo> *Schedule;
	eMMCInfo *eMMCOperation;
	
	void init()
	{
		Fd = Size = Pos = 0;
		
		PureIoHandlingParticle = new List <StartFinishTimeInfo> ();
		PureIoHandlingTime = 0;
		
		FileName = "";
		R_W_type = "";
		
		MappedBlockIO = NULL;
		MappedEmmcIO = NULL;
		
		Interrupt = new List <InterruptInfo> ();
		TotalInterruptHandlingTime = 0;
		SoftInterrupt = new List <SoftInterruptInfo> ();
		TotalSoftInterruptHandlingTime = 0;
		
		eMMCOperation = NULL;
	}
	
public:

	IOInfo(): StartFinishTimeInfo()
	{
		init();
	}
	IOInfo( Node<ParseEntry> * _io_start, Node<ParseEntry> *_io_end, int _Fd, long _Size, long _Pos, string _filename): StartFinishTimeInfo( _io_start, _io_end)
	{
		init();
		
		Fd = _Fd;
		Size = _Size;
		
		FileName = _filename;

		Pos = _Pos;
		
		identify_R_W_type( _io_start );
	}
	IOInfo( Node<ParseEntry> * _io_start, Node<ParseEntry> *_io_end): StartFinishTimeInfo( _io_start, _io_end)
	{
		init();
		
		identify_R_W_type( _io_start );
	}
	~IOInfo()
	{
		delete PureIoHandlingParticle;
		delete Interrupt;
		delete SoftInterrupt;
	}
	
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
	
	double getTotalInterruptHandlingTime()
	{
		return TotalInterruptHandlingTime;
	}
	
	void setTotalInterruptHandlingTime(double _value)
	{
		TotalInterruptHandlingTime = _value;
	}
	void addTotalInterruptHandlingTime(double _value)
	{
		TotalInterruptHandlingTime += _value;
	}
	List <SoftInterruptInfo> *getSoftInterrupt();
	double getTotalSoftInterruptHandlingTime()
	{
		return TotalSoftInterruptHandlingTime;
	}
	
	void setTotalSoftInterruptHandlingTime(double _value)
	{
		TotalSoftInterruptHandlingTime = _value;
	}
	void addTotalSoftInterruptHandlingTime(double _value)
	{
		TotalSoftInterruptHandlingTime += _value;
	}
	
	List <ScheduleInfo> *getSchedule();
	
	double getPureIoHandlingTime();
	
	void setPureIoHandlingTime(double _value);
	
	void addPureIoHandlingTime(double _value);
	
	BlockInfo *getMappedBlockIO()
	{
		return MappedBlockIO;
	}
	
	void setMappedBlockIO(BlockInfo *_block_io)
	{
		MappedBlockIO = _block_io;
	}
		
	List <StartFinishTimeInfo> *getPureIoHandlingParticle();
	
	void printPureIoHandling(ofstream & _out);

	void printVFSInfo(ofstream & _out);
	
};

#endif /* defined(__HFSParser__IOInfo__) */
