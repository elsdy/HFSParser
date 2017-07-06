//
//  ProcessInfo.cpp
//  HFSParser
//
//  Created by Dongyun Shin on 3/21/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#include "ProcessInfo.h"

ProcessInfo::ProcessInfo()
{
	init();
}

ProcessInfo::~ProcessInfo()
{
	ProcessId = -1;
	ProcessName = "null";

	if( LogList)
	{
		delete LogList;
		LogList = NULL;
	}
	if(FileIOList)
	{
		delete FileIOList;
		FileIOList = NULL;
	}
	if( DirectIOList )
	{
		delete DirectIOList;
		DirectIOList = NULL;
	}

	if( BufferedIOList )
	{
		delete BufferedIOList;
		BufferedIOList = NULL;
	}

	if( NetIOList )
	{
		delete NetIOList;
		NetIOList = NULL;
	}

	if( SysfsIOList )
	{
		delete SysfsIOList;
		SysfsIOList = NULL;
	}

	if (UnknownIOList )
	{
		delete UnknownIOList;
		UnknownIOList = NULL;
	}

	if(CpuExecutionTime)
	{
		delete CpuExecutionTime;
		CpuExecutionTime = NULL;
	}
}

void ProcessInfo::init()
{
	ProcessId = 0;
	ProcessName = "";
	LogList = new List <ParseEntry>();

	FileIOList = new List <IOInfo> ();
	TotalNonFileIoHandlingTime = 0;
	TotalFileIoHandlingTime = 0;
	TotalInterruptHandlingtimeInFileIO = 0;
	TotalSoftIrqHandlingtimeInFileIO = 0;
	TotalPureFileIoHandlingTime = 0;

	DirectIOList = new List <IOInfo> ();
	BufferedIOList = new List <IOInfo> ();
	NetIOList = new List <IOInfo> ();
	SysfsIOList = new List <IOInfo> ();

	UnknownIOList = new List <IOInfo> ();

	CpuExecutionTime = new List <StartFinishTimeInfo> ();
	TotalCpuExecutionTime = 0;
}

int ProcessInfo::getKey()
{
	return ProcessId;
}

int ProcessInfo::getProcessId()
{
	return ProcessId;
}

void ProcessInfo::setProcessId(int _pid)
{
	ProcessId = _pid;
}

string ProcessInfo::getProcessName()
{
	return ProcessName;
}

void ProcessInfo::setProcessName(string _name)
{
	ProcessName = _name;
}

List <ParseEntry>* ProcessInfo::getLogList()
{
	return LogList;
}

List <IOInfo> *ProcessInfo::getFileIOList()
{
	return FileIOList;
}

void ProcessInfo::setFileIOList( List <IOInfo> * _file_io_list)
{
	FileIOList = _file_io_list;
}

double ProcessInfo::getTotalNonFileIoHandlingTime()
{
	return TotalNonFileIoHandlingTime;
}

void ProcessInfo::addTotalNonFileIoHandlingTime(double _value)
{
	TotalNonFileIoHandlingTime += _value;
}

double ProcessInfo::getTotalFileIoHandlingTime()
{
	return TotalFileIoHandlingTime;
}

double ProcessInfo::getTotalSoftIrqHandlingtimeInFileIO()
{
	return TotalSoftIrqHandlingtimeInFileIO;
}
void ProcessInfo::addTotalSoftIrqHandlingtimeInFileIO(double _value)
{
	TotalSoftIrqHandlingtimeInFileIO += _value;
}
double ProcessInfo::getTotalPureFileIoHandlingTime()
{
	return TotalPureFileIoHandlingTime;
}
void ProcessInfo::addTotalPureFileIoHandlingTime(double _value)
{
	TotalPureFileIoHandlingTime += _value;
}

void ProcessInfo::addTotalInterruptHandlingtimeInFileIO(double _value)
{
	TotalInterruptHandlingtimeInFileIO += _value;
}

void ProcessInfo::addTotalFileIoHandlingTime(double _value)
{
	TotalFileIoHandlingTime += _value;
}

double ProcessInfo::getTotalInterruptHandlingtimeInFileIO()
{
	return TotalInterruptHandlingtimeInFileIO;
}

List <IOInfo> *ProcessInfo::getNetIOList()
{
	return NetIOList;
}
void ProcessInfo::setNetIOList(List <IOInfo> *_NetIOList)
{
	NetIOList = _NetIOList;
}


List <IOInfo> *ProcessInfo::getSysfsIOList()
{
	return SysfsIOList;
}

void ProcessInfo::setSysfsIOList( List <IOInfo> *_SysfsIOList)
{
	SysfsIOList = _SysfsIOList;
}

List <StartFinishTimeInfo> *ProcessInfo::getCpuExecutionTime()
{
	return CpuExecutionTime;
}

void ProcessInfo::setCpuExecutionTime(List <StartFinishTimeInfo> * _value)
{
	CpuExecutionTime = _value;
}

double ProcessInfo::getTotalCpuExecutionTime()
{
	return TotalCpuExecutionTime;
}
void ProcessInfo::addTotalCpuExecutionTime(double _value)
{
	TotalCpuExecutionTime += _value;
}

List <IOInfo> *ProcessInfo::getDirectIOList()
{
	return DirectIOList;
}

List <IOInfo> *ProcessInfo::getBufferedIOList()
{
	return BufferedIOList;
}

List <IOInfo> *ProcessInfo::getUnknownIOList()
{
	return UnknownIOList;
}

void ProcessInfo::printLogList(ofstream &_fout)
{
	Node <ParseEntry> * cur = LogList->getHead();
	while( cur != NULL)
	{
		cur->entry->printParseEntry(_fout);
		cur = cur->next;
	}
}

void ProcessInfo::print()
{
	cout << "pid: " << getProcessId() << endl;
}
