/* 
 * Drag[en]gine IGDE Skin Editor
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

#ifndef _SEPROPERTYNODE_H_
#define _SEPROPERTYNODE_H_

#include "../../mapped/seMapped.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNode.h>

class seProperty;
class sePropertyNodeGroup;

class deEngine;


/**
 * Texture property node.
 */
class sePropertyNode : public deObject{
public:
	typedef deTObjectReference<sePropertyNode> Ref;
	
	/** Node types. */
	enum eNodeTypes{
		/** Group node. */
		entGroup,
		
		/** Image node. */
		entImage,
		
		/** Shape node. */
		entShape,
		
		/** Text node. */
		entText
	};
	
	/** Mapped. */
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
	deEngine &pEngine;
	
	sePropertyNodeGroup *pParent;
	sePropertyNode *pMaskParent;
	
	eNodeTypes pNodeType;
	
	decPoint3 pPosition;
	decPoint3 pSize;
	float pRotation;
	float pShearing;
	
	float pBrightness;
	float pContrast;
	float pGamma;
	decColor pColorize;
	
	float pTransparency;
	sePropertyNode *pMask;
	deSkinPropertyNode::eCombineModes pCombineMode;
	
	seMapped::Ref *pMapped;
	int pMappedCount;
	
	bool pSelected;
	bool pActive;
	
	
	
protected:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create node. */
	sePropertyNode( eNodeTypes nodeType, deEngine &engine, int mappedCount );
	
	/** Create copy of node. */
	sePropertyNode( const sePropertyNode &node );
	
	/** Clean up node. */
	virtual ~sePropertyNode();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Engine. */
	inline deEngine &GetEngine() const{ return pEngine; }
	
	
	
	/** Parent node or \em NULL. */
	inline sePropertyNodeGroup *GetParent() const{ return pParent; }
	
	/** Set parent node or \em NULL. */
	void SetParent( sePropertyNodeGroup *parent );
	
	/** Mask parent node or \em NULL. */
	inline sePropertyNode *GetMaskParent() const{ return pMaskParent; }
	
	/** Set mask parent node or \em NULL. */
	void SetMaskParent( sePropertyNode *maskParent );
	
	/** Parent property or \em NULL if not present. */
	virtual seProperty *GetProperty() const;
	
	
	
	/** Node type. */
	inline eNodeTypes GetNodeType() const{ return pNodeType; }
	
	
	
	/** Position in pixels relative to top-left-front corner. */
	inline const decPoint3 &GetPosition() const{ return pPosition; }
	
	/** Set position in pixels relative to top-left-front corner. */
	void SetPosition( const decPoint3 &position );
	
	/** Size in pixels. */
	inline const decPoint3 &GetSize() const{ return pSize; }
	
	/** Set size in pixels. */
	void SetSize( const decPoint3 &size );
	
	/** Counter clock-wise rotation in radians. */
	inline float GetRotation() const{ return pRotation; }
	
	/** Set counter clock-wise rotation in radians. */
	void SetRotation( float rotation );
	
	/** Shearing in degreed. */
	inline float GetShearing() const{ return pShearing; }
	
	/** Set shearing in degrees. */
	void SetShearing( float shearing );
	
	/**
	 * Create canvas transformation matrix.
	 * 
	 * Contains rotation and shearing.
	 */
	decTexMatrix2 CreateCanvasTransformMatrix() const;
	
	/**
	 * Create parent transformation matrix.
	 * 
	 * Contains position, size, rotation and shearing relative to parent.
	 */
	decTexMatrix2 CreateParentTransformMatrix() const;
	
	/**
	 * Create screen transformation matrix.
	 * 
	 * Contains position, size, rotation and shearing relative to screen.
	 * Combines all parent transformation matrices in all parents up to
	 * the top level parent node.
	 */
	decTexMatrix2 CreateScreenTransformMatrix() const;
	
	/** Set geometry from matrix. */
	void SetFromMatrix( const decTexMatrix2 &matrix, const decPoint3 &referenceSize, float referenceRotation );
	
	
	
	/** Brightness. */
	inline float GetBrightness() const{ return pBrightness; }
	
	/** Set brightness. */
	void SetBrightness( float brightness );
	
	/** Contrast. */
	inline float GetContrast() const{ return pContrast; }
	
	/** Set contrast. */
	void SetContrast( float contrast );
	
	/** Gamma. */
	inline float GetGamma() const{ return pGamma; }
	
	/** Set gamma. */
	void SetGamma( float gamma );
	
	/** Colorize. */
	inline const decColor &GetColorize() const{ return pColorize; }
	
	/** Set colorize. */
	void SetColorize( const decColor &color );
	
	/** Create canvas color transformation matrix. */
	decColorMatrix CreateCanvasColorTransformMatrix() const;
	
	
	
	/** Transparency in the range from 0 to 1 where 1 is opaque and 0 fully transparent. */
	inline float GetTransparency() const{ return pTransparency; }
	
	/** Set transparency in the range from 0 to 1 where 1 is opaque and 0 fully transparent. */
	void SetTransparency( float transparency );
	
	/** Mask node or \em NULL if not masked. */
	inline sePropertyNode *GetMask() const{ return pMask; }
	
	/** Set mask node or \em NULL if not masked. */
	void SetMask( sePropertyNode *mask );
	
	/** Combine mode. */
	inline deSkinPropertyNode::eCombineModes GetCombineMode() const{ return pCombineMode; }
	
	/** Set combien mode. */
	void SetCombineMode( deSkinPropertyNode::eCombineModes mode );
	
	
	
	/** Mapped or nullptr. */
	const seMapped::Ref &GetMappedFor( int type ) const;
	
	/** Set mapped or nullptr. */
	void SetMappedFor( int type, seMapped *mapped );
	
	
	
	/** Node is the active one. */
	inline bool GetActive() const{ return pActive; }
	
	/** Set if node is the active one. */
	void SetActive( bool active );
	
	/** Node is selected. */
	inline bool GetSelected() const{ return pSelected; }
	
	/** Set if node is selected. */
	void SetSelected( bool selected );
	
	
	
	/** Notify skin listeners node structre changed. */
	void NotifyStructreChanged();
	
	/** Notify skin listeners node parameters changed. */
	void NotifyChanged();
	
	
	
	/** Create copy of node. */
	virtual sePropertyNode *Copy() const = 0;
	
	/** Update resources. */
	virtual void UpdateResources();
	/*@}*/
};

#endif
