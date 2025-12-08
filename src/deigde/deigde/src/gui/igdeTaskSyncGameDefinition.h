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
	
	igdeGameDefinition::Ref pOldProjectGameDef;
	igdeGameDefinition::Ref pOldGameDef;
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
	igdeTaskSyncGameDefinition(igdeWindowMain &windowMain);
	
	/** \brief Clean up task. */
	virtual ~igdeTaskSyncGameDefinition();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Reload XML Element Classes only. */
	void SetReloadXMLElementClasses(bool reloadXMLElementClasses);
	
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
	void pUpdateProgress(bool force);
	void pLoadProjectGameDefinition();
	void pCreateEditorTasks();
};

#endif
