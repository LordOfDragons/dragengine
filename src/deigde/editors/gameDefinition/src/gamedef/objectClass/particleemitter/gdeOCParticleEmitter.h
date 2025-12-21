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

#ifndef _GDEOCPARTICLEEMITTER_H_
#define _GDEOCPARTICLEEMITTER_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief Object class particle emitter.
 */
class gdeOCParticleEmitter : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<gdeOCParticleEmitter> Ref;

	/** \brief Type holding list. */
	typedef decTObjectOrderedSet<gdeOCParticleEmitter> List;
	
	
	/** \brief Properties. */
	enum eProperties{
		/** \brief Path to particle emitter file. */
		epPath,
		
		/** \brief Casting. */
		epCasting,
		
		/** \brief Attach position. */
		epAttachPosition,
		
		/** \brief Attach rotation. */
		epAttachRotation
	};
	
	/** \brief Triggers. */
	enum eTriggers{
		/** \brief Activated. */
		etCasting
	};
	
	
	
private:
	decString pPath;
	decVector pPosition;
	decVector pRotation;
	decString pBoneName;
	bool pCasting;
	
	decString pPropertyNames[epAttachRotation + 1];
	decString pTriggerNames[etCasting + 1];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create particle emitter. */
	gdeOCParticleEmitter();
	
	/** \brief Create copy of particle emitter. */
	gdeOCParticleEmitter(const gdeOCParticleEmitter &emitter);
	
	/** \brief Clean up particle emitter. */
	virtual ~gdeOCParticleEmitter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Path of particle emitter to use. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** \brief Set path of particle emitter to use. */
	void SetPath(const char *path);
	
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition(const decVector &position);
	
	/** \brief Rotation. */
	inline const decVector &GetRotation() const{ return pRotation; }
	
	/** \brief Set orientation. */
	void SetRotation(const decVector &orientation);
	
	/** \brief Name of the bone to attach to or empty string. */
	inline const decString &GetBoneName() const{ return pBoneName; }
	
	/** \brief Set name of the bone to attach to or empty string. */
	void SetBoneName(const char *boneName);
	
	/** \brief Casting. */
	inline bool GetCasting() const{ return pCasting; }
	
	/** \brief Set if casting. */
	void SetCasting(bool casting);
	
	
	
	
	/** \brief Property name is set. */
	bool IsPropertySet(eProperties property) const;
	
	/** \brief Name of property. */
	const decString &GetPropertyName(eProperties property) const;
	
	/** \brief Set name of property. */
	void SetPropertyName(eProperties property, const char *name);
	
	/** \brief One or more properties use a name. */
	bool HasPropertyWithName(const char *name) const;
	
	
	
	/** \brief Trigger name is set. */
	bool IsTriggerSet(eTriggers trigger) const;
	
	/** \brief Name of trigger. */
	const decString &GetTriggerName(eTriggers trigger) const;
	
	/** \brief Set name of trigger. */
	void SetTriggerName(eTriggers trigger, const char *name);
	/*@}*/
};

#endif
