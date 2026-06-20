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

#ifndef _IGDEMETAPROPERTYSTORAGESET_H_
#define _IGDEMETAPROPERTYSTORAGESET_H_

#include "igdeMetaPropertyStorage.h"

#include <dragengine/deObject.h>


/**
 * \brief Set meta property storage.
 * 
 * T is the value type and P the meta property type. T has to match the expected value type of P.
 */
template<typename T, typename P>
class igdeMetaPropertyStorageSet : public igdeMetaPropertyStorage<P>{
private:
	typename P::SetType pValue, pSelection;
	typename P::ObjectTypeRef pActive;
	std::function<void(const typename P::ObjectTypeRef&)> pOnObjectAdded, pOnObjectRemoved;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create set meta property storage. */
	igdeMetaPropertyStorageSet(P &property, const deTObjectReference<igdeMetaContext> &context) :
	igdeMetaPropertyStorage<P>(property, context),
	pValue(property.GetDefaultValueType()){
	}
	
	/** \brief Create set meta property storage with initial value. */
	igdeMetaPropertyStorageSet(P &property, const deTObjectReference<igdeMetaContext> &context, const typename P::SetType &initialValue) :
	igdeMetaPropertyStorage<P>(property, context),
	pValue(initialValue){
	}
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Get value. */
	inline const typename P::SetType &GetValue() const{ return pValue; }
	
	/** \brief Set value. */
	void SetValue(const typename P::SetType &value, bool notify = true){
		if(pValue == value){
			return;
		}
		
		if(pOnObjectRemoved){
			pValue.Visit([&](const typename P::ObjectTypeRef &object){
				if(!value.Has(object)){
					pOnObjectRemoved(object);
				}
			});
		}
		
		if(pOnObjectAdded){
			value.Visit([&](const typename P::ObjectTypeRef &object){
				if(!pValue.Has(object)){
					pOnObjectAdded(object);
				}
			});
		}
		
		pValue = value;
		
		igdeMetaPropertyStorage<P>::OnValueChanged();
		if(notify){
			igdeMetaPropertyStorage<P>::Property().NotifyValueChanged(igdeMetaPropertyStorage<P>::Context());
		}
	}
	
	/** \brief Get selection. */
	inline const typename P::SetType &GetSelection() const{ return pSelection; }
	
	/** \brief Set selection. */
	void SetSelection(const typename P::SetType &selection, bool notify = true){
		if(pSelection == selection){
			return;
		}
		
		pSelection = selection;
		if(notify){
			igdeMetaPropertyStorage<P>::Property().NotifySelectionChanged(igdeMetaPropertyStorage<P>::Context());
		}
	}
	
	/** \brief Get active object. */
	inline const typename P::ObjectTypeRef &GetActive() const{ return pActive; }
	
	/** \brief Set active object. */
	void SetActive(const typename P::ObjectTypeRef &active, bool notify = true){
		if(pActive == active){
			return;
		}
		
		pActive = active;
		if(notify){
			igdeMetaPropertyStorage<P>::Property().NotifyActiveChanged(igdeMetaPropertyStorage<P>::Context());
		}
	}
	
	/** \brief Set value. */
	void SetValue(const igdeMetaPropertyStorageSet<T, P> &value, bool notify = true){
		SetValue(value.GetValue(), notify);
	}
	
	
	/** \brief Function to call for objects added to the set. */
	inline const std::function<void(const typename P::ObjectTypeRef&)> &GetOnObjectAdded() const{ return pOnObjectAdded; }
	
	/** \brief Set function to call for objects added to the set. */
	template <typename F>
	void SetOnObjectAdded(F&& func){
		pOnObjectAdded = std::forward<F>(func);
	}
	
	/** \brief Set function to call for objects added to the set. */
	template <typename F>
	void SetOnObjectAdded(const F& func){
		pOnObjectAdded = func;
	}
	
	/** \brief Function to call for objects removed from the set. */
	inline const std::function<void(const typename P::ObjectTypeRef&)> &GetOnObjectRemoved() const{ return pOnObjectRemoved; }
	
	/** \brief Set function to call for objects removed from the set. */
	template <typename F>
	void SetOnObjectRemoved(F&& func){
		pOnObjectRemoved = std::forward<F>(func);
	}
	
	/** \brief Set function to call for objects removed from the set. */
	template <typename F>
	void SetOnObjectRemoved(const F& func){
		pOnObjectRemoved = func;
	}
	/*@}*/
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Implicit conversion operator. */
	operator const typename P::SetType&() const{
		return GetValue();
	}
	
	/** \brief Assignment operator. */
	igdeMetaPropertyStorageSet<T, P> &operator=(const typename P::SetType &value){
		SetValue(value);
		return *this;
	}
	
	/** \brief Assignment operator. */
	igdeMetaPropertyStorageSet<T, P> &operator=(const igdeMetaPropertyStorageSet<T, P> &other){
		SetValue(other.GetValue());
		return *this;
	}
	
	/** \brief Value is equal. */
	bool operator==(const typename P::SetType &value) const{
		return pValue == value;
	}
	
	/** \brief Access storage type functions. */
	inline typename P::SetType* operator->(){
		return &pValue;
	}
	
	/** \brief Access storage type functions. */
	inline const typename P::SetType* operator->() const{
		return &pValue;
	}
	/*@}*/
};

#endif
