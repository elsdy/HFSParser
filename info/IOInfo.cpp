//
//  IOInfo.cpp
//  HFSParser
//
//  Created by Dongyun Shin on 3/21/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#include "IOInfo.h"

IOInfo::IOInfo():StartFinishTimeInfo()
{
	init();
}
IOInfo::IOInfo( Node<ParseEntry> * _io_start, Node<ParseEntry> *_io_end, int _Fd, long _Size, long _Pos, string _filename): StartFinishTimeInfo( _io_start, _io_end)
{
	init();

	Fd = _Fd;
	Size = _Size;

	FileName = _filename;

	Pos = _Pos;

	identify_R_W_type( _io_start );
}
IOInfo::IOInfo( Node<ParseEntry> * _io_start, Node<ParseEntry> *_io_end): StartFinishTimeInfo( _io_start, _io_end)
{
	init();

	identify_R_W_type( _io_start );
}
IOInfo::~IOInfo()
{
	delete PureIoHandlingParticle;
	delete Interrupt;
	delete SoftInterrupt;
}

void IOInfo::init()
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

int IOInfo::getFd()
{
	return Fd;
}
void IOInfo::setFd(int _Fd)
{
	Fd =_Fd;
}

long IOInfo::getSize()
{
	return Size;
}
void IOInfo::setSize(long _Size)
{
	Size = _Size;
}
long IOInfo::getPos()
{
	return Pos;
}
void IOInfo::setPos(long _Pos)
{
	Pos = _Pos;
}

string IOInfo::getFileName()
{
	return FileName;
}
void IOInfo::setFileName(string _FileName)
{
	FileName = _FileName;
}

string IOInfo::getR_W_type()
{
	return R_W_type;
}
void IOInfo::setR_W_type(string _R_W_type)
{
	R_W_type = _R_W_type;
}


int IOInfo::identify_R_W_type( Node<ParseEntry> * _io_start)
{
	if( string::npos != _io_start->entry->getFunctionName().find("read") )
	{
		R_W_type = "sys_read";
		
		return 1;
	}
	else if ( string::npos != _io_start->entry->getFunctionName().find("write") )
	{
		R_W_type = "sys_write";
		
		return 2;
	}
	return -1;
}

List <InterruptInfo> * IOInfo::getInterrupt()
{
	return Interrupt;
}

List <SoftInterruptInfo> *IOInfo::getSoftInterrupt()
{
	return SoftInterrupt;
}
List <ScheduleInfo> *IOInfo::getSchedule()
{
	return Schedule;
}

double IOInfo::getPureIoHandlingTime()
{
	return PureIoHandlingTime;
}

void IOInfo::setPureIoHandlingTime(double _value)
{
	PureIoHandlingTime = _value;
}

void IOInfo::addPureIoHandlingTime(double _value)
{
	PureIoHandlingTime += _value;
}

List <StartFinishTimeInfo> * IOInfo::getPureIoHandlingParticle()
{
	return PureIoHandlingParticle;
}

void IOInfo::printPureIoHandling(ofstream & _out)
{
	PureIoHandlingParticle->printList(_out);
}

double IOInfo::getTotalInterruptHandlingTime()
{
	return TotalInterruptHandlingTime;
}

void IOInfo::setTotalInterruptHandlingTime(double _value)
{
	TotalInterruptHandlingTime = _value;
}

void IOInfo::addTotalInterruptHandlingTime(double _value)
{
	TotalInterruptHandlingTime += _value;
}

double IOInfo::getTotalSoftInterruptHandlingTime()
{
	return TotalSoftInterruptHandlingTime;
}

void IOInfo::setTotalSoftInterruptHandlingTime(double _value)
{
	TotalSoftInterruptHandlingTime = _value;
}

void IOInfo::addTotalSoftInterruptHandlingTime(double _value)
{
	TotalSoftInterruptHandlingTime += _value;
}

BlockInfo *IOInfo::getMappedBlockIO()
{
	return MappedBlockIO;
}

void IOInfo::setMappedBlockIO(BlockInfo *_block_io)
{
	MappedBlockIO = _block_io;
}




