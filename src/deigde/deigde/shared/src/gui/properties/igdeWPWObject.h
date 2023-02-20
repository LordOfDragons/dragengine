/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _IGDEWPWOBJECT_H_
#define _IGDEWPWOBJECT_H_

#include <dragengine/common/math/decMath.h>

#include "../igdeButtonReference.h"
#include "../igdeCheckBoxReference.h"
#include "../igdeContainerReference.h"
#include "../igdeTextFieldReference.h"
#include "../composed/igdeEditVectorReference.h"
#include "../event/igdeActionListener.h"
#include "../event/igdeActionReference.h"
#include "../layout/igdeContainerFlow.h"


class igdeWObject;


/**
 * \brief Window properties wrapper object.
 * 
 * Provides support for configurating a wrapper object. This panel is supposed to be used
 * on a properties window typically inside a group box. An object wrapper is useful for
 * editors to provide a static environments or adding objects to an environment to test
 * features of whatever they are editing. The object wrapper makes it easy to setup an
 * object using a game definition object class. In addition properties of the wrapped object
 * can be set to configured certain object features. To use this class assign an wrapper
 * object. Whenever a change is made the set action listener is called. The object wrapper
 * object is not held in any way. The application is responsible to ensure the wrapper
 * object exists as long as it is assigned to this widget.
 */
class DE_DLL_EXPORT igdeWPWObject : public igdeContainerFlow, igdeActionListener{
private:
	igdeWObject *pObject;
	
	igdeTextFieldReference pEditClass;
	igdeButtonReference pBtnClass;
	
	igdeEditVectorReference pEditPosition;
	igdeEditVectorReference pEditOrientation;
	igdeEditVectorReference pEditScaling;
	
	igdeCheckBoxReference pChkVisible;
	igdeCheckBoxReference pChkDynamicCollider;
	
	igdeActionReference pAction;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	igdeWPWObject( igdeEnvironment &environment );
	
	/** \brief Create panel. */
	igdeWPWObject( igdeEnvironment &environment, igdeAction *action );
	
	
	
protected:
	/** \brief Clean up panel. */
	virtual ~igdeWPWObject();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Object or NULL if not set. */
	inline igdeWObject *GetObject() const{ return pObject; }
	
	/** \brief Set object or NULL to clear. */
	void SetObject( igdeWObject *object );
	
	/** \brief Update widget after object changed outside. */
	void UpdateObject();
	
	
	
	/** \deprecated */
	inline igdeWObject *GetObjectWrapper() const{ return GetObject(); }
	inline void SetObjectWrapper( igdeWObject *objectWrapper ){ SetObject( objectWrapper ); }
	inline void UpdateObjectWrapper(){ UpdateObject(); }
	
	
	
	/** \brief Action or NULL. */
	inline igdeAction *GetAction() const{ return pAction; }
	
	/** \brief Set action or NULL. */
	void SetAction( igdeAction *action );
	
	/**
	 * \brief Sky parameters changed.
	 * 
	 * Called if user changes any of the sky parameters. Default implementation calls
	 * OnAction on set action object if present.
	 */
	virtual void OnAction();
	
	/** \brief Action parameters changed. */
	virtual void OnParameterChanged( igdeAction *action );
	
	/** \brief Action has been destroyed. */
	virtual void OnDestroyed( igdeAction *action );
	/*@}*/
	
	
	
private:
	void pCreateContent();
};

#endif
