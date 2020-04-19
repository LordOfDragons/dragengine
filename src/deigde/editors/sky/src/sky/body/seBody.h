/* 
 * Drag[en]gine IGDE Sky Editor
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

#ifndef _SEBODY_H_
#define _SEBODY_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/skin/deSkinReference.h>

class seLayer;

class deEngine;



/**
 * \brief Sky body.
 */
class seBody : public deObject{
private:
	deEngine *pEngine;
	
	seLayer *pLayer;
	
	decVector pOrientation;
	decVector2 pSize;
	decColor pColor;
	decString pPathSkin;
	deSkinReference pEngSkin;
	
	bool pSelected;
	bool pActive;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sky body. */
	seBody( deEngine *engine );
	
protected:
	/** \brief Clean up sky body. */
	virtual ~seBody();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	
	/** \brief Parent layer or \em NULL. */
	inline seLayer *GetLayer() const{ return pLayer; }
	
	/** \brief Set parent layer or \em NULL. */
	void SetLayer( seLayer *layer );
	
	/** \brief Orientation. */
	inline const decVector &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decVector &orientation );
	
	/** \brief Size. */
	inline const decVector2 &GetSize() const{ return pSize; }
	
	/** \brief Set size. */
	void SetSize( const decVector2 &size );
	
	/** \brief Color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Set color. */
	void SetColor( const decColor &color );
	
	/** \brief Skin path. */
	inline const decString &GetSkinPath() const{ return pPathSkin; }
	
	/** \brief Set skin path. */
	void SetSkinPath( const char *skinPath );
	
	/** \brief Engine skin or \em NULL. */
	inline deSkin *GetEngineSkin() const{ return pEngSkin; }
	
	/** \brief Body is the active one. */
	inline bool GetActive() const{ return pActive; }
	
	/** \brief Set if body is the active one. */
	void SetActive( bool active );
	
	/** \brief Body is selected. */
	inline bool GetSelected() const{ return pSelected; }
	
	/** \brief Set if body is selected. */
	void SetSelected( bool selected );
	
	/** \brief Notify listeners body parameters changed. */
	void NotifyBodyChanged();
	
	/** \brief Update relative resources after changing the base path. */
	void UpdateRelativeResources();
	/*@}*/
	
	
	
private:
	void pUpdateSkin();
};

#endif
