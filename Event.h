//
//  Event.h
//  HFSParser
//
//  Created by Dongyun Shin on 4/3/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#ifndef HFSParser_Event_h
#define HFSParser_Event_h

#define BLOCK_BIO_REMAP_EVENT			"block_bio_remap"
#define BLOCK_BIO_QUEUE_EVENT			"block_bio_queue"
#define BLOCK_GETRQ_EVENT				"block_getrq"
#define BLOCK_PLUG_EVENT				"block_plug"
#define BLOCK_BIO_BACKMERGE_EVENT		"block_bio_backmerge"
#define BLOCK_BIO_FRONTMERGE_EVENT		"block_bio_frontmerge"
#define BLOCK_RQ_INSERT_EVENT			"block_rq_insert"
#define BLOCK_UNPLUG_EVENT 				"block_unplug"
#define BLOCK_RQ_ISSUE_EVENT 			"block_rq_issue"
#define BLOCK_RQ_COMPLETE_EVENT 		"block_rq_complete"

#define BLOCK_BIO_REMAP 		1
#define BLOCK_BIO_QUEUE 			2
#define BLOCK_GETRQ 				3
#define BLOCK_PLUG 				4
#define BLOCK_BIO_BACKMERGE 	5
#define BLOCK_BIO_FRONTMERGE 	6
#define BLOCK_RQ_INSERT 			7
#define BLOCK_UNPLUG 			8
#define BLOCK_RQ_ISSUE 			9
#define BLOCK_RQ_COMPLETE 		10

#endif
