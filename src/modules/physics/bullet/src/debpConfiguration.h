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
	debpConfiguration(dePhysicsBullet *bullet);
	/** Cleans up the bullet configuration. */
	~debpConfiguration();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Determines if the point 2 point constraint is enabled. */
	inline bool GetEnableConstraintPoint2Point() const{return pEnableConstraintPoint2Point;}
	/** Sets if the point 2 point constraint is enabled. */
	void SetEnableConstraintPoint2Point(bool enable);
	/** Determines if the hinge constraint is enabled. */
	inline bool GetEnableConstraintHinge() const{return pEnableConstraintHinge;}
	/** Sets if the hinge constraint is enabled. */
	void SetEnableConstraintHinge(bool enable);
	/** Determines if the cone twist constraint is enabled. */
	inline bool GetEnableConstraintConeTwist() const{return pEnableConstraintConeTwist;}
	/** Sets if the cone twist constraint is enabled. */
	void SetEnableConstraintConeTwist(bool enable);
	/** Determines if the slider constraint is enabled. */
	inline bool GetEnableConstraintSlider() const{return pEnableConstraintSlider;}
	/** Sets if the slider constraint is enabled. */
	void SetEnableConstraintSlider(bool enable);
	
	/** Determines if prop fields are simulated. */
	inline bool GetSimulatePropFields() const{return pSimulatePropFields;}
	/** Sets if prop fields are simulated. */
	void SetSimulatePropFields(bool simulatePropFields);
	/*@}*/
	
	/** @name Loading and Saving */
	/*@{*/
	/** Loads the configuration from the file if existing. */
	void LoadConfig();
	/** Saves the configuration to file. */
	/*@}*/
	void SaveConfig();
	
private:
	decXmlElementTag *pGetTagAt(decXmlElementTag *tag, int index);
	decXmlAttValue *pFindAttribute(decXmlElementTag *tag, const char *name);
	
	void pParseConfig(decXmlElementTag *root);
	void pParseProperty(decXmlElementTag *root);
};

#endif
