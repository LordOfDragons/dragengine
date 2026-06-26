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

#ifndef _IGDEMETAPROPERTYSTORAGELIST_H_
#define _IGDEMETAPROPERTYSTORAGELIST_H_

#include "igdeMetaPropertyStorage.h"

#include <dragengine/common/collection/decTSet.h>
#include <dragengine/common/collection/decTOrderedSet.h>


/**
 * \brief List meta property storage.
 * 
 * T is the value type and P the meta property type. T has to match the expected value type of P.
 */
template<typename T, typename P, typename L = decTObjectOrderedSet<T>, typename S = decTObjectSet<T>>
class igdeMetaPropertyStorageList : public igdeMetaPropertyStorage<P>{
public:
	using ObjectRef = deTObjectReference<T>;
	using ListType = L;
	using SelectionType = S;
	
private:
	ListType pValue;
	SelectionType pSelection;
	ObjectRef pActive;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list meta property storage. */
	igdeMetaPropertyStorageList(P &property, const deTObjectReference<igdeMetaContext> &context) :
	igdeMetaPropertyStorage<P>(property, context)
	{
		if constexpr (requires() { { property.GetDefaultValueType() } -> std::convertible_to<ListType>; } ) {
			pValue = property.GetDefaultValueType();
		}
	}
	
	/** \brief Create list meta property storage with initial value. */
	igdeMetaPropertyStorageList(P &property, const deTObjectReference<igdeMetaContext> &context, const ListType &initialValue) :
	igdeMetaPropertyStorage<P>(property, context),
	pValue(initialValue){
	}
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Objects added to the list. */
	igdeTEvent<const ObjectRef&> onObjectAdded;
	
	/** \brief Objects removed from the list. */
	igdeTEvent<const ObjectRef&> onObjectRemoved;
	
	/** \brief Active object changed. */
	igdeTEvent<> onActiveChanged;
	
	
	/** \brief Get value. */
	inline const ListType &GetValue() const{ return pValue; }
	
	/** \brief Set value. */
	void SetValue(const ListType &value, bool notify = true){
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
	
	/** \brief Set value. */
	void SetValue(const igdeMetaPropertyStorageList<T,P,L,S> &value, bool notify = true){
		SetValue(value.GetValue(), notify);
	}
	/*@}*/
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Implicit conversion operator. */
	operator const ListType&() const{
		return GetValue();
	}
	
	/** \brief Assignment operator. */
	igdeMetaPropertyStorageList<T,P,L,S> &operator=(const ListType &value){
		SetValue(value);
		return *this;
	}
	
	/** \brief Assignment operator. */
	igdeMetaPropertyStorageList<T,P,L,S> &operator=(const igdeMetaPropertyStorageList<T,P,L,S> &other){
		SetValue(other.GetValue());
		return *this;
	}
	
	/** \brief Value is equal. */
	bool operator==(const ListType &value) const{
		return pValue == value;
	}
	
	/** \brief Access storage type functions. */
	inline ListType* operator->(){
		return &pValue;
	}
	
	/** \brief Access storage type functions. */
	inline const ListType* operator->() const{
		return &pValue;
	}
	/*@}*/
};

#endif
