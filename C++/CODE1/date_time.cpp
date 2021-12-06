#include "date_time.hpp"

/****************************************
 Date functions
****************************************/
 
uint8_t dk::MonthDays(uint8_t month, uint16_t year)
{
    const uint32_t mask1 = 0xAD5;
    if((month>12)||(month==0))
    {
    	return 0;
	}
	else
	{
		return  (month == 2)?28+IsLeapYear(year):30+(0!=(mask1&(1<<(month-1))));
	}
}

bool dk::IsLeapYear(uint16_t year)
{
    return !(year%4) && year%100 || !(year%400);
}

uint8_t dk::CheckDate(uint8_t day,uint8_t month,uint16_t year)
{
	uint8_t State=0;
	if((day>31)||(day==0)){
		State|=1;
	}
	if((month>dk::MonthDays(month,year))||(dk::MonthDays(month,year)==0))
	{
		State|=2;
	}
	
	return State;
}

uint8_t dk::CheckDate(dk::date &DateParam)
{
	uint8_t State=0;
	if((DateParam.day>31)||(DateParam.day==0)){
		State|=1;
	}
	if((DateParam.month>dk::MonthDays(DateParam.month,DateParam.year))||(dk::MonthDays(DateParam.month,DateParam.year)==0))
	{
		State|=2;
	}
	return State;
}



uint8_t dk::Set(dk::date &DateParam,dk::date &DateValue)
{
	uint8_t State=0;
	if((State=CheckDate(DateValue))==0)
	{
		memcpy(&DateParam,&DateValue,sizeof(dk::date));
	}
	return State;
}

uint8_t dk::Set(dk::date &DateParam,uint8_t day,uint8_t month,uint16_t year)
{
	uint8_t State=0;
	if((State=CheckDate(day,month,year))==0)
	{
		DateParam.day=day;
		DateParam.month=month;
		DateParam.year=year;
	}
	return State;
}

bool dk::Equal(dk::date &DateParam,uint8_t day,uint8_t month,uint16_t year)
{
	return ((DateParam.day==day)&&(DateParam.month==month)&&(DateParam.year==year));
}

bool dk::Equal(dk::date &Date1,dk::date &Date2)
{
	return ((Date1.day==Date2.day)&&(Date1.month==Date2.month)&&(Date1.year==Date2.year));
}

bool dk::Less(dk::date &DateParam,uint8_t day,uint8_t month,uint16_t year)
{
	if(DateParam.year==year){
		if(DateParam.month==month){
			return (DateParam.day<day);
		}else{
			return (DateParam.month<month);
		}
	}else{
		return (DateParam.year<year);
	}	 
}

bool dk::Less(dk::date &Date1,dk::date &Date2)
{
	if(Date1.year==Date2.year){
		if(Date1.month==Date2.month){
			return (Date1.day<Date2.day);
		}else{
			return (Date1.month<Date2.month);
		}
	}else{
		return (Date1.year<Date2.year);
	}	 
}

bool dk::Grater(dk::date &DateParam,uint8_t day,uint8_t month,uint16_t year)
{
	if(DateParam.year==year){
		if(DateParam.month==month){
			return (DateParam.day>day);
		}else{
			return (DateParam.month>month);
		}
	}else{
		return (DateParam.year>year);
	}	 
}

bool dk::Grater(dk::date &Date1,dk::date &Date2)
{
	if(Date1.year==Date2.year){
		if(Date1.month==Date2.month){
			return (Date1.day>Date2.day);
		}else{
			return (Date1.month>Date2.month);
		}
	}else{
		return (Date1.year>Date2.year);
	}	 
}


void dk::Print(dk::date &DateParam)
{
	using namespace std;
	cout.fill('0');
	cout<<setw(2)
				 <<static_cast<int>(DateParam.day)<<"."
	             <<static_cast<int>(DateParam.month)<<"."
				 <<setw(4)<<DateParam.year;

}

#if 1
uint8_t dk::Read(dk::date &DateParam)
{
	using namespace std;
	int32_t tmp1;
	cout<<"input Day: ";
	cin>>tmp1;
	DateParam.day=tmp1;
	cout<<"input Mounth: ";
	cin>>tmp1;
	DateParam.month=tmp1;
	cout<<"input Year: ";
	cin>>tmp1;
	DateParam.year=tmp1;
	return CheckDate(DateParam);
}
#endif

