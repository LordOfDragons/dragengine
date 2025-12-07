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

#ifndef _DEGLDGPPANELFILEFORMATS_H_
#define _DEGLDGPPANELFILEFORMATS_H_

#include "../../foxtoolkit.h"

class deglDialogGameProblems;
class delFileFormat;



/**
 * Game Problems Dialog File Formats Panel.
 * Shows problems with the file formats used by the game.
 */
class deglDGPPanelFileFormats : public FXVerticalFrame{
	FXDECLARE(deglDGPPanelFileFormats)
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
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create dialog. */
	deglDGPPanelFileFormats(deglDialogGameProblems *parentDialog, FXComposite *container);
	
	/** Clean up dialog. */
	virtual ~deglDGPPanelFileFormats();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parent dialog. */
	inline deglDialogGameProblems *GetParentDialog() const{ return pParentDialog; }
	
	/** System is working. */
	inline bool GetStatusWorking() const{ return pStatusWorking; }
	
	/** Update panel. */
	void UpdatePanel();
	
	/** Rebuild file format list. */
	void RebuildFormatList();
	/*@}*/
};

#endif
