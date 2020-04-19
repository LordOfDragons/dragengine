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

// include only once
#ifndef _DEBPCOMMANDEXECUTER_H_
#define _DEBPCOMMANDEXECUTER_H_

// includes

// predefinitions
class dePhysicsBullet;

class decUnicodeArgumentList;
class decUnicodeString;



/**
 * @brief Command Executer.
 *
 * Executes commands given on the module console.
 */
class debpCommandExecuter{
private:
	dePhysicsBullet *pBullet;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new command executer. */
	debpCommandExecuter( dePhysicsBullet *bullet );
	/** Cleans up the command executer. */
	~debpCommandExecuter();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Executes a command given on the console. */
	void ExecuteCommand( const decUnicodeArgumentList &command, decUnicodeString &answer );
	/*@}*/
	
private:
	void pHelp( const decUnicodeArgumentList &command, decUnicodeString &answer );
};

// end of include only once
#endif
