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

#ifndef _DESKY_H_
#define _DESKY_H_

#include "../deResource.h"
#include "../../common/math/decMath.h"

class deBaseGraphicSky;
class deSkyLayer;
class deSkyController;
class deSkyLink;
class deSkyManager;


/**
 * \brief Sky definition for world resources.
 *
 * Supports differenf kinds of skies. The most simple is using a 6 sided sky box with
 * static or dynamic images on the sides. Another way is to use the layer system.
 * This system considers the sky to be projected onto a unit sphere at the center you
 * are standing. The sky is rendered behind anything else in the scene. Each layer
 * can be assigned a rotation, and image and a set of sky bodies. All sky bodies in
 * the layer use the same image. Layers are rendered in ascending order that they are
 * created.
 * 
 * Sky layer properties can be dynamically modified using controllers and targets.
 * Sky instances provide the controllers defined in the sky resource allowing the
 * user to modify the controller at runtime.
 */
class deSky : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deSky> Ref;
	
	
	
private:
	decColor pBgColor;
	
	deSkyController *pControllers;
	int pControllerCount;
	
	deSkyLink *pLinks;
	int pLinkCount;
	
	deSkyLayer *pLayers;
	int pLayerCount;
	
	deBaseGraphicSky *pPeerGraphic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sky resource. */
	deSky( deSkyManager *manager );
	
protected:
	/**
	 * \brief Clean up sky resource.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deSky();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Background color. */
	inline const decColor &GetBgColor() const{ return pBgColor; }
	
	/** \brief Set background color. */
	void SetBgColor( const decColor &color );
	/*@}*/
	
	
	
	/** \name Controllers */
	/*@{*/
	/** \brief Number of controllers. */
	inline int GetControllerCount() const{ return pControllerCount; }
	
	/**
	 * \brief Set number of controllers.
	 * 
	 * Sets all controllers to default vaules.
	 */
	void SetControllerCount( int count );
	
	/**
	 * \brief Controller at index.
	 * \throws deeOutOfBoundary \em index is less than 0 or larger than or equal
	 * to GetControllerCount().
	 */
	deSkyController &GetControllerAt( int index ) const;
	
	/** \brief Index of named controller or -1 if absent. */
	int IndexOfControllerNamed( const char *name ) const;
	
	
	
	/** \brief Number of links. */
	inline int GetLinkCount() const{ return pLinkCount; }
	
	/**
	 * \brief Set number of links.
	 * 
	 * Sets all links to default vaules.
	 */
	void SetLinkCount( int count );
	
	/**
	 * \brief Link at index.
	 * \throws deeOutOfBoundary \em index is less than 0 or larger than or equal
	 * to GetLinkCount().
	 */
	deSkyLink &GetLinkAt( int index ) const;
	
	
	
	/** \brief Number of layers. */
	inline int GetLayerCount() const{ return pLayerCount; }
	
	/**
	 * \brief Set number of layers.
	 * 
	 * Sets all links to default vaules.
	 */
	void SetLayerCount( int count );
	
	/**
	 * \brief Layer at index.
	 * \throws deeOutOfBoundary \em index is less than 0 or larger than or equal
	 * to GetLayerCount().
	 */
	deSkyLayer &GetLayerAt( int index ) const;
	
	
	
	/**
	 * \brief Notify peer parameters changed.
	 * 
	 * Use after changing one or more parameters. If possible avoid changing
	 * parameters while sky instances use this sky.
	 */
	void NotifyParametersChanged();
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer or NULL if not set. */
	inline deBaseGraphicSky *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer. */
	void SetPeerGraphic( deBaseGraphicSky *peer );
	/*@}*/
};

#endif
