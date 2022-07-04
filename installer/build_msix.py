#!/usr/bin/python3

import os, sys, shutil, requests, zipfile, io, fnmatch

def copyArchiveFiles(artifact, basePath):
	with zipfile.ZipFile(artifact, mode='r') as z:
		for n in z.namelist():
			if n.startswith('@ProgramFiles/'):
				path = 'VFS/ProgramFilesX64/' + n[14:]
			elif n.startswith('@System/'):
				path = 'VFS/SystemX64/' + n[8:]
			path = os.path.join(basePath, path)
			os.makedirs(os.path.dirname(path), exist_ok=True)
			with z.open(n) as f:
				with open(path, 'wb') as g:
					g.write(f.read())

def copyDataFiles(directory, basePath):
	dataFiles = []
	for root, dirs, files in os.walk(directory):
		if '.svn' in dirs:
			dirs.remove('.svn')
		if '.git' in dirs:
			dirs.remove('.git')
		for s in files:
			dataFiles.append(os.path.join(root, s))
	dataFiles = [x for x in dataFiles if not fnmatch.fnmatch(x, '*.in')]
	
	cutLen = len(directory.split('/'))
	for f in dataFiles:
		path = f.split('/')
		target = os.path.join(basePath, *path[cutLen:])
		os.makedirs(os.path.dirname(target), exist_ok=True)
		shutil.copyfile(f, target)

# get input
if len(sys.argv) != 2:
	print('./build_msix.py version')
	print('')
	print('version => major.minor{.patch}')
	sys.exit(0)

versionRelease = sys.argv[1]

versionMsix = versionRelease
while len(versionMsix.split('.')) < 4:
	versionMsix = '{}.0'.format(versionMsix)
print('MSIX Version: {}'.format(versionMsix))

# prepare
srcDir = os.getcwd()
buildDirName = 'build_msix'
buildDir = os.path.join(os.getcwd(), buildDirName)
artifactDir = os.path.join(os.getcwd(), 'artifacts')

if os.path.exists(buildDir):
	shutil.rmtree(buildDir)

os.makedirs(buildDir)

# check for artifacts
artifactDragengine = os.path.join(artifactDir, 'dragengine-{}-windows64.zip'.format(versionRelease))
artifactIgde = os.path.join(artifactDir, 'deigde-{}-windows64.zip'.format(versionRelease))

if not os.path.exists(artifactDragengine) or not os.path.exists(artifactIgde):
	print('Download GitHub Release Action run Artifacts "archives-windows"')
	print('and place them in directory "artifacts"')

# dragengine
copyArchiveFiles(artifactDragengine, os.path.join(buildDir, 'msix/dragengine'))
copyDataFiles(os.path.join(srcDir, 'msix-data/dragengine'), os.path.join(buildDir, 'msix/dragengine'))

with open(os.path.join(srcDir, 'msix-data/dragengine/AppxManifest.xml.in'), 'r') as f:
	content = f.read()
	content = content.replace('%VERSION%', versionMsix)
	with open(os.path.join(buildDir, 'msix/dragengine/AppxManifest.xml'), 'w') as g:
		g.write(content)

cmdline = 'docker run -it --rm --volume="{}":/sources compile-windows-crosscompile makemsix pack -d "{}" -p "{}"'.format(
	srcDir,
	'/sources/{}/msix/dragengine'.format(buildDirName),
	'/sources/{}/dragengine-{}.msix'.format(buildDirName, versionRelease))
if os.system(cmdline):
	print(cmdline)
	print('failed')
	sys.exit(1)

# igde
# copied until microsoft provides a solution for this problem
copyArchiveFiles(artifactDragengine, os.path.join(buildDir, 'msix/deigde'))
# end of microsoft hack
copyArchiveFiles(artifactIgde, os.path.join(buildDir, 'msix/deigde'))
copyDataFiles(os.path.join(srcDir, 'msix-data/deigde'), os.path.join(buildDir, 'msix/deigde'))

with open(os.path.join(srcDir, 'msix-data/deigde/AppxManifest.xml.in'), 'r') as f:
	content = f.read()
	content = content.replace('%VERSION%', versionMsix)
	with open(os.path.join(buildDir, 'msix/deigde/AppxManifest.xml'), 'w') as g:
		g.write(content)

cmdline = 'docker run -it --rm --volume="{}":/sources compile-windows-crosscompile makemsix pack -d "{}" -p "{}"'.format(
	srcDir,
	'/sources/{}/msix/deigde'.format(buildDirName),
	'/sources/{}/deigde-{}.msix'.format(buildDirName, versionRelease))
if os.system(cmdline):
	print(cmdline)
	print('failed')
	sys.exit(1)
