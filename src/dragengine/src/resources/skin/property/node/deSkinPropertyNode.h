/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DESKINPROPERTYNODE_H_
#define _DESKINPROPERTYNODE_H_

#include "../../../../common/math/decMath.h"

class deSkinPropertyNodeVisitor;


/**
 * \brief Skin property node for constructed texture property.
 */
class deSkinPropertyNode{
public:
	/**
	 * \brief Combine modes indicating how the canvas is combined with the content below.
	 * 
	 * The meaning of the combine modes depends on the texture property the canvas is used on.
	 * If nothing special is marked the meaning of suitable for "color" texture property is used.
	 */
	enum eCombineModes{
		/**
		 * \brief Blend canvas with content below using combine strength.
		 * 
		 * Result will be <em>d.rgba = (d.rgb * (1 - strength)) + s.rgb * strength</em> where
		 * d=destination, s=source and strength=combine-strength.
		 * 
		 * For "normal" texture property d.rgb will be normalized like a normal vector.
		 */
		ecmBlend,
		
		/**
		 * \brief Overlay canvas to content below using combine strength.
		 * 
		 * Result will be <em>d.rgb += s.rgb * strength</em> where d=destination, s=source
		 * and strength=combine-strength.
		 * 
		 * For "normal" texture property the result will be the source normal added to the
		 * destination normal and the result normalized. This allows to super-impose fine
		 * grained normals over coarse grained normals. The calculation is carried out in
		 * texture space.
		 */
		ecmOverlay
	};
	
	
	
private:
	decPoint3 pPosition;
	decPoint3 pSize;
	float pRotation;
	float pShear;
	
	float pBrightness;
	float pContrast;
	float pGamma;
	decColor pColorize;
	
	float pTransparency;
	deSkinPropertyNode *pMask;
	eCombineModes pCombineMode;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create constructed property node. */
	deSkinPropertyNode();
	
	/** \brief Clean up constructed property node. */
	virtual ~deSkinPropertyNode();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position in pixels relative to top-left-front corner. */
	inline const decPoint3 &GetPosition() const{ return pPosition; }
	
	/** \brief Set position in pixels relative to top-left-front corner. */
	void SetPosition( const decPoint3 &position );
	
	/** \brief Size in pixels. */
	inline const decPoint3 &GetSize() const{ return pSize; }
	
	/** \brief Set size in pixels. */
	void SetSize( const decPoint3 &size );
	
	/** \brief Counter clock-wise rotation in radians. */
	inline float GetRotation() const{ return pRotation; }
	
	/** \brief Set counter clock-wise rotation in radians. */
	void SetRotation( float rotation );
	
	/** \brief Shearing in percentage along other axis. */
	inline float GetShear() const{ return pShear; }
	
	/** \brief Set shearing in percentage along other axis. */
	void SetShear( float shear );
	
	
	
	/** \brief Brightness. */
	inline float GetBrightness() const{ return pBrightness; }
	
	/** \brief Set brightness. */
	void SetBrightness( float brightness );
	
	/** \brief Contrast. */
	inline float GetContrast() const{ return pContrast; }
	
	/** \brief Set contrast. */
	void SetContrast( float contrast );
	
	/** \brief Gamma. */
	inline float GetGamma() const{ return pGamma; }
	
	/** \brief Set gamma. */
	void SetGamma( float gamma );
	
	/** \brief Colorize. */
	inline const decColor &GetColorize() const{ return pColorize; }
	
	/** \brief Set colorize. */
	void SetColorize( const decColor &color );
	
	
	
	/** \brief Transparency in the range from 0 to 1 where 1 is opaque and 0 fully transparent. */
	inline float GetTransparency() const{ return pTransparency; }
	
	/** \brief Set transparency in the range from 0 to 1 where 1 is opaque and 0 fully transparent. */
	void SetTransparency( float transparency );
	
	/** \brief Mask node or NULL if not masked. */
	inline deSkinPropertyNode *GetMask() const{ return pMask; }
	
	/** \brief Set mask node or NULL if not masked. */
	void SetMask( deSkinPropertyNode *mask );
	
	/** \brief Combine mode. */
	inline eCombineModes GetCombineMode() const{ return pCombineMode; }
	
	/** \brief Set combien mode. */
	void SetCombineMode( eCombineModes mode );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits node. */
	virtual void Visit( deSkinPropertyNodeVisitor &visitor );
	/*@}*/
};

#endif
