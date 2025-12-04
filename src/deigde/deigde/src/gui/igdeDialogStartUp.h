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

#ifndef _IGDEDIALOGSTARTUP_H_
#define _IGDEDIALOGSTARTUP_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/dialog/igdeDialog.h>
#include <deigde/gui/event/igdeAction::Ref.h>


class igdeWindowMain;


/**
 * \brief Start-Up dialog.
 * 
 * Shows the last loaded documents and buttons to either load an enty
 * from the list, loading a file from a dialog, creating a new game
 * project or closing the application.
 */
class igdeDialogStartUp : public igdeDialog{
private:
	igdeWindowMain &pWindowMain;
	
	igdeListBoxReference pListRecentProjects;
	
	igdeAction::Ref pActionLoadRecent;
	igdeAction::Ref pActionLoadFile;
	igdeAction::Ref pActionNewProject;
	igdeAction::Ref pActionQuit;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	igdeDialogStartUp( igdeWindowMain &windowMain );
	
protected:
	/** \brief Clean up dialog. */
	virtual ~igdeDialogStartUp();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline igdeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Load selected recent game project. */
	void LoadSelectedRecentProject();
	
	/** \brief Load game project from file. */
	void LoadProjectFromFile();
	
	/** \brief Create new game project. */
	void NewGameProject();
	/*@}*/
	
	
	
private:
	void pUpdateRecentProjectList();
};

#endif
