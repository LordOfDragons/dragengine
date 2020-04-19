/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEOCBILLBOARD_H_
#define _GDEOCBILLBOARD_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief Object class billboard.
 */
class gdeOCBillboard : public deObject{
public:
	/** \brief Properties. */
	enum eProperties{
		/** \brief Skin. */
		epSkin,
		
		/** \brief Axis. */
		epAxis,
		
		/** \brief Offset. */
		epOffset,
		
		/** \brief Locked. */
		epLocked,
		
		/** \brief Spherical. */
		epSpherical,
		
		/** \brief Render into environment maps. */
		epRenderEnvMap,
		
		/** \brief Attach position. */
		epAttachPosition
	};
	
	
	
private:
	decString pSkinPath;
	decVector pAxis;
	decVector2 pSize;
	decVector2 pOffset;
	bool pLocked;
	bool pSpherical;
	bool pSizeFixedToScreen;
	
	bool pDoNotScale;
	bool pPartialHide;
	bool pRenderEnvMap;
	decDVector pPosition;
	decString pBoneName;
	
	decString pPropertyNames[ epAttachPosition + 1 ];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create billboard. */
	gdeOCBillboard();
	
	/** \brief Create copy of billboard. */
	gdeOCBillboard( const gdeOCBillboard &billboard );
	
	/** \brief Clean up billboard. */
	virtual ~gdeOCBillboard();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Skin path. */
	inline const decString &GetSkinPath() const{ return pSkinPath; }
	
	/** \brief Set skin path. */
	void SetSkinPath( const char *path );
	
	/** \brief Axis. */
	inline const decVector &GetAxis() const{ return pAxis; }
	
	/** \brief Set axis. */
	void SetAxis( const decVector &axis );
	
	/** \brief Size. */
	inline const decVector2 &GetSize() const{ return pSize; }
	
	/** \brief Set size. */
	void SetSize( const decVector2 &size );
	
	/** \brief Offset. */
	inline const decVector2 &GetOffset() const{ return pOffset; }
	
	/** \brief Set offset. */
	void SetOffset( const decVector2 &offset );
	
	/** \brief Billboard is locked. */
	inline bool GetLocked() const{ return pLocked; }
	
	/** \brief Set if billboard is locked. */
	void SetLocked( bool locked );
	
	/** \brief Billboard is spherical. */
	inline bool GetSpherical() const{ return pSpherical; }
	
	/** \brief Set if billboard is spherical. */
	void SetSpherical( bool spherical );
	
	/** \brief Billboard size is fixed to the screen size. */
	inline bool GetSizeFixedToScreen() const{ return pSizeFixedToScreen; }
	
	/** \brief Set if billboard size is fixed to the screen size. */
	void SetSizeFixedToScreen( bool sizeFixedToScreen );
	
	
	
	/** \brief Determines if this billboard should not be scaled. */
	inline bool GetDoNotScale() const{ return pDoNotScale; }
	
	/** \brief Sets if this billboard should not be scaled. */
	void SetDoNotScale( bool doNotScale );
	
	/** \brief Determines if the billboard can be partially hidden. */
	inline bool GetPartialHide() const{ return pPartialHide; }
	
	/** \brief Sets if the billboard can be partially hidden. */
	void SetPartialHide( bool partiallyHidden );
	
	/** \brief Determines if the billboard is rendered into environment map probes. */
	inline bool GetRenderEnvMap() const{ return pRenderEnvMap; }
	
	/** \brief Sets if the billboard is rendered into environment map probes. */
	void SetRenderEnvMap( bool renderEnvMap );
	
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decDVector &position );
	
	/** \brief Name of the bone to attach to or empty string. */
	inline const decString &GetBoneName() const{ return pBoneName; }
	
	/** \brief Set name of bone to attach to or empty string. */
	void SetBoneName( const char *boneName );
	
	
	
	/** \brief Property name is set. */
	bool IsPropertySet( eProperties property ) const;
	
	/** \brief Name of property. */
	const decString &GetPropertyName( eProperties property ) const;
	
	/** \brief Set name of property. */
	void SetPropertyName( eProperties property, const char *name );
	
	/** \brief One or more properties use a name. */
	bool HasPropertyWithName( const char *name ) const;
	/*@}*/
};

#endif
