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

#ifndef _SKYEWINDOWMAINLISTENER_H_
#define _SKYEWINDOWMAINLISTENER_H_

#include "../sky/skyeSkyListener.h"

class skyeWindowMain;



/**
 * \brief Window main listener.
 */
class skyeWindowMainListener : public skyeSkyListener{
public:
	typedef deTObjectReference<skyeWindowMainListener> Ref;
	
private:
	skyeWindowMain &pWindow;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	skyeWindowMainListener(skyeWindowMain &window);
	
	/** \brief Clean up listener. */
protected:
	~skyeWindowMainListener() override;
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Changed or saved state changed. */
	void StateChanged(skyeSky *sky) override;
	
	/** \brief Undo changed. */
	void UndoChanged(skyeSky *sky) override;
	
	/** \brief The view changed. */
	void ViewChanged(skyeSky *sky) override;
	/*@}*/
};

#endif
