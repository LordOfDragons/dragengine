/* 
 * Drag[en]gine GUI Launcher
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

#ifndef _DEGLPELISTITEMMODULE_H_
#define _DEGLPELISTITEMMODULE_H_

#include "../foxtoolkit.h"

#include <delauncher/engine/modules/delEngineModule.h>

#include <dragengine/common/string/decString.h>

class deglPanelEngine;



/**
 * Engine Panel List Item Module.
 */
class deglPEListItemModule : public FXIconItem{
	FXDECLARE( deglPEListItemModule )
protected:
	deglPEListItemModule();
	
private:
	deglPanelEngine *pPanelEngine;
	const delEngineModule::Ref pModule;
	decString pTypeString;
	decString pStatusString;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create list item. */
	deglPEListItemModule( deglPanelEngine *panelEngine, delEngineModule *module );
	
	/** Clean up list item. */
	virtual ~deglPEListItemModule();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Engine panel. */
	inline deglPanelEngine *GetPanelEngine() const{ return pPanelEngine; }
	
	/** Module object. */
	inline delEngineModule *GetModule() const{ return pModule; }
	
	/** Type string. */
	inline const decString &GetTypeString() const{ return pTypeString; }
	
	/** Status string. */
	inline const decString &GetStatusString() const{ return pStatusString; }
	/*@}*/
};

#endif
