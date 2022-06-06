/* 
 * Drag[en]gine IGDE Project Editor
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

#ifndef _PROJDIALOGDISTRIBUTE_H_
#define _PROJDIALOGDISTRIBUTE_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeTextAreaReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/dialog/igdeDialog.h>
#include <deigde/gui/event/igdeActionExternOpenReference.h>

#include <dragengine/common/string/decString.h>

class projWindowMain;
class projProfile;
class projTaskDistribute;


/**
 * \brief Distribute dialog.
 * 
 * Provides a combo box for the user to select the profile to projribute. Once the process
 * is started progress information is listed below and updated as the process progresses.
 */
class projDialogDistribute : public igdeDialog{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<projDialogDistribute> Ref;
	
private:
	projWindowMain &pWindowMain;
	projProfile *pProfile;
	
	igdeActionExternOpenReference pActionShowInFSManager;
	
	igdeTextFieldReference pEditDelgaPath;
	igdeTextFieldReference pEditDelgaSize;
	igdeTextFieldReference pEditDelgaFileCount;
	igdeTextFieldReference pEditDelgaDirCount;
	igdeTextAreaReference pEditLogs;
	igdeButtonReference pBtnShowInFSMgr;
	
	projTaskDistribute *pTaskDistribute;
	decString pLastTaskMessage;
	
	bool pCloseDialogOnFinished;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	projDialogDistribute( projWindowMain &windowMain, projProfile *profile );
	
	/** \brief Clean up dialog. */
	virtual ~projDialogDistribute();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline projWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Profile. */
	inline projProfile &GetProfile() const{ return *pProfile; }
	
	/** \brief Add log message. */
	void LogMessage( const char *message );
	
	/**
	 * \brief Game like frame update while running modal.
	 * 
	 * Called while the dialog is inside Run(). Can be used to do updating on a regular basis.
	 */
	virtual void OnFrameUpdate();
	
	/** \brief Close dialog once finished. */
	void SetCloseDialogOnFinished( bool closeDialogOnFinished );
	/*@}*/
	
	
	
protected:
	void pStartBuilding();
};

#endif
