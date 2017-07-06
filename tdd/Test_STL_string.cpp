//
//  Test_STL.cpp
//  HFSParser
//
//  Created by Dongyun Shin on 4/1/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#include "Test_STL_string.h"

void test_find_last_not_of2()
{
	string test = "	jbd2/mmcblk0p23-572   [002] ...1    65.318175: block_bio_remap: 179,0 WS 3198552 + 8 <- (179,23) 20056";
	string::size_type posPlus = test.find_first_of("+");
	
	string::size_type posNum = test.find_last_not_of(" 0123456789+", posPlus);
	posNum = test.find_first_of("0123456789", posNum);
	
	string substr = test.substr(posNum, posPlus);
	
	cout << substr << endl;
	
	string::size_type posNumEnd = test.find_last_not_of("+", posPlus);
	
	substr = test.substr(posNum, posNumEnd-posNum);
	
	cout << substr << endl;
		
	posNum = test.find_first_of("0123456789", posPlus);
	posNumEnd = test.find_first_not_of("0123456789", posNum);
	
	string size = test.substr(posNum, posNumEnd - posNum);
	
	cout << size << endl;
}



//2014_04_04_Fri_PM_05_11_39
void test_find_last_not_of()
{
	// The first member function
	// searches for a single character in a string
	string str1 ( "dddd-1dd4-abdd" );
	cout << "The original string str1 is: " << str1 << endl;
	basic_string <char>::size_type indexCh1a, indexCh1b;
	static const basic_string <char>::size_type npos = -1;
	
	indexCh1a = str1.find_last_not_of ( "d" , 7 );
	if ( indexCh1a != npos )
		cout << "The index of the last non 'd'\n found before the "
		<< "7th position in str1 is: " << indexCh1a << endl;
	else
		cout << "The non 'd' character was not found ." << endl;
	
	indexCh1b = str1.find_last_not_of  ( "d" );
	if ( indexCh1b != npos )
		cout << "The index of the non 'd' found in str1 is: "
		<< indexCh1b << endl << endl;
	else
		cout << "The Character 'non x' was not found in str1."
		<< endl << endl;
	
	// The second member function searches a string
	// for a substring as specified by a C-string
	string str2 ( "BBB-1111" );
	cout << "The original string str2 is: " << str2 << endl;
	basic_string <char>::size_type indexCh2a, indexCh2b;
	
	const char *cstr2 = "B1";
	indexCh2a = str2.find_last_not_of  ( cstr2 , 6 );
	if ( indexCh2a != npos )
		cout << "The index of the last occurrence of a "
		<< "element\n not of 'B1' in str2 before the 6th "
		<< "position is: " << indexCh2a << endl;
	else
		cout << "Elements not of the substring 'B1' were not "
		<< "\n found in str2 before the 6th position."
		<< endl;
	
	const char *cstr2b = "B-1";
	indexCh2b = str2.find_last_not_of  ( cstr2b );
	if ( indexCh2b != npos )
		cout << "The index of the last element not "
		<< "in 'B-1'\n is: "
		<< indexCh2b << endl << endl;
	else
		cout << "The elements of the substring 'B-1' were "
		<< "not found in str2 ."
		<< endl << endl;
	
	// The third member function searches a string
	// for a substring as specified by a C-string
	string str3 ( "444-555-GGG" );
	cout << "The original string str3 is: " << str3 << endl;
	basic_string <char>::size_type indexCh3a, indexCh3b;
	
	const char *cstr3a = "45G";
	indexCh3a = str3.find_last_not_of ( cstr3a );
	if ( indexCh3a != npos )
		cout << "The index of the last occurrence of an "
		<< "element in str3\n other than one of the "
		<< "characters in '45G' is: " << indexCh3a
		<< endl;
	else
		cout << "Elements in str3 contain only characters "
		<< " in the string  '45G'. "
		<< endl;
	
	const char *cstr3b = "45G";
	indexCh3b = str3.find_last_not_of ( cstr3b , 6 , indexCh3a - 1 );
	if (indexCh3b != npos )
		cout << "The index of the penultimate occurrence of an "
		<< "element\n not in '45G' in str3 is: "
		<< indexCh3b << endl << endl;
	else
		cout << "Elements in str3 contain only characters "
		<< " in the string '45G'. "
		<< endl  << endl;
	
	// The fourth member function searches a string
	// for a substring as specified by a string
	string str4 ( "12-ab-12-ab" );
	cout << "The original string str4 is: " << str4 << endl;
	basic_string <char>::size_type indexCh4a, indexCh4b;
	
	string str4a ( "b-a" );
	indexCh4a = str4.find_last_not_of  ( str4a , 5 );
	if ( indexCh4a != npos )
		cout << "The index of the last occurrence of an "
		<< "element not\n in 'b-a' in str4 before the 5th "
		<< "position is: " << indexCh4a << endl;
	else
		cout << "Elements other than those in the substring"
		<< " 'b-a' were not found in the string str4."
		<< endl;
	
	string str4b ( "12" );
	indexCh4b = str4.find_last_not_of ( str4b  );
	if ( indexCh4b != npos )
		cout << "The index of the last occurrence of an "
		<< "element not in '12'\n in str4 before the end "
		<< "position is: " << indexCh4b << endl;
	else
		cout << "Elements other than those in the substring"
		<< " '12'\n were not found in the string str4."
		<< endl;
}

void test_substr()
{
	string temp = "sys_read_pre / sys_read_post / 1";
	
	string::size_type pos = temp.find_first_not_of(" /");
	string::size_type del = temp.find_first_of("/");
	
	cout << temp << endl;
	
	cout <<temp.substr(pos, del) << endl;
}

void test_tokenize()
{
	string str("This is a string to be split");
	string token;
	stringstream stream(str);
	
	while (stream >> token)
		cout << token<< endl;
}
