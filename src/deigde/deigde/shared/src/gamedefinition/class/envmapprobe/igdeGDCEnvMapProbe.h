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

#ifndef _IGDEGDCENVMAPPROBE_H_
#define _IGDEGDCENVMAPPROBE_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/deObject.h>



/**
 * \brief Game Definition Class Environment Map Probe.
 */
class DE_DLL_EXPORT igdeGDCEnvMapProbe : public deObject{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeGDCEnvMapProbe> Ref;


public:
	/** \brief Properties. */
	enum eProperties{
		epInfluenceArea,
		epInfluenceBorderSize,
		epInfluencePriority,
		epReflectionShape,
		epReflectionMask,
		epAttachPosition,
		epAttachRotation
	};
	
	
	
private:
	decVector pPosition;
	decQuaternion pOrientation;
	decVector pScaling;
	
	decShapeList pShapeListInfluence;
	decShape *pShapeReflection;
	decShapeList pShapeListReflectionMask;
	float pInfluenceBorderSize;
	int pInfluencePriority;
	
	decString pPropertyNames[ epAttachRotation + 1 ];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game definition class environment map probe. */
	igdeGDCEnvMapProbe();
	
	/** \brief Create copy of a game definition class environment map probe. */
	igdeGDCEnvMapProbe( const igdeGDCEnvMapProbe &probe );
	
	/** \brief Clean up game definition class environment map probe. */
	virtual ~igdeGDCEnvMapProbe();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Scaling. */
	inline const decVector &GetScaling() const{ return pScaling; }
	
	/** \brief Set scaling. */
	void SetScaling( const decVector &scaling );
	
	/** \brief Influence shape list. */
	inline const decShapeList &GetShapeListInfluence() const{ return pShapeListInfluence; }
	
	/** \brief Set influence shape list. */
	void SetShapeListInfluence( const decShapeList &shapeList );
	
	/** \brief Reflection shape or NULL if the environment map is global. */
	inline decShape *GetShapeReflection() const{ return pShapeReflection; }
	
	/** \brief Set reflection shape or NULL if the environment map is global. */
	void SetShapeReflection( decShape *shape );
	
	/** \brief Reflection mask shape list. */
	inline const decShapeList &GetShapeListReflectionMask() const{ return pShapeListReflectionMask; }
	
	/** \brief Set reflection mask shape list. */
	void SetShapeListReflectionMask( const decShapeList &shapeList );
	
	/** \brief Influence border size. */
	inline float GetInfluenceBorderSize() const{ return pInfluenceBorderSize; }
	
	/** \brief Set influence border size. */
	void SetInfluenceBorderSize( float borderSize );
	
	/** \brief Influence priority. */
	inline int GetInfluencePriority() const{ return pInfluencePriority; }
	
	/** \brief Set influence priority. */
	void SetInfluencePriority( int priority );
	
	/** \brief Determines if a property name is set or not (empty string). */
	bool IsPropertySet( int property ) const;
	
	/** \brief Name of the given property. */
	const decString &GetPropertyName( int property ) const;
	
	/** \brief Set name of the given property. */
	void SetPropertyName( int property, const char *name );
	
	/** \brief Determines if one or more properties use a name. */
	bool HasPropertyWithName( const char *name ) const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
