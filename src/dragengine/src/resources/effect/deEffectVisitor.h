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

#ifndef _DEEFFECTVISITOR_H_
#define _DEEFFECTVISITOR_H_


// definitions

class deEffect;
class deEffectFilterKernel;
class deEffectOverlayImage;
class deEffectColorMatrix;
class deEffectDistortImage;


/**
 * \brief Effect Visitor Class.
 *
 * Visits effects.
 */
class deEffectVisitor{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new effect visitor object. */
	deEffectVisitor();
	
	/** \brief Clean up effect visitor. */
	virtual ~deEffectVisitor();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit effect. */
	virtual void VisitEffect( deEffect &effect );
	
	/** \brief Visit effect filter kernel. */
	virtual void VisitFilterKernel( deEffectFilterKernel &effect );
	
	/** \brief Visit overlay image effect. */
	virtual void VisitOverlayImage( deEffectOverlayImage &effect );
	
	/** \brief Visit color matrix effect. */
	virtual void VisitColorMatrix( deEffectColorMatrix &effect );
	
	/** \brief Visit pixel matrix effect. */
	virtual void VisitDistortImage( deEffectDistortImage &effect );
	/*@}*/
};

#endif
