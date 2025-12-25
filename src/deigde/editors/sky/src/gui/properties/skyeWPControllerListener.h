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

#ifndef _SKYEWPCONTROLLERLISTENER_H_
#define _SKYEWPCONTROLLERLISTENER_H_

#include "../../sky/skyeSkyListener.h"

class skyeWPController;


/**
 * \brief Controller properties panel listener.
 */
class skyeWPControllerListener : public skyeSkyListener{
public:
	typedef deTObjectReference<skyeWPControllerListener> Ref;
	
private:
	skyeWPController &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	skyeWPControllerListener(skyeWPController &panel);
	
	/** \brief Clean up listener. */
	~skyeWPControllerListener() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Controller added or removed. */
	void ControllerStructureChanged (skyeSky *sky) override;
	
	/** \brief Controller changed. */
	void ControllerChanged(skyeSky *sky, skyeController *controller) override;
	
	/** \brief Controller name changed. */
	void ControllerNameChanged(skyeSky *sky, skyeController *controller) override;
	
	/** \brief Controller changed value. */
	void ControllerValueChanged(skyeSky *sky, skyeController *controller) override;
	
	/** \brief Active controller changed. */
	void ActiveControllerChanged(skyeSky *sky) override;
	/*@}*/
};

#endif
