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

#ifndef _SEWPDYNAMICSKINLISTENER_H_
#define _SEWPDYNAMICSKINLISTENER_H_

#include "../../../skin/seSkinListener.h"

class seWPDynamicSkin;



/**
 * \brief Texture panel listener.
 */
class seWPDynamicSkinListener : public seSkinListener{
private:
	seWPDynamicSkin &pPanel;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seWPDynamicSkinListener(seWPDynamicSkin &panel);
	
	/** \brief Clean up listener. */
	~seWPDynamicSkinListener() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Dynamic skin renderables have been added or removed. */
	void DynamicSkinRenderableStructureChanged (seSkin *skin) override;
	
	/** \brief A dynamic skin renderable changed. */
	void DynamicSkinRenderableChanged(seSkin *skin, seDynamicSkinRenderable *renderable) override;
	
	/** \brief A dynamic skin renderable name changed. */
	void DynamicSkinRenderableNameChanged(seSkin *skin, seDynamicSkinRenderable *renderable) override;
	
	/** \brief Active dynamic skin renderable changed. */
	void DynamicSkinActiveRenderableChanged(seSkin *skin) override;
	/*@}*/
};

#endif
