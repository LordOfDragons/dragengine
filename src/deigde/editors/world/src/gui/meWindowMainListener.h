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

#ifndef _MEWINDOWMAINLISTENER_H_
#define _MEWINDOWMAINLISTENER_H_

#include "../world/meWorldNotifier.h"

class meWindowMain;



/**
 * \brief Window Main Listener.
 */
class meWindowMainListener : public meWorldNotifier{
private:
	meWindowMain &pWindowMain;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	meWindowMainListener(meWindowMain &windowMain);
	
	/** \brief Clean up listener. */
	virtual ~meWindowMainListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Element or work mode changed. */
	virtual void ModeChanged(meWorld *world);
	
	/** \brief Changed or saved state changed. */
	virtual void StateChanged(meWorld *world);
	
	/** \brief Height terrain state changed. */
	virtual void HTStateChanged(meWorld *world);
	
	/** \brief Undos changed. */
	virtual void UndoChanged(meWorld *world);
	
	/** \brief Lighting parameters changed. */
	virtual void LightingChanged(meWorld *world);
	
	/** \brief Editing parameters changed. */
	virtual void EditingChanged(meWorld *world);
	
	/** \brief Object selection changed. */
	virtual void ObjectSelectionChanged(meWorld *world);
	
	/** \brief Object changed. */
	virtual void ObjectChanged(meWorld *world, meObject *object);
	
	/** \brief Decal selection changed. */
	virtual void DecalSelectionChanged(meWorld *world);
	
	/** \brief Decal changed. */
	virtual void DecalChanged(meWorld *world, meDecal *decal);
	/*@}*/
};

#endif
