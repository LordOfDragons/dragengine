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

#ifndef _IGDEMETAPROPERTYSTORAGESTRING_H_
#define _IGDEMETAPROPERTYSTORAGESTRING_H_

#include "igdeMetaPropertyStorage.h"

#include <dragengine/common/string/decString.h>


/**
 * \brief String meta property storage.
 * 
 * T is the value type and P the meta property type. T has to match the expected value type of P.
 */
template<typename P>
class igdeMetaPropertyStorageString : public igdeMetaPropertyStorage<P>{
private:
	decString pValue;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create string meta property storage. */
	igdeMetaPropertyStorageString(P &property, const deTObjectReference<igdeMetaContext> &context) :
	igdeMetaPropertyStorage<P>(property, context),
	pValue(property.GetDefaultValue()){
	}
	
	/** \brief Create string meta property storage with initial value. */
	igdeMetaPropertyStorageString(P &property, const deTObjectReference<igdeMetaContext> &context, const decString &initialValue) :
	igdeMetaPropertyStorage<P>(property, context),
	pValue(initialValue){
	}
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Get value. */
	inline const decString &GetValue() const{ return pValue; }
	
	/** \brief Set value. */
	void SetValue(const char *value, bool notify = true){
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
	void SetValue(const decString &value, bool notify = true){
		SetValue(value.GetString(), notify);
	}
	
	/** \brief Set value. */
	void SetValue(const igdeMetaPropertyStorageString<P> &value, bool notify = true){
		SetValue(value.GetValue(), notify);
	}
	/*@}*/
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Implicit conversion operator. */
	operator const decString&() const{
		return GetValue();
	}
	
	/** \brief Implicit conversion operator. */
	operator const char*() const{
		return GetValue().GetString();
	}
	
	/** \brief Assignment operator. */
	igdeMetaPropertyStorageString<P> &operator=(const decString &value){
		SetValue(value);
		return *this;
	}
	
	/** \brief Assignment operator. */
	igdeMetaPropertyStorageString<P> &operator=(const char *value){
		SetValue(value);
		return *this;
	}
	
	/** \brief Assignment operator. */
	igdeMetaPropertyStorageString<P> &operator=(const igdeMetaPropertyStorageString<P> &other){
		SetValue(other.GetValue());
		return *this;
	}
	
	/** \brief Value is equal. */
	bool operator==(const decString &value) const{
		return pValue == value;
	}
	
	/** \brief Value is equal. */
	bool operator==(const char *value) const{
		return pValue == value;
	}
	
	/** \brief Access storage type functions. */
	inline decString* operator->(){
		return &pValue;
	}
	
	/** \brief Access storage type functions. */
	inline const decString* operator->() const{
		return &pValue;
	}
	/*@}*/
};

#endif
