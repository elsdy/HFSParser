//
//  Bitset.cpp
//  HFSParser
//
//  Created by Dongyun Shin on 6/3/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#include "Bitset.h"

BitSet::BitSet()
{
	init();
}
BitSet::~BitSet()
{

}

void BitSet::init()
{
	Value = 0;
}

int BitSet::compareValue(int _value)
{
	int test_value = Value & _value;
	if( test_value == _value)
		return 1;
	else
		return 0;
}
void BitSet::setValue(int _value)
{
	Value = Value | _value;
}
void BitSet::clearValue(int _value)
{
	Value = Value & ~_value;
}

