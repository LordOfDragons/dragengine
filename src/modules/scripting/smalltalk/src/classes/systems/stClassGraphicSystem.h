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

#ifndef _STCLASSGRAPHICSYSTEM_H_
#define _STCLASSGRAPHICSYSTEM_H_

#include "../../stCommon.h"

class ScriptingSmalltalk;



/**
 * \brief Script class graphic system.
 */
class stClassGraphicSystem{
private:
	ScriptingSmalltalk &pST;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	stClassGraphicSystem( ScriptingSmalltalk &st );
	
	/** \brief Clean up class. */
	virtual ~stClassGraphicSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline ScriptingSmalltalk &GetST() const{ return pST; }
	
	/** \brief Set up links. */
	void SetUpLinks();
	/*@}*/
	
	
	
	/** \name cCall Methods */
	/*@{*/
	/** \brief Width of the render window. */
	static int ccGetWindowWidth( OOP self );
	
	/** \brief Height of the render window. */
	static int ccGetWindowHeight( OOP self );
	
	/** \brief Set window geometry. */
	static void ccSetWindowSize( OOP self, int width, int height );
	
	/** \brief Set window title. */
	static void ccSetWindowTitle( OOP self, const char *title );
	
	/** \brief Primary render target canvas view. */
	static OOP ccGetPrimaryCanvas( OOP self );
	/*@}*/
};

#endif
