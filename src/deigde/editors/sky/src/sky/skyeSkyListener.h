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

#ifndef _SKYESKYLISTENER_H_
#define _SKYESKYLISTENER_H_

#include <dragengine/deObject.h>
#include <dragengine/resources/sky/deSkyLayer.h>

class skyeBody;
class skyeController;
class skyeLayer;
class skyeLink;
class skyeSky;



/**
 * \brief Sky listener.
 */
class skyeSkyListener : public deObject{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<skyeSkyListener> Ref;


public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	skyeSkyListener();
	
protected:
	/** \brief Clean up listener. */
	virtual ~skyeSkyListener();
	/*@}*/
	
	
	
public:
	/** \name Notifications */
	/*@{*/
	/** \brief Changed or saved state changed. */
	virtual void StateChanged(skyeSky *sky);
	
	/** \brief Undo changed. */
	virtual void UndoChanged(skyeSky *sky);
	
	/** \brief Sky parameters changed. */
	virtual void SkyChanged(skyeSky *sky);
	
	/** \brief The environment object changed. */
	virtual void EnvObjectChanged(skyeSky *sky);
	
	/** \brief The view changed. */
	virtual void ViewChanged(skyeSky *sky);
	
	/** \brief The camera changed. */
	virtual void CameraChanged(skyeSky *sky);
	
	
	
	/** \brief Controller added or removed. */
	virtual void ControllerStructureChanged (skyeSky *sky);
	
	/** \brief Controller changed. */
	virtual void ControllerChanged(skyeSky *sky, skyeController *controller);
	
	/** \brief Controller name changed. */
	virtual void ControllerNameChanged(skyeSky *sky, skyeController *controller);
	
	/** \brief Controller changed value. */
	virtual void ControllerValueChanged(skyeSky *sky, skyeController *controller);
	
	/** \brief Controller selection changed. */
	virtual void ControllerSelectionChanged(skyeSky *sky);
	
	/** \brief Active controller changed. */
	virtual void ActiveControllerChanged(skyeSky *sky);
	
	
	
	/** \brief Link added or removed. */
	virtual void LinkStructureChanged (skyeSky *sky);
	
	/** \brief Link changed. */
	virtual void LinkChanged(skyeSky *sky, skyeLink *link);
	
	/** \brief Link name changed. */
	virtual void LinkNameChanged(skyeSky *sky, skyeLink *link);
	
	/** \brief Link selection changed. */
	virtual void LinkSelectionChanged(skyeSky *sky);
	
	/** \brief Active controller changed. */
	virtual void ActiveLinkChanged(skyeSky *sky);
	
	
	
	/** \brief Layer added or removed. */
	virtual void LayerStructureChanged (skyeSky *sky);
	
	/** \brief Layer changed. */
	virtual void LayerChanged(skyeSky *sky, skyeLayer *layer);
	
	/** \brief Layer name changed. */
	virtual void LayerNameChanged(skyeSky *sky, skyeLayer *layer);
	
	/** \brief Layer selection changed. */
	virtual void LayerSelectionChanged(skyeSky *sky);
	
	/** \brief Active layer changed. */
	virtual void ActiveLayerChanged(skyeSky *sky);
	
	
	
	/** \brief Body added or removed. */
	virtual void BodyStructureChanged (skyeSky *sky, skyeLayer *layer);
	
	/** \brief Body changed. */
	virtual void BodyChanged(skyeSky *sky, skyeLayer *layer, skyeBody *body);
	
	/** \brief Body selection changed. */
	virtual void BodySelectionChanged(skyeSky *sky, skyeLayer *layer);
	
	/** \brief Active body changed. */
	virtual void ActiveBodyChanged(skyeSky *sky, skyeLayer *layer);
	
	
	
	/** \brief Target changed. */
    virtual void TargetChanged(skyeSky *sky, skyeLayer *layer, deSkyLayer::eTargets target);
	
	/** \brief Active target changed. */
	virtual void ActiveTargetChanged(skyeSky *sky, skyeLayer *layer);
	/*@}*/
};

#endif
