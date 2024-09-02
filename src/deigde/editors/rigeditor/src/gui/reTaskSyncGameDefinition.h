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

#ifndef _RAETASKSYNCGAMEDEFINITION_H_
#define _RETASKSYNCGAMEDEFINITION_H_

#include <deigde/gui/igdeStepableTask.h>

class reWindowMain;



/**
 * \brief Synchronize game definition task.
 */
class reTaskSyncGameDefinition : public igdeStepableTask{
private:
	reWindowMain &pWindowMain;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create task. */
	reTaskSyncGameDefinition( reWindowMain &windowMain );
	
	/** \brief Clean up task. */
	virtual ~reTaskSyncGameDefinition();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
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
};

#endif
