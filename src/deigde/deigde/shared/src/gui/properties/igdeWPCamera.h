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

#ifndef _IGDEWPCAMERA_H_
#define _IGDEWPCAMERA_H_

#include <dragengine/common/math/decMath.h>

#include "../igdeTextFieldReference.h"
#include "../composed/igdeEditVectorReference.h"
#include "../event/igdeActionListener.h"
#include "../event/igdeActionReference.h"
#include "../layout/igdeContainerFlow.h"


class igdeCamera;


/**
 * \brief Window Properties Camera.
 * 
 * Provides support for configurating a wrapper camera. This panel is supposed to be used
 * on a properties window typically inside a group box. An camera wrapper is useful for
 * editors to provide a static environments or adding cameras to an environment to test
 * features of whatever they are editing. The camera wrapper makes it easy to setup an
 * camera using a game definition camera class. In addition properties of the wrapped camera
 * can be set to configured certain camera features. To use this class assign an wrapper
 * camera. Whenever a change is made the set action listener is called. The camera wrapper
 * camera is not held in any way. The application is responsible to ensure the wrapper
 * camera exists as long as it is assigned to this widget.
 */
class igdeWPCamera : public igdeContainerFlow, igdeActionListener{
private:
	igdeCamera *pCamera;
	
	igdeEditVectorReference pEditPosition;
	igdeEditVectorReference pEditRotation;
	igdeEditVectorReference pEditViewDir;
	igdeTextFieldReference pEditOrbitDistance;
	igdeTextFieldReference pEditFov;
	igdeTextFieldReference pEditFovRatio;
	igdeTextFieldReference pEditImageDist;
	igdeTextFieldReference pEditViewDist;
	igdeTextFieldReference pEditExposure;
	igdeTextFieldReference pEditLowInt;
	igdeTextFieldReference pEditHiInt;
	igdeTextFieldReference pEditAdaptTime;
	
	igdeActionReference pAction;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	igdeWPCamera( igdeEnvironment &environment );
	
	/** \brief Create panel. */
	igdeWPCamera( igdeEnvironment &environment, igdeAction *action );
	
	
	
protected:
	/** \brief Clean up panel. */
	virtual ~igdeWPCamera();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Camera or NULL if not set. */
	inline igdeCamera *GetCamera() const{ return pCamera; }
	
	/** \brief Set camera or NULL if not set. */
	void SetCamera( igdeCamera *camera );
	
	/** \brief Update widget after camera changed outside. */
	void UpdateCamera();
	
	/** \brief Update camera view direction only. */
	void UpdateViewDirection();
	
	
	
	/** \brief Action or NULL. */
	inline igdeAction *GetAction() const{ return pAction; }
	
	/** \brief Set action or NULL. */
	void SetAction( igdeAction *action );
	
	/**
	 * \brief Sky parameters changed.
	 * 
	 * Called if user changes any of the sky parameters. Default implementation calls
	 * OnAction on set action camera if present.
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
