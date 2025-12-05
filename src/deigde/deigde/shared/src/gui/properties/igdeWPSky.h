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

#ifndef _IGDEWPSTATICSKY_H_
#define _IGDEWPSTATICSKY_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decObjectList.h>

#include "../igdeButton.h"
#include "../igdeContainer.h"
#include "../igdeLabel.h"
#include "../igdeTextField.h"
#include "../composed/igdeEditSliderText.h"
#include "../event/igdeActionListener.h"
#include "../event/igdeAction.h"
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
	class DE_DLL_EXPORT Controller : public deObject{
	public:
		typedef deTObjectReference<Controller> Ref;
	
	
		int controller;
		igdeLabel::Ref label;
		igdeEditSliderText::Ref slider;
		decString name;
		float minimum;
		float maximum;
	};
	
	
	
private:
	igdeWSky *pSky;
	
	igdeTextField::Ref pEditSkyPath;
	igdeButton::Ref pBtnSkyPath;
	igdeButton::Ref pBtnFromGDSky;
	
	igdeContainer::Ref pFraControllers;
	decObjectList pControllers;
	
	igdeAction::Ref pAction;
	
	
	
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
