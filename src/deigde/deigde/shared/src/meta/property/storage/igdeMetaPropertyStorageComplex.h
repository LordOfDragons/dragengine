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

#ifndef _IGDEMETAPROPERTYSTORAGECOMPLEX_H_
#define _IGDEMETAPROPERTYSTORAGECOMPLEX_H_

#include "igdeMetaPropertyStorage.h"


/**
 * \brief Complex meta property storage.
 * 
 * T is the value type and P the meta property type. T has to match the expected value type of P.
 */
template<typename T, typename P>
class igdeMetaPropertyStorageComplex : public igdeMetaPropertyStorage<P>{
private:
	T pValue = {};
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create complex meta property storage. */
	igdeMetaPropertyStorageComplex(P &property, const deTObjectReference<igdeMetaContext> &context) :
	igdeMetaPropertyStorage<P>(property, context),
	pValue(property.GetDefaultValue()){
	}
	
	/** \brief Create complex meta property storage with initial value. */
	igdeMetaPropertyStorageComplex(P &property, const deTObjectReference<igdeMetaContext> &context, const T &initialValue) :
	igdeMetaPropertyStorage<P>(property, context),
	pValue(initialValue){
	}
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Get value. */
	inline const T &GetValue() const{ return pValue; }
	
	/** \brief Set value. */
	void SetValue(const T &value, bool notify = true){
		if(pValue == value){
			return;
		}
		
		pValue = value;
		igdeMetaPropertyStorage<P>::OnValueChanged();
		if(notify){
			igdeMetaPropertyStorage<P>::Property().NotifyValueChanged(igdeMetaPropertyStorage<P>::Context());
		}
	}
	
	/** \brief Set value. */
	void SetValue(const igdeMetaPropertyStorageComplex<T, P> &value, bool notify = true){
		SetValue(value.GetValue(), notify);
	}
	/*@}*/
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Implicit conversion operator. */
	operator const T&() const{
		return GetValue();
	}
	
	/** \brief Assignment operator. */
	igdeMetaPropertyStorageComplex<T, P> &operator=(const T &value){
		SetValue(value);
		return *this;
	}
	
	/** \brief Assignment operator. */
	igdeMetaPropertyStorageComplex<T, P> &operator=(const igdeMetaPropertyStorageComplex<T, P> &other){
		SetValue(other.GetValue());
		return *this;
	}
	
	/** \brief Value is equal. */
	bool operator==(const T &value) const{
		return pValue == value;
	}
	
	/** \brief Access storage type functions. */
	inline T* operator->(){
		return &pValue;
	}
	
	/** \brief Access storage type functions. */
	inline const T* operator->() const{
		return &pValue;
	}
	/*@}*/
};

#endif
