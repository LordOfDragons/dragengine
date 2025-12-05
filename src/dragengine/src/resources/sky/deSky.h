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
class DE_DLL_EXPORT deSky : public deResource{
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
