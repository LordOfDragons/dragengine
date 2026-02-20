/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "deoalImpulseResponse.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>



// Class deoalImpulseResponse
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalImpulseResponse::deoalImpulseResponse() = default;

deoalImpulseResponse::deoalImpulseResponse(const deoalImpulseResponse &response) :
pImpulses(response.pImpulses){
}

deoalImpulseResponse::~deoalImpulseResponse() = default;



// Manegement
///////////////

void deoalImpulseResponse::Add(float time, float low, float medium, float high){
	pImpulses.Add({time, low, medium, high});
}

void deoalImpulseResponse::Add(const deoalImpulseResponse &response){
	pImpulses += response.pImpulses;
}

void deoalImpulseResponse::Insert(float time, float low, float medium, float high){
	pInsert(time, low, medium, high);
}

void deoalImpulseResponse::Insert(const deoalImpulseResponse &response){
	if(response.pImpulses.IsEmpty()){
		return;
	}
	
	pImpulses.EnlargeCapacity(pImpulses.GetCount() + response.pImpulses.GetCount());
	
	response.pImpulses.Visit([&](const sImpulse &src){
		pInsert(src.time, src.low, src.medium, src.high);
	});
}

void deoalImpulseResponse::InsertScaled(const deoalImpulseResponse &response, float scale){
	if(response.pImpulses.IsEmpty()){
		return;
	}
	
	pImpulses.EnlargeCapacity(pImpulses.GetCount() + response.pImpulses.GetCount());
	
	response.pImpulses.Visit([&](const sImpulse &src){
		pInsert(src.time, src.low * scale, src.medium * scale, src.high * scale);
	});
}

void deoalImpulseResponse::Clear(){
	pImpulses.RemoveAll();
}

void deoalImpulseResponse::ReserveSize(int size){
	pImpulses.EnlargeCapacity(size);
}



void deoalImpulseResponse::Square(){
	pImpulses.Visit([](sImpulse &impulse){
		impulse.low *= impulse.low;
		impulse.medium *= impulse.medium;
		impulse.high *= impulse.high;
	});
}

void deoalImpulseResponse::PressureToSPL(){
	pImpulses.Visit([](sImpulse &impulse){
		if(impulse.low > 1e-10f){
			impulse.low = 20.0f * log10f(impulse.low);
			
		}else{
			impulse.low = -200.0f;
		}
		
		if(impulse.medium > 1e-10f){
			impulse.medium = 20.0f * log10f(impulse.medium);
			
		}else{
			impulse.medium = -200.0f;
		}
		
		if(impulse.high > 1e-10f){
			impulse.high = 20.0f * log10f(impulse.high);
			
		}else{
			impulse.high = -200.0f;
		}
	});
}

void deoalImpulseResponse::IntensityToSIL(){
	pImpulses.Visit([](sImpulse &impulse){
		if(impulse.low > 1e-10f){
			impulse.low = 10.0f * log10f(impulse.low);
			
		}else{
			impulse.low = -100.0f;
		}
		
		if(impulse.medium > 1e-10f){
			impulse.medium = 10.0f * log10f(impulse.medium);
			
		}else{
			impulse.medium = -100.0f;
		}
		
		if(impulse.high > 1e-10f){
			impulse.high = 10.0f * log10f(impulse.high);
			
		}else{
			impulse.high = -100.0f;
		}
	});
}

void deoalImpulseResponse::IntensityToPressure(){
	// intensity can be converted into sound power level like this:
	//   spl = 10 * log10(intensity)
	// 
	// sound power level is the same in value as the sound pressure level.
	// with this we can go back to the pressure using this formula:
	//   pressure = pow(2, spl / 6)
	// 
	// this is actually the same as going directly from intensity to pressure like this:
	//   pressure = sqrt(intensity)
	pImpulses.Visit([](sImpulse &impulse){
		impulse.low = sqrtf(impulse.low);
		impulse.medium = sqrtf(impulse.medium);
		impulse.high = sqrtf(impulse.high);
	});
}

