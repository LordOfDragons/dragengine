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

#ifndef _IGDEMETAPROPERTYSTORAGESTRINGSET_H_
#define _IGDEMETAPROPERTYSTORAGESTRINGSET_H_

#include "igdeMetaPropertyStorage.h"

#include <dragengine/common/string/decStringSet.h>


/**
 * \brief String set meta property storage.
 * 
 * T is the value type and P the meta property type. T has to match the expected value type of P.
 */
template<typename P>
class igdeMetaPropertyStorageStringSet : public igdeMetaPropertyStorage<P>{
private:
	decStringSet pValue, pSelection;
	typename P::StringRef pActive;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create string set meta property storage. */
	igdeMetaPropertyStorageStringSet(P &property, const deTObjectReference<igdeMetaContext> &context) :
	igdeMetaPropertyStorage<P>(property, context),
	pValue(property.GetDefaultValue()){
	}
	
	/** \brief Create string set meta property storage with initial value. */
	igdeMetaPropertyStorageStringSet(P &property, const deTObjectReference<igdeMetaContext> &context, const decStringSet &initialValue) :
	igdeMetaPropertyStorage<P>(property, context),
	pValue(initialValue){
	}
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Get value. */
	inline const decStringSet &GetValue() const{ return pValue; }
	
	/** \brief Set value. */
	void SetValue(const decStringSet &value, bool notify = true){
		if(pValue == value){
			return;
		}
		
		pValue = value;
		igdeMetaPropertyStorage<P>::onValueChanged();
		if(notify){
			igdeMetaPropertyStorage<P>::Property().NotifyValueChanged(igdeMetaPropertyStorage<P>::Context());
		}
	}
	
	/** \brief Set value. */
	void SetValue(const igdeMetaPropertyStorageStringSet<P> &value, bool notify = true){
		SetValue(value.GetValue(), notify);
	}
	
	/** \brief Get selection. */
	inline const decStringSet &GetSelection() const{ return pSelection; }
	
	/** \brief Set selection. */
	void SetSelection(const decStringSet &selection, bool notify = true){
		if(pSelection == selection){
			return;
		}
		
		pSelection = selection;
		if(notify){
			igdeMetaPropertyStorage<P>::Property().NotifySelectionChanged(igdeMetaPropertyStorage<P>::Context());
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
			igdeMetaPropertyStorage<P>::Property().NotifyActiveChanged(igdeMetaPropertyStorage<P>::Context());
		}
	}
	/*@}*/
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Implicit conversion operator. */
	operator const decStringSet&() const{
		return GetValue();
	}
	
	/** \brief Assignment operator. */
	igdeMetaPropertyStorageStringSet<P> &operator=(const decStringSet &value){
		SetValue(value);
		return *this;
	}
	
	/** \brief Assignment operator. */
	igdeMetaPropertyStorageStringSet<P> &operator=(const igdeMetaPropertyStorageStringSet<P> &other){
		SetValue(other.GetValue());
		return *this;
	}
	
	/** \brief Value is equal. */
	bool operator==(const decStringSet &value) const{
		return pValue == value;
	}
	
	/** \brief Access storage type functions. */
	inline decStringSet* operator->(){
		return &pValue;
	}
	
	/** \brief Access storage type functions. */
	inline const decStringSet* operator->() const{
		return &pValue;
	}
	/*@}*/
};

#endif
