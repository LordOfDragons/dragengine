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

#ifndef _SEVCIKEYHANDLING_H_
#define _SEVCIKEYHANDLING_H_

#include <deigde/gui/event/igdeMouseKeyListener.h>

class seViewConstructedView;



/**
 * \brief Key handling listener.
 * 
 * Process single key strokes to edit nodes but only if no mouse interaction is going on.
 */
class seVCIKeyHandling : public igdeMouseKeyListener{
private:
	seViewConstructedView &pView;
	bool pIsLeftMouseButtonPressed;
	
	
	
public:
	using Ref = deTObjectReference<seVCIKeyHandling>;
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seVCIKeyHandling(seViewConstructedView &view);
	
	/** \brief Clean up listener. */
protected:
	~seVCIKeyHandling() override;
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief User pressed down key while view is focused.
	 */
	void OnKeyPress(igdeWidget *widget, deInputEvent::eKeyCodes keyCode, int key) override;
	
	/**
	 * \brief User released down key while view is focused.
	 */
	void OnKeyRelease(igdeWidget *widget, deInputEvent::eKeyCodes keyCode, int key) override;
	
	/**
	 * \brief User pressed down mouse button.
	 */
	void OnButtonPress(igdeWidget *widget, int button, const decPoint &position, int modifiers) override;
	
	/**
	 * \brief User released down mouse button.
	 */
	void OnButtonRelease(igdeWidget *widget, int button, const decPoint &position, int modifiers) override;
	
	/**
	 * \brief User wheeled mouse.
	 */
	void OnMouseWheeled(igdeWidget *widget, const decPoint &position, const decPoint &change, int modifiers) override;
	
	/**
	 * \brief User double clicked mouse button.
	 */
	void OnDoubleClicked(igdeWidget *widget, int button, const decPoint &position, int modifiers) override;
	/*@}*/
	
	
	
protected:
	void ShowContextMenu(const decPoint &position);
};

#endif
