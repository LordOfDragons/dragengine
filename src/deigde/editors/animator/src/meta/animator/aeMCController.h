/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _AEMCCONTROLLER_H_
#define _AEMCCONTROLLER_H_

#include <deigde/meta/igdeMetaContext.h>

class aeWindowMain;
class aeController;


/**
 * Controller meta context.
 */
class aeMCController : public igdeMetaContext{
public:
	using Ref = deTObjectReference<aeMCController>;
	
private:
	aeWindowMain &pWindowMain;
	aeController *pController;
	deTObjectReference<aeController> pGuardController;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create controller meta context. */
	explicit aeMCController(aeWindowMain &windowMain, aeController *controller);
	
protected:
	/**
	 * Clean up object.
	 */
	virtual ~aeMCController() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Window main. */
	inline aeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** Controller. */
	inline aeController *GetController() const{ return pController; }
	
	/** Controller reference. */
	aeController &GetControllerRef() const;
	
	/** Capture context. */
	Ref Capture() const;
	
	/** Undo system or nullptr to apply actions immediately. */
	igdeUndoSystem *GetUndoSystem() const override;
	
	/** \brief Clipboard or nullptr if not supported. */
	igdeClipboard *GetClipboard() const override;
	/*@}*/
};

#endif
