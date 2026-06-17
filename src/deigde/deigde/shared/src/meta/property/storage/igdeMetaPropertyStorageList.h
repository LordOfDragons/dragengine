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

#include <dragengine/common/collection/decTOrderedSet.h>


/**
 * \brief List meta property storage.
 * 
 * T is the value type and P the meta property type. T has to match the expected value type of P.
 */
template<typename T, typename P, typename ListType = decTObjectOrderedSet<T>>
class igdeMetaPropertyStorageList : public igdeMetaPropertyStorage<P>{
public:
	using ObjectRef = deTObjectReference<T>;
	
private:
	ListType pValue, pSelection;
	ObjectRef pActive;
	std::function<void(const ObjectRef&)> pOnObjectAdded, pOnObjectRemoved;
	std::function<void()> pOnActiveChanged;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list meta property storage. */
	igdeMetaPropertyStorageList(P &property, const deTObjectReference<igdeMetaContext> &context) :
	igdeMetaPropertyStorage<P>(property, context){
	}
	
	/** \brief Create list meta property storage with initial value. */
	igdeMetaPropertyStorageList(P &property, const deTObjectReference<igdeMetaContext> &context, const ListType &initialValue) :
	igdeMetaPropertyStorage<P>(property, context),
	pValue(initialValue){
	}
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Get value. */
	inline const ListType &GetValue() const{ return pValue; }
	
	/** \brief Set value. */
	void SetValue(const ListType &value, bool notify = true){
		if(pValue == value){
			return;
		}
		
		if(pOnObjectRemoved){
			pValue.Visit([&](const ObjectRef &object){
				if(!value.Has(object)){
					pOnObjectRemoved(object);
				}
			});
		}
		
		if(pOnObjectAdded){
			value.Visit([&](const ObjectRef &object){
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
	inline const ListType &GetSelection() const{ return pSelection; }
	
	/** \brief Set selection. */
	void SetSelection(const ListType &selection, bool notify = true){
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
	void SetActive(const ObjectRef &active, bool notify = true){
		if(pActive == active){
			return;
		}
		
		pActive = active;
		if(pOnActiveChanged){
			pOnActiveChanged();
		}
		if(notify){
			igdeMetaPropertyStorage<P>::Property().NotifyActiveChanged(igdeMetaPropertyStorage<P>::Context());
		}
	}
	
	/** \brief Set active object. */
	void SetActive(T* active, bool notify = true){
		if(pActive == active){
			return;
		}
		
		pActive = active;
		if(pOnActiveChanged){
			pOnActiveChanged();
		}
		if(notify){
			igdeMetaPropertyStorage<P>::Property().NotifyActiveChanged(igdeMetaPropertyStorage<P>::Context());
		}
	}
	
	
	/** \brief Function to call for objects added to the list. */
	inline const std::function<void(const ObjectRef&)> &GetOnObjectAdded() const{ return pOnObjectAdded; }
	
	/** \brief Set function to call for objects added to the list. */
	template <typename F>
	void SetOnObjectAdded(F&& func){
		pOnObjectAdded = std::forward<F>(func);
	}
	
	/** \brief Set function to call for objects added to the list. */
	template <typename F>
	void SetOnObjectAdded(const F& func){
		pOnObjectAdded = func;
	}
	
	/** \brief Function to call for objects removed from the list. */
	inline const std::function<void(const ObjectRef&)> &GetOnObjectRemoved() const{ return pOnObjectRemoved; }
	
	/** \brief Set function to call for objects removed from the list. */
	template <typename F>
	void SetOnObjectRemoved(F&& func){
		pOnObjectRemoved = std::forward<F>(func);
	}
	
	/** \brief Set function to call for objects removed from the list. */
	template <typename F>
	void SetOnObjectRemoved(const F& func){
		pOnObjectRemoved = func;
	}
	
	/** \brief Function to call if active object changed before listeners are notified. */
	inline const std::function<void()> &GetOnActiveChanged() const{ return pOnActiveChanged; }
	
	/** \brief Set function to call if active object changed before listeners are notified. */
	template <typename F>
	void SetOnActiveChanged(F&& func){
		pOnActiveChanged = std::forward<F>(func);
	}
	
	/** \brief Set function to call if active object changed before listeners are notified. */
	template <typename F>
	void SetOnActiveChanged(const F& func){
		pOnActiveChanged = func;
	}
	/*@}*/
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Implicit conversion operator. */
	operator const ListType&() const{
		return GetValue();
	}
	
	/** \brief Assignment operator. */
	igdeMetaPropertyStorageList<T, P, ListType> &operator=(const ListType &value){
		SetValue(value);
		return *this;
	}
	
	/** \brief Assignment operator. */
	igdeMetaPropertyStorageList<T, P, ListType> &operator=(const igdeMetaPropertyStorageList<T, P, ListType> &other){
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
