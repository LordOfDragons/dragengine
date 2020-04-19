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

#ifndef _SEVIEWCONSTRUCTED_H_
#define _SEVIEWCONSTRUCTED_H_

#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeScrollBarReference.h>
#include <deigde/gui/igdeSpinTextFieldReference.h>
#include <deigde/gui/layout/igdeContainerBorder.h>

#include <dragengine/common/math/decMath.h>

class seSkin;
class seWindowMain;
class seViewConstructedView;
class seViewConstructedListener;



/**
 * \brief Constructed property preview container.
 */
class seViewConstructed : public igdeContainerBorder{
private:
	seWindowMain &pWindowMain;
	seViewConstructedListener *pListener;
	
	decPoint pContentSize;
	decPoint pScrollOffset;
	
	igdeScrollBarReference pSBHorizontal;
	igdeScrollBarReference pSBVertical;
	igdeComboBoxReference pCBZoom;
	igdeSpinTextFieldReference pSpinLayer;
	seViewConstructedView *pViewNode;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create constructed property view. */
	seViewConstructed( seWindowMain &windowMain );
	
protected:
	/** \brief Clean up constructed property view. */
	virtual ~seViewConstructed();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Skin. */
	seSkin *GetSkin() const;
	
	/** \brief Set skin. */
	void SetSkin( seSkin *skin );
	
	/** \brief Constructed view. */
	inline seViewConstructedView &GetViewNode() const{ return *pViewNode; }
	
	
	
	/** \brief Set if rendering is enabled. */
	void SetEnableRendering( bool enable );
	
	
	
	/** \brief Game engine has been started. */
	void OnAfterEngineStart();
	
	/** \brief Game engine is about to be stopped. */
	void OnBeforeEngineStop();
	
	/** \brief Reset view. */
	void ResetView();
	
	/** \brief Game like frame update. */
	virtual void OnFrameUpdate( float elapsed );
	
	/** \brief Widget size changed. */
	virtual void OnResize();
	
	/** \brief View scrolled. */
	void OnScrolled();
	
	
	
	/** \brief Update scrollbar ranges. */
	void UpdateScrollbarRanges();
	
	/** \brief Update layer ranges. */
	void UpdateLayerRanges();
	
	/** \brief Update active layer. */
	void UpdateActiveLayer();
	/*@}*/
};

#endif
