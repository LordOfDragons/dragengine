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

#ifndef _IGDEWPSTATICSKY_H_
#define _IGDEWPSTATICSKY_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decObjectList.h>

#include "../igdeButtonReference.h"
#include "../igdeContainerReference.h"
#include "../igdeLabelReference.h"
#include "../igdeTextFieldReference.h"
#include "../composed/igdeEditSliderTextReference.h"
#include "../event/igdeActionListener.h"
#include "../event/igdeActionReference.h"
#include "../layout/igdeContainerFlow.h"


class igdeWSky;


/**
 * \brief Sky wrapper properties panel.
 * 
 * Provides support for configurating a sky wrapper objects. This panel is supposed
 * to be used on a properties window typically inside a group box. A sky wrapper is
 * useful for editors to provide a sky in a world for quick editing. The sky wrapper
 * makes it easy to setup a sky from a game definition sky entry or by proiding a
 * custom sky object. This class provides the support to easily change the
 * configuraton of such a sky wrapper with little code. To use this class assign a
 * sky wrapper object. Whenever a change is made SEL_COMMAND is send to the target
 * of this widget. The sky wrapper object is not held in any way. The application is
 * responsible to ensure the wrapper object exists as long as it is assigned to
 * this widget.
 */
class DE_DLL_EXPORT igdeWPSky : public igdeContainerFlow, igdeActionListener{
private:
	class Controller : public deObject{
	public:
		typedef deTObjectReference<Controller> Ref;
		
		int controller;
		igdeLabelReference label;
		igdeEditSliderTextReference slider;
		decString name;
		float minimum;
		float maximum;
	};
	
	
	
private:
	igdeWSky *pSky;
	
	igdeTextFieldReference pEditSkyPath;
	igdeButtonReference pBtnSkyPath;
	igdeButtonReference pBtnFromGDSky;
	
	igdeContainerReference pFraControllers;
	decObjectList pControllers;
	
	igdeActionReference pAction;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	igdeWPSky( igdeEnvironment &environment );
	
	/** \brief Create panel. */
	igdeWPSky( igdeEnvironment &environment, igdeAction *action );
	
	
	
protected:
	/** \brief Clean up panel. */
	virtual ~igdeWPSky();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Sky wrapper or NULL if not set. */
	inline igdeWSky *GetSky() const{ return pSky; }
	
	/** \brief Set sky wrapper or NULL if not set. */
	void SetSky( igdeWSky *sky );
	
	/** \brief Update widget after sky changed outside. */
	void UpdateSky();
	
	/** \brief Rebuild controllers list. */
	void RebuildControllers();
	
	/** \brief Update controller. */
	void UpdateController( int controller );
	
	/** \brief Update controller value. */
	void UpdateControllerValue( int controller );
	
	
	
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
