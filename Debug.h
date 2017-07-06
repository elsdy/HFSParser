//
//  Debug.h
//  HFSParser
//
//  Created by Dongyun Shin on 6/3/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#ifndef HFSParser_Debug_h
#define HFSParser_Debug_h

#pragma mark - 디버깅을 위한 파일 이름 짓기
#define PLATFORM1 "iodin_iozone1"
#define PLATFORM2 "nexus_iozone1"
#define PLATFORM3 "packed"

#define DEBUG_DIRECTORY			"debug/"
#define PROCESS_LOG_DIRECTORY	"process_log_directory/"

#define LOG_PARSE_LIST(x) 				DEBUG_DIRECTORY x "_ParseResult.txt"
#define LOG_PROCESS_PARSE_LIST(x, y) 				DEBUG_DIRECTORY x "_" y "_ParseResult.txt"

#define LOG_ALL_BLOCK_IO_EVENT(x)		DEBUG_DIRECTORY x "_AllBlockEvent.txt"
#define LOG_BLOCK_IO_LIST(x) 			DEBUG_DIRECTORY x "_BlockIoList.txt"
#define LOG_INCOMPLETE_BLOCK_IO_LIST(x) DEBUG_DIRECTORY x "_IncompleteBlockIoList.txt"

#define LOG_ALL_EMMC_IO_EVENT(x)		DEBUG_DIRECTORY x "_AllEmmcEvent.txt"
#define LOG_EMMC_IO_LIST(x) 			DEBUG_DIRECTORY x "_EmmcIoList.txt"
#define LOG_BLOCK_EMMC_MAPPING(x)	DEBUG_DIRECTORY x "_BlockIoListWithMappedEmmc.txt"
#define LOG_BLOCK_EMMC_MAPPING_IN_PACKED_RELATION_SHIP(x) DEBUG_DIRECTORY x "_PackedEmmcCommand.txt"

#define LOG_ALL_SCHEDULE_EVENT(x)		DEBUG_DIRECTORY x "_AllScheduleEvent.txt"
#define LOG_SCHEDULE_LIST(x)		DEBUG_DIRECTORY x "_ScheduleList.txt"

#endif
