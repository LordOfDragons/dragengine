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

#ifndef _IGDEGDCSPEAKER_H_
#define _IGDEGDCSPEAKER_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>



/**
 * \brief Game definition class speaker.
 */
class igdeGDCSpeaker : public deObject{
public:
	/** \brief Properties. */
	enum eProperties{
		epSound,
		epLooping,
		epVolume,
		epRange,
		epRollOff,
		epDistanceOffset,
		epPlaySpeed,
		epPlaying,
		epAttachPosition,
		epAttachRotation
	};
	
	/** \brief Trigger. */
	enum eTriggers{
		etPlaying,
		etMuted
	};
	
	
	
private:
	decString pPathSound;
	decVector pPosition;
	decQuaternion pOrientation;
	decString pBoneName;
	bool pLooping;
	bool pPlaying;
	float pVolume;
	float pRange;
	float pRollOff;
	float pDistanceOffset;
	float pPlaySpeed;
	
	decString pPropertyNames[ epAttachRotation + 1 ];
	decString pTriggerNames[ etMuted + 1 ];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class speaker. */
	igdeGDCSpeaker();
	
	/** \brief Create copy of class speaker. */
	igdeGDCSpeaker( const igdeGDCSpeaker &speaker );
	
protected:
	/** \brief Clean up class speaker. */
	virtual ~igdeGDCSpeaker();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Path of sound to use. */
	inline const decString &GetPathSound() const{ return pPathSound; }
	
	/** \brief Set path of sound to use. */
	void SetPathSound( const char *path );
	
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
	
	/** \brief Speaker is looping. */
	inline bool GetLooping() const{ return pLooping; }
	
	/** \brief Set if speaker is looping. */
	void SetLooping( bool looping );
	
	/** \brief Speaker is playing back. */
	inline bool GetPlaying() const{ return pPlaying; }
	
	/** \brief Set if speaker is playing back. */
	void SetPlaying( bool playing );
	
	/** \brief Volume. */
	inline float GetVolume() const{ return pVolume; }
	
	/** \brief Set volume. */
	void SetVolume( float volume );
	
	/** \brief Range beyond which the sound is inaudible. */
	inline float GetRange() const{ return pRange; }
	
	/** \brief Set range beyond which the sound is inaudible. */
	void SetRange( float range );
	
	/** \brief Roll off factor. */
	inline float GetRollOff() const{ return pRollOff; }
	
	/** \brief Set roll off factor. */
	void SetRollOff( float rollOff );
	
	/** \brief Distance offset factor. */
	inline float GetDistanceOffset() const{ return pDistanceOffset; }
	
	/** \brief Set Distance offset factor. */
	void SetDistanceOffset( float distanceOffset );
	
	/** \brief Play speed. */
	inline float GetPlaySpeed() const{ return pPlaySpeed; }
	
	/** \brief Set play speed. */
	void SetPlaySpeed( float playSpeed );
	
	
	
	/** \brief Property name is set or not (empty string). */
	bool IsPropertySet( eProperties property ) const;
	
	/** \brief Name of the given property. */
	const decString &GetPropertyName( eProperties property ) const;
	
	/** \brief Set name of the given property. */
	void SetPropertyName( eProperties property, const char *name );
	
	/** \brief One or more properties use a name. */
	bool HasPropertyWithName( const char *name ) const;
	
	
	
	/** \brief Trigger name is set or not (empty string). */
	bool IsTriggerSet( eTriggers trigger ) const;
	
	/** \brief Name of the given trigger. */
	const decString &GetTriggerName( eTriggers trigger ) const;
	
	/** \brief Set name of the given trigger. */
	void SetTriggerName( eTriggers trigger, const char *name );
	/*@}*/
};

#endif
