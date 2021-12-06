#ifndef _DLLIST_HPP_
#define _DLLIST_HPP_

/*********************************************************************************
 *								Doubly linked list
 *
 *                first                current               last
 *            +------------+       +------------+       +------------+
 *            |        next| ----> |        next| ----> |        next| ----> NULL
 * NULL <---- |prev        | <---- |prev        | <---- |prev        |
 *            +------------+       +------------+       +------------+
 *
 *********************************************************************************/               

#include "config.hpp" 
#include "date_time.hpp"

namespace dk{
 
#if defined(intops)  
typedef int DataType;
#elif defined(timeops)
typedef dk::time DataType;
#endif

struct dllistelem
{
	dk::dllistelem *next_;
	dk::dllistelem *prev_;
	dk::DataType data_;
};

struct dllist
{
	dk::dllistelem *first_;
	dk::dllistelem *current_;
	dk::dllistelem *last_;
};

dk::dllist *create(); 
bool addlast(dk::dllist *,const dk::DataType&);
bool addfirst(dk::dllist *,const dk::DataType&);
bool insertnext(dk::dllist *,const dk::DataType&);
bool insertprev(dk::dllist *,const dk::DataType&);
bool deletenext(dk::dllist *);
bool deleteprev(dk::dllist *);

bool movenext(dk::dllist *);
bool moveprev(dk::dllist *);
bool getdata(dk::dllist *,DataType &data);
bool isempty(dk::dllist *);

bool clear(dk::dllist *);
#if defined(intops)
bool printcur(dk::dllist *list);
bool printlist(dk::dllist *list);
#endif

bool Apply(dk::dllist *list,void (*operation)(dk::DataType&));

}
#endif//_DLLIST_HPP_
