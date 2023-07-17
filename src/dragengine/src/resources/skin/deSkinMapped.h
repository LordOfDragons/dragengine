/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _DESKINPMAPPED_H_
#define _DESKINPMAPPED_H_

#include "../../deObject.h"
#include "../../common/curve/decCurveBezier.h"
#include "../../common/math/decMath.h"
#include "../../common/string/decString.h"


/**
 * \brief Mapped value for skin properties.
 *
 * Samples curve to produce value to use. Different inputs can be used for the curve:
 * - Time: Use elapsed time.
 * - Bone Position X: Use named bone X position.
 * - Bone Position Y: Use named bone Y position.
 * - Bone Position Z: Use named bone Z position.
 * - Bone Rotation X: Use named bone X rotation in the range 0 to 1 (0 to 360 degrees).
 * - Bone Rotation Y: Use named bone Y rotation in the range 0 to 1 (0 to 360 degrees).
 * - Bone Rotation Z: Use named bone Z rotation in the range 0 to 1 (0 to 360 degrees).
 * - Bone Scale X: Use named bone X scale.
 * - Bone Scale Y: Use named bone Y scale.
 * - Bone Scale Z: Use named bone Z scale.
 * 
 * The input range maps the input value to the output value range used to sample the curve.
 * This allows to easily modify play speed (for the elapsed time case) or bone state range
 * (for bone related types). For bone related times the bone name has to be set too.
 * By default input values are wrapped around inside input range. This is best used for
 * time based repeating animations. For bone related input it can be useful to enable
 * clamping to avoid values wrapping around.
 */
class DE_DLL_EXPORT deSkinMapped : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deSkinMapped> Ref;
	
	/** \brief Input types. */
	enum eInputTypes{
		/** \brief Use elapsed time. */
		eitTime,
		
		/** \brief Use named bone X position. */
		eitBonePositionX,
		
		/** \brief Use named bone Y position. */
		eitBonePositionY,
		
		/** \brief Use named bone Z position. */
		eitBonePositionZ,
		
		/** \brief Use named bone X rotation in degrees. */
		eitBoneRotationX,
		
		/** \brief Use named bone Y rotation in degrees. */
		eitBoneRotationY,
		
		/** \brief Use named bone Z rotation in degrees. */
		eitBoneRotationZ,
		
		/** \brief Use named bone X scale. */
		eitBoneScaleX,
		
		/** \brief Use named bone Y scale. */
		eitBoneScaleY,
		
		/** \brief Use named bone Z scale. */
		eitBoneScaleZ,
		
		/** \brief Use renderable (value or color type). */
		eitRenderable
	};
	
	/** \brief Renderable component. */
	enum eRenderableComponent{
		/** \brief Red component if renderable is of color type. */
		ercRed,
		
		/** \brief Green component if renderable is of color type. */
		ercGreen,
		
		/** \brief Blue component if renderable is of color type. */
		ercBlue,
		
		/** \brief Alpha component if renderable is of color type. */
		ercAlpha
	};
	
	
	
private:
	decString pName;
	decCurveBezier pCurve;
	eInputTypes pInputType;
	float pInputLower;
	float pInputUpper;
	bool pInputClamped;
	float pOutputLower;
	float pOutputUpper;
	decString pBone;
	decString pRenderable;
	eRenderableComponent pRenderableComponent;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create new mapped value.
	 * 
	 * \param name Name of mapped value.
	 */
	deSkinMapped ( const char *name );
	
	/** \brief Clean up skin property color. */
	virtual ~deSkinMapped();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Curve. */
	inline decCurveBezier &GetCurve(){ return pCurve; }
	inline const decCurveBezier &GetCurve() const{ return pCurve; }
	
	/** \brief Input type. */
	inline eInputTypes GetInputType() const{ return pInputType; }
	
	/** \brief Set input type. */
	void SetInputType( eInputTypes inputType );
	
	/** \brief Lower input range. */
	inline float GetInputLower() const{ return pInputLower; }
	
	/** \brief Set lower input range. */
	void SetInputLower( float lower );
	
	/** \brief Upper input range. */
	inline float GetInputUpper() const{ return pInputUpper; }
	
	/** \brief Set upper input range. */
	void SetInputUpper( float upper );
	
	/** \brief Input value is clamped to range instead of wrapping around. */
	inline bool GetInputClamped() const{ return pInputClamped; }
	
	/** \brief Set if input value is clamped to range instead of wrapping around. */
	void SetInputClamped( bool inputClamped );
	
	/** \brief Lower output range. */
	inline float GetOutputLower() const{ return pOutputLower; }
	
	/** \brief Set lower output range. */
	void SetOutputLower( float lower );
	
	/** \brief Upper output range. */
	inline float GetOutputUpper() const{ return pOutputUpper; }
	
	/** \brief Set upper output range. */
	void SetOutputUpper( float upper );
	
	/** \brief Bone name if bone related input type is used. */
	inline const decString &GetBone() const{ return pBone; }
	
	/** \brief Set bone name if bone related input type is used. */
	void SetBone( const char *bone );
	
	/** \brief Renderable name if renderable related input type is used. */
	inline const decString &GetRenderable() const{ return pRenderable; }
	
	/** \brief Set renderable name if renderable related input type is used. */
	void SetRenderable( const char *renderable );
	
	/** \brief Color component to use if renderable is of color type. */
	inline eRenderableComponent GetRenderableComponent() const{ return pRenderableComponent; }
	
	/** \brief Set color component to use if renderable is of color type. */
	void SetRenderableComponent( eRenderableComponent component );
	
	
	
	/** \brief Assign from other mapped value. */
	deSkinMapped &operator=( const deSkinMapped &mapped );
	/*@}*/
};

#endif
