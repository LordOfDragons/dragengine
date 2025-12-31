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

#ifndef _GDEWINDOWMAINLISTENER_H_
#define _GDEWINDOWMAINLISTENER_H_

#include "../gamedef/gdeGameDefinitionListener.h"

class gdeWindowMain;



/**
 * \brief Main window game definition listener.
 */
class gdeWindowMainListener : public gdeGameDefinitionListener{
public:
	typedef deTObjectReference<gdeWindowMainListener> Ref;
	
private:
	gdeWindowMain &pWindow;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	gdeWindowMainListener(gdeWindowMain &window);
	
	/** \brief Clean up listener. */
protected:
	~gdeWindowMainListener() override;
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Changed or saved state changed. */
	void StateChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Undo changed. */
	void UndoChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief View properties changed. */
	void ViewChanged(gdeGameDefinition *gameDefinition) override;
	/*@}*/
};

#endif
