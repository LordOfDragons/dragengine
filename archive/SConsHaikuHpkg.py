from SConsCommon import *

import os
import re
import shutil

Import('parent_env parent_targets parent_report')

if not parent_env['OSBeOS']:
	Return()

envPackage = parent_env.Clone()

reWindowsDrive = re.compile('^([A-Z]:[/\\\\])|([A-Z][A-Z0-9]*//)', re.I)
def normalizePath(path):
	path = os.path.normpath(path)
	path = os.path.splitdrive(path)[1] # return (drive, tail)
	# for cross compiling splitdrive wont work since python is compiled for
	# the host system and os.path methods work with host system parameters.
	# use a simple regular expression check to see if we need to cut the
	# start of the string
	match = reWindowsDrive.match(path)
	if match:
		path = path[match.end():]
	return path

def createHaikuHpkg(env, target, source):
	pathHpkg = target[0].abspath
	pathPackageInfo = env['PackageInfo'].abspath
	dirCollect = env['CollectDir'].abspath
	
	# delete package file if present
	if os.path.exists(pathHpkg):
		os.unlink(pathHpkg)
	
	# delete collect dir if existing and create it in case the archive is empty
	if os.path.exists(dirCollect):
		shutil.rmtree(dirCollect)
	os.makedirs(dirCollect)
	
	# copy all files to package into the collect directory
	for path, node in env['PackageFiles'].items():
		if path[0:1] == '/':
			path = path[1:]
		fileDir = os.path.join(dirCollect, os.path.dirname(path))
		if not os.path.exists(fileDir):
			os.makedirs(fileDir)
		shutil.copy(node.abspath, fileDir)
	shutil.copy(pathPackageInfo, os.path.join(dirCollect, '.PackageInfo'))
	
	if os.system('package create -q -C "{}" "{}"'.format(dirCollect, pathHpkg)):
		raise Exception("Create package failed")
	
	# delete collect dir since it is not required anymore and will be deleted the next time anyway
	shutil.rmtree(dirCollect)

# fetch values in expanded form for later use
versionString = '1.2'
if envPackage['force_version']:
	versionString = envPackage['force_version']

# collect files to archive
filesEngine = {}
filesEngineDevelop = {}
filesIGDE = {}
filesIGDEDevelop = {}

for target in parent_targets.values():
	if 'archive-engine' in target:
		filesEngine.update(target['archive-engine'])
	if 'archive-engine-develop' in target:
		filesEngineDevelop.update(target['archive-engine-develop'])
	if 'archive-launcher' in target:
		filesEngine.update( target['archive-launcher'])
	if 'archive-igde' in target:
		filesIGDE.update(target['archive-igde'])
	if 'archive-igde-develop' in target:
		filesIGDEDevelop.update(target['archive-igde-develop'])

# create builders
package = []

filename = 'dragengine-{}-1-x86_64.hpkg'.format(versionString)
package.append(envPackage.Command(filename, filesEngine.values(),
	envPackage.Action(createHaikuHpkg, 'Packaging {}'.format(filename)),
	CollectDir=envPackage.Dir('hpkg_dragengine'),
	PackageInfo=envPackage.File('haiku_packageinfo/dragengine').srcnode(),
	PackageFiles=filesEngine))

filename = 'dragengine_devel-{}-1-x86_64.hpkg'.format(versionString)
package.append(envPackage.Command(filename, filesEngineDevelop.values(),
	envPackage.Action(createHaikuHpkg, 'Packaging {}'.format(filename)),
	CollectDir=envPackage.Dir('hpkg_dragengine_develop'),
	PackageInfo=envPackage.File('haiku_packageinfo/dragengine-develop').srcnode(),
	PackageFiles=filesEngineDevelop))

filename = 'deigde-{}-1-x86_64.hpkg'.format(versionString)
package.append(envPackage.Command(filename, filesIGDE.values(),
	envPackage.Action(createHaikuHpkg, 'Packaging {}'.format(filename)),
	CollectDir=envPackage.Dir('hpkg_deigde'),
	PackageInfo=envPackage.File('haiku_packageinfo/igde').srcnode(),
	PackageFiles=filesIGDE))

filename = 'deigde_develop-{}-1-x86_64.hpkg'.format(versionString)
package.append(envPackage.Command(filename, filesIGDEDevelop.values(),
	envPackage.Action(createHaikuHpkg, 'Packaging {}'.format(filename)),
	CollectDir=envPackage.Dir('hpkg_deigde_develop'),
	PackageInfo=envPackage.File('haiku_packageinfo/igde-develop').srcnode(),
	PackageFiles=filesIGDEDevelop))

for target in parent_targets.values():
	if 'package' in target:
		envPackage.Depends(package, target['build'])

envPackage.Alias('package', package)

parent_targets['package'] = {'name': 'Packageing Drag[en]gine Game Engine'}
