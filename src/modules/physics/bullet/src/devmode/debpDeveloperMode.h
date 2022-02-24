/* 
 * Drag[en]gine Bullet Physics Module
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

#ifndef _DEBPDEVELOPERMODE_H_
#define _DEBPDEVELOPERMODE_H_

#include <dragengine/common/utils/decLayerMask.h>

class dePhysicsBullet;

class debpWorld;

class decUnicodeArgumentList;
class decUnicodeString;



/**
 * Developer Mode.
 *
 * Provides access to the developer mode. This is not required for games
 * nor editing tools and is used only by the module developers for testing
 * and trouble shooting.
 * 
 * The Take-Snapshot is intended to store a COLLADE snapshot of the next
 * collision detection run requested by the host application. After the
 * snapshot is done the flag is reset.
 */
class debpDeveloperMode{
private:
	dePhysicsBullet &pBullet;
	bool pEnabled;
	
	bool pTakeSnapshot;
	decLayerMask pShowCategory;
	int pHighlightResponseType;
	bool pHighlightDeactivation;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create developer mode. */
	debpDeveloperMode( dePhysicsBullet &bullet );
	
	/** Clean up developer mode. */
	~debpDeveloperMode();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * Executes a command.
	 * \details If the command is recognized true is returned otherwise false.
	 */
	bool ExecuteCommand( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	/** Developer mode is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** Show category layer mask. */
	inline const decLayerMask &GetShowCategory() const{ return pShowCategory; }
	
	/** Highlight response type or -1 if disabled. */
	inline int GetHighlightResponseType() const{ return pHighlightResponseType; }
	
	/** Highlight deactivation state. */
	inline bool GetHighlightDeactivation() const{ return pHighlightDeactivation; }
	
	/**
	 * Take snapshot of world if required.
	 * 
	 * By default this does nothing unless a snapshot has been requested beforehand in which
	 * case the given world is written to a file using the collada exporter provided in BULLET.
	 */
	void TakeSnapshot( debpWorld *world );
	/*@}*/
	
	
	
private:
	void pCmdHelp( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdEnable( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdTakeSnapshot( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdShowCategory( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdHighlightResponseType( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdHighlightDeactivation( const decUnicodeArgumentList &command, decUnicodeString &answer );
	void pCmdDebugEnable( const decUnicodeArgumentList &command, decUnicodeString &answer );
};

#endif
