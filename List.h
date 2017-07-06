//
//  List.h
//  HFSParser
//
//  Created by Dongyun Shin on 3/20/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#ifndef __HFSParser__List__
#define __HFSParser__List__

#include <iostream>
#include <string>

#include "Node.h"
#include "List.h"
#include "Node.h"
#include "ParseEntry.h"
#include "StartFinishTimeInfo.h"

using namespace std;

template <typename T>
class List
{
private:
	Node <T> *head, *tail;
	long size;

	void init()
	{
		head = tail = NULL;
		size = 0;
	}
	
public:
	List()
	{
		init();
	}
	
	~List()
	{
		Node <T> *cur = head;
		while( cur )
		{
			Node <T> *cur_next = cur->next;
			
			delete cur;
			size--;
			
			cur = cur_next;
		}
		head = tail = NULL;
		assert(size == 0); // the destruction should come accross that the size is 0.
	}
	
	long getSize()
	{
		return size;
	}

	
	T* getEntryinFirstNode()
	{
		return head->entry;
	}
	
	T* getEntryinLastNode()
	{
		return tail->entry;
	}
	
	Node<T> *getHead() {
		return head;
	}
	void setHead( Node <T> * _head)
	{
		head = _head;
	}
	Node<T> *getTail() {
		return tail;
	}
	void setTail( Node <T> * _tail)
	{
		tail = _tail;
	}

	int push_back(Node <T> *node )
	{
		// empty
		if( head == NULL)
		{
			head = node;
			tail = node;
			
			size++;
			return 1;
		}
		else
		{
			tail->next = node;
			node->prev = tail;
			node->next = NULL;
			tail = node;
			
			size++;
			return 1;
		}
	}

	int copy_and_push_back(Node <T> *node )
	{
		Node <T> *new_node = new Node <T> (node->entry);

		// empty
		if( head == NULL)
		{
			head = new_node;
			tail = new_node;

			size++;
			return 1;
		}
		else
		{
			tail->next = new_node;
			new_node->prev = tail;
			new_node->next = NULL;
			tail = new_node;

			size++;
			return 1;
		}
	}

	int push_front(Node <T> *node)
	{
		// empty
		if( head == NULL)
		{
			head = node;
			tail = node;
			
			size++;
			return 1;

		}
		else
		{
			head->prev = node;
			node->next = head;
			node->prev = NULL;
			head = node;
			
			size++;
			return 1;
		}
	}

	int copy_and_push_front(Node <T> *node)
	{
		Node <T> *new_node = new Node <T> (node->entry);

		// empty
		if( head == NULL)
		{
			head = new_node;
			tail = new_node;

			size++;
			return 1;

		}
		else
		{
			head->prev = new_node;
			new_node->next = head;
			new_node->prev = NULL;
			head = new_node;

			size++;
			return 1;
		}
	}
	
#pragma mark - not tested yet
	int push_after(Node <T> *after, Node <T> *node)
	{
		if ( head == NULL || after == NULL || node == NULL)
			assert (0);
		
		if (after->next == NULL)
		{
			push_back(node);
		}
		else
		{
			node->prev = after;
			node->next = after->next;
			
			after->next->prev = node;
			after->next = node;
			
			size++;
		}
		return 1;
	}
	
	int pop_back(Node <T> **node)
	{
		if(size == 1)
		{
			*node = tail;
			(*node)->next = NULL;
			(*node)->prev = NULL;
			
			head = tail = NULL;
			
			size--;
			
			return 1;
		}
		else
		{
			if(tail != NULL)
			{
				*node = tail;
				tail = tail->prev;
				tail->next = NULL;
				(*node)->prev = NULL;
				size--;
				return 1;
			}
			else
			{
				*node = NULL;
				return -1;
			}
		}
	}
	
	int increaseSize()
	{
		if(size < 0)
			return -1;
		else
		{
			size++;
			return 1;
		}
	}
	
	int decreaseSize()
	{
		if(size <= 0)
			return -1;
		else
		{
			size--;
			return 1;
		}
	}
		
