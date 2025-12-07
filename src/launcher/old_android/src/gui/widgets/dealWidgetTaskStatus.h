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

#ifndef _DEALWIDGETTASKSTATUS_H_
#define _DEALWIDGETTASKSTATUS_H_

#include "dealWidgetLayoutStack.h"
#include "../../common/string/decString.h"

class dealWidgetLabel;
class dealWidgetProgressBar;


/**
 * \brief Display status of a task for example installing something.
 */
class dealWidgetTaskStatus : public dealWidgetLayoutStack{
private:
	dealWidgetLabel *pLabelName;
	dealWidgetLabel *pLabelStatus;
	dealWidgetProgressBar *pProgressBar;
	dealWidget *pBgPending;
	dealWidget *pBgFinished;
	
	decString pName;
	decString pStatus;
	
	bool pFinished;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	dealWidgetTaskStatus(dealDisplay &display);
	
	/** \brief Clean up widget. */
	virtual ~dealWidgetTaskStatus();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Task name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set task name. */
	void SetName(const char *name);
	
	/** \brief Task status text. */
	inline const decString &GetStatus() const{ return pStatus; }
	
	/** \brief Set task status text. */
	void SetStatus(const char *text);
	
	/** \brief Task progress in the range from 0 to progress range. */
	float GetProgress() const;
	
	/** \brief Set task progress range. */
	void SetProgressRange(int range);
	
	/** \brief Set task progress in the range from 0 to progress range - 1. */
	void SetProgress(int progress);
	
	
	
	/** \brief Finished. */
	inline bool GetFinished() const{ return pFinished; }
	
	/** \brief Set finished. */
	void SetFinished(bool finished);
	/*@}*/
	
	
	
private:
	void pBuildContent();
};

#endif
