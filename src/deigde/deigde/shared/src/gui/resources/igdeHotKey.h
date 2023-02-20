/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

#ifndef _IGDEHOTKEY_H_
#define _IGDEHOTKEY_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputEvent.h>



/**
 * \brief IGDE UI Hot Key.
 */
class DE_DLL_EXPORT igdeHotKey{
private:
	deInputEvent::eKeyCodes pKeyCode;
	int pKey;
	int pModifiers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty hot-key which equals to no hot-key set. */
	igdeHotKey();
	
	/** \brief Create hot-key. */
	igdeHotKey( int modifiers, deInputEvent::eKeyCodes keyCode );
	
	/** \brief Create hot-key. */
	igdeHotKey( int modifiers, int key );
	
	/** \brief Create copy of hot-key. */
	igdeHotKey( const igdeHotKey &hotKey );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Platform independent key code or ekcUndefined if not set. */
	inline deInputEvent::eKeyCodes GetKeyCode() const{ return pKeyCode; }
	
	/** \brief Set platform independent key code or ekcUndefined if not set. */
	void SetKeyCode( deInputEvent::eKeyCodes keyCode );
	
	/** \brief Platform dependent key or 0 if not set. */
	inline int GetKey() const{ return pKey; }
	
	/** \brief Set platform dependent key or 0 if not set. */
	void SetKey( int key );
	
	/**
	 * \brief Modifiers using flags from deInputEvent::eStateModifiers.
	 * 
	 * Only these modifiers are supported:
	 * - esmShift
	 * - esmControl
	 * - esmAlt
	 */
	inline int GetModifiers() const{ return pModifiers; }
	
	/**
	 * \brief Set modifiers using flags from deInputEvent::eStateModifiers.
	 * 
	 * Only these modifiers are supported:
	 * - esmShift
	 * - esmControl
	 * - esmAlt
	 */
	void SetModifiers( int modifiers );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Hot-keys are equal. */
	const bool operator==( const igdeHotKey &hotKey ) const;
	
	/** \brief Set hot-key. */
	igdeHotKey &operator=( const igdeHotKey &hotKey );
	/*@}*/
};

#endif
