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
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meWindowMainListener> Ref;
	
	

private:
	meWindowMain &pWindowMain;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	meWindowMainListener(meWindowMain &windowMain);
	
	/** \brief Clean up listener. */
	~meWindowMainListener() override;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Element or work mode changed. */
	void ModeChanged(meWorld *world) override;
	
	/** \brief Changed or saved state changed. */
	void StateChanged(meWorld *world) override;
	
	/** \brief Height terrain state changed. */
	void HTStateChanged(meWorld *world) override;
	
	/** \brief Undos changed. */
	void UndoChanged(meWorld *world) override;
	
	/** \brief Lighting parameters changed. */
	void LightingChanged(meWorld *world) override;
	
	/** \brief Editing parameters changed. */
	void EditingChanged(meWorld *world) override;
	
	/** \brief Object selection changed. */
	void ObjectSelectionChanged(meWorld *world) override;
	
	/** \brief Object changed. */
	void ObjectChanged(meWorld *world, meObject *object) override;
	
	/** \brief Decal selection changed. */
	void DecalSelectionChanged(meWorld *world) override;
	
	/** \brief Decal changed. */
	void DecalChanged(meWorld *world, meDecal *decal) override;
	/*@}*/
};

#endif
