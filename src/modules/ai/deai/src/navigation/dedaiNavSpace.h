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

#ifndef _DEDAINAVSPACE_H_
#define _DEDAINAVSPACE_H_

#include <dragengine/systems/modules/ai/deBaseAINavigationSpace.h>

class dedaiSpace;
class dedaiWorld;
class deDEAIModule;

class deNavigationSpace;



/**
 * \brief Navigation space peer.
 */
class dedaiNavSpace : public deBaseAINavigationSpace{
private:
	deDEAIModule &pDEAI;
	const deNavigationSpace &pNavigationSpace;
	
	dedaiWorld *pParentWorld;
	
	dedaiSpace *pSpace;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	dedaiNavSpace( deDEAIModule &deai, const deNavigationSpace &navigationSpace );
	
	/** \brief Clean up peer. */
	virtual ~dedaiNavSpace();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief AI module. */
	inline deDEAIModule &GetDEAI() const{ return pDEAI; }
	
	/** \brief Navigation space. */
	inline const deNavigationSpace &GetNavigationSpace() const{ return pNavigationSpace; }
	
	
	
	/** \brief Parent world or \em NULL. */
	inline dedaiWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or \em NULL. */
	void SetParentWorld( dedaiWorld *world );
	
	
	
	/** \brief Space. */
	inline dedaiSpace *GetSpace() const{ return pSpace; }
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Layer changed. */
	virtual void LayerChanged();
	
	/** \brief Type changed. */
	virtual void TypeChanged();
	
	/** \brief Snapping parameters changed. */
	virtual void SnappingChanged();
	
	/** \brief Blocker shape list changed. */
	virtual void BlockerShapeChanged();
	
	/** \brief Layout changed. */
	virtual void LayoutChanged();
	
	/** \brief Blocking priority changed. */
	virtual void BlockingPriorityChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
