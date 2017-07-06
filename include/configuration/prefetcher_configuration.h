//
//  prefetcher_configuration.h
//  HFSParser
//
//  Created by Dongyun Shin on 5/	13/	16.
//  Copyright © 20	16 Dongyun Shin. All rights reserved.
//

#ifndef prefetcher_configuration_h
#define prefetcher_configuration_h

#pragma mark - buffer size
#define BUFF_SIZE 4096

#pragma mark - the generated code as an output for prefetcing
#define PREFECHING_CODE "prefetcher.c"

#pragma mark - the number of the target functions
#define NUMBEROF_PREFETCHED_CALLS \
		PREFETCH_MMAP + PREFETCH_VFSCALL + PREFETCH_OPENAT +\
		PREFETCH_OPEN + PREFETCH_LSTAT_STAT + PREFETCH_FORK +\
		PREFETCH_GETDENTS + PREFETCH_ACCESS + PREFETCH_FACCESSAT +\
		PREFETCH_EXECVEAT + PREFETCH_EXECVE + PREFETCH_CREAT +\
		PREFETCH_READLINK + PREFETCH_PREFETCH_FSTATFS64 +\
		PREFETCH_PREFETCH_STATFS64 + PREFETCH_FSTATFS + PREFETCH_STATFS +\
		PREFETCH_TRUNCATE + PREFETCH_FADVISE64_64 + PREFETCH_FSTATFS +\
		PREFETCH_FADVISE64 + PREFETCH_FSTAT 				

#pragma mark - check for including the target functions with the name

// 로그에서 빼고 더하고를 결정
#define PREFETCH_MMAP	 			1
#define PREFETCH_VFSCALL			1
#define PREFETCH_OPENAT 			1
#define PREFETCH_OPEN 				1
#define PREFETCH_LSTAT_STAT			1
#define PREFETCH_FORK 				1
#define PREFETCH_GETDENTS 			1
#define PREFETCH_ACCESS 			1
#define PREFETCH_FACCESSAT 			1
#define PREFETCH_EXECVEAT 			1
#define PREFETCH_EXECVE 			1
#define PREFETCH_CREAT 				1
#define PREFETCH_READLINK 			1
#define PREFETCH_PREFETCH_FSTATFS64	1
#define PREFETCH_PREFETCH_STATFS64 	1
#define PREFETCH_FSTATFS 			1
#define PREFETCH_STATFS 			1
#define PREFETCH_TRUNCATE 			1
#define PREFETCH_FADVISE64_64		1
#define PREFETCH_FSTATFS 			1
#define PREFETCH_FADVISE64 			1
#define PREFETCH_FSTAT 				1

#define PREFETCH_HMF                1

#endif /* prefetcher_configuration_h */
