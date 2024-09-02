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

#ifndef _DESKINPROPERTYCONSTRUCTED_H_
#define _DESKINPROPERTYCONSTRUCTED_H_

#include "deSkinProperty.h"
#include "../../../common/math/decMath.h"

class deSkinPropertyNodeGroup;


/**
 * \brief Skin texture constructed property.
 */
class DE_DLL_EXPORT deSkinPropertyConstructed : public deSkinProperty{
private:
	deSkinPropertyNodeGroup *pContent;
	decColor pColor;
	bool pTileX;
	bool pTileY;
	int pBitCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create constructed property.
	 * \param type Type of the skin property obtained from adding a property
	 * name to the texture property map object held by the engine.
	 */
	deSkinPropertyConstructed( const char *type );
	
	/** \brief Clean up constructed property. */
	virtual ~deSkinPropertyConstructed();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Content. */
	inline deSkinPropertyNodeGroup &GetContent() const{ return *pContent; }
	
	/** \brief Base color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Set base color. */
	void SetColor( const decColor &color );
	
	/** \brief Nodes are tiled along x axis. */
	inline bool GetTileX() const{ return pTileX; }
	
	/** \brief Set if nodes are tiled along x axis. */
	void SetTileX( bool tileX );
	
	/** \brief Nodes are tiled along y axis. */
	inline bool GetTileY() const{ return pTileY; }
	
	/** \brief Set if nodes are tiled along y axis. */
	void SetTileY( bool tileY );
	
	/**
	 * \brief Bit count.
	 * 
	 * Can be 8, 16 or 32.
	 */
	inline int GetBitCount() const{ return pBitCount; }
	
	/**
	 * \brief Set bit count.
	 * 
	 * Allowed are 8, 16 or 32.
	 */
	void SetBitCount( int bitCount );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit property. */
	virtual void Visit( deSkinPropertyVisitor &visitor );
	/*@}*/
};

#endif
