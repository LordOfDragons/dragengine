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

#ifndef _GDEOCENVMAPPROBE_H_
#define _GDEOCENVMAPPROBE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief Object class environment map probe.
 */
class gdeOCEnvMapProbe : public deObject{
	public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<gdeOCEnvMapProbe> Ref;

	/** \brief Type holding list. */
	typedef decTObjectOrderedSet<gdeOCEnvMapProbe> List;
	
	
	/** \brief Properties. */
	enum eProperties{
		/** \brief Influence area. */
		epInfluenceArea,
		
		/** \brief Influence border size. */
		epInfluenceBorderSize,
		
		/** \brief Influence priority. */
		epInfluencePriority,
		
		/** \brief Reflection shape. */
		epReflectionShape,
		
		/** \brief Reflection mask. */
		epReflectionMask,
		
		/** \brief Attach position. */
		epAttachPosition,
		
		/** \brief Attach orientation. */
		epAttachRotation
	};
	
	
	
private:
	decVector pPosition;
	decVector pRotation;
	decVector pScaling;
	
	decShapeList pShapeListInfluence;
	decShape *pShapeReflection;
	decShapeList pShapeListReflectionMask;
	float pInfluenceBorderSize;
	int pInfluencePriority;
	
	decString pPropertyNames[epAttachRotation + 1];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create environment map probe. */
	gdeOCEnvMapProbe();
	
	/** \brief Create copy of environment map probe. */
	gdeOCEnvMapProbe(const gdeOCEnvMapProbe &probe);
	
	/** \brief Clean up environment map probe. */
	virtual ~gdeOCEnvMapProbe();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition(const decVector &position);
	
	/** \brief Rotation. */
	inline const decVector &GetRotation() const{ return pRotation; }
	
	/** \brief Set orientation. */
	void SetRotation(const decVector &orientation);
	
	/** \brief Scaling. */
	inline const decVector &GetScaling() const{ return pScaling; }
	
	/** \brief Set scaling. */
	void SetScaling(const decVector &scaling);
	
	
	
	/** \brief Influence shape list. */
	inline decShapeList &GetShapeListInfluence(){ return pShapeListInfluence; }
	inline const decShapeList &GetShapeListInfluence() const{ return pShapeListInfluence; }
	
	/** \brief Reflection shape or \em nullptr if probe is global. */
	inline decShape *GetShapeReflection() const{ return pShapeReflection; }
	
	/** \brief Set reflection shape or \em nullptr if probe is global. */
	void SetShapeReflection(decShape *shape);
	
	/** \brief Reflection mask shape list. */
	inline decShapeList &GetShapeListReflectionMask(){ return pShapeListReflectionMask; }
	inline const decShapeList &GetShapeListReflectionMask() const{ return pShapeListReflectionMask; }
	
	/** \brief Influence border size. */
	inline float GetInfluenceBorderSize() const{ return pInfluenceBorderSize; }
	
	/** \brief Set influence border size. */
	void SetInfluenceBorderSize(float borderSize);
	
	/** \brief Influence priority. */
	inline int GetInfluencePriority() const{ return pInfluencePriority; }
	
	/** \brief Set influence priority. */
	void SetInfluencePriority(int priority);
	
	
	
	/** \brief Property name is set. */
	bool IsPropertySet(int property) const;
	
	/** \brief Name of property. */
	const decString &GetPropertyName(int property) const;
	
	/** \brief Set name of property. */
	void SetPropertyName(int property, const char *name);
	
	/** \brief One or more properties use a name. */
	bool HasPropertyWithName(const char *name) const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