void deoalImpulseResponse::PressureToIntensity(){
	// inverse of IntensityToPressure:
	//   pressure = sqrt(intensity)
	//   intensity = pressure * pressure
	// 
	// thus this is the same as Square()
	Square();
}

void deoalImpulseResponse::BackwardIntegrate(){
	if(pImpulses.IsEmpty()){
		return;
	}
	
	// backwards integrate. each impulse is going to the sum of all intensity impulses
	// (squared pressure values) with the same delay time or later. hence the first value is
	// the sum of the intensity of all impulses. the next impulse is the same sum minus the
	// first intensity impulse value and so forth. the last impulse has only the intensity
	// value of itself.
	// 
	// the way this is calculated quickly is to sum values up backwards by progressively adding
	// the result values from the right neighbor until arriving at the first impulse.
	// 
	// eventually all results are converted to dB (sound intensity level) using 10*log10(value)
	int i;
	for(i=pImpulses.GetCount()-2; i>=0; i--){
		const sImpulse &prev = pImpulses[i + 1];
		sImpulse &impulse = pImpulses[i];
		impulse.low += prev.low;
		impulse.medium += prev.medium;
		impulse.high += prev.high;
	}
	
	IntensityToSIL();
}

deoalImpulseResponse::sImpulse deoalImpulseResponse::Slopes() const{
	sImpulse result{};
	if(pImpulses.GetCount() < 2){
		return result;
	}
	
	const sImpulse &impulseFirst = pImpulses.First();
	const sImpulse &impulseLast = pImpulses.Last();
	
	result.time = impulseLast.time - impulseFirst.time;
	if(result.time < FLOAT_SAFE_EPSILON){
		result.time = 0.0f;
		return result;
	}
	
	const float factor = 1.0f / result.time;
	result.low = (impulseLast.low - impulseFirst.low) * factor;
	result.medium = (impulseLast.medium - impulseFirst.medium) * factor;
	result.high = (impulseLast.high - impulseFirst.high) * factor;
	return result;
}

deoalImpulseResponse::sImpulse deoalImpulseResponse::ReverberationTime(float dropDb) const{
	sImpulse result{};
	if(pImpulses.GetCount() < 2 || dropDb < FLOAT_SAFE_EPSILON){
		return result;
	}
	
	// calculate reverberationTime[s] = -dropDb[dB] / slope[dB/s]
	const sImpulse slopes(Slopes());
	dropDb = -dropDb;
	
	result.time = slopes.time;
	if(slopes.low != 0.0f){
		result.low = dropDb / slopes.low;
	}
	if(slopes.medium != 0.0f){
		result.medium = dropDb / slopes.medium;
	}
	if(slopes.high != 0.0f){
		result.high = dropDb / slopes.high;
	}
	
	return result;
}



// Operators
//////////////

deoalImpulseResponse &deoalImpulseResponse::operator=(const deoalImpulseResponse &response){
	Clear();
	Add(response);
	return *this;
}

deoalImpulseResponse &deoalImpulseResponse::operator*=(float factor){
	pImpulses.Visit([&](sImpulse &impulse){
		impulse.low *= factor;
		impulse.medium *= factor;
		impulse.high *= factor;
	});
	return *this;
}



// Private Functions
//////////////////////

void deoalImpulseResponse::pInsert(float time, float low, float medium, float high){
	if(pImpulses.IsEmpty()){
		pImpulses.Add({time, low, medium, high});
		return;
	}
	
	int last = pImpulses.GetCount() - 1;
	int first = 0;
	
	while(first != last){
		const int center = (first + last) / 2;
		if(time < pImpulses[center].time){
			last = center;
			
		}else{
			first = center + 1;
		}
	}
	
	if(time > pImpulses[first].time){
		first++;
	}
	
	pImpulses.Insert({time, low, medium, high}, first);
}
