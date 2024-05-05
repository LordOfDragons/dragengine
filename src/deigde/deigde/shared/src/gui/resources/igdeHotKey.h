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
