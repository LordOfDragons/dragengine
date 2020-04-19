/* 
 * Drag[en]gine OpenAL Audio Module
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

#ifndef _DEOALCOMPONENTTEXTURE_H_
#define _DEOALCOMPONENTTEXTURE_H_

class deoalComponent;
class deoalAComponentTexture;
class deoalSkin;

class deComponentTexture;



/**
 * \brief Component texture.
 */
class deoalComponentTexture{
private:
	deoalComponent &pComponent;
	const int pIndex;
	deoalAComponentTexture *pATexture;
	
	deoalSkin *pSkin;
	
	bool pDirtyTexture;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create component texture. */
	deoalComponentTexture( deoalComponent &component, int index );
	
	/** \brief Clean up component texture. */
	~deoalComponentTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Component. */
	inline deoalComponent &GetComponent() const{ return pComponent; }
	
	/** \brief Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** \brief Audio component texture. */
	inline deoalAComponentTexture *GetATexture() const{ return pATexture; }
	
	/** \brief Synchronize. */
	void Synchronize();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Texture changed. */
	void TextureChanged( const deComponentTexture &texture );
	
	/** \brief Mark texture dirty. */
	void MarkDirty();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
