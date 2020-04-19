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

#ifndef _DEEFFECTVISITORIDENTIFY_H_
#define _DEEFFECTVISITORIDENTIFY_H_

#include "deEffectVisitor.h"


/**
 * \brief Animator Rule Visitor Identify.
 *
 * Animation effect visitor providing effect identification. After visiting a
 * effect the visitor can be asked what kind of effect has been visited. Useful
 * to identify effects and to carry out actions only for certain effect types.
 * Furthermore save casting is provided. If the cast is not valid an
 * exception is raised.
 */
class deEffectVisitorIdentify : public deEffectVisitor{
public:
	enum eEffects{
		eetUnknown,
		eetFilterKernel,
		eetOverlayImage,
		eetColorMatrix,
		eetDistortImage
	};
	
	
	
private:
	deEffect *pEffect;
	eEffects pType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animator effect visitor identify object. */
	deEffectVisitorIdentify();
	
	/** \brief Clean up animator effect visitor identify. */
	virtual ~deEffectVisitorIdentify();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Effect type. */
	inline eEffects GetType() const{ return pType; }
	
	/** \brief Determines if this is an unknown effect. */
	inline bool IsUnknown() const{ return pType == eetUnknown; }
	
	/** \brief Determines if this is a filter kernel effect. */
	inline bool IsFilterKernel() const{ return pType == eetFilterKernel; }
	
	/** \brief Determines if this is a overlay image effect. */
	inline bool IsOverlayImage() const{ return pType == eetOverlayImage; }
	
	/** \brief Determines if this is a color matrix effect. */
	inline bool IsColorMatrix() const{ return pType == eetColorMatrix; }
	
	/** \brief Determines if this is a pixel matrix effect. */
	inline bool IsDistortImage() const{ return pType == eetDistortImage; }
	
	/** \brief Cast to a filter kernel effect. */
	deEffectFilterKernel &CastToFilterKernel() const;
	
	/** \brief Cast to a overlay image effect. */
	deEffectOverlayImage &CastToOverlayImage() const;
	
	/** \brief Cast to a color matrix effect. */
	deEffectColorMatrix &CastToColorMatrix() const;
	
	/** \brief Cast to a distort image effect. */
	deEffectDistortImage &CastToDistortImage() const;
	
	/** \brief Resets the visitor. */
	void Reset();
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
