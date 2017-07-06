//
//  ResultForFullAnalysis.cpp
//  HFSParser
//
//  Created by Dongyun Shin on 6/26/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#include "ResultForFullAnalysis.h"

void ResultForFullAnalysis::countEmmcIO(List <eMMCInfo> *_emmc_io_list, long &_result)
{
	_result = _emmc_io_list->getSize();
}

void ResultForFullAnalysis::calculateEmmcIORequestSize(List <eMMCInfo> * _emmc_io_list, long &_result)
{
	Node <eMMCInfo> * emmc_iter = _emmc_io_list->getHead();
	
	long total_block_count = 0;
	
	while(emmc_iter)
	{
		total_block_count += emmc_iter->entry->getCntBlocks();
		
		emmc_iter = emmc_iter->next;
	}
	
	_result = total_block_count;
}

int ResultForFullAnalysis::calculateEmmcTotalResponseTime(List <eMMCInfo> *_emmc_io_list, double & _result)
{
	Node <eMMCInfo> * emmc_iter = _emmc_io_list->getHead();
	
	double total_response_time = 0;
	
	while(emmc_iter)
	{
		//		total_response_time += emmc_iter->entry->get();
		
		emmc_iter = emmc_iter->next;
	}
	
	_result = total_response_time;
	
	return 1;
}

#define PRINT_IO_HAVE_INTERRUPT_IN_IT 0
void ResultForFullAnalysis::printIOFromProcess(List <ProcessInfo> *_process_list)
{
	Node <ProcessInfo> * process_iter = _process_list->getHead();
	
	long ret = 0;
	
	while (process_iter)
	{
#if DEBUG_printIOFromProcess
		if(process_iter->entry->getName() == "sdcard")
			cout << "debug to stop" << endl;
#endif
		List <IOInfo> * io_info_list = process_iter->entry->getFileIOList();
		
		ret = io_info_list->getSize();
		
		Node <IOInfo> *io_node = io_info_list->getHead();
		while(io_node)
		{
//#if DEBUG_printIOFromProcess
//			if( io_node->entry->getInterrupt()->getSize() || io_node->entry->getSoftInterrupt()->getSize() )
//				cout << "a IO with interrupt" << endl;
//#endif

#if PRINT_IO_HAVE_INTERRUPT_IN_IT
			if( io_node->entry->getTotalInterruptHandlingTime() || io_node->entry->getTotalSoftInterruptHandlingTime() ) 	// TotalInterruptTime
			   printIOInfoResult(io_node, process_iter);
#else
			printIOInfoResult(io_node, process_iter);
#endif
			io_node = io_node->next;
		}
		
		process_iter = process_iter->next;
	}
}

void ResultForFullAnalysis::printIOInfoResult(Node <IOInfo> *_io, Node <ProcessInfo> * _process)
{
	Result::preparePrintData();
	
	Result::printData(All);
	Result::printData(ENTRY_TYPE_SYSIO);
	Result::printData(_io->entry->getStart()->getLineNumber());
	Result::printData(_process->entry->getProcessName());
	Result::printData(_process->entry->getProcessName()); // origin process name
	Result::printData(_process->entry->getProcessId());
	Result::printData(_process->entry->getProcessId()); // origin process id
	Result::printData(_io->entry->getStart()->getApproxTimeStamp());
//	Result::printData(_io->entry->getIoStartLog()->entry->getTimeStamp());
	Result::printData(_io->entry->getFinish()->getApproxTimeStamp());
//	Result::printData(_io->entry->getIoFinishLog()->entry->getTimeStamp());
	Result::printData(_io->entry->getR_W_type());
	Result::printData(_io->entry->getSize());
	Result::printData(_io->entry->getFileName());
	Result::printData("0"); // ionode
	Result::printData("0"); // mapped_syscall_io
	Result::printData("0");  // access_address
	Result::printData(_io->entry->getFinish()->getApproxTimeStamp() - _io->entry->getStart()->getApproxTimeStamp());
	Result::printData(_io->entry->getPureIoHandlingTime()); // PureIOResTime
	Result::printData(_io->entry->getPureIoHandlingTime() - _io->entry->getTotalInterruptHandlingTime() - _io->entry->getTotalSoftInterruptHandlingTime()); 	// TotalInterruptTime
	
	long data_left = Result::checkHowManyDataShouldBePrinted();
	if( data_left != 0 )
		assert(0);
}

void ResultForFullAnalysis::printIOSchedule(List <BlockInfo> *_block_io_list, List <ProcessInfo> *_process_list)
{
	Node <BlockInfo> * block_io_info = _block_io_list->getHead();
	
	while(block_io_info)
	{
		printIoScheduleResult(block_io_info, _process_list);
		
		block_io_info = block_io_info->next;
	}
}

