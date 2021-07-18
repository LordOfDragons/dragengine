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

#ifndef _DESYNTHESIZERSOURCE_H_
#define _DESYNTHESIZERSOURCE_H_

#include "../deSynthesizerControllerTarget.h"
#include "../../../deObject.h"
#include "../../../common/collection/decObjectOrderedSet.h"

class deSynthesizer;
class deSynthesizerEffect;
class deSynthesizerSourceVisitor;


/**
 * \brief Synthesizer source.
 */
class deSynthesizerSource : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deSynthesizerSource> Ref;
	
	
	
public:
	/** \brief Mixing modes. */
	enum eMixModes{
		/** \brief Blend sound output of this source over the previous sound. */
		emmBlend,
		
		/** \brief Add sound output to the previous sounds clipping if too loud. */
		emmAdd
	};
	
	
	
private:
	deSynthesizer *pSynthesizer;
	eMixModes pMixMode;
	float pBlendFactor;
	bool pEnabled;
	
	float pMinVolume;
	float pMaxVolume;
	float pMinPanning;
	float pMaxPanning;
	
	deSynthesizerControllerTarget pTargetBlendFactor;
	deSynthesizerControllerTarget pTargetVolume;
	deSynthesizerControllerTarget pTargetPanning;
	
	decObjectOrderedSet pEffects;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer source. */
	deSynthesizerSource();
	
protected:
	/**
	 * \brief Clean up synthesizer source.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deSynthesizerSource();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Parent synthesizer. */
	inline deSynthesizer *GetSynthesizer() const{ return pSynthesizer; }
	
	/**
	 * \brief Set parent synthesizer.
	 * 
	 * This function has to be used only by the deSynthesizer class to
	 * assign itself as a parent. Do not call this function directly.
	 */
	void SetSynthesizer( deSynthesizer *synthesizer );
	
	/** \brief Source is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if source is enabled. */
	void SetEnabled( bool enabled );
	
	/** \brief Mixing mode. */
	inline eMixModes GetMixMode() const{ return pMixMode; }
	
	/** \brief Set mixing mode. */
	void SetMixMode( eMixModes mode );
	
	/** \brief Blend factor. */
	inline float GetBlendFactor() const{ return pBlendFactor; }
	
	/** \brief Set blend factor. */
	void SetBlendFactor( float factor );
	
	
	
	/** \brief Volume minimum relative to speaker volume in the range from 0 to 1. */
	inline float GetMinVolume() const{ return pMinVolume; }
	
	/** \brief Set minimum volume relative to speaker volume in the range from 0 to 1. */
	void SetMinVolume( float volume );
	
	/** \brief Maximum volume relative to speaker volume in the range from 0 to 1. */
	inline float GetMaxVolume() const{ return pMaxVolume; }
	
	/** \brief Set maximum volume relative to speaker volume in the range from 0 to 1. */
	void SetMaxVolume( float volume );
	
	/** \brief Minimum panning in the range from -1(left) to 1(right). */
	inline float GetMinPanning() const{ return pMinPanning; }
	
	/** \brief Set minimum panning in the range from -1(left) to 1(right). */
	void SetMinPanning( float panning );
	
	/** \brief Maximum panning in the range from -1(left) to 1(right). */
	inline float GetMaxPanning() const{ return pMaxPanning; }
	
	/** \brief Set maximum panning in the range from -1(left) to 1(right). */
	void SetMaxPanning( float panning );
	
	
	
	/** \brief Blend factor target. */
	inline deSynthesizerControllerTarget &GetTargetBlendFactor(){ return pTargetBlendFactor; }
	inline const deSynthesizerControllerTarget &GetTargetBlendFactor() const{ return pTargetBlendFactor; }
	
	/** \brief Volume target. */
	inline deSynthesizerControllerTarget &GetTargetVolume(){ return pTargetVolume; }
	inline const deSynthesizerControllerTarget &GetTargetVolume() const{ return pTargetVolume; }
	
	/** \brief Panning target. */
	inline deSynthesizerControllerTarget &GetTargetPanning(){ return pTargetPanning; }
	inline const deSynthesizerControllerTarget &GetTargetPanning() const{ return pTargetPanning; }
	/*@}*/
	
	
	
	/** \name Effects */
	/*@{*/
	/** \brief Number of effects. */
	int GetEffectCount() const;
	
	/** \brief Effect at index. */
	deSynthesizerEffect *GetEffectAt( int index ) const;
	
	/** \brief Add effect. */
	void AddEffect( deSynthesizerEffect *effect );
	
	/** \brief Remove effect. */
	void RemoveEffect( deSynthesizerEffect *effect );
	
	/** \brief Remove all effects. */
	void RemoveAllEffects();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit source. */
	virtual void Visit( deSynthesizerSourceVisitor &visitor );
	/*@}*/
};

#endif
