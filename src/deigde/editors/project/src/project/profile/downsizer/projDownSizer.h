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
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<projDownSizer> Ref;


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
