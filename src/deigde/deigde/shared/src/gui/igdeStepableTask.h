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

#ifndef _IGDESTEPABLETASK_H_
#define _IGDESTEPABLETASK_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief Stepable task.
 *
 * Base class for stepable tasks. Such tasks split lengthy operations into
 * manageable work units so called steps. The length of one step is not
 * defined but should be reasonably small to tell the user visually that
 * the task is still working. Each task has an info text and progress
 * value to communicate to the user the current state. After each step the
 * GUI updates with the stored information. Two changed flags help to
 * avoid updating if nothing changed. They are set by this base class
 * therefore subclasses do not have to worry about those flags. Just set
 * the status whenever it changes and leave the hard work for this class.
 * The return value of the Step call indicates if further steps are
 * required or if the task finished. Progress is measured from 0 to 1
 * usually displayed as 100 percent.
 */
class DE_DLL_EXPORT igdeStepableTask : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeStepableTask> Ref;
	
	
private:
	decString pMessage;
	float pProgress;
	bool pDirtyMessage;
	bool pDirtyProgress;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create task. */
	igdeStepableTask();
	
protected:
	/** \brief Clean up task. */
	~igdeStepableTask() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Message. */
	inline const decString &GetMessage() const{ return pMessage; }
	
	/** \brief Set message. */
	void SetMessage(const char *message);
	
	/** \brief Progress. */
	inline float GetProgress() const{ return pProgress; }
	
	/** \brief Set progress. */
	void SetProgress(float progress);
	
	/** \brief Message is dirty. */
	inline bool GetDirtyMessage() const{ return pDirtyMessage; }
	
	/** \brief Set message dirty flag. */
	void SetDirtyMessage(bool dirty);
	
	/** \brief Progress is dirty. */
	inline bool GetDirtyProgress() const{ return pDirtyProgress; }
	
	/** \brief Set if progress is dirty. */
	void SetDirtyProgress(bool dirty);
	
	
	
	/**
	 * \brief Run next step of task.
	 * 
	 * The step should consume a small amount of time to allow the GUI to stay responsive.
	 * In case of an error an exception has to be thrown which aborts the task.
	 * 
	 * \retval true Task requires more steps.
	 * \retval false Task finished after this step.
	 */
	virtual bool Step() = 0;
	/*@}*/
};

#endif
