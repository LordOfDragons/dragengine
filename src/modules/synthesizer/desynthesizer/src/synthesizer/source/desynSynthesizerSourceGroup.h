/* 
 * Drag[en]gine Synthesizer Module
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

#ifndef _DESYNSYNTHESIZERSOURCEGROUP_H_
#define _DESYNSYNTHESIZERSOURCEGROUP_H_

#include "desynSynthesizerSource.h"
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceGroup.h>

class deSynthesizerSourceGroup;
class desynSynthesizerSource;



/**
 * \brief Synthesizer source group.
 */
class desynSynthesizerSourceGroup : public desynSynthesizerSource{
private:
	desynSynthesizerSource **pSources;
	int pSourceCount;
	
	deSynthesizerSourceGroup::eApplicationTypes pApplicationType;
	float pSelectRange;
	
	desynSynthesizerTarget pTargetSelect;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer source. */
	desynSynthesizerSourceGroup( desynSynthesizer &synthesizer, int firstLink,
		const deSynthesizerSourceGroup &source );
	
	/** \brief Clean up synthesizer source. */
	virtual ~desynSynthesizerSourceGroup();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Select target. */
	inline desynSynthesizerTarget &GetTargetSelect(){ return pTargetSelect; }
	inline const desynSynthesizerTarget &GetTargetSelect() const{ return pTargetSelect; }
	
	
	
	/** \brief Select value. */
	float GetSelect( const desynSynthesizerInstance &instance, int sample ) const;
	
	
	
	/**
	 * \brief State data size of the source itself.
	 * \details Store state data position and return required state data size. Default implementation
	 *          stores the offset and returns 0.
	 */
	virtual int StateDataSizeSource( int offset );
	
	/** \brief Init state data of source itself. */
	virtual void InitStateDataSource( char *stateData );
	
	/**
	 * \brief Generate sound using source.
	 * \details Actual sound generation implementation of the source. Used to split general sound
	 *          processing of the source from the actual source specific sound generation.
	 * \param[in,out] stateData State at start of buffer. Update with state at start of next buffer.
	 * \param[out] buffer Buffer to store samples in.
	 * \param[in] samples Number of samples to produce.
	 */
	virtual void GenerateSourceSound( const desynSynthesizerInstance &instance, char *stateData,
		float *buffer, int samples, float curveOffset, float curveFactor );
	
	/** \brief Generate sound using all mode. */
	void GenerateSoundAll( const desynSynthesizerInstance &instance, char *stateData,
		float *buffer, int samples, float curveOffset, float curveFactor );
	
	/** \brief Generate sound using select mode. */
	void GenerateSoundSelect( const desynSynthesizerInstance &instance, char *stateData,
		float *buffer, int samples, float curveOffset, float curveFactor );
	
	/** \brief Generate sound using solo mode. */
	void GenerateSoundSolo( const desynSynthesizerInstance &instance, char *stateData,
		float *buffer, int samples, float curveOffset, float curveFactor );
	
	/**
	 * \brief Skip sound.
	 * \details Actual skip implementation of the source. Used to split general skip processing of
	 *          source from the actual source specific skip processing and allow effect processing.
	 * \param[in,out] stateData State at start of skipping. Update with state after skipping.
	 * \param[in] samples Number of samples to skip.
	 */
	virtual void SkipSourceSound( const desynSynthesizerInstance &instance, char *stateData,
		int samples, float curveOffset, float curveFactor );
	/*@}*/
	
	
	
private:
	void pCreateSources( desynSynthesizer &synthesizer, int firstLink, const deSynthesizerSourceGroup &source );
	void pClearSources();
};

#endif
