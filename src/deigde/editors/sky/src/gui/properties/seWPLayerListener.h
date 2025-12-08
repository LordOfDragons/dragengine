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
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<seWPLayerListener> Ref;
	
	

private:
	seWPLayer &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seWPLayerListener(seWPLayer &panel);
	
protected:
	/** \brief Clean up listener. */
	~seWPLayerListener() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Link added or removed. */
	void LinkStructureChanged (seSky *sky) override;
	
	/** \brief Link name changed. */
	void LinkNameChanged(seSky *sky, seLink *link) override;
	
	
	
	/** \brief Layer added or removed. */
	void LayerStructureChanged (seSky *sky) override;
	
	/** \brief Layer changed. */
	void LayerChanged(seSky *sky, seLayer *layer) override;
	
	/** \brief Layer name changed. */
	void LayerNameChanged(seSky *sky, seLayer *layer) override;
	
	/** \brief Active layer changed. */
	void ActiveLayerChanged(seSky *sky) override;
	
	
	
	/** \brief Body added or removed. */
	void BodyStructureChanged (seSky *sky, seLayer *layer) override;
	
	/** \brief Body changed. */
	void BodyChanged(seSky *sky, seLayer *layer, seBody *body) override;
	
	/** \brief Active body changed. */
	void ActiveBodyChanged(seSky *sky, seLayer *layer) override;
	
	
	
	/** \brief Target changed. */
    virtual void TargetChanged(seSky *sky, seLayer *layer, deSkyLayer::eTargets target);
	
	/** \brief Active target changed. */
	void ActiveTargetChanged(seSky *sky, seLayer *layer) override;
	/*@}*/
};

#endif