#if 0
uint8_t dk::Read(dk::date &DateParam)
{
	using namespace std;
	int32_t tmp1,tmp2,tmp3;
	char c;
	cout<<"input Date in format DD.MM.YYY: ";
	cin >> tmp1 &&
	cin >> c && c == '.' &&
	cin >> tmp2 &&
	cin >> c && c == '.' &&
	cin >> tmp3;
	if(!cin){cin.clear();}
	while(cin.get() != '\n');
	
	DateParam.day=tmp1;
	DateParam.month=tmp2;
	DateParam.year=tmp3;
	return CheckDate(DateParam);
}
#endif

/****************************************
 Time functions
****************************************/
uint8_t dk::Check(uint8_t hour,uint8_t min,uint8_t sec)
{
	uint8_t State=0;
	if(sec>60){
		State|=1;
	}
	if(min>60){
		State|=2;
	}
	if(hour>24){
		State|=4;
	}
	return State;
}
uint8_t dk::Check(dk::time &TimeParam)
{
	uint8_t State=0;
	if(TimeParam.sec>60){
		State|=1;
	}
	if(TimeParam.min>60){
		State|=2;
	}
	if(TimeParam.hour>24){
		State|=4;
	}
	return State;
}

uint8_t dk::Set(dk::time &TimeParam,uint8_t hour,uint8_t min,uint8_t sec=0)
{
		uint8_t State;
		if((State=Check(hour,min,sec))==0)
		{
			TimeParam.sec  = sec;
			TimeParam.min  = min;
			TimeParam.hour = hour;	
		}
		return State;
}

uint8_t dk::Set(dk::time &TimeParam,dk::time &TimeValue)
{
	uint8_t State;
	if((State=Check(TimeValue))==0)
	{
		memcpy(&TimeParam,&TimeValue,sizeof(dk::time));
	}
	return State;
}

bool dk::Equal(dk::time &TimeParam,uint8_t hour,uint8_t min,uint8_t sec=0)
{
	return ((TimeParam.hour==hour)&&(TimeParam.min==min)&&(TimeParam.sec==sec));
}

bool dk::Equal(dk::time &Time1,dk::time &Time2)
{
	return ((Time1.hour==Time2.hour)&&(Time1.min==Time2.min)&&(Time1.sec==Time2.sec));
}

bool dk::Less(dk::time &TimeParam,uint8_t hour,uint8_t min,uint8_t sec=0)
{
	if(TimeParam.hour==hour){
		if(TimeParam.min==min){
			return (TimeParam.sec<sec);
		}else{
			return (TimeParam.min<min);
		}
	}else{
		return (TimeParam.hour<hour);
	}	 
}

bool dk::Less(dk::time &Time1,dk::time &Time2)
{
	if(Time1.hour==Time2.hour){
		if(Time1.min==Time2.min){
			return (Time1.sec<Time2.sec);
		}else{
			return (Time1.min<Time2.min);
		}
	}else{
		return (Time1.hour<Time2.hour);
	}	 
}

bool dk::Grater(dk::time &TimeParam,uint8_t hour,uint8_t min,uint8_t sec=0)
{
	if(TimeParam.hour==hour){
		if(TimeParam.min==min){
			return (TimeParam.sec>sec);
		}else{
			return (TimeParam.min>min);
		}
	}else{
		return (TimeParam.hour>hour);
	}		 
}

bool dk::Grater(dk::time &Time1,dk::time &Time2)
{
	if(Time1.hour==Time2.hour){
		if(Time1.min==Time2.min){
			return (Time1.sec>Time2.sec);
		}else{
			return (Time1.min>Time2.min);
		}
	}else{
		return (Time1.hour>Time2.hour);
	}		 
}


void dk::Print(dk::time &TimeParam)
{
	using namespace std;
	cout.fill('0');
	cout<<setw(2)
				 <<static_cast<int>(TimeParam.hour)<<":"
	             <<static_cast<int>(TimeParam.min)<<":"
				 <<static_cast<int>(TimeParam.sec);

}

uint8_t dk::Read(dk::time &TimeParam)
{
	using namespace std;
	int32_t tmp1;
	cout<<"input Hour: ";
	cin>>tmp1;
	TimeParam.hour=tmp1;
	cout<<"input Minut: ";
	cin>>tmp1;
	TimeParam.min=tmp1;
	cout<<"input Seconds: ";
	cin>>tmp1;
	TimeParam.sec=tmp1;
	return Check(TimeParam);
}


