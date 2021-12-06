#ifndef _ADC_H_
#define _ADC_H_
#include "CORE\CORE.h"
#ifdef USECONF
#include "CONF.h"
#endif//USECONF

#if (defined(ADC1V1)||defined(ADC2V1))
#include "ADCV1.h"
#endif//(defined(ADC1V2)||defined(ADC2V2))

#if (defined(ADC1V2)||defined(ADC2V2))
#include "ADCV2.h"
#endif//(defined(ADC1V2)||defined(ADC2V2))

#if (defined(ADC1V3)||defined(ADC2V3))
#include "ADCV3.h"
#endif//(defined(ADC1V2)||defined(ADC2V2))

#endif//_ADC_H_