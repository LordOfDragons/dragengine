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
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meWPViewListener> Ref;
	
	

private:
	meWPView &pPanel;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	meWPViewListener(meWPView &panel);
	
	/** \brief Clean up listener. */
	~meWPViewListener() override;
	/*@}*/
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Sky changed. */
	void SkyChanged(meWorld *world) override;
	
	/** Background object changed. */
	void BgObjectChanged(meWorld *world) override;
	
	/** \brief Limit box changed. */
	void LimitBoxChanged(meWorld *world) override;
	
	/** \brief Editing parameters changed. */
	void EditingChanged(meWorld *world) override;
	
	/** \brief Trigger table changed. */
	void TriggerTableChanged(meWorld *world) override;
	
	/** \brief Class (partial) hide tags changed. */
	void ClassHideTagsChanged(meWorld *world) override;
	
	/** \brief Object changed. */
	void ObjectChanged(meWorld *world, meObject *object) override;
	
	/** \brief Object properties changed. */
	void ObjectPropertiesChanged(meWorld *world, meObject *object) override;
	
	/** \brief Active camera changed. */
	void ActiveCameraChanged(meWorld *world) override;
	
	/** \brief Camera changed. */
	void CameraChanged(meWorld *world, meCamera *camera) override;
	/*@}*/
};

#endif
