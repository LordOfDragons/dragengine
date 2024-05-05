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

// include only once
#ifndef _DECRBPANELCONSOLE_H_
#define _DECRBPANELCONSOLE_H_

// includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "foxtoolkit.h"

// predefinitions
class decrbWindowMain;
class deEngine;
class decUnicodeLineBuffer;
class decUnicodeString;



/**
 * Panel allowing to send commands directly to the module using the
 * module console ability of the engine.
 */
class decrbPanelConsole : public FXVerticalFrame{
	FXDECLARE( decrbPanelConsole )
	
protected:
	decrbPanelConsole();
	
public:
	enum eFoxIDs{
		ID_CANVAS = FXTopWindow::ID_LAST,
		ID_CBMODULE,
		ID_EDITCOMMAND,
		ID_LAST
	};
		
private:
	decrbWindowMain *pWndMain;
	decUnicodeLineBuffer *pLog;
	
	FXComboBox *pCBModule;
	FXTextField *pEditCommand;
	FXText *pEditLog;
	
public:
	// constructor, destructor
	decrbPanelConsole( decrbWindowMain *windowMain, FXComposite *container );
	virtual ~decrbPanelConsole();
	
	// management
	void UpdateModulesList();
	void AddToConsole( decUnicodeString &text );
	void ClearConsole();
	
	// events
	long onCBModuleChanged( FXObject *sender, FXSelector selector, void *data );
	long onEditSendCommand( FXObject *sender, FXSelector selector, void *data );
	
private:
};

// end of include only once
#endif
