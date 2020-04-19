/* 
 * Drag[en]gine Basic Crash Recovery Module
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
#ifndef _DECRBWINDOWMAIN_H_
#define _DECRBWINDOWMAIN_H_

// includes
#include "foxtoolkit.h"
#undef PI

// predefinitions
class deEngine;
class deCRBasic;
class deErrorTraceValue;
class decrbPanelStatus;
class decrbPanelErrorTrace;
class decrbPanelParameters;
class decrbPanelModuleStatus;
class decrbPanelConsole;



// class decrbWindowMain
class decrbWindowMain : public FXMainWindow{
	FXDECLARE( decrbWindowMain )
	
protected:
	decrbWindowMain();
	
public:
	enum eFoxIDs{
		ID_CANVAS=FXMainWindow::ID_LAST,
		ID_TABPANELS,
		ID_BTNCONTINUE,
		ID_BTNSHUTDOWN,
		ID_LAST
	};
	
private:
	deCRBasic *pCRBasic;
	deEngine *pEngine;
	FXTabBook *pTabPanels;
	
	FXButton *pBtnContinue;
	FXButton *pBtnShutdown;
	
	decrbPanelStatus *pPanelStatus;
	decrbPanelErrorTrace *pPanelErrorTrace;
	decrbPanelParameters *pPanelParameters;
	decrbPanelModuleStatus *pPanelModuleStatus;
	decrbPanelConsole *pPanelConsole;
	
public:
	// constructor, destructor
	decrbWindowMain( FXApp* app, deCRBasic *crBasic );
	virtual ~decrbWindowMain();
	virtual void create();
	
	// management
	void CleanUp();
	
	inline deEngine *GetEngine() const{ return pEngine; }
	static const char *GetTextForError( const char *error );
	
	// callbacks
	long onClose( FXObject *sender, FXSelector selector, void *data );
	long onBtnContinue( FXObject *sender, FXSelector selector, void *data );
	long onBtnShutdown( FXObject *sender, FXSelector selector, void *data );
};

// end of include only once
#endif
