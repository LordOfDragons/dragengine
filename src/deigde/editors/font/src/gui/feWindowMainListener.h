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

#ifndef _FEWINDOWMAINLISTENER_H_
#define _FEWINDOWMAINLISTENER_H_

#include "../font/feFontNotifier.h"

class feWindowMain;



/**
 * \brief Window editor listener.
 */
class feWindowMainListener : public feFontNotifier{
public:
	typedef deTObjectReference<feWindowMainListener> Ref;
	
private:
	feWindowMain &pWindow;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	feWindowMainListener(feWindowMain &window);
	
	/** \brief Clean up listener. */
protected:
	~feWindowMainListener() override;
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Element or work mode changed. */
	void ModeChanged(feFont *font) override;
	
	/** \brief Changed or saved state changed. */
	void StateChanged(feFont *font) override;
	
	/** \brief Undos changed. */
	void UndoChanged(feFont *font) override;
	/*@}*/
};

#endif
