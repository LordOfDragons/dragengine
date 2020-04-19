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

#ifndef _DESYNSYNTHESIZERTARGET_H_
#define _DESYNSYNTHESIZERTARGET_H_

class desynSynthesizerInstance;
class desynSynthesizerLink;
class desynSynthesizer;

class deSynthesizerControllerTarget;
class deSynthesizerLink;



/**
 * \brief Synthesizer target.
 */
class desynSynthesizerTarget{
private:
	const desynSynthesizerLink **pLinks;
	int pLinkCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer source. */
	desynSynthesizerTarget( const desynSynthesizer &synthesizer, int firstLink,
		const deSynthesizerControllerTarget &target );
	
	/** \brief Clean up synthesizer source. */
	virtual ~desynSynthesizerTarget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of links. */
	inline int GetLinkCount() const{ return pLinkCount; }
	
	/** \brief Link at index. */
	const desynSynthesizerLink &GetLinkAt( int index ) const;
	
	
	
	/** \brief Value of target. */
	float GetValue( const desynSynthesizerInstance &instance, int sample, float defaultValue ) const;
	/*@}*/
};

#endif