void ResultForFullAnalysis::printBlockIO(List <BlockInfo> *_block_io_list, List <ProcessInfo> *_process_list)
{
	Node <BlockInfo> * block_io_info = _block_io_list->getHead();
	
	while(block_io_info)
	{
		printBlockInfoResult(block_io_info, _process_list);
		
		block_io_info = block_io_info->next;
	}
}

void ResultForFullAnalysis::printIoScheduleResult(Node <BlockInfo> *_block_io_info, List <ProcessInfo> *_process_list)
{
	int pid = 0;
	
	Result::preparePrintData();
	
	Result::printData(All);
	Result::printData(ENTRY_TYPE_IO_SCHEUDLER);
	if( _block_io_info->entry->getBlock_bio_remap() )
	{
		Result::printData(_block_io_info->entry->getBlock_bio_remap()->getLineNumber());
		pid = _block_io_info->entry->getBlock_bio_remap()->getProcessID();
	}
	else
	{
		Result::printData(_block_io_info->entry->getBlock_bio_queue()->getLineNumber());
		pid = _block_io_info->entry->getBlock_bio_queue()->getProcessID();
	}
	
	Node <ProcessInfo> * target_process = NULL;
	_process_list->searchNodeWithKey(pid, &target_process);
	Result::printData(target_process->entry->getProcessName());
	
	Result::printData(target_process->entry->getProcessName()); // origin process name
	
	Result::printData(target_process->entry->getProcessId());
	Result::printData(target_process->entry->getProcessId()); // origin process id
	
	if( _block_io_info->entry->getBlock_bio_remap() )
	{
		Result::printData(_block_io_info->entry->getBlock_bio_remap()->getApproxTimeStamp());
	}
	else
	{
		Result::printData(_block_io_info->entry->getBlock_bio_queue()->getApproxTimeStamp());
	}
	
	Result::printData(_block_io_info->entry->getBlock_rq_issue()->getApproxTimeStamp());
	
	Result::printData(_block_io_info->entry->getR_W());
	Result::printData(_block_io_info->entry->getSize_RqIssue());
	
	Result::printData(" "); // ionode
	Result::printData("0"); // ionode
	Result::printData("0"); // mapped_syscall_io
	Result::printData(_block_io_info->entry->getStartAddr_BioQueue()); // access_address
	
	// IoRespTime
	if( _block_io_info->entry->getBlock_bio_remap() )
	{
		Result::printData( _block_io_info->entry->getBlock_rq_issue()->getApproxTimeStamp() - _block_io_info->entry->getBlock_bio_remap()->getApproxTimeStamp());
	}
	else if ( _block_io_info->entry->getBlock_bio_queue() )
	{
		Result::printData( _block_io_info->entry->getBlock_rq_issue()->getApproxTimeStamp() - _block_io_info->entry->getBlock_bio_queue()->getApproxTimeStamp());
	}
	
	// PureIoRespTime1
	Result::printData("0"); // PureIOResTime
	
	// PureIoRespTime2
	Result::printData("0");// TotalInterruptTime
	

	long data_left = Result::checkHowManyDataShouldBePrinted();
		if( data_left != 0 )
			assert(0);
}

void ResultForFullAnalysis::printBlockInfoResult(Node <BlockInfo> * _block_io_info, List <ProcessInfo> *_process_list)
{
	Result::preparePrintData();
	
	Result::printData(All);
	Result::printData(ENTRY_TYPE_BLOCK_IO_TRANSFER);
	Result::printData(_block_io_info->entry->getBlock_rq_issue()->getLineNumber());
	
	int pid = _block_io_info->entry->getBlock_rq_issue()->getProcessID();
	Node <ProcessInfo> * target_process = NULL;
	_process_list->searchNodeWithKey(pid, &target_process);
	Result::printData(target_process->entry->getProcessName());
	
	Result::printData(target_process->entry->getProcessName()); // origin process name
	
	Result::printData(target_process->entry->getProcessId());
	Result::printData(target_process->entry->getProcessId()); // origin process id
	
	Result::printData(_block_io_info->entry->getBlock_rq_issue()->getApproxTimeStamp());
	Result::printData(_block_io_info->entry->getBlock_rq_complete()->getApproxTimeStamp());
	
	Result::printData(_block_io_info->entry->getR_W());
	Result::printData(_block_io_info->entry->getSize_RqIssue());
	
	Result::printData(" "); // file name
	
	Result::printData("0"); // ionode
	Result::printData("0"); // mapped_syscall_io
	Result::printData(_block_io_info->entry->getStartAddr_RqIssue()); //access_address
	
	// IoRespTime
	Result::printData( _block_io_info->entry->getBlock_rq_complete()->getApproxTimeStamp() - _block_io_info->entry->getBlock_rq_issue()->getApproxTimeStamp());

	Result::printData("0"); // PureIoRespTime1
	Result::printData("0"); // PureIoRespTime2

	long data_left = Result::checkHowManyDataShouldBePrinted();
	if( data_left != 0 )
		assert(0);
}


