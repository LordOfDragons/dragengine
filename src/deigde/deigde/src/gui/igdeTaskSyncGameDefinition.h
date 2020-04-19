/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDETASKSYNCGAMEDEFINITION_H_
#define _IGDETASKSYNCGAMEDEFINITION_H_

#include <deigde/gui/igdeStepableTask.h>

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/string/decStringList.h>

class igdeWindowMain;
class igdeGameDefinition;



/**
 * \brief Synchronize game definition task.
 * 
 * Synchronizes game definition for all editor modules.
 */
class igdeTaskSyncGameDefinition : public igdeStepableTask{
protected:
	/** \brief States. */
	enum eStates{
		/** \brief Reload project game definition. */
		esReloadProjectGameDef,
		
		/** \brief Process editor update tasks. */
		esEditorUpdateTasks,
		
		/** \brief Finished. */
		esFinished
	};
	
	
	
private:
	igdeWindowMain &pWindowMain;
	decPointerList pEditorTasks;
	
	igdeGameDefinition *pOldProjectGameDef;
	igdeGameDefinition *pOldGameDef;
	eStates pState;
	int pTaskIndex;
	
	float pLastProgress;
	decString pLastMessage;
	
	float pBaseProgress;
	float pProgressFactor;
	
	bool pReloadXMLElementClasses;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create task. */
	igdeTaskSyncGameDefinition( igdeWindowMain &windowMain );
	
	/** \brief Clean up task. */
	virtual ~igdeTaskSyncGameDefinition();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Reload XML Element Classes only. */
	void SetReloadXMLElementClasses( bool reloadXMLElementClasses );
	
	/**
	 * \brief Run next step of task.
	 * 
	 * The step should consume a small amount of time to allow the GUI to stay responsive.
	 * In case of an error an exception has to be thrown which aborts the task.
	 * 
	 * \retval true Task requires more steps.
	 * \retval false Task finished after this step.
	 */
	virtual bool Step();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pUpdateProgress( bool force );
	void pLoadProjectGameDefinition();
	void pCreateEditorTasks();
};

#endif
