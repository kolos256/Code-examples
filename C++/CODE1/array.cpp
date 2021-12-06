#include "array.hpp"


#if 0
dk::ArrType *dk::Create(uint32_t Size)
{
	return new dk::ArrType[Size];	
}
dk::ArrType *dk::Resize(dk::ArrType array[],uint32_t NewSize)
{
	using namespace std;
	dk::ArrType *tmp1=NULL;
	uint32_t tmp2;
	#if defined(DBG)
	cout<<"array_size: "<<(_msize(array)/sizeof(dk::ArrType));
	#endif
	if(NewSize!=0)
	{	
		tmp1 = new dk::ArrType[NewSize];
		if(tmp1==NULL)
		{
			return NULL;
		}
		if(array!=NULL)
		{	
			for(tmp2=0;tmp2<(_msize(array)/sizeof(dk::ArrType));tmp2++)
			{
				tmp1[tmp2]=array[tmp2];
			}
		}
	}
	delete[] array;
	return tmp1;
}
void dk::Remove(dk::ArrType array[])
{
	delete[] array;
}

void dk::FillArr(dk::ArrType array[],uint32_t elems, uint32_t strt_pos,dk::ArrType strt_val,int32_t inc_val)
{
	uint32_t tmp1;
	for(tmp1=strt_pos;tmp1<(elems+strt_pos);tmp1++,strt_val+=inc_val)
	{
		array[tmp1]=strt_val;
	}
}

void dk::ShowArr(dk::ArrType array[],uint32_t elems, uint32_t strt_pos,uint32_t linelength)
{
	using namespace std;
	uint32_t tmp1,tmp2=strt_pos;
	#if defined(DBG)
	cout<<"strt_pos: "<<strt_pos<<" elems: "<<elems<<" array: "<<array<<endl;
	#endif
	if(array==NULL)
	{
		cout<<"null array"<<endl;
		cout.flush();
		return;
	}
	while(elems>0)
	{
		
		tmp1=(elems>linelength)?linelength:elems;
		elems-=tmp1;
		tmp1+=tmp2;
		while(tmp2<tmp1)
		{
			cout<<setw(3)<<array[tmp2++]<<"|";
		}
		cout<<endl;
	}
	cout.flush();

}
#endif
	
