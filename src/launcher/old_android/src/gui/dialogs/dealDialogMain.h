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

#ifndef _DEALDIALOGMAIN_H_
#define _DEALDIALOGMAIN_H_

#include "../dealDialog.h"
#include "dealWidgetButton.h"


class dealWidgetTaskStatus;


/**
 * \brief Main dialog.
 * 
 */
class dealDialogMain : public dealDialog{
private:
	dealWidgetTaskStatus *pTaskCheckEngine;
	dealWidgetButton::Ref pButtonRun;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	dealDialogMain(dealDisplay &display);
	
	/** \brief Clean up dialog. */
	~dealDialogMain() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Check engine task. */
	inline dealWidgetTaskStatus &GetTaskCheckEngine(){ return *pTaskCheckEngine; }
	
	
	
	/** \brief Dialog has been activated. */
	void OnActivate() override;
	
	/** \brief Dialog has been deactivated. */
	void OnDeactivate() override;
	
	
	
	/** \brief Set if run button is enabled. */
	void SetRunEnabled(bool enabled);
	/*@}*/
	
	
	
private:
	void pBuildContent();
};

#endif
