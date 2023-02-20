/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEGDCPARTICLEEMITTER_H_
#define _IGDEGDCPARTICLEEMITTER_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>



/**
 * \brief Game Definition Class Particle Emitter.
 */
class DE_DLL_EXPORT igdeGDCParticleEmitter : public deObject{
public:
	/** \brief Properties. */
	enum eProperties{
		epPath,
		epCasting,
		epAttachPosition,
		epAttachRotation
	};
	
	/** \brief Trigger. */
	enum eTriggers{
		etCasting
	};
	
	
	
private:
	decString pPath;
	decVector pPosition;
	decQuaternion pOrientation;
	decString pBoneName;
	bool pCasting;
	
	decString pPropertyNames[ epAttachRotation + 1 ];
	decString pTriggerNames[ etCasting + 1 ];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class particle emitter. */
	igdeGDCParticleEmitter();
	
	/** \brief Create copy of a game definition class particle emitter. */
	igdeGDCParticleEmitter( const igdeGDCParticleEmitter &emitter );
	
protected:
	/** \brief Clean up class particle emitter. */
	virtual ~igdeGDCParticleEmitter();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Path of the particle emitter to use. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** \brief Set path of the particle emitter to use. */
	void SetPath( const char *path );
	
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Name of the bone to attach to or empty string. */
	inline const decString &GetBoneName() const{ return pBoneName; }
	
	/** \brief Set name of bone to attach to or empty string. */
	void SetBoneName( const char *boneName );
	
	/** \brief Casting. */
	inline bool GetCasting() const{ return pCasting; }
	
	/** \brief Set if casting. */
	void SetCasting( bool casting );
	
	
	
	/** \brief Property name is set (not empty string). */
	bool IsPropertySet( eProperties property ) const;
	
	/** \brief Name of property. */
	const decString &GetPropertyName( eProperties property ) const;
	
	/** \brief Set name of property. */
	void SetPropertyName( eProperties property, const char *name );
	
	/** \brief One or more properties use name. */
	bool HasPropertyWithName( const char *name ) const;
	
	
	
	/** \brief Trigger name is set (not empty string). */
	bool IsTriggerSet( eTriggers trigger ) const;
	
	/** \brief Name of trigger. */
	const decString &GetTriggerName( eTriggers trigger ) const;
	
	/** \brief Set name of trigger. */
	void SetTriggerName( eTriggers trigger, const char *name );
	/*@}*/
};

#endif
