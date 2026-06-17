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

#ifndef _IGDEMETAPROPERTYSTORAGEINTEGER_H_
#define _IGDEMETAPROPERTYSTORAGEINTEGER_H_

#include "igdeMetaPropertyStoragePrimitive.h"

#include <dragengine/common/math/decMath.h>


/**
 * \brief Integer meta property storage.
 * 
 * T is the value type and P the meta property type. T has to match the expected value type of P.
 */
template<typename P>
class igdeMetaPropertyStorageInteger : public igdeMetaPropertyStorage<P>{
private:
	int pValue = {};
	int pLowerLimit = 0;
	int pUpperLimit = 100;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create integer meta property storage. */
	igdeMetaPropertyStorageInteger(P &property, const deTObjectReference<igdeMetaContext> &context) :
	igdeMetaPropertyStorage<P>(property, context),
	pValue(property.GetDefaultValue()),
	pLowerLimit(property.GetLowerLimit()),
	pUpperLimit(property.GetUpperLimit()){
	}
	
	/** \brief Create integer meta property storage with initial value. */
	igdeMetaPropertyStorageInteger(P &property, const deTObjectReference<igdeMetaContext> &context, int initialValue) :
	igdeMetaPropertyStorage<P>(property, context),
	pValue(initialValue),
	pLowerLimit(property.GetLowerLimit()),
	pUpperLimit(property.GetUpperLimit()){
	}
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Lower limit. */
	inline int GetLowerLimit() const{ return pLowerLimit; }
	
	/** \brief Set lower limit. */
	void SetLowerLimit(int value){
		if(pLowerLimit == value){
			return;
		}
		
		pLowerLimit = value;
		
		if(igdeMetaPropertyStorage<P>::Property().GetEnableLowerLimit()){
			igdeMetaPropertyStorage<P>::Property().NotifyLimitsChanged(igdeMetaPropertyStorage<P>::Context());
			SetValue(pValue);
		}
	}
	
	/** \brief Upper limit. */
	inline int GetUpperLimit() const{ return pUpperLimit; }
	
	/** \brief Set upper limit. */
	void SetUpperLimit(int value){
		if(pUpperLimit == value){
			return;
		}
		
		pUpperLimit = value;
		
		if(igdeMetaPropertyStorage<P>::Property().GetEnableUpperLimit()){
			igdeMetaPropertyStorage<P>::Property().NotifyLimitsChanged(igdeMetaPropertyStorage<P>::Context());
			SetValue(pValue);
		}
	}
	
	/** \brief Get value. */
	inline int GetValue() const{ return pValue; }
	
	/** \brief Set value. */
	void SetValue(int value, bool notify = true){
		const auto &property = igdeMetaPropertyStorage<P>::Property();
		if(property.GetEnableLowerLimit()){
			value = decMath::max(value, pLowerLimit);
		}
		if(property.GetEnableUpperLimit()){
			value = decMath::min(value, pUpperLimit);
		}
		
		if(pValue == value){
			return;
		}
		
		pValue = value;
		igdeMetaPropertyStorage<P>::OnValueChanged();
		if(notify){
			igdeMetaPropertyStorage<P>::Property().NotifyValueChanged(igdeMetaPropertyStorage<P>::Context());
		}
	}
	/*@}*/
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Implicit conversion operator. */
	operator int() const{
		return GetValue();
	}
	
	/** \brief Assignment operator. */
	igdeMetaPropertyStorageInteger<P> &operator=(int value){
		SetValue(value);
		return *this;
	}
	
	/** \brief Assignment operator. */
	igdeMetaPropertyStorageInteger<P> &operator=(const igdeMetaPropertyStorageInteger<P> &other){
		SetValue(other.GetValue());
		return *this;
	}
	
	/** \brief Value is equal. */
	bool operator==(int value) const{
		return pValue == value;
	}
	/*@}*/
};

#endif
