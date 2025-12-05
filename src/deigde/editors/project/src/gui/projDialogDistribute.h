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

#ifndef _PROJDIALOGDISTRIBUTE_H_
#define _PROJDIALOGDISTRIBUTE_H_

#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/dialog/igdeDialog.h>
#include <deigde/gui/event/igdeActionExternOpen.h>

#include <dragengine/common/string/decString.h>

class projWindowMain;
class projProfile;
class projTaskDistribute;


/**
 * Distribute dialog.
 * 
 * Provides a combo box for the user to select the profile to projribute. Once the process
 * is started progress information is listed below and updated as the process progresses.
 */
class projDialogDistribute : public igdeDialog{
public:
	/** Type holding strong reference. */
	typedef deTObjectReference<projDialogDistribute> Ref;
	
	
private:
	projWindowMain &pWindowMain;
	projProfile *pProfile;
	
	igdeActionExternOpen::Ref pActionShowInFSManager;
	
	igdeTextField::Ref pEditDelgaPath;
	igdeTextField::Ref pEditDelgaSize;
	igdeTextField::Ref pEditDelgaFileCount;
	igdeTextField::Ref pEditDelgaDirCount;
	igdeTextArea::Ref pEditLogs;
	igdeButton::Ref pBtnShowInFSMgr;
	
	projTaskDistribute *pTaskDistribute;
	decString pLastTaskMessage;
	
	bool pCloseDialogOnFinished;
	bool pPrintToConsole;
	
	bool pSuccess;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create dialog. */
	projDialogDistribute( projWindowMain &windowMain, projProfile *profile );
	
	/** Clean up dialog. */
	virtual ~projDialogDistribute();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Main window. */
	inline projWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** Profile. */
	inline projProfile &GetProfile() const{ return *pProfile; }
	
	/** Add log message. */
	void LogMessage( const char *message );
	
	/**
	 * Game like frame update while running modal.
	 * 
	 * Called while the dialog is inside Run(). Can be used to do updating on a regular basis.
	 */
	virtual void OnFrameUpdate();
	
	/** Close dialog once finished. */
	void SetCloseDialogOnFinished( bool closeDialogOnFinished );
	
	/** Print to console in addition to loggers. */
	void SetPrintToConsole( bool printToConsole );
	
	/** Success. */
	inline bool GetSuccess() const{ return pSuccess; }
	/*@}*/
	
	
	
protected:
	void pStartBuilding();
};

#endif
