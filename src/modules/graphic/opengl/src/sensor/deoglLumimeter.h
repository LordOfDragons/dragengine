/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLLUMIMETER_H_
#define _DEOGLLUMIMETER_H_

#include <dragengine/systems/modules/graphic/deBaseGraphicLumimeter.h>
#include <dragengine/common/math/decMath.h>

class deoglRLumimeter;

class deGraphicOpenGl;
class deLumimeter;



/**
 * \brief Lumimeter peer.
 */
class deoglLumimeter : public deBaseGraphicLumimeter{
public:
	deGraphicOpenGl &pOgl;
	const deLumimeter &pLumimeter;
	
	deoglRLumimeter *pRLumimeter;
	
	float pLuminance;
	decColor pColor;
	
	bool pDirtyLumimeter;
	bool pDirtyOctree;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create lumimeter peer. */
	deoglLumimeter( deGraphicOpenGl &ogl, const deLumimeter &lumimeter );
	
	/** \brief Clean up lumimeter peer. */
	virtual ~deoglLumimeter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Lumimeter. */
	inline const deLumimeter &GetLumimeter() const{ return pLumimeter; }
	
	
	
	/** \brief Render lumimter. */
	inline deoglRLumimeter *GetRLumimeter() const{ return pRLumimeter; }
	
	/** \brief Update render thread counterpart if required. */
	void SyncToRender();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Direction changed. */
	virtual void DirectionChanged();
	
	/** \brief Cone parameter changed. */
	virtual void ConeParameterChanged();
	/*@}*/
	
	
	
	/** \name Measuring */
	/*@{*/
	/** \brief Measures the luminance. */
	virtual float MeasureLuminance();
	
	/** \brief Measures the color. */
	virtual decColor MeasureColor();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
