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

#ifndef _IGDEDESTATUS_H_
#define _IGDEDESTATUS_H_

#include "../igdeTextArea.h"
#include "../igdeIconListBox.h"
#include "../igdeButton.h"
#include "../layout/igdeContainerFlow.h"

class igdeDialogEngine;
class deBaseSystem;


/**
 * Panel showing the status of the game engine. Can also start and stop
 * the engine.
 */
class DE_DLL_EXPORT igdeDEStatus : public igdeContainerFlow{
private:
	igdeDialogEngine &pDialogEngine;
	
	igdeTextArea::Ref pTextStatus;
	igdeIconListBox::Ref pListSystems;
	igdeButton::Ref pBtnStart;
	igdeButton::Ref pBtnStop;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	igdeDEStatus(igdeDialogEngine &windowEngine);
	
	
	
protected:
	/** \brief Cleans up panel. */
	virtual ~igdeDEStatus();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	void UpdateStatus();
	
	/** \brief Start game engine. */
	void StartEngine();
	
	/** \brief Stop game engine. */
	void StopEngine();
	/*@}*/
	
	
	
private:
	void pAddSystem(deBaseSystem *system);
};

#endif