void ResultForFullAnalysis::printEmmcIO(List <eMMCInfo> *_emmc_io_list, List <ProcessInfo> *_process_list)
{
	Node <eMMCInfo> * emmc_io_info = _emmc_io_list->getHead();
	
	while(emmc_io_info)
	{
		printEmmcInfoResult(emmc_io_info, _process_list);
		
		emmc_io_info = emmc_io_info->next;
	}
}

void ResultForFullAnalysis::printEmmcInfoResult( Node <eMMCInfo> *_emmc_io_info, List <ProcessInfo> *_process_list)
{
	Result::preparePrintData();

	Result::printData(All);
	Result::printData(ENTRY_TYPE_EMMC_IO);
	Result::printData(_emmc_io_info->entry->getStart()->getLineNumber());
	
	int pid = _emmc_io_info->entry->getStart()->getProcessID();
	Node <ProcessInfo> * target_process = NULL;
	_process_list->searchNodeWithKey(pid, &target_process);
	Result::printData(target_process->entry->getProcessName());
	
	Result::printData(target_process->entry->getProcessName()); // origin process name
	
	Result::printData(target_process->entry->getProcessId());
	Result::printData(target_process->entry->getProcessId()); // origin process id
	
	Result::printData(_emmc_io_info->entry->getStart()->getApproxTimeStamp());
	Result::printData(_emmc_io_info->entry->getFinish()->getApproxTimeStamp());
	
	string cmd = "CMD" + to_string(_emmc_io_info->entry->getCMD());
	Result::printData(cmd); // rw
	Result::printData(_emmc_io_info->entry->getCntBlocks()  * _emmc_io_info->entry->getBlockSz()); // iosize
	
	Result::printData(" ");
	
	Result::printData("0"); // ionode
	Result::printData("0"); // mapped_syscall_io
	Result::printData(_emmc_io_info->entry->getStartAddress()); // access_address
	
	Result::printData(_emmc_io_info->entry->getElapsedTime()); // IoRespTime
	Result::printData("0"); // PureIoRespTime1
	Result::printData("0"); // PureIoRespTime2
	
	long data_left = Result::checkHowManyDataShouldBePrinted();
	if( data_left != 0 )
		assert(0);
}

void ResultForFullAnalysis::printCpuExecutionTime(List <ProcessInfo> *_process_list)
{
	Node <ProcessInfo> * process_iter = _process_list->getHead();
		
	while (process_iter)
	{
		List <StartFinishTimeInfo> *cpu_execution_time_list = process_iter->entry->getCpuExecutionTime();
		Node <StartFinishTimeInfo> *cpu_execution_time_iter = cpu_execution_time_list->getHead();
		
		while(cpu_execution_time_iter)
		{
			printCPUExecutioTimeInfo(cpu_execution_time_iter, process_iter);
			
			cpu_execution_time_iter = cpu_execution_time_iter->next;
		}
				
		process_iter = process_iter->next;
	}
}

int ResultForFullAnalysis::printCPUExecutioTimeInfo(Node <StartFinishTimeInfo> *_cpu_execution_time, Node <ProcessInfo> *_process)
{
	Result::preparePrintData();

	Result::printData(All);
	Result::printData(ENTRY_TYPE_CPU_EXECUTE);
	Result::printData(_cpu_execution_time->entry->getStart()->getLineNumber());
	Result::printData(_process->entry->getProcessName());
	Result::printData(_process->entry->getProcessName()); // origin process name
	Result::printData(_process->entry->getProcessId());
	Result::printData(_process->entry->getProcessId()); // origin process id
	Result::printData(_cpu_execution_time->entry->getStart()->getApproxTimeStamp());
	Result::printData(_cpu_execution_time->entry->getFinish()->getApproxTimeStamp());
	Result::printData(" "); // rw
	Result::printData("-1"); // iosize
	Result::printData(" "); // file name
	
	Result::printData("0"); // ionode
	Result::printData("0"); // mapped_syscall_io
	Result::printData("0"); // access_address
	
	Result::printData(_cpu_execution_time->entry->getElapsedTime()); // IoRespTime
	Result::printData("0"); // PureIOResTime
	Result::printData("0");// TotalInterruptTime
	
	long data_left = Result::checkHowManyDataShouldBePrinted();
	if( data_left != 0 )
		assert(0);
	
	return 1;
}