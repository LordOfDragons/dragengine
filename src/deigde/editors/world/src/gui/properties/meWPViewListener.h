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

#ifndef _MEWPVIEWLISTENER_H_
#define _MEWPVIEWLISTENER_H_

#include "../../world/meWorldNotifier.h"

class meWPView;



/**
 * \brief View Panel Listener
 */
class meWPViewListener : public meWorldNotifier{
private:
	meWPView &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	meWPViewListener( meWPView &panel );
	
	/** \brief Clean up listener. */
	virtual ~meWPViewListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Sky changed. */
	virtual void SkyChanged( meWorld *world );
	
	/** Background object changed. */
	virtual void BgObjectChanged( meWorld *world );
	
	/** \brief Editing parameters changed. */
	virtual void EditingChanged( meWorld *world );
	
	/** \brief Trigger table changed. */
	virtual void TriggerTableChanged( meWorld *world );
	
	/** \brief Class (partial) hide tags changed. */
	virtual void ClassHideTagsChanged( meWorld *world );
	
	/** \brief Object changed. */
	virtual void ObjectChanged( meWorld *world, meObject *object );
	
	/** \brief Object properties changed. */
	virtual void ObjectPropertiesChanged( meWorld *world, meObject *object );
	
	/** \brief Active camera changed. */
	virtual void ActiveCameraChanged( meWorld *world );
	
	/** \brief Camera changed. */
	virtual void CameraChanged( meWorld *world, meCamera *camera );
	/*@}*/
};

#endif
