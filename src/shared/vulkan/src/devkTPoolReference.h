/* 
 * Drag[en]gine Shared Vulkan
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
