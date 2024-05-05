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

#ifndef _IGDEACTIONLISTENER_H_
#define _IGDEACTIONLISTENER_H_

#include <dragengine/dragengine_export.h>

class igdeAction;


/**
 * \brief IGDE UI Action Listener.
 * 
 * Listener used by UI elements connected to an igdeAction instance to be notified if
 * action parameters change. Action listeners are stored as pointer so the subclass
 * has to keep a reference to the action instance.
 */
class DE_DLL_EXPORT igdeActionListener{
protected:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	igdeActionListener();
	
	/** \brief Clean up action listener. */
	virtual ~igdeActionListener();
	/*@}*/
	
	
	
public:
	/** \text Management */
	/*@{*/
	/** \brief Action parameters changed. */
	virtual void OnParameterChanged( igdeAction *action ) = 0;
	
	/** \brief Action has been destroyed. */
	virtual void OnDestroyed( igdeAction *action ) = 0;
	/*@}*/
};

#endif
