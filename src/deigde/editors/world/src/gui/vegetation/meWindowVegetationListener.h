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

#ifndef _MEWINDOWVEGETATIONLISTENER_H_
#define _MEWINDOWVEGETATIONLISTENER_H_

#include "../../world/meWorldNotifier.h"

class meWindowVegetation;



/**
 * \brief Vegetation Editing Window Listener.
 */
class meWindowVegetationListener : public meWorldNotifier{
private:
	meWindowVegetation &pWindow;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meWindowVegetationListener( meWindowVegetation &window );
	
	/** \brief Clean up object. */
	virtual ~meWindowVegetationListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Active sector changed. */
	virtual void ActiveSectorChanged( meWorld *world );
	
	/** \brief Height terrain sector vegetation layer count changed. */
	virtual void HTVLayerCountChanged( meWorld *world );
	
	/** \brief Height terrain sector active vegetation layer changed. */
	virtual void HTActiveVLayerChanged( meWorld *world );
	
	/** \brief Height terrain sector vegetation layer changed. */
	virtual void HTVLayerChanged( meWorld *world,
		meHTVegetationLayer *vlayer );
	
	/** \brief Height terrain sector vegetation layer variation count changed. */
	virtual void HTVLVariationCountChanged( meWorld *world,
		meHTVegetationLayer *vlayer );
	
	/** \brief Height terrain sector active vegetation layer variation changed. */
	virtual void HTVLActiveVariationChanged( meWorld *world,
		meHTVegetationLayer *vlayer );
	
	/** \brief Height terrain sector vegetation layer variation changed. */
	virtual void HTVLVariationChanged( meWorld *world,
		meHTVegetationLayer *vlayer, meHTVVariation *variation );
	
	/** \brief Height terrain sector vegetation layer rule count changed. */
	virtual void HTVLRuleCountChanged( meWorld *world,
		meHTVegetationLayer *vlayer );
	
	/** \brief Height terrain sector active vegetation layer rule changed. */
	virtual void HTVLActiveRuleChanged( meWorld *world,
		meHTVegetationLayer *vlayer );
	
	/** \brief Height terrain sector vegetation layer rule changed. */
	virtual void HTVLRuleChanged( meWorld *world,
		meHTVegetationLayer *vlayer, meHTVRule *rule );
	
	/** \brief Height terrain sector vegetation layer rule moved. */
	virtual void HTVLRuleMoved( meWorld *world,
		meHTVegetationLayer *vlayer, meHTVRule *rule );
	
	/** \brief Height terrain sector vegetation layer link count changed. */
	virtual void HTVLLinkCountChanged( meWorld *world,
		meHTVegetationLayer *vlayer );
	/*@}*/
};

#endif
