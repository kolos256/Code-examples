#include <iostream>
#include "dllist.hpp"



dk::dllist *dk::create()
{
	dk::dllist *list=NULL;
	list=new dk::dllist;
	if(list!=NULL)
	{
		list->first_=list->current_=list->last_=NULL;
	}
	return list;
}


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

bool dk::addfirst(dk::dllist *list,const dk::DataType &data)
{
	dk::dllistelem *newelem=NULL;
	if(list==NULL)
	{
		return true;
	}
	newelem=new dk::dllistelem;
	if(newelem==NULL)
	{
		return true;
	}
	newelem->prev_=NULL;
	newelem->data_=data;
	if(list->current_==NULL)
	{
		newelem->next_=NULL;
		list->first_=list->last_=list->current_= newelem;
	}
	else
	{
		newelem->next_=list->first_;
		list->first_->prev_=newelem;
		list->first_=newelem;
	}
	return false;
}


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

bool dk::addlast(dk::dllist *list,const dk::DataType &data)
{
	dk::dllistelem *newelem=NULL;
	if(list==NULL)
	{
		return true;
	}
	newelem=new dk::dllistelem;
	if(newelem==NULL)
	{
		return true;
	}
	newelem->next_=NULL;
	newelem->data_=data;
	if(list->current_==NULL)
	{
		newelem->prev_=NULL;
		list->last_=list->first_=list->current_= newelem;
	}
	else
	{
		newelem->prev_=list->last_;
		list->last_->next_=newelem;
		list->last_=newelem;
	}
	return false;
}

bool dk::getdata(dk::dllist *list,dk::DataType &data)
{
	if(list==NULL){return true;}
	if(list->current_==NULL){return true;}
	data=list->current_->data_;
	return false;
}

bool dk::movenext(dk::dllist *list)
{
	if(list==NULL){return true;}
	if(list->current_==NULL){return true;}
	if(list->current_->next_==NULL){return true;}
	list->current_=list->current_->next_;
	return false;
}

bool dk::moveprev(dk::dllist *list)
{
	if(list==NULL){return true;}
	if(list->current_==NULL){return true;}
	if(list->current_->prev_==NULL){return true;}
	list->current_=list->current_->prev_;
	return false;
}


bool dk::insertnext(dk::dllist *list,const dk::DataType& data)
{
	dk::dllistelem *newelem=NULL;
	if(list==NULL)
	{
		return true;
	}
	newelem=new dk::dllistelem;
	if(newelem==NULL)
	{
		return true;
	}
	newelem->data_=data;
	if(list->current_==NULL)
	{
		
		newelem->next_=newelem->prev_=NULL;
		list->last_=list->first_=list->current_= newelem;
	}
	else
	{
		if(list->last_==list->current_)
		{
			list->last_=newelem;
		}
		newelem->next_=list->current_->next_;
		if(list->current_->next_!=NULL)
		{
			list->current_->next_->prev_=newelem;
		}
				
		newelem->prev_=list->current_;
		list->current_->next_=newelem;
	}
	return false;
}


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

bool dk::insertprev(dk::dllist *list,const dk::DataType& data)
{
	using namespace std;
	dk::dllistelem *newelem=NULL;
	if(list==NULL)
	{
		return true;
	}
	newelem=new dk::dllistelem;
	if(newelem==NULL)
	{
		return true;
	}
	newelem->data_=data;
	if(list->current_==NULL)
	{
		newelem->prev_=newelem->next_=NULL;
		list->last_=list->first_=list->current_= newelem;
	}
	else
	{
		#if 0
		cout<<"st2:"<<endl;
		cout<<"list->current_: "<<list->current_<<endl;
		cout<<"list->first_: "<<list->first_<<endl;
		cout<<"list->last_: "<<list->last_<<endl;
		cout<<"newelem: "<<newelem<<endl;
		#endif
		
		if(list->first_==list->current_)
		{
			list->first_=newelem;
		}
		newelem->next_=list->current_;
		if(list->current_->prev_!=NULL)
		{
			list->current_->prev_->next_=newelem;
		}
		newelem->prev_=list->current_->prev_;
		list->current_->prev_=newelem;
		#if 0
		cout<<"st3:"<<endl;
		cout<<"list->current_:"<<list->current_<<endl;
		cout<<"list->first_:"<<list->first_<<endl;
		cout<<"list->last_:"<<list->last_<<endl;
		cout<<"newelem: "<<newelem<<endl;
		#endif
		
	}
	return false;
}

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

bool dk::deletenext(dk::dllist *list)
{
	dk::dllistelem *tmp;
	if(list==NULL){return true;}
	if(list->current_==NULL){return true;}
	if(list->current_->next_==NULL)
	{
		return true;
	}
	else
	{
		tmp=list->current_->next_;
		if(list->last_==list->current_->next_)
		{
			list->first_=list->current_;
		}
		list->current_->next_=tmp->next_;
		if(tmp->next_!=NULL)
		{
			list->current_->next_->prev_=list->current_;
		}
		delete tmp;
		return false;
	}
	
}

bool dk::deleteprev(dk::dllist *list)
{
	dk::dllistelem *tmp;
	if(list==NULL){return true;}
	if(list->current_==NULL){return true;}
	if(list->current_->prev_==NULL)
	{
		return true;
	}
	else
	{
		tmp=list->current_->prev_;
		if(list->first_==list->current_->prev_)
		{
			list->first_=list->current_;
		}
		list->current_->prev_=tmp->prev_;
		if(tmp->prev_!=NULL)
		{
			list->current_->prev_->next_=list->current_;
		}
		delete tmp;
		return false;
	}
}

bool dk::isempty(dk::dllist *list)
{
	if(list==NULL){return true;}
	if(list->current_==NULL){return true;}
	return false;
}

#if defined(intops)
bool dk::printcur(dk::dllist *list)
{
	using namespace std;
	if(list==NULL){
		cout<<"null list pointer!!!"<<endl;
		return true;
	}
	if(list->current_==NULL){
		cout<<"empty list!!!"<<endl;
		return true;
	}
	cout<<"cur elem: "<<list->current_->data_<<endl;
	return false;
}

bool dk::printlist(dk::dllist *list)
{
	using namespace std;
	dk::dllistelem *curelem;
	if(list==NULL){
		cout<<"null list pointer!!!"<<endl;
		return true;
	}
	if(list->current_==NULL){
		cout<<"empty list!!!"<<endl;
		return true;
	}
	cout<<'|';
	curelem=list->first_;
	while(curelem!=NULL)
	{
		if(curelem==list->current_)
		{
			cout<<'*';	
		}
		cout<<curelem->data_<<'|';
		curelem=curelem->next_;
	}
	cout<<endl;
	return false;
}
#endif

bool dk::clear(dk::dllist *list)
{
	dk::dllistelem *curelem,*tmp;
	if(list==NULL){
		return true;
	}
	if(list->current_==NULL){
		return true;
	}
	curelem=list->first_;
	while(curelem!=NULL)
	{
		tmp=curelem;
		curelem=curelem->next_;
		delete tmp;
	}
	list->first_=list->last_=list->current_=NULL;
	return false;
}

bool dk::Apply(dk::dllist *list,void (*operation)(dk::DataType&))
{
	dk::dllistelem *curelem;
	if(list==NULL){
		return true;
	}
	if(list->current_==NULL){
		return true;
	}
	curelem=list->first_;
	while(curelem!=NULL)
	{
		operation(curelem->data_);
		curelem=curelem->next_;
	}
	return false;
}
