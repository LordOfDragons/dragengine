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

#ifndef _DEALWIDGETPROGRESSBAR_H_
#define _DEALWIDGETPROGRESSBAR_H_

#include "dealWidgetLayoutStack.h"
#include "../../common/string/decString.h"

class dealWidgetLabel;


/**
 * \brief Progress bar with custom integer range.
 */
class dealWidgetProgressBar : public dealWidgetLayoutStack{
private:
	dealWidget *pBoxProgress;
	dealWidgetLabel *pLabelProgress;
	
	int pMinProgress;
	int pMaxProgress;
	int pProgress;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	dealWidgetProgressBar(dealDisplay &display);
	
	/** \brief Create widget. */
	dealWidgetProgressBar(dealDisplay &display, int minProgress, int maxProgress);
	
	/** \brief Clean up widget. */
	~dealWidgetProgressBar() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Minimum progress range. */
	inline int GetMinProgress() const{ return pMinProgress; }
	
	/** \brief Maximum progress range. */
	inline int GetMaxProgress() const{ return pMaxProgress; }
	
	/** \brief Set progress range. */
	void SetRange(int minProgress, int maxProgress);
	
	/** \brief Progress. */
	inline int GetProgress() const{ return pProgress; }
	
	/** \brief Set progress. */
	void SetProgress(int progress);
	
	/** \brief Progress as percentage from 0 to 1. */
	float GetProgressPercentage() const;
	
	
	
	/** \brief Size changed. */
	void OnSizeChanged() override;
	
	/** \brief Padding changed. */
	void OnPaddingChanged() override;
	/*@}*/
	
	
	
private:
	void pBuildContent();
	void pUpdateContent();
};

#endif
