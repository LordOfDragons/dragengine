/* 
 * Drag[en]gine Bullet Physics Module
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

#ifndef _DEBTCONFIGURATION_H_
#define _DEBTCONFIGURATION_H_

class dePhysicsBullet;
class decBaseFileReader;
class decXmlElementTag;
class decXmlAttValue;



/**
 * @brief Bullet Configuration.
 *
 * Handles the configuration of the bullet module.
 */
class debpConfiguration{
private:
	dePhysicsBullet *pBullet;
	
	bool pEnableConstraintPoint2Point;
	bool pEnableConstraintHinge;
	bool pEnableConstraintConeTwist;
	bool pEnableConstraintSlider;
	
	bool pSimulatePropFields;
	
public:
	/** @name Constructor, destructor */
	/*@{*/
	/** Creates a new bullet configuration. */
	debpConfiguration( dePhysicsBullet *bullet );
	/** Cleans up the bullet configuration. */
	~debpConfiguration();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Determines if the point 2 point constraint is enabled. */
	inline bool GetEnableConstraintPoint2Point() const{ return pEnableConstraintPoint2Point; }
	/** Sets if the point 2 point constraint is enabled. */
	void SetEnableConstraintPoint2Point( bool enable );
	/** Determines if the hinge constraint is enabled. */
	inline bool GetEnableConstraintHinge() const{ return pEnableConstraintHinge; }
	/** Sets if the hinge constraint is enabled. */
	void SetEnableConstraintHinge( bool enable );
	/** Determines if the cone twist constraint is enabled. */
	inline bool GetEnableConstraintConeTwist() const{ return pEnableConstraintConeTwist; }
	/** Sets if the cone twist constraint is enabled. */
	void SetEnableConstraintConeTwist( bool enable );
	/** Determines if the slider constraint is enabled. */
	inline bool GetEnableConstraintSlider() const{ return pEnableConstraintSlider; }
	/** Sets if the slider constraint is enabled. */
	void SetEnableConstraintSlider( bool enable );
	
	/** Determines if prop fields are simulated. */
	inline bool GetSimulatePropFields() const{ return pSimulatePropFields; }
	/** Sets if prop fields are simulated. */
	void SetSimulatePropFields( bool simulatePropFields );
	/*@}*/
	
	/** @name Loading and Saving */
	/*@{*/
	/** Loads the configuration from the file if existing. */
	void LoadConfig();
	/** Saves the configuration to file. */
	/*@}*/
	void SaveConfig();
	
private:
	decXmlElementTag *pGetTagAt( decXmlElementTag *tag, int index );
	decXmlAttValue *pFindAttribute( decXmlElementTag *tag, const char *name );
	
	void pParseConfig( decXmlElementTag *root );
	void pParseProperty( decXmlElementTag *root );
};

#endif