	/**
	 @param
	 S _key: generic type
	 Node <T> ** _ret:
	 @warning 리스트 안에 중복된 내용이 있을 경우 _key값을 가진 처음 노드가 리턴된다.
	 */
#define RET_SNWK_FOUND 1
#define RET_SNWK_NOT_FOUND -1
	template <typename S>
	int searchNodeWithKey(S _key, Node<T> ** _ret)
	{
		Node <T> *cur = head;
		while( cur != NULL  )
		{
			S cmp_key = cur->entry->getKey();
			if(  cmp_key == _key )
			{
				* _ret = cur;
				return RET_SNWK_FOUND;
			}
			cur = cur->next;
		}
		return RET_SNWK_NOT_FOUND;
	}
	
	// 해당 리스트에서 _from에서 _to 까지 삭제한다.
	int removeFromTo(Node <T> *_from, Node <T> *_to)
	{
		Node <T> *temp = NULL;
		
		Node <T> *iter = _from;
		while(iter != _to)
		{
			if(iter == NULL)
			{
				return -1;
			}
			
			extractNode( iter, &temp );
			delete iter;
			
			iter = temp;
		}
		
		// delete _to Node
		extractNode( iter, &temp );
		delete iter;
		
		return 1;
	}
	
	/**
	 @brief
	     _node (to be extracted)
	       !
	 ----O----O----O----O----O----O----
	               |
	             _new_pos
	 */
	int extractNode(Node <T> *_node, Node <T> ** _new_pos )
	{
		// usually can't reach
		if( size == 0)
			return -1;
		else if( size == 1)
		{
			head = NULL;
			tail = NULL;
			
			_node->prev = _node->next = NULL;
			
			size--;
			
			if( _new_pos != NULL)
				*_new_pos = NULL;
			
			return 2;
		}
		else if( size > 1)
		{
			if( _node == head )
			{
				head = head->next;
				head->prev = NULL;
				
				_node->next = NULL;
				
				if( _new_pos != NULL)
					*_new_pos = head;
			}
			else if (_node == tail)
			{
				tail = tail->prev;
				tail->next = NULL;
				
				_node->prev = NULL;
				
				if( _new_pos != NULL)
					*_new_pos = tail;
			}
			else
			{
				if( _new_pos != NULL)
					*_new_pos = _node->next;
				
				_node->prev->next = _node->next;
				_node->next->prev = _node->prev;
				
				_node->prev = NULL;
				_node->next = NULL;
			}
			
			size--;
			return 1;
		}
		else
		{
			assert(0);
		}
	}
	
	int removeNode(Node <T> *_node )
	{
		// usually can't reach
		if( size == 0)
			return -1;
		
		else if( size == 1)
		{
			head = NULL;
			tail = NULL;
			
			
			_node->prev = _node->next = NULL;
			
			size--;
			
			delete _node;
			
			return 2;
		}
		else if( size > 1)
		{
			if( _node == head )
			{
				head = head->next;
				head->prev = NULL;
				
				_node->next = NULL;
			}
			else if (_node == tail)
			{
				tail = tail->prev;
				tail->next = NULL;
				
				_node->prev = NULL;
				
			}
			else
			{
				_node->prev->next = _node->next;
				_node->next->prev = _node->prev;
				
				_node->prev = NULL;
				_node->next = NULL;
				
			}
			
			delete _node;
			size--;
			
			return 1;
		}
		else
		{
			return -1;
		}
	}
	
	void moveNodeToOtherList( Node <T> *_node, List <T> *_new_list)
	{
		extractNode(_node, NULL);
		
		_new_list->push_back(_node);
	}

	
#pragma mark - for debugging
	void printList()
	{
		Node <T> *cur = head;
		
		while(cur != NULL)
		{
			cur->entry->print();
			cur = cur->next;
		}
	}
	
	void printList(string _output_file)
	{
		ofstream output;
		output.open(_output_file.c_str(),ios::out);
		
		Node<T> *cur = head;
		
		output << "list's size: " << size << endl << endl;;
		int i = 1;
		while( cur != NULL)
		{
			output << "index #" << i++ << endl;
			cur->entry->print(output);
			output << endl;
			
			cur = cur->next;
		}
		output.close();
	}
	
	void printList(ofstream & _out)
	{
		Node<T> *cur = head;
		
		_out << "list's size: " << size << endl << endl;;
		int i = 1;
		while( cur != NULL)
		{
			_out << "index #" << i++ << endl;
			cur->entry->print(_out);
			_out << endl;
			
			cur = cur->next;
		}

	}

};


#endif /* defined(__HFSParser__List__) */
