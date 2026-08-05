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

#ifndef _IGDEMETAPROPERTYSTORAGESTRINGLIST_H_
#define _IGDEMETAPROPERTYSTORAGESTRINGLIST_H_

#include "igdeMetaPropertyStorage.h"

#include <dragengine/common/string/decStringList.h>


/**
 * \brief String list meta property storage.
 *
 * T is the value type and P the meta property type. T has to match the expected value type of P.
 */
template<typename P>
class igdeMetaPropertyStorageStringList : public igdeMetaPropertyStorage<P>{
private:
	decStringList pValue, pSelection;
	typename P::StringRef pActive;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create string list meta property storage. */
	igdeMetaPropertyStorageStringList(P &property, const deTObjectReference<igdeMetaContext> &context) :
	igdeMetaPropertyStorage<P>(property, context){
	}
	
	/** \brief Create string list meta property storage with initial value. */
	igdeMetaPropertyStorageStringList(P &property, const deTObjectReference<igdeMetaContext> &context,
		const decStringList &initialValue) :
	igdeMetaPropertyStorage<P>(property, context),
	pValue(initialValue){
	}
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Get value. */
	inline const decStringList &GetValue() const{ return pValue; }
	
	/** \brief Set value. */
	void SetValue(const decStringList &value, bool notify = true){
		if(pValue == value){
			return;
		}
		
		pValue = value;
		this->onValueChanged();
		if(notify){
			this->Property().NotifyValueChanged(this->Context());
		}
	}
	
	/** \brief Set value. */
	void SetValue(const igdeMetaPropertyStorageStringList<P> &value, bool notify = true){
		SetValue(value.GetValue(), notify);
	}
	
	/** \brief Get selection. */
	inline const decStringList &GetSelection() const{ return pSelection; }
	
	/** \brief Set selection. */
	void SetSelection(const decStringList &selection, bool notify = true){
		if(pSelection == selection){
			return;
		}
		
		pSelection = selection;
		if(notify){
			this->Property().NotifySelectionChanged(this->Context());
		}
	}
	
	/** \brief Get active string. */
	inline const typename P::StringRef &GetActive() const{ return pActive; }
	
	/** \brief Set active string. */
	void SetActive(const typename P::StringRef &active, bool notify = true){
		if(pActive == active){
			return;
		}
		
		pActive = active;
		if(notify){
			this->Property().NotifyActiveChanged(this->Context());
		}
	}
	/*@}*/
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Implicit conversion operator. */
	operator const decStringList&() const{
		return GetValue();
	}
	
	/** \brief Assignment operator. */
	igdeMetaPropertyStorageStringList<P> &operator=(const decStringList &value){
		SetValue(value);
		return *this;
	}
	
	/** \brief Assignment operator. */
	igdeMetaPropertyStorageStringList<P> &operator=(const igdeMetaPropertyStorageStringList<P> &other){
		SetValue(other.GetValue());
		return *this;
	}
	
	/** \brief Value is equal. */
	bool operator==(const decStringList &value) const{
		return pValue == value;
	}
	
	/** \brief Access storage type functions. */
	inline decStringList* operator->(){
		return &pValue;
	}
	
	/** \brief Access storage type functions. */
	inline const decStringList* operator->() const{
		return &pValue;
	}
	/*@}*/
};

#endif
