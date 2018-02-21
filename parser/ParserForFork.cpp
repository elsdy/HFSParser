//
//  ParserForFork.cpp
//  HFSParser_Xcode
//
//  Created by Dongyun Shin on 2/1/16.
//  Copyright © 2016 Dongyun Shin. All rights reserved.
//

#include "ParserForFork.h"

//http://www.cplusplus.com/reference/map/multimap/equal_range/
//http://en.cppreference.com/w/cpp/regex
//http://www.regular-expressions.info/stdregex.html
//http://www.hanbit.co.kr/network/view.html?bi_id=1218
//https://solarianprogrammer.com/2011/10/12/cpp-11-regex-tutorial/
//http://www.codeguru.com/cpp/cpp/cpp_mfc/stl/article.php/c15339/A-TR1-Tutorial-Regular-Expressions.htm

//void genParentChildTree()
//{
//	multimap<int , int > m;
//
//	m.insert( pair<int,int>( 5, 100) );
//	m.insert( pair<int,int>( 3, 50) );
//	m.insert( pair<int,int>( 7, 80) );
//	m.insert( pair<int,int>( 2, 100) );
//	m.insert( pair<int,int>( 4, 100) );
//	m.insert( pair<int,int>( 7, 200) );
//	m.insert( pair<int,int>( 7, 300) );
//
//	pair<multimap<int,int>::iterator, multimap<int, int>::iterator > iter_pair;
//	iter_pair = m.equal_range( 7 );
//	multimap<int,int>::iterator iter;
//	for( iter = iter_pair.first ; iter != iter_pair.second ; iter++)
//		cout << "m["<< iter->first <<"]: " << iter->second << endl;
//}

multimap <int, int> pidtree;

/*
1. open "input/fork.log"
- the file comes from the command
	system("grep sched_process_fork input/vfs.log > input/fork.log");
2. read a line from "input/fork.log"
3. make a tree which have a relationship showing parent and child
*/
#define DEBUG_buildForkTree 0
void genParentChildTree()
{
	ifstream input_file;

	// file open
    
    // to do: input/fork.log -> #define or use
    // string full_input_file_path = InputFileDirectory + _file_to_open;
	input_file.open("fork.log", ios::in);
	if (!input_file.is_open()) {
		assert(0);
	}

	// get line
	char buf[MAX_LINE];
	while(!input_file.eof())
	{
		input_file.getline(buf, MAX_LINE);
#if DEBUG_buildForkTree
		cout << buf << endl;
#endif
		if(buf[0] == '\0')
			break;

		string buf_string = buf;

		regex pid_regex("pid=([\\d]+).+child_pid=([\\d]+)"); // file=cat, pgoff=11, pgsize=32
		string pid_replace("$1"), child_pid_replace("$2");

		string  str_pid = regex_replace(buf_string, pid_regex, pid_replace, regex_constants::format_no_copy);

		string  str_child_pid = regex_replace(buf_string, pid_regex, child_pid_replace, regex_constants::format_no_copy);

		int pid = stoi(str_pid);
		int child = stoi(str_child_pid);

		pidtree.insert(pair <int, int> (pid, child));

#if DEBUG_buildForkTree
#endif
	}
	cout << "complete building pid tree" << endl;
}


#define DEBUG_testFileNamePath 0
void testFileNamePath()
{
	ifstream input_file;
	// file open
	input_file.open("input/test.log", ios::in);
	if (!input_file.is_open()) {
		assert(0);
	}

	// get line
	char buf[MAX_LINE];
	while(!input_file.eof())
	{
		input_file.getline(buf, MAX_LINE);

#if DEBUG_testFileNamePath
		cout << buf << endl;
#endif

		string filename = buf;

		regex filename_reg ("file=([\\d\\w.:/-]+).*");
		string filename_replace ("$1");

		string  str_filename = regex_replace(filename, filename_reg, filename_replace, regex_constants::format_no_copy);

		cout << str_filename << endl;

		Tokenizer target_str (str_filename, ":");
		string token;
		string str_ret;
		while((token = target_str.next()) != "")
		{
			if(token == "/")
				break;
			str_ret = token + str_ret;
			str_ret = "/" + str_ret;
		}
		cout << str_ret << endl << endl;

//		buildCorrectFilePath(str_filename);
		}
}


void printForkTree()
{
	int m;

	cout << "pid to search child processes!!" << endl;
	while(1) {
		cout << ":"; 	cin >> m;

		// menu exit
		if(m == -1){
			cout << "exit" << endl;
			return;
		}

		// search with the key
		pair<multimap<int,int>::iterator, multimap<int, int>::iterator > iter_pair;
		iter_pair = pidtree.equal_range( m );

		// no results
		if( iter_pair.first == iter_pair.second ) {
			cout << "no results" << endl;
			continue;
		} else { // results exits
			multimap<int,int>::iterator iter;
			for( iter = iter_pair.first ; iter != iter_pair.second ; iter++)
				cout << "pidtree["<< iter->first <<"]: " << iter->second << endl;
		}
	}
}
