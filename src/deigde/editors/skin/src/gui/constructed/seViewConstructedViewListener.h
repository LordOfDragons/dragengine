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

#ifndef _SEVIEWCONSTRUCTEDVIEWLISTENER_H_
#define _SEVIEWCONSTRUCTEDVIEWLISTENER_H_

#include "../../skin/seSkinListener.h"

class seViewConstructedView;



/**
 * \brief Constructed view listener.
 */
class seViewConstructedViewListener : public seSkinListener{
private:
	seViewConstructedView &pView;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seViewConstructedViewListener(seViewConstructedView &view);
	
	/** \brief Clean up listener. */
	virtual ~seViewConstructedViewListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Active texture changed. */
	virtual void ActiveTextureChanged(seSkin *skin);
	
	/** \brief Property changed. */
	virtual void PropertyChanged(seSkin *skin, seTexture *texture, seProperty *property);
	
	/** \brief Active property changed. */
	virtual void ActivePropertyChanged(seSkin *skin, seTexture *texture);
	
	/** \brief Property node structre changed. */
	virtual void PropertyNodeStructureChanged(seSkin *skin, seTexture *texture, seProperty *property);
	
	/** \brief Property node changed. */
	virtual void PropertyNodeChanged(seSkin *skin, seTexture *texture, seProperty *property, sePropertyNode *node);
	
	/** \brief Active property node changed. */
	virtual void PropertyActiveNodeChanged(seSkin *skin, seTexture *texture, seProperty *property);
	
	/** \brief Property selected nodes changed. */
	virtual void PropertyNodeSelectionChanged(seSkin *skin, seTexture *texture, seProperty *property);
	
	/** \brief Active property node group changed. */
	virtual void PropertyActiveNodeGroupChanged(seSkin *skin, seTexture *texture, seProperty *property);
	
	/** \brief Active property node layer changed. */
	virtual void PropertyActiveNodeLayerChanged(seSkin *skin, seTexture *texture, seProperty *property);
	/*@}*/
};

#endif
