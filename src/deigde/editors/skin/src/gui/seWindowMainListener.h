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

#ifndef _SEWINDOWMAINLISTENER_H_
#define _SEWINDOWMAINLISTENER_H_

#include "../skin/seSkinListener.h"

class seWindowMain;



/**
 * \brief Window Main Listener.
 */
class seWindowMainListener : public seSkinListener{
private:
	seWindowMain &pWindow;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seWindowMainListener(seWindowMain &window);
	
	/** \brief Clean up listener. */
	~seWindowMainListener() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Changed or saved state changed. */
	void StateChanged(seSkin *skin) override;
	
	/** \brief Undo changed. */
	void UndoChanged(seSkin *skin) override;
	
	/** \brief Textures have been added or removed. */
	void TextureStructureChanged (seSkin *skin) override;
	
	/** \brief Properties have been added or removed. */
	void PropertyStructureChanged (seSkin *skin, seTexture *texture) override;
	/*@}*/
};

#endif
