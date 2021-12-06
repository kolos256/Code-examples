#include <iostream>
#include "array.hpp"
#include "dllist.hpp"
#include "date_time.hpp"
#include "config.hpp"

using namespace std;
#if 0
#if defined(intops)
void Print(dk::DataType& data)
{
	cout<<data<<'|';
}
#elif defined(timeops)
void Print(dk::DataType& data)
{
	using namespace std;
	cout.fill('0');
	cout<<setw(2)
				 <<static_cast<int>(data.hour)<<":"
	             <<static_cast<int>(data.min)<<":"
				 <<static_cast<int>(data.sec)<<'|';

}

#endif
#endif


int main(int argc, char** argv) {
	int data;
	#if 0
	dk::time time1;
	
	dk::dllist *list1=NULL; 
	//dk::printcur(list1);
	list1=dk::create();
	//dk::printcur(list1);
	#if defined(intops)
	cout<<"addlast(list1,10)"<<endl;
	dk::addlast(list1,10);
	dk::printlist(list1);

	cout<<"addlast(list1,5)"<<endl;
	dk::addlast(list1,5);
	dk::printlist(list1);
	cout<<"addfirst(list1,20)"<<endl;
	dk::addfirst(list1,20);
	dk::printlist(list1);
	cout<<"addfirst(list1,40)"<<endl;
	dk::addfirst(list1,40);
	dk::printlist(list1);

	cout<<"insertnext(list1,55)"<<endl;
	dk::insertnext(list1,55);
	printlist(list1);
	
	cout<<"insertprev(list1,33)"<<endl;
	dk::insertprev(list1,33);
	printlist(list1);
	
	cout<<"deleteprev(list1)"<<endl;
	dk::deleteprev(list1);
	dk::printlist(list1);
	cout<<"deletenext(list1)"<<endl;
	dk::deletenext(list1);
	dk::printlist(list1);
	
	cout<<"Apply(list1,PrintOp);"<<endl;
	dk::Apply(list1,Print);cout<<endl;
	cout<<"clear(list1)"<<endl;
	dk::clear(list1);
	dk::printlist(list1);
	#elif defined(timeops)
	dk::Set(time1,10,20,30);
	cout<<"addlast(10:20:30)"<<endl;
	dk::addlast(list1,time1);
	dk::Apply(list1,Print);
	cout<<endl;
	dk::Set(time1,20,30,40);
	cout<<"addlast(20:30:40)"<<endl;
	dk::addlast(list1,time1);
	dk::Apply(list1,Print);
	cout<<endl;

	#endif
	#endif
	return 0;
}
