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

#ifndef _SEWPLAYERLISTENER_H_
#define _SEWPLAYERLISTENER_H_

#include "../../sky/seSkyListener.h"

class seWPLayer;



/**
 * \brief Layer properties panel listener.
 */
class seWPLayerListener : public seSkyListener{
private:
	seWPLayer &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seWPLayerListener(seWPLayer &panel);
	
protected:
	/** \brief Clean up listener. */
	virtual ~seWPLayerListener();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Link added or removed. */
	virtual void LinkStructureChanged (seSky *sky);
	
	/** \brief Link name changed. */
	virtual void LinkNameChanged(seSky *sky, seLink *link);
	
	
	
	/** \brief Layer added or removed. */
	virtual void LayerStructureChanged (seSky *sky);
	
	/** \brief Layer changed. */
	virtual void LayerChanged(seSky *sky, seLayer *layer);
	
	/** \brief Layer name changed. */
	virtual void LayerNameChanged(seSky *sky, seLayer *layer);
	
	/** \brief Active layer changed. */
	virtual void ActiveLayerChanged(seSky *sky);
	
	
	
	/** \brief Body added or removed. */
	virtual void BodyStructureChanged (seSky *sky, seLayer *layer);
	
	/** \brief Body changed. */
	virtual void BodyChanged(seSky *sky, seLayer *layer, seBody *body);
	
	/** \brief Active body changed. */
	virtual void ActiveBodyChanged(seSky *sky, seLayer *layer);
	
	
	
	/** \brief Target changed. */
    virtual void TargetChanged(seSky *sky, seLayer *layer, deSkyLayer::eTargets target);
	
	/** \brief Active target changed. */
	virtual void ActiveTargetChanged(seSky *sky, seLayer *layer);
	/*@}*/
};

#endif
