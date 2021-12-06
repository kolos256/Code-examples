#ifndef _DATE_TIME_HPP_
#define _DATE_TIME_HPP_

#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <mem.h>
#include <algorithm>

namespace dk{

struct date{
	uint8_t day;
    uint8_t month;
    uint16_t year;
};

struct time{
	uint8_t sec;
    uint8_t min;
    uint8_t hour;
};

bool IsLeapYear(uint16_t year);
uint8_t MonthDays(uint8_t month, uint16_t year);

uint8_t CheckDate(uint8_t day,uint8_t month,uint16_t year);
uint8_t CheckDate(dk::date &DateParam);
void Print(dk::date &DateParam);
uint8_t Set(dk::date &DateParam,dk::date &DateValue);
uint8_t Set(dk::date &DateParam,uint8_t day,uint8_t month,uint16_t year);
uint8_t Read(dk::date &DateParam);
bool Equal(dk::date &DateParam,uint8_t day,uint8_t month,uint16_t year);
bool Equal(dk::date &Date1,dk::date &Date2);
bool Less(dk::date &DateParam,uint8_t day,uint8_t month,uint16_t year);
bool Less(dk::date &Date1,dk::date &Date2);
bool Grater(dk::date &DateParam,uint8_t day,uint8_t month,uint16_t year);
bool Grater(dk::date &Date1,dk::date &Date2);

uint8_t Check(uint8_t hour,uint8_t min,uint8_t sec);
uint8_t Check(dk::time &TimeParam);
void Print(dk::time &TimeParam);
uint8_t Set(dk::time &TimeParam,uint8_t sec,uint8_t min,uint8_t hour);
uint8_t Set(dk::time &TimeParam,dk::time &TimeValue);
uint8_t Read(dk::time &TimeParam);
bool Equal(dk::time &TimeParam,uint8_t hour,uint8_t min,uint8_t sec);
bool Equal(dk::time &Time1,dk::time &Time2);
bool Less(dk::time &TimeParam,uint8_t hour,uint8_t min,uint8_t sec);
bool Less(dk::time &Time1,dk::time &Time2);
bool Grater(dk::time &TimeParam,uint8_t hour,uint8_t min,uint8_t sec);
bool Grater(dk::time &Time1,dk::time &Time2);

}
#endif//_DATE_TIME_HPP_
