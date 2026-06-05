/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDELISTENERS_H_
#define _IGDELISTENERS_H_

#include <functional>
#include <dragengine/deTObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decTOrderedSet.h>


/**
 * \brief Template for safe listener lists.
 * 
 * Safe listeners allow adding and removing listeners while notifying them.
 */
template<typename L>
class DE_DLL_EXPORT igdeTListenerList{
private:
	decTObjectOrderedSet<L> pListeners, pLockedListeners;
	int pLockCount = 0;
	bool pHasChanged = false;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener list. */
	igdeTListenerList() = default;
	
	/** \brief Clean up listener list. */
	~igdeTListenerList() = default;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Add listener. */
	void Add(const deTObjectReference<L> &listener){
		DEASSERT_NOTNULL(listener)
		pModifyList([&](decTObjectOrderedSet<L> &list){
			list.Add(listener);
		});
	}
	
	/** \brief Remove listener. */
	void Remove(const deTObjectReference<L> &listener){
		pModifyList([&](decTObjectOrderedSet<L> &list){
			list.Remove(listener);
		});
	}
	
	/** \brief Notify listeners. */
	void Notify(const std::function<void(const deTObjectReference<L>&)> &function){
		pLock();
		try{
			pListeners.Visit([&](const deTObjectReference<L> &listener){
				function(listener);
			});
			
		}catch(...){
			pUnlock();
			throw;
		}
		pUnlock();
	}
	/*@}*/
	
	
private:
	void pModifyList(const std::function<void(decTObjectOrderedSet<L> &list)> &function){
		if(pLockCount == 0){
			function(pListeners);
			
		}else{
			if(!pHasChanged){
				pLockedListeners = pListeners;
				pHasChanged = true;
			}
			function(pLockedListeners);
		}
	}
	
	void pLock(){
		if(pLockCount == 0){
			pHasChanged = false;
		}
		pLockCount++;
	}
	
	void pUnlock(){
		DEASSERT_TRUE(pLockCount > 0)
		pLockCount--;
		if(pLockCount == 0 && pHasChanged){
			pListeners = std::move(pLockedListeners);
			pHasChanged = false;
		}
	}
};

#endif
