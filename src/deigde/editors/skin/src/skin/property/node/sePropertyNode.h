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

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNode.h>

class seProperty;
class sePropertyNodeGroup;

class deEngine;


/**
 * \brief Texture property node.
 */
class sePropertyNode : public deObject{
public:
	/** \brief Node types. */
	enum eNodeTypes{
		/** \brief Group node. */
		entGroup,
		
		/** \brief Image node. */
		entImage,
		
		/** \brief Shape node. */
		entShape,
		
		/** \brief Text node. */
		entText
	};
	
	
	
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
	
	bool pSelected;
	bool pActive;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create node. */
	sePropertyNode( eNodeTypes nodeType, deEngine &engine );
	
	/** \brief Create copy of node. */
	sePropertyNode( const sePropertyNode &node );
	
	/** \brief Clean up node. */
	virtual ~sePropertyNode();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Engine. */
	inline deEngine &GetEngine() const{ return pEngine; }
	
	
	
	/** \brief Parent node or \em NULL. */
	inline sePropertyNodeGroup *GetParent() const{ return pParent; }
	
	/** \brief Set parent node or \em NULL. */
	void SetParent( sePropertyNodeGroup *parent );
	
	/** \brief Mask parent node or \em NULL. */
	inline sePropertyNode *GetMaskParent() const{ return pMaskParent; }
	
	/** \brief Set mask parent node or \em NULL. */
	void SetMaskParent( sePropertyNode *maskParent );
	
	/** \brief Parent property or \em NULL if not present. */
	virtual seProperty *GetProperty() const;
	
	
	
	/** \brief Node type. */
	inline eNodeTypes GetNodeType() const{ return pNodeType; }
	
	
	
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
	
	/** \brief Shearing in degreed. */
	inline float GetShearing() const{ return pShearing; }
	
	/** \brief Set shearing in degrees. */
	void SetShearing( float shearing );
	
	/**
	 * \brief Create canvas transformation matrix.
	 * 
	 * Contains rotation and shearing.
	 */
	decTexMatrix2 CreateCanvasTransformMatrix() const;
	
	/**
	 * \brief Create parent transformation matrix.
	 * 
	 * Contains position, size, rotation and shearing relative to parent.
	 */
	decTexMatrix2 CreateParentTransformMatrix() const;
	
	/**
	 * \brief Create screen transformation matrix.
	 * 
	 * Contains position, size, rotation and shearing relative to screen.
	 * Combines all parent transformation matrices in all parents up to
	 * the top level parent node.
	 */
	decTexMatrix2 CreateScreenTransformMatrix() const;
	
	/** \brief Set geometry from matrix. */
	void SetFromMatrix( const decTexMatrix2 &matrix, const decPoint3 &referenceSize, float referenceRotation );
	
	
	
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
	
	/** \brief Create canvas color transformation matrix. */
	decColorMatrix CreateCanvasColorTransformMatrix() const;
	
	
	
	/** \brief Transparency in the range from 0 to 1 where 1 is opaque and 0 fully transparent. */
	inline float GetTransparency() const{ return pTransparency; }
	
	/** \brief Set transparency in the range from 0 to 1 where 1 is opaque and 0 fully transparent. */
	void SetTransparency( float transparency );
	
	/** \brief Mask node or \em NULL if not masked. */
	inline sePropertyNode *GetMask() const{ return pMask; }
	
	/** \brief Set mask node or \em NULL if not masked. */
	void SetMask( sePropertyNode *mask );
	
	/** \brief Combine mode. */
	inline deSkinPropertyNode::eCombineModes GetCombineMode() const{ return pCombineMode; }
	
	/** \brief Set combien mode. */
	void SetCombineMode( deSkinPropertyNode::eCombineModes mode );
	
	
	
	/** \brief Node is the active one. */
	inline bool GetActive() const{ return pActive; }
	
	/** \brief Set if node is the active one. */
	void SetActive( bool active );
	
	/** \brief Node is selected. */
	inline bool GetSelected() const{ return pSelected; }
	
	/** \brief Set if node is selected. */
	void SetSelected( bool selected );
	
	
	
	/** \brief Notify skin listeners node structre changed. */
	void NotifyStructreChanged();
	
	/** \brief Notify skin listeners node parameters changed. */
	void NotifyChanged();
	
	
	
	/** \brief Create copy of node. */
	virtual sePropertyNode *Copy() const = 0;
	
	/** \brief Update resources. */
	virtual void UpdateResources();
	/*@}*/
};

#endif
