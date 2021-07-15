/* 
 * Drag[en]gine Shared Vulkan
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include <stdlib.h>
#include <string.h>

#include "devkPipelineConfiguration.h"
#include "../descriptor/devkDescriptorSetLayout.h"
#include "../descriptor/devkDescriptorSetLayoutConfiguration.h"

#include <dragengine/common/exceptions.h>


// class devkPipelineConfiguration
////////////////////////////////////

devkPipelineConfiguration::devkPipelineConfiguration() :
pType( etGraphics ),
pDescriptorSetLayout( nullptr ),
pShaderVertex( nullptr ),
pShaderTessellationControl( nullptr ),
pShaderTessellationEvaluation( nullptr ),
pShaderGeometry( nullptr ),
pShaderFragment( nullptr ),
pShaderCompute( nullptr ),
pShaderRayGen( nullptr ),
pShaderAnyHit( nullptr ),
pShaderClosestHit( nullptr ),
pShaderMiss( nullptr ),
pShaderIntersection( nullptr ),
pShaderCallable( nullptr ),
pShaderTask( nullptr ),
pShaderMesh( nullptr ){
}

devkPipelineConfiguration::devkPipelineConfiguration( const devkPipelineConfiguration &configuration ) :
pType( etGraphics ),
pDescriptorSetLayout( nullptr ),
pShaderVertex( nullptr ),
pShaderTessellationControl( nullptr ),
pShaderTessellationEvaluation( nullptr ),
pShaderGeometry( nullptr ),
pShaderFragment( nullptr ),
pShaderCompute( nullptr ),
pShaderRayGen( nullptr ),
pShaderAnyHit( nullptr ),
pShaderClosestHit( nullptr ),
pShaderMiss( nullptr ),
pShaderIntersection( nullptr ),
pShaderCallable( nullptr ),
pShaderTask( nullptr ),
pShaderMesh( nullptr )
{
	*this = configuration;
}

devkPipelineConfiguration::~devkPipelineConfiguration(){
}



// Management
///////////////

void devkPipelineConfiguration::SetType( eType type ){
	pType = type;
}

void devkPipelineConfiguration::SetDescriptorSetLayout( devkDescriptorSetLayout *layout ){
	pDescriptorSetLayout = layout;
}


void devkPipelineConfiguration::SetShaderVertex( devkShaderModule *shader ){
	pShaderVertex = shader;
}

void devkPipelineConfiguration::SetShaderTessellationControl( devkShaderModule *shader ){
	pShaderTessellationControl = shader;
}

void devkPipelineConfiguration::SetShaderTessellationEvaluation( devkShaderModule *shader ){
	pShaderTessellationEvaluation = shader;
}

void devkPipelineConfiguration::SetShaderGeometry( devkShaderModule *shader ){
	pShaderGeometry = shader;
}

void devkPipelineConfiguration::SetShaderFragment( devkShaderModule *shader ){
	pShaderFragment = shader;
}



void devkPipelineConfiguration::SetShaderCompute( devkShaderModule *shader ){
	pShaderCompute = shader;
}



void devkPipelineConfiguration::SetShaderRayGen( devkShaderModule *shader ){
	pShaderRayGen = shader;
}

void devkPipelineConfiguration::SetShaderAnyHit( devkShaderModule *shader ){
	pShaderAnyHit = shader;
}

void devkPipelineConfiguration::SetShaderClosestHit( devkShaderModule *shader ){
	pShaderClosestHit = shader;
}

void devkPipelineConfiguration::SetShaderMiss( devkShaderModule *shader ){
	pShaderMiss = shader;
}

void devkPipelineConfiguration::SetShaderIntersection( devkShaderModule *shader ){
	pShaderIntersection = shader;
}

void devkPipelineConfiguration::SetShaderCallable( devkShaderModule *shader ){
	pShaderCallable = shader;
}


void devkPipelineConfiguration::SetShaderTask( devkShaderModule *shader ){
	pShaderTask = shader;
}

void devkPipelineConfiguration::SetShaderMesh( devkShaderModule *shader ){
	pShaderMesh = shader;
}



void devkPipelineConfiguration::SetSpecialization( devkSpecialization *specialization ){
	pSpecialization = specialization;
}



// Operators
//////////////

bool devkPipelineConfiguration::operator==( const devkPipelineConfiguration &configuration ) const{
	return pType == configuration.pType
	&& pDescriptorSetLayout == configuration.pDescriptorSetLayout
	&& pShaderVertex == configuration.pShaderVertex
	&& pShaderTessellationControl == configuration.pShaderTessellationControl
	&& pShaderTessellationEvaluation == configuration.pShaderTessellationEvaluation
	&& pShaderGeometry == configuration.pShaderGeometry
	&& pShaderFragment == configuration.pShaderFragment
	&& pShaderCompute == configuration.pShaderCompute
	&& pShaderRayGen == configuration.pShaderRayGen
	&& pShaderAnyHit == configuration.pShaderAnyHit
	&& pShaderClosestHit == configuration.pShaderClosestHit
	&& pShaderMiss == configuration.pShaderMiss
	&& pShaderIntersection == configuration.pShaderIntersection
	&& pShaderCallable == configuration.pShaderCallable
	&& pShaderTask == configuration.pShaderTask
	&& pShaderMesh == configuration.pShaderMesh
	&& pSpecialization == configuration.pSpecialization;
}

devkPipelineConfiguration &devkPipelineConfiguration::operator=( const devkPipelineConfiguration &configuration ){
	pType = configuration.pType;
	pDescriptorSetLayout = configuration.pDescriptorSetLayout;
	pShaderVertex = configuration.pShaderVertex;
	pShaderTessellationControl = configuration.pShaderTessellationControl;
	pShaderTessellationEvaluation = configuration.pShaderTessellationEvaluation;
	pShaderGeometry = configuration.pShaderGeometry;
	pShaderFragment = configuration.pShaderFragment;
	pShaderCompute = configuration.pShaderCompute;
	pShaderRayGen = configuration.pShaderRayGen;
	pShaderAnyHit = configuration.pShaderAnyHit;
	pShaderClosestHit = configuration.pShaderClosestHit;
	pShaderMiss = configuration.pShaderMiss;
	pShaderIntersection = configuration.pShaderIntersection;
	pShaderCallable = configuration.pShaderCallable;
	pShaderTask = configuration.pShaderTask;
	pShaderMesh = configuration.pShaderMesh;
	pSpecialization = configuration.pSpecialization;
	return *this;
}
