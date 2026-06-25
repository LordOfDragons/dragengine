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

#ifndef _IGDEMETAPROPERTYSTORAGETREELIST_H_
#define _IGDEMETAPROPERTYSTORAGETREELIST_H_
	
#include "igdeMetaPropertyStorage.h"

#include <dragengine/common/collection/decTSet.h>


/**
 * \brief Tree list meta property storage.
 * 
 * T is the object type and P the meta property type. T has to match the expected object type of P.
 */
template<typename T, typename P>
class igdeMetaPropertyStorageTreeList : public igdeMetaPropertyStorage<P>{
public:
	using ObjectRef = deTObjectReference<T>;
	using ExpandedSet = decTObjectSet<T>;
	
private:
	ExpandedSet pExpanded;
	ObjectRef pActive;
	std::function<void()> pOnActiveChanged;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create tree list meta property storage. */
	igdeMetaPropertyStorageTreeList(P &property, const deTObjectReference<igdeMetaContext> &context) :
	igdeMetaPropertyStorage<P>(property, context){
	}
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Get expanded state. */
	inline const ExpandedSet &GetExpanded() const{ return pExpanded; }
	
	/** \brief Set expanded state. */
	void SetExpanded(const ExpandedSet &expanded, bool notify = true){
		if(pExpanded == expanded){
			return;
		}
		
		pExpanded = expanded;
		if(notify){
			igdeMetaPropertyStorage<P>::Property().NotifyExpandedChanged(igdeMetaPropertyStorage<P>::Context());
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
	/** \brief Assignment operator. */
	igdeMetaPropertyStorageTreeList<T, P> &operator=(
		const igdeMetaPropertyStorageTreeList<T, P> &other) = delete;
	
	/** \brief Value is equal. */
	bool operator==(const igdeMetaPropertyStorageTreeList<T, P> &other) const = delete;
	/*@}*/
};

#endif
