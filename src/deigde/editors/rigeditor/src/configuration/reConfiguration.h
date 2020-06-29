/* 
 * Drag[en]gine IGDE Rig Editor
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

#ifndef _RECONFIGURATION_H_
#define _RECONFIGURATION_H_

class reWindowMain;



/**
 * \brief Editor configuration.
 */
class reConfiguration{
private:
	reWindowMain &pWindowMain;
	
	float pGridSize;
	bool pSnapToGrid;
	float pRotSnap;
	float pSensitivity;
	
	bool pPreventSaving;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create configuration. */
	reConfiguration( reWindowMain &windowMain );
	
	/** \brief Clean up configuration. */
	~reConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline reWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Grid size. */
	inline float GetGridSize() const{ return pGridSize; }
	
	/** \brief Set grid size. */
	void SetGridSize( float size );
	
	/** \brief Snap elements to grid. */
	inline bool GetSnapToGrid() const{ return pSnapToGrid; }
	
	/** \brief Set snap elements to grid. */
	void SetSnapToGrid( bool snap );
	
	/** \brief Rotation snap angle. */
	inline float GetRotSnapAngle() const{ return pRotSnap; }
	
	/** \brief Set rotation snap angle. */
	void SetRotSnapAngle( float angle );
	
	/** \brief Mouse sensitivity. */
	inline float GetSensitivity() const{ return pSensitivity; }
	
	/** \brief Set mouse sensitivity. */
	void SetSensitivity( float sensitivity );
	
	
	
	/** \brief Prevent saving. */
	inline bool GetPreventSaving() const{ return pPreventSaving; }
	
	/** \brief Set prevent saving. */
	void SetPreventSaving( bool preventSaving );
	
	/** \brief Load configuration. */
	void LoadConfiguration();
	
	/** \brief Save configuration. */
	void SaveConfiguration();
	/*@}*/
	
	
	
private:
	void pReset();
};

#endif
