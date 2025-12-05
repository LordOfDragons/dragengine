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

#ifndef _GDEOCSPEAKER_H_
#define _GDEOCSPEAKER_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief Object class speaker.
 */
class gdeOCSpeaker : public deObject{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<gdeOCSpeaker> Ref;


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
	static const int EP_COUNT = epAttachRotation + 1;
	
	/** \brief Triggers. */
	enum eTriggers{
		etPlaying,
		etMuted,
	};
	static const int ET_COUNT = etMuted + 1;
	
	
	
private:
	decString pPathSound;
	decVector pPosition;
	decVector pRotation;
	decString pBoneName;
	bool pLooping;
	bool pPlaying;
	float pVolume;
	float pRange;
	float pRollOff;
	float pDistanceOffset;
	float pPlaySpeed;
	
	decString pPropertyNames[ EP_COUNT ];
	decString pTriggerNames[ ET_COUNT ];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create speaker. */
	gdeOCSpeaker();
	
	/** \brief Create copy of speaker. */
	gdeOCSpeaker( const gdeOCSpeaker &speaker );
	
	/** \brief Clean up speaker. */
	virtual ~gdeOCSpeaker();
	/*@}*/
	
	
	
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
	
	/** \brief Rotation. */
	inline const decVector &GetRotation() const{ return pRotation; }
	
	/** \brief Set orientation. */
	void SetRotation( const decVector &orientation );
	
	/** \brief Name of the bone to attach to or empty string. */
	inline const decString &GetBoneName() const{ return pBoneName; }
	
	/** \brief Set name of the bone to attach to or empty string. */
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
	
	/** \brief Distance offset. */
	inline float GetDistanceOffset() const{ return pDistanceOffset; }
	
	/** \brief Set distance offset. */
	void SetDistanceOffset( float distanceOffset );
	
	/** \brief Play speed. */
	inline float GetPlaySpeed() const{ return pPlaySpeed; }
	
	/** \brief Set play speed. */
	void SetPlaySpeed( float playSpeed );
	
	
	
	/** \brief Property name is set. */
	bool IsPropertySet( eProperties property ) const;
	
	/** \brief Name of property. */
	const decString &GetPropertyName( eProperties property ) const;
	
	/** \brief Set name of property. */
	void SetPropertyName( eProperties property, const char *name );
	
	/** \brief One or more properties use a name. */
	bool HasPropertyWithName( const char *name ) const;
	
	
	
	/** \brief Trigger name is set. */
	bool IsTriggerSet( eTriggers trigger ) const;
	
	/** \brief Name of trigger. */
	const decString &GetTriggerName( eTriggers trigger ) const;
	
	/** \brief Set name of trigger. */
	void SetTriggerName( eTriggers trigger, const char *name );
	/*@}*/
};

#endif
