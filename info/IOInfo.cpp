//
//  IOInfo.cpp
//  HFSParser
//
//  Created by Dongyun Shin on 3/21/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#include "IOInfo.h"

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

void IOInfo::printVFSInfo(ofstream & _out)
{

}


