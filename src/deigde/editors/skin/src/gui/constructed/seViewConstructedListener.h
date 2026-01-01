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

#ifndef _SEVIEWCONSTRUCTEDLISTENER_H_
#define _SEVIEWCONSTRUCTEDLISTENER_H_

#include "../../skin/seSkinListener.h"

class seViewConstructed;



/**
 * \brief Constructed view listener.
 */
class seViewConstructedListener : public seSkinListener{
public:
	typedef deTObjectReference<seViewConstructedListener> Ref;
	
private:
	seViewConstructed &pView;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seViewConstructedListener(seViewConstructed &view);
	
	/** \brief Clean up listener. */
protected:
	~seViewConstructedListener() override;
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Active texture changed. */
	void ActiveTextureChanged(seSkin *skin) override;
	
	/** \brief Property changed. */
	void PropertyChanged(seSkin *skin, seTexture *texture, seProperty *property) override;
	
	/** \brief Active property changed. */
	void ActivePropertyChanged(seSkin *skin, seTexture *texture) override;
	
	/** \brief Property node changed. */
	void PropertyNodeChanged(seSkin *skin, seTexture *texture, seProperty *property, sePropertyNode *node) override;
	
	/** \brief Active property node layer changed. */
	void PropertyActiveNodeLayerChanged(seSkin *skin, seTexture *texture, seProperty *property) override;
	/*@}*/
};

#endif
