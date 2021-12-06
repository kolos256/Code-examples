#ifndef _ARRAY_HPP_
#define _ARRAY_HPP_

#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <mem.h>
#include <algorithm>

template <typename T>
class Arr
{
	private:
		T *ArrPtr_;
		size_t Size_;
	public:
		Arr(size_t size);	
		
};

template <typename T>
Arr<T>::Arr(size_t size)
{   
	Size_= size;
    ArrPtr_ = new T[size]; 
}

#if 0
namespace dk{
	typedef uint16_t ArrType;
	
	dk::ArrType *Create(uint32_t Size);
	dk::ArrType *Resize(dk::ArrType array[],uint32_t Size);
	void Remove(dk::ArrType array[]);
	
	void FillArr(dk::ArrType array[],uint32_t elems,uint32_t strt_pos=0,dk::ArrType strt_val=0,int32_t inc_val=1);
	void ShowArr(dk::ArrType array[],uint32_t elems, uint32_t strt_pos=0,uint32_t linelength=20);
}
#endif

#endif//_ARRAY_HPP_
