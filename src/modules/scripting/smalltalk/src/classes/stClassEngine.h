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

#ifndef _STCLASSENGINE_H_
#define _STCLASSENGINE_H_

#include "../stCommon.h"

class ScriptingSmalltalk;



/**
 * \brief Script class engine.
 */
class stClassEngine{
private:
	ScriptingSmalltalk &pST;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new script class. */
	stClassEngine( ScriptingSmalltalk &st );
	
	/** \brief Clean up class. */
	virtual ~stClassEngine();
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
	/** \brief exit the game. */
	static void ccQuit( OOP self );
	
	/** \brief Elapsed time. */
	static float ccElapsedTime( OOP self );
	
	/** \brief Reset timers. */
	static void ccResetTimers( OOP self );
	
	/** \brief Game object. */
	static OOP ccGame( OOP self );
	
	/** \brief Average frame rate. */
	static int ccFPSRate( OOP self );
	
	/** \brief Command line arguments. */
	static OOP ccArguments( OOP self );
	/*@}*/
};

#endif
