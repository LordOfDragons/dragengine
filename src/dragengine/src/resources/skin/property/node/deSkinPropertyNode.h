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

#ifndef _DESKINPROPERTYNODE_H_
#define _DESKINPROPERTYNODE_H_

#include "../../../../common/math/decMath.h"

class deSkinPropertyNodeVisitor;


/**
 * \brief Skin property node for constructed texture property.
 */
class DE_DLL_EXPORT deSkinPropertyNode{
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
	
	/** \brief Mapped. */
	enum eMapped{
		emPositionX, //<! Position X-Component
		emPositionY, //<! Position Y-Component
		emPositionZ, //<! Position Z-Component
		emSizeX, //<! Size X-Component
		emSizeY, //<! Size Y-Component
		emSizeZ, //<! Size Z-Component
		emRotation, //<! Rotation
		emShear, //<! Shear
		emBrightness, //<! Brightness
		emContrast, //<! Constrast
		emGamma, //<! Gamma
		emColorizeRed, //<! Colorize red component
		emColorizeGreen, //<! Colorize green component
		emColorizeBlue, //<! Colorize blue component
		emTransparency //<! Transparency
	};
	
	static const int MappedCount = emTransparency + 1;
	
	
	
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
	
	int pMapped[ MappedCount ];
	
	
	
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
	
	
	
	/** \brief Index of mapped value or -1 to use static value. */
	int GetMappedFor( eMapped mapped ) const;
	
	/** \brief Set index of mapped value or -1 to use static value. */
	void SetMappedFor( eMapped mapped, int index );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits node. */
	virtual void Visit( deSkinPropertyNodeVisitor &visitor );
	/*@}*/
};

#endif
