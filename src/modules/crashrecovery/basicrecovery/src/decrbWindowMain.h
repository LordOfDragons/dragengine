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
	FXDECLARE(decrbWindowMain)
	
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
	decrbWindowMain(FXApp* app, deCRBasic *crBasic);
	virtual ~decrbWindowMain();
	virtual void create();
	
	// management
	void CleanUp();
	
	inline deEngine *GetEngine() const{return pEngine;}
	static const char *GetTextForError(const char *error);
	
	// callbacks
	long onClose(FXObject *sender, FXSelector selector, void *data);
	long onBtnContinue(FXObject *sender, FXSelector selector, void *data);
	long onBtnShutdown(FXObject *sender, FXSelector selector, void *data);
};

// end of include only once
#endif
