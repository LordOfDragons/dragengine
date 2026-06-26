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
#include <dragengine/common/collection/decTSet.h>


/**
 * \brief Set meta property storage.
 * 
 * T is the value type and P the meta property type. T has to match the expected value type of P.
 */
template<typename T, typename P, typename S = decTObjectSet<T>, typename E = decTObjectSet<T>>
class igdeMetaPropertyStorageSet : public igdeMetaPropertyStorage<P>{
public:
	using ObjectRef = deTObjectReference<T>;
	using SetType = S;
	using SelectionType = E;
	
private:
	SetType pValue;
	SelectionType pSelection;
	ObjectRef pActive;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create set meta property storage. */
	igdeMetaPropertyStorageSet(P &property, const deTObjectReference<igdeMetaContext> &context) :
	igdeMetaPropertyStorage<P>(property, context)
	{
		if constexpr (requires() { { property.GetDefaultValueType() } -> std::convertible_to<SetType>; } ) {
			pValue = property.GetDefaultValueType();
		}
	}
	
	/** \brief Create set meta property storage with initial value. */
	igdeMetaPropertyStorageSet(P &property, const deTObjectReference<igdeMetaContext> &context, const SetType &initialValue) :
	igdeMetaPropertyStorage<P>(property, context),
	pValue(initialValue){
	}
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Objects added to the set. */
	igdeTEvent<const ObjectRef&> onObjectAdded;
	
	/** \brief Objects removed from the set. */
	igdeTEvent<const ObjectRef&> onObjectRemoved;
	
	/** \brief Active object changed. */
	igdeTEvent<> onActiveChanged;
	
	
	/** \brief Get value. */
	inline const SetType &GetValue() const{ return pValue; }
	
	/** \brief Set value. */
	void SetValue(const SetType &value, bool notify = true){
		if(pValue == value){
			return;
		}
		
		if(onObjectRemoved){
			pValue.Visit([&](const ObjectRef &object){
				if(!value.Has(object)){
					onObjectRemoved(object);
				}
			});
		}
		
		if(onObjectAdded){
			value.Visit([&](const ObjectRef &object){
				if(!pValue.Has(object)){
					onObjectAdded(object);
				}
			});
		}
		
		pValue = value;
		
		igdeMetaPropertyStorage<P>::onValueChanged();
		if(notify){
			igdeMetaPropertyStorage<P>::Property().NotifyValueChanged(igdeMetaPropertyStorage<P>::Context());
		}
	}
	
	/** \brief Set value. */
	void SetValue(const igdeMetaPropertyStorageSet<T,P,S,E> &value, bool notify = true){
		SetValue(value.GetValue(), notify);
	}
	
	/** \brief Get selection. */
	inline const SelectionType &GetSelection() const{ return pSelection; }
	
	/** \brief Set selection. */
	void SetSelection(const SelectionType &selection, bool notify = true){
		if(pSelection == selection){
			return;
		}
		
		pSelection = selection;
		if(notify){
			igdeMetaPropertyStorage<P>::Property().NotifySelectionChanged(igdeMetaPropertyStorage<P>::Context());
		}
	}
	
	/** \brief Get active object. */
	inline const ObjectRef &GetActive() const{ return pActive; }
	
	/** \brief Set active object. */
	void SetActive(T* active, bool notify = true){
		if(pActive == active){
			return;
		}
		
		pActive = active;
		onActiveChanged();
		if(notify){
			igdeMetaPropertyStorage<P>::Property().NotifyActiveChanged(igdeMetaPropertyStorage<P>::Context());
		}
	}
	
	/** \brief Set active object. */
	void SetActive(const ObjectRef &active, bool notify = true){
		SetActive(active.Pointer(), notify);
	}
	/*@}*/
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Implicit conversion operator. */
	operator const SetType&() const{
		return GetValue();
	}
	
	/** \brief Assignment operator. */
	igdeMetaPropertyStorageSet<T,P,S,E> &operator=(const SetType &value){
		SetValue(value);
		return *this;
	}
	
	/** \brief Assignment operator. */
	igdeMetaPropertyStorageSet<T,P,S,E> &operator=(const igdeMetaPropertyStorageSet<T,P,S,E> &other){
		SetValue(other.GetValue());
		return *this;
	}
	
	/** \brief Value is equal. */
	bool operator==(const SetType &value) const{
		return pValue == value;
	}
	
	/** \brief Access storage type functions. */
	inline SetType* operator->(){
		return &pValue;
	}
	
	/** \brief Access storage type functions. */
	inline const SetType* operator->() const{
		return &pValue;
	}
	/*@}*/
};

#endif
