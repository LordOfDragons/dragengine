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

#ifndef _DECTLISTENERS_H_
#define _DECTLISTENERS_H_

#include "../../common/collection/decTOrderedSet.h"


/**
 * \brief Listeners.
 * 
 * Supports safely adding and removing listeners while visiting.
 */
template<typename T>
class decTListeners{
public:
	/** \brief Listener reference. */
	using ListenerRef = deTObjectReference<T>;
	
	
private:
	decTObjectOrderedSet<T> pListeners, pModifiedListeners;
	int pLockCount;
	bool pModified;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new listeners object. */
	decTListeners() : pLockCount(0), pModified(false){
	}
	
	/** \brief Cleans up listeners object. */
	~decTListeners() = default;
	
	decTListeners(const decTListeners<T> &) = delete;
	decTListeners(decTListeners<T> &&) = delete;
	decTListeners<T> &operator=(const decTListeners<T> &) = delete;
	decTListeners<T> &operator=(decTListeners<T> &&) = delete;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Add listener if absent. */
	void Add(T *listener){
		DEASSERT_NOTNULL(listener)
		
		if(pLockCount == 0){
			pListeners.Add(listener);
			
		}else if(!pModified){
			if(!pListeners.Has(listener)){
				pModifiedListeners = pListeners;
				pModifiedListeners.Add(listener);
				pModified = true;
			}
			
		}else{
			pModifiedListeners.Add(listener);
		}
	}
	
	/** \brief Remove listener if present. */
	void Remove(T *listener){
		DEASSERT_NOTNULL(listener)
		
		if(pLockCount == 0){
			pListeners.Remove(listener);
			
		}else if(!pModified){
			if(pListeners.Has(listener)){
				pModifiedListeners = pListeners;
				pModifiedListeners.Remove(listener);
				pModified = true;
			}
			
		}else{
			pModifiedListeners.Remove(listener);
		}
	}
	
	/** \brief Remove all listeners. */
	void RemoveAll(){
		if(pLockCount == 0){
			pListeners.RemoveAll();
			
		}else if(!pModified){
			if(pListeners.IsNotEmpty()){
				pModifiedListeners.RemoveAll(); // should be already empty
				pModified = true;
			}
			
		}else{
			pModifiedListeners.RemoveAll();
		}
	}
	
	/** \brief Visit listeners. */
	template<typename F>
	void Visit(F &&function){
		pLock();
		try{
			pListeners.Visit(function);
			
		}catch(...){
			pUnlock();
			throw;
		}
		pUnlock();
	}
	
private:
	void pLock(){
		pLockCount++;
	}
	
	void pUnlock(){
		if(--pLockCount == 0 && pModified){
			pListeners = std::move(pModifiedListeners);
			pModified = false;
		}
	}
	/*@}*/
};

#endif
