/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDEMETAPROPERTYSTORAGEFLOAT_H_
#define _IGDEMETAPROPERTYSTORAGEFLOAT_H_

#include "igdeMetaPropertyStoragePrimitive.h"

#include <dragengine/common/math/decMath.h>


/**
 * \brief Float meta property storage.
 * 
 * T is the value type and P the meta property type. T has to match the expected value type of P.
 */
template<typename P>
class igdeMetaPropertyStorageFloat : public igdeMetaPropertyStorage<P>{
private:
	float pValue = {};
	float pLowerLimit = 0.0f;
	float pUpperLimit = 1.0f;
	float pTickSpacing = 0.1f;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create float meta property storage with initial value. */
	igdeMetaPropertyStorageFloat(P &property, const deTObjectReference<igdeMetaContext> &context, float initialValue) :
	igdeMetaPropertyStorage<P>(property, context),
	pValue(initialValue),
	pLowerLimit(property.GetLowerLimit()),
	pUpperLimit(property.GetUpperLimit()),
	pTickSpacing(property.GetTickSpacing()){
	}
	
	/** \brief Create float meta property storage. */
	igdeMetaPropertyStorageFloat(P &property, const deTObjectReference<igdeMetaContext> &context) :
	igdeMetaPropertyStorageFloat<P>(property, context, property.GetDefaultValue()){
	}
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Get value. */
	inline float GetValue() const{ return pValue; }
	
	/** \brief Set value. */
	void SetValue(float value, bool notify = true){
		const auto &property = igdeMetaPropertyStorage<P>::Property();
		if(property.GetEnableLowerLimit()){
			value = decMath::max(value, pLowerLimit);
		}
		if(property.GetEnableUpperLimit()){
			value = decMath::min(value, pUpperLimit);
		}
		
		if(fabsf(pValue - value) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		pValue = value;
		igdeMetaPropertyStorage<P>::onValueChanged();
		if(notify){
			igdeMetaPropertyStorage<P>::Property().NotifyValueChanged(igdeMetaPropertyStorage<P>::Context());
		}
	}
	
	/** \brief Set value. */
	void SetValue(const igdeMetaPropertyStorageFloat<P> &value, bool notify = true){
		SetValue(value.GetValue(), notify);
	}
	/*@}*/
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Lower limit. */
	inline float GetLowerLimit() const{ return pLowerLimit; }
	
	/** \brief Set lower limit. */
	void SetLowerLimit(float value){
		if(fabsf(pLowerLimit - value) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		pLowerLimit = value;
		igdeMetaPropertyStorage<P>::Property().NotifyLimitsChanged(igdeMetaPropertyStorage<P>::Context());
		
		if(igdeMetaPropertyStorage<P>::Property().GetEnableLowerLimit()){
			SetValue(pValue);
		}
	}
	
	/** \brief Upper limit. */
	inline float GetUpperLimit() const{ return pUpperLimit; }
	
	/** \brief Set upper limit. */
	void SetUpperLimit(float value){
		if(fabsf(pUpperLimit - value) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		pUpperLimit = value;
		igdeMetaPropertyStorage<P>::Property().NotifyLimitsChanged(igdeMetaPropertyStorage<P>::Context());
		
		if(igdeMetaPropertyStorage<P>::Property().GetEnableUpperLimit()){
			SetValue(pValue);
		}
	}
	
	/** \brief Tick spacing. */
	inline float GetTickSpacing() const{ return pTickSpacing; }
	
	/** \brief Set tick spacing. */
	void SetTickSpacing(float value){
		if(fabsf(pTickSpacing - value) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		pTickSpacing = decMath::max(value, 0.0f);
		igdeMetaPropertyStorage<P>::Property().NotifyLimitsChanged(igdeMetaPropertyStorage<P>::Context());
	}
	
	/** \brief Implicit conversion operator. */
	operator float() const{
		return GetValue();
	}
	
	/** \brief Assignment operator. */
	igdeMetaPropertyStorageFloat<P> &operator=(float value){
		SetValue(value);
		return *this;
	}
	
	/** \brief Assignment operator. */
	igdeMetaPropertyStorageFloat<P> &operator=(const igdeMetaPropertyStorageFloat<P> &other){
		SetValue(other.GetValue());
		return *this;
	}
	
	/** \brief Value is equal. */
	bool operator==(float value) const{
		return pValue == value;
	}
	/*@}*/
};

#endif
