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
private:
	int Value;
public:
	BitSet()
	{
		init();
	}
	~BitSet()
	{
		
	}
	void init()
	{
		Value = 0;
	}
	
#pragma mark - check if this entry is collected or not
	int compareValue(int _value)
	{
		int test_value = Value & _value;
		if( test_value == _value)
			return 1;
		else
			return 0;
	}
	void setValue(int _value)
	{
		Value = Value | _value;
	}
	void clearValue(int _value)
	{
		Value = Value & ~_value;
	}

};

#endif /* defined(__HFSParser__Bitset__) */
