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

#ifndef _DEOGLDECALLISTENER_H_
#define _DEOGLDECALLISTENER_H_

#include <dragengine/deObject.h>

class deoglRDecal;


/**
 * Render decal listener.
 */
class deoglDecalListener : public deObject{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create decal listener. */
	deoglDecalListener();
	
	/** Clean up decal listener. */
	virtual ~deoglDecalListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Decal has been destroyed. */
	virtual void DecalDestroyed( deoglRDecal &decal );
	
	/** Decal geometry changed. */
	virtual void GeometryChanged( deoglRDecal &decal );
	
	/** Texture changed. */
	virtual void TextureChanged( deoglRDecal &decal );
	
	/** TUC changed. */
	virtual void TUCChanged( deoglRDecal &decal );
	/*@}*/
};

#endif
