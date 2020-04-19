/* 
 * Drag[en]gine IGDE Project Editor
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

#ifndef _PROJDOWNSIZER_H_
#define _PROJDOWNSIZER_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decStringSet.h>


class projProfile;



/**
 * \brief Resource downsizer.
 * 
 * Resources matching pattern list are down-sized to a indicated quality level. Quality level
 * of 100 indicates no quality reduction with values below indicating increasing quality
 * degradation. For images this would equal to the compression quality being increased with
 * lower quality setting. Optionall images can be scaled down in size by powers of 2. The
 * down-sizer tries to apply a logic best fitting the resources matching pattern list.
 */
class projDownSizer : public deObject{
private:
	projProfile *pProfile;
	
	decString pDescription;
	decStringSet pPatterns;
	int pQuality;
	int pScale;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create down sizer. */
	projDownSizer();
	
	/** \brief Create copy of down-sizer. */
	projDownSizer( const projDownSizer &downSizer );
	
protected:
	/** \brief Clean up down-sizer. */
	virtual ~projDownSizer();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Parent profile. */
	inline projProfile *GetProfile() const{ return pProfile; }
	
	/** \brief Set parent profile. */
	void SetProfile( projProfile *profile );
	
	
	
	/** \brief Description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description. */
	void SetDescription( const char *description );
	
	/** \brief Set of file patterns to apply down-sizing to. */
	inline const decStringSet &GetPatterns() const{ return pPatterns; }
	void SetPatterns( const decStringSet &patterns );
	
	/** \brief Target quality level in the range from 100 (best) to 0 (worst). */
	inline int GetQuality() const{ return pQuality; }
	
	/** \brief Set target quality level in the range from 100 (best) to 0 (worst). */
	void SetQuality( int quality );
	
	/** \brief Number of times to reduce size by factor 2. */
	inline int GetScale() const{ return pScale; }
	
	/** \brief Set number of times to reduce size by factor 2. */
	void SetScale( int scale );
	
	
	
	/** \brief Notify listeners profile changed. */
	void NotifyChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
