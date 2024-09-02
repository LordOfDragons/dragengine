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

#ifndef _DEVKTPOOLREFERENCE_H_
#define _DEVKTPOOLREFERENCE_H_

#include <dragengine/common/exceptions.h>


/**
 * \brief Pool reference template.
 */
template<class T> class devkTPoolReference{
private:
	T *pSlot;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	devkTPoolReference() : pSlot( nullptr ){
	}
	
	devkTPoolReference( T *slot ) : pSlot( nullptr ){
		*this = slot;
	}
	
	~devkTPoolReference(){
		*this = nullptr;
	}
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	bool operator!() const{
		return pSlot == nullptr;
	}
	
	operator bool() const{
		return pSlot != nullptr;
	}
	
	inline operator T*() const{
		return pSlot;
	}
	
	/**
	 * \throws deeNullPointer if slot is nullptr.
	 */
	operator T&() const{
		if( ! pSlot ){
			DETHROW( deeNullPointer );
		}
		return *pSlot;
	}
	
	/**
	 * \throws deeNullPointer if slot is nullptr.
	 */
	T* operator->() const{
		if( ! pSlot ){
			DETHROW( deeNullPointer );
		}
		return pSlot;
	}
	
	devkTPoolReference &operator=( T *slot ){
		if( pSlot ){
			pSlot->ReturnToPool();
		}
		pSlot = slot;
		return *this;
	}
	
	inline bool operator==( T *slot ) const{
		return pSlot == slot;
	}
	
	inline bool operator!=( T *slot ) const{
		return pSlot != slot;
	}
	/*@}*/
	
	
	
private:
	devkTPoolReference( const devkTPoolReference & ){
		DETHROW( deeInvalidAction );
	}
	
	devkTPoolReference &operator=( const devkTPoolReference & ){
		DETHROW( deeInvalidAction );
	}
};

#endif
