//
//  ResultForMobile.h
//  HFSParser
//
//  Created by Dongyun Shin on 6/26/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#ifndef __HFSParser__ResultForMobile__
#define __HFSParser__ResultForMobile__

#include <iostream>

#include "Result.h"
#include "Parser.h"

class ResultForMobile: public Result
{
private:
	long TestInfoId;
	string Name;
		
	void init()
	{
		TestInfoId = 0;
		Name = "";
		
	}
	
public:
	
	/**
	 @brief
	 find a target process whose name is _process_name in a _process_list. 
	 analyze the emmc IO between the start time when the target process starts and the finish when the target process finishes
	 @param
	 _process_name: target process to be analyzed
	 _process_list: a list which should include the process whose name is _process_name
	 _emmc_list: a list which consists of emmc io sequence,
	 @return
	 print the analyzed result
	 */
#define DEBUG_printProcessSummaryForMobile 0
	void printProcessSummaryForMobile(string _process_name, List<ProcessInfo> *_process_list, long _test_id, List<eMMCInfo> *_emmc_list);
	
void printProcessSummaryForMobileWithWorkloadSet( List<string> *_workload_set, List<ProcessInfo> *_process_list, long _test_id, List<eMMCInfo> *_emmc_list)
	{
		cout << endl << "printProcessSummaryForMobileWithWorkloadSet started" << endl;
		
		Node <string> *workload_iter = _workload_set->getHead();
		while(workload_iter)
		{
			printProcessSummaryForMobile( *(workload_iter->entry), _process_list, _test_id, _emmc_list);
			
			workload_iter = workload_iter->next;
		}
		
	}
	
	ResultForMobile(string __file_name_to_open): Result(__file_name_to_open)
	{
		cout << "Result For a mobile version will be generated" << endl;
		
		init();
	}
	
	~ResultForMobile()
	{
		
	}
	
	// ******************* /
	// **** get methods *****/
	// ******************* /
	
	long getTestInfoId()
	{
		return TestInfoId;
	}
	string & getName()
	{
		return Name;
	}
#define DEBUG_loadTargetWorkloadName 0
	void loadTargetWorkloadNameForMobile(string _configuration_file);

};

#endif /* defined(__HFSParser__ResultForMobile__) */
