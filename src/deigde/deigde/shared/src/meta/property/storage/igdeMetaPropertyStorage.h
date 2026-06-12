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

#ifndef _IGDEMETAPROPERTYSTORAGE_H_
#define _IGDEMETAPROPERTYSTORAGE_H_

#include <functional>

#include <dragengine/deTObjectReference.h>

class igdeMetaProperty;
class igdeMetaContext;


/**
 * \brief Meta property storage.
 * 
 * P is the meta property type.
 */
template<typename P>
class DE_DLL_EXPORT igdeMetaPropertyStorage{
private:
	P &pProperty;
	const deTObjectReference<igdeMetaContext> &pContext;
	std::function<void()> pOnValueChanged;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create meta property storage. */
	igdeMetaPropertyStorage(P &property, const deTObjectReference<igdeMetaContext> &context) :
	pProperty(property), pContext(context){
	}
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Property. */
	inline P &Property(){ return pProperty; }
	inline const P &Property() const{ return pProperty; }
	
	/** \brief Context. */
	inline const deTObjectReference<igdeMetaContext> &Context() const{ return pContext; }
	
	/** \brief Set function to call if value changed before listeners are notified. */
	template <typename F>
	void SetOnChanged(F&& func){
		pOnValueChanged = std::forward<F>(func);
	}
	
	/** \brief Call value changed function if set. */
	void OnValueChanged(){
		if(pOnValueChanged){
			pOnValueChanged();
		}
	}
	/*@}*/
};

#endif
