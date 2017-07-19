//
//  Bitset.h
//  HFSParser
//
//  Created by Dongyun Shin on 6/3/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#ifndef __HFSParser__Bitset__
#define __HFSParser__Bitset__

#include <iostream>

#define ALL			0xFFFF
#define MASK			0x0000

class BitSet
{
public:
	BitSet();
	~BitSet();


private:
	void init();


private:
	int Value;


public:
#pragma mark - check if this entry is collected or not
	int compareValue(int _value);
	void setValue(int _value);
	void clearValue(int _value);


};

#endif /* defined(__HFSParser__Bitset__) */
