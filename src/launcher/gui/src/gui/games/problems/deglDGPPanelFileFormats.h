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

#ifndef _DEGLDGPPANELFILEFORMATS_H_
#define _DEGLDGPPANELFILEFORMATS_H_

#include "../../foxtoolkit.h"

class deglDialogGameProblems;
class deglFileFormat;



/**
 * @brief Game Problems Dialog File Formats Panel.
 * Shows problems with the file formats used by the game.
 */
class deglDGPPanelFileFormats : public FXVerticalFrame{
	FXDECLARE( deglDGPPanelFileFormats )
protected:
	deglDGPPanelFileFormats();
	
public:
	enum eFoxIDs{
		ID_SELF = FXVerticalFrame::ID_LAST,
		ID_LIST_FORMATS,
		ID_LAST
	};
	
private:
	deglDialogGameProblems *pParentDialog;
	bool pStatusWorking;
	
	FXIconList *pListFormats;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new dialog. */
	deglDGPPanelFileFormats( deglDialogGameProblems *parentDialog, FXComposite *container );
	/** Cleans up the dialog. */
	virtual ~deglDGPPanelFileFormats();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the parent dialog. */
	inline deglDialogGameProblems *GetParentDialog() const{ return pParentDialog; }
	/** Determines if the system is working. */
	inline bool GetStatusWorking() const{ return pStatusWorking; }
	
	/** Update panel. */
	void UpdatePanel();
	/** Rebuild file format list. */
	void RebuildFormatList();
	/*@}*/
	
	/** @name Events */
	/*@{*/
	/*@}*/
	
private:
};

// end of include only once
#endif
