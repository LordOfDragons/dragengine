/* 
 * Drag[en]gine SmallTalk Script Module
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

#ifndef _STCLASSSCRIPTING_H_
#define _STCLASSSCRIPTING_H_

#include "../stCommon.h"

class ScriptingSmalltalk;



/**
 * \brief Script class Scripting.
 *
 * Internal scripting class used only to bridge between smalltalk
 * land and c++ land. There exists only one instance of this
 * class and it is not accessible by to smalltalk at all.
 */
class stClassScripting{
private:
	ScriptingSmalltalk *pST;
	OOP pSingleton;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	stClassScripting( ScriptingSmalltalk *st );
	
	/** \brief Cleans up class. */
	virtual ~stClassScripting();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Script module instance.
	 * \details Static call as it is used to obtain the entry point into c++ land from smalltalk.
	 */
	static ScriptingSmalltalk &GetSTFromOOP( OOP object );
	
	/** \brief Singleton smalltalk object. */
	inline OOP GetSingleton() const{ return pSingleton; }
	
	/** \brief Create singleton. */
	void CreateSingleton();
	
	/** \brief Free singleton. */
	void FreeSingleton();
	/*@}*/
};

#endif
