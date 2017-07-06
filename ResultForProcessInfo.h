//
//  ResultForProcessInfo.h
//  HFSParser
//
//  Created by Dongyun Shin on 7/29/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#ifndef __HFSParser__ResultForProcessInfo__
#define __HFSParser__ResultForProcessInfo__

#include "Result.h"
#include "ProcessInfo.h"
#include "List.h"

class ResultForProcessInfo: public Result
{
private:
	
	
public:
	ResultForProcessInfo(string __file_name_to_open): Result(__file_name_to_open)
	{
		cout << "Result For process info will be generated" << endl;
		
		init();
	}
	
	~ResultForProcessInfo()
	{
		
	}
	
#define DEBUG_printTimeCompositionForProcess 0
#define PRINT_ADDITIONAL_INFO 1
	void printTimeCompositionForProcess( List <ProcessInfo> * _process_list, int _cpu_num)
	{
		Node <ProcessInfo> * process_iter = _process_list->getHead();
		
		while(process_iter)
		{
			if(process_iter->entry->getCpuExecutionTime()->getSize() <= 1)
			{
				process_iter = process_iter->next;
				continue;
			}
			
			double start_time = process_iter->entry->getCpuExecutionTime()->getHead()->entry->getStart()->getTimeStamp();
			double finish_time = process_iter->entry->getCpuExecutionTime()->getTail()->entry->getFinish()->getTimeStamp();
			
			double process_elapse_time =  finish_time - start_time;
			double total_elapse_cpu_time = process_elapse_time * _cpu_num;
			
			double target_process_cpu_executione_time = process_iter->entry->getTotalCpuExecutionTime();
			
			double target_process_io_cpu_time = process_iter->entry->getTotalFileIoHandlingTime();
			double target_process_io_interrupt_time = process_iter->entry->getTotalInterruptHandlingtimeInFileIO();
			double target_process_io_softirq_time = process_iter->entry->getTotalSoftIrqHandlingtimeInFileIO();
			double target_process_pure_io_time = target_process_io_cpu_time - target_process_io_interrupt_time - target_process_io_softirq_time;
			double target_process_non_io_time = target_process_cpu_executione_time - target_process_io_cpu_time;
			
			
			double other_process_time = total_elapse_cpu_time - target_process_cpu_executione_time;
			
#if DEBUG_printTimeCompositionForProcess
			if( process_elapse_time < target_process_cpu_executione_time )
			{
				double temp_target_process_cpu_executione_time = target_process_cpu_executione_time;
				cout << fixed;
				cout.precision(6);
				cout << "debug to stop: " << endl;
				cout << "process_elapse_time: " << finish_time << " - " << start_time << " = " << process_elapse_time << endl;
				cout << "target_process_cpu_executione_time: " << target_process_cpu_executione_time << endl;
				Node <StartFinishTimeInfo> * iter = process_iter->entry->getCpuExecutionTime()->getHead();
				while (iter)
				{
					iter->entry->print();
					cout << "target_process_cpu_executione_time: " << temp_target_process_cpu_executione_time <<  " - " << iter->entry->getElapsedTime() << " = ";
					temp_target_process_cpu_executione_time -= iter->entry->getElapsedTime();
					cout << temp_target_process_cpu_executione_time << endl;
					cout << endl;

					iter = iter->next;
				}

			}
#endif

			Result::preparePrintData();
			Result::getOutputFile() << fixed;
			Result::getOutputFile().precision(6);
			Result::printData(process_iter->entry->getProcessName() ); // process_name
			Result::printData(process_iter->entry->getProcessId() ); // pid
			Result::printData(start_time ); // start_time
			Result::printData(finish_time );  // finish_time
			Result::printData(process_elapse_time);
			Result::printData(total_elapse_cpu_time); // elasped time
			Result::printData(target_process_cpu_executione_time ); //  target_process_cpu_executione_time
			Result::printData(target_process_io_cpu_time ); // target_process_io_cpu_time
			Result::printData(target_process_io_interrupt_time ); // target_process_io_interrupt_time
			Result::printData(target_process_io_softirq_time ); // target_process_io_softirq_time
			Result::printData(target_process_pure_io_time ); // target_process_pure_io_time
			Result::printData(target_process_non_io_time ); // target_process_non_io_time
			Result::printData(other_process_time);
#if PRINT_ADDITIONAL_INFO
			Result::printData(target_process_cpu_executione_time*100/total_elapse_cpu_time, "%");
			Result::printData(target_process_pure_io_time*100/total_elapse_cpu_time, "%");
			Result::printData(target_process_pure_io_time*100/process_elapse_time, "%");
#endif
			long data_left = Result::checkHowManyDataShouldBePrinted();
			if( data_left != 0 )
				assert(0);
			
			process_iter = process_iter->next;
		}
		
	}
};

#endif
