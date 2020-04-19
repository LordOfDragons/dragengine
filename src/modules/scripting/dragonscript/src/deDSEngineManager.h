/* 
 * Drag[en]gine DragonScript Script Module
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

#ifndef _DEDSENGINEMANAGER_H_
#define _DEDSENGINEMANAGER_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;



/**
 * @brief DragonScript Engine Manager.
 */
class deDSEngineManager : public dsBaseEngineManager{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new dragonscript engine manager. */
	deDSEngineManager( deScriptingDragonScript *ds );
	/** Cleans up the dragonscript engine manager. */
	virtual ~deDSEngineManager();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	// message output
	void OutputMessage(const char *Message);
	void OutputWarning(const char *Message, int WarnID, dsScriptSource *Script, int Line, int Position);
	void OutputWarningMore(const char *Message);
	void OutputError(const char *Message, int ErrorID, dsScriptSource *Script, int Line, int Position);
	void OutputErrorMore(const char *Message);
	// parser management
	bool ContinueParsing();
	/*@}*/
};

// end of include only once
#endif
