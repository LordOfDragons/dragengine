#!/usr/bin/env python3
"""
Script to update Visual Studio project files (.vcxproj) with correct header and source files.
Scans the source directories and updates ClCompile and ClInclude entries in .vcxproj files.

NOTE
This script has been created by copilot. If you find any issues, please fix them manually.

NOTE
If new modules or editors are added update the script to process them accordingly.
"""

import os
import sys
import xml.etree.ElementTree as ET
from pathlib import Path
from typing import List, Tuple, Set

# XML namespace for MSBuild
NS = {'msbuild': 'http://schemas.microsoft.com/developer/msbuild/2003'}
ET.register_namespace('', 'http://schemas.microsoft.com/developer/msbuild/2003')

exclude_files = {
	# Add any files to be excluded from the project here
	'deOSConsole.cpp', 'deOSConsole.h',
	'deOSAndroid.cpp', 'deOSAndroid.h',
	'deOSBeOS.cpp', 'deOSBeOS.h',
	'deOSMacOS.cpp', 'deOSMacOS.h',
	'deOSUnix.cpp', 'deOSUnix.h',
	'deOSWebWasm.cpp', 'deOSWebWasm.h'
}


def find_source_files(src_dir: Path, extensions: Set[str]) -> List[str]:
	"""Find all source files with given extensions in a directory."""
	files = []
	if not src_dir.exists():
		return files
	
	for ext in extensions:
		files.extend(sorted(src_dir.rglob(f'*{ext}')))
	
	return [str(f) for f in files]


def make_relative_path(file_path: str, project_dir: str) -> str:
	"""Convert absolute path to relative path from project directory."""
	file_p = Path(file_path).resolve()
	proj_p = Path(project_dir).resolve()
	
	try:
		rel_path = file_p.relative_to(proj_p)
		# Convert to Windows-style path
		return str(rel_path).replace('/', '\\')
	except ValueError:
		# If can't make relative, use ../ to go up
		rel_path = os.path.relpath(file_path, project_dir)
		return rel_path.replace('/', '\\')


def get_source_dir_from_vcxproj(vcxproj_path: Path) -> List[Path]:
	"""Determine the source directory from .vcxproj file location."""
	# Map vcxproj path to source directory
	vcxproj_str = str(vcxproj_path)
	vs_dir = Path(__file__).parent.resolve()
	root_dir = vs_dir.parent
	
	# Dragengine
	if 'vs/dragengine/dragengine.vcxproj' in vcxproj_str:
		return [root_dir / 'src' / 'dragengine' / 'src',
			root_dir / 'src' / 'dragengine' / 'base64']
	
	# Modules
	if '/modules/ai/deai/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'ai' / 'deai' / 'src']
	if '/modules/animation/deanim/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'animation' / 'deanim' / 'src']
	if '/modules/animator/deanimator/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'animator' / 'deanimator' / 'src']
	if '/modules/archive/delga/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'archive' / 'delga' / 'src',
			root_dir / 'src' / 'modules' / 'archive' / 'delga' / 'minizip' / 'unzip.c',
			root_dir / 'src' / 'modules' / 'archive' / 'delga' / 'minizip' / 'ioapi.c']
	if '/modules/audio/audnull/' in vcxproj_str or '/modules/audio/null/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'audio' / 'null' / 'src']
	if '/modules/audio/openal/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'audio' / 'openal' / 'src']
	if '/modules/combined/fbx/animation/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'combined' / 'fbx' / 'src' / 'animation']
	if '/modules/combined/fbx/model/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'combined' / 'fbx' / 'src' / 'model']
	if '/modules/combined/fbx/rig/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'combined' / 'fbx' / 'src' / 'rig']
	if '/modules/combined/fbx/skin/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'combined' / 'fbx' / 'src' / 'skin']
	if '/modules/crashrecovery/simplyquit/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'crashrecovery' / 'simplyquit' / 'src']
	if '/modules/crashrecovery/basicrecovery/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'crashrecovery' / 'basicrecovery' / 'src']
	if '/modules/font/defont/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'font' / 'defont' / 'src']
	if '/modules/font/freetype/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'font' / 'freetype' / 'src']
	if '/modules/graphic/null/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'graphic' / 'null' / 'src']
	if '/modules/graphic/opengl/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'graphic' / 'opengl' / 'src']
	if '/modules/image/ies/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'image' / 'ies' / 'src']
	if '/modules/image/jpeg/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'image' / 'jpeg' / 'src']
	if '/modules/image/png3d/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'image' / 'png3d' / 'src']
	if '/modules/image/png/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'image' / 'png' / 'src']
	if '/modules/image/tga/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'image' / 'tga' / 'src']
	if '/modules/image/webp3d/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'image' / 'webp3d' / 'src']
	if '/modules/image/webp/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'image' / 'webp' / 'src']
	if '/modules/input/w32input/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'input' / 'w32input' / 'src']
	if '/modules/langpack/delangpack/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'langpack' / 'delangpack' / 'src']
	if '/modules/model/demodel/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'model' / 'demodel' / 'src']
	if '/modules/network/basic/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'network' / 'basic' / 'src']
	if '/modules/occlusionmesh/deocclusionmesh/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'occlusionmesh' / 'deocclusionmesh' / 'src']
	if '/modules/physics/bullet/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'physics' / 'bullet' / 'src',
			root_dir / 'src' / 'modules' / 'physics' / 'bullet' / 'Bullet']
	if '/modules/rig/derig/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'rig' / 'derig' / 'src']
	if '/modules/scripting/dragonscript/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'scripting' / 'dragonscript' / 'src']
	if '/modules/service/eossdk/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'service' / 'eossdk' / 'src']
	if '/modules/service/modio/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'service' / 'modio' / 'src']
	if '/modules/service/steamsdk/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'service' / 'steamsdk' / 'src']
	if '/modules/service/msgdk/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'service' / 'msgdk' / 'src']
	if '/modules/skin/deskin/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'skin' / 'deskin' / 'src']
	if '/modules/sound/oggvorbis/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'sound' / 'oggvorbis' / 'src']
	if '/modules/synthesizer/desynthesizer/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'synthesizer' / 'desynthesizer' / 'src']
	if '/modules/video/apng/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'video' / 'apng' / 'src']
	if '/modules/video/theora/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'video' / 'theora' / 'src']
	if '/modules/video/webm/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'video' / 'webm' / 'src']
	if '/modules/vr/null/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'vr' / 'null' / 'src']
	if '/modules/vr/openxr/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'vr' / 'openxr' / 'src']
	
	# Launchers
	if '/launcher/console/' in vcxproj_str:
		return [root_dir / 'src' / 'launcher' / 'console' / 'src']
	if '/launcher/gui_engine/' in vcxproj_str:
		return [root_dir / 'src' / 'launcher' / 'gui' / 'src' / 'engine']
	if '/launcher/gui/' in vcxproj_str and 'gui_engine' not in vcxproj_str:
		return [root_dir / 'src' / 'launcher' / 'gui' / 'src']
	if '/launcher/shared/' in vcxproj_str:
		return [root_dir / 'src' / 'launcher' / 'shared' / 'src']
	if '/launcher/live/' in vcxproj_str:
		return [root_dir / 'src' / 'launcher' / 'live' / 'src']
	
	# IGDE
	if '/igde/igde/igde.vcxproj' in vcxproj_str:
		return [root_dir / 'src' / 'deigde' / 'deigde' / 'src']
	if '/igde/shared/igde_shared.vcxproj' in vcxproj_str:
		return [root_dir / 'src' / 'deigde' / 'deigde' / 'shared' / 'src']
	
	# IGDE Editors
	if '/igde/editors/animator/' in vcxproj_str:
		return [root_dir / 'src' / 'deigde' / 'editors' / 'animator' / 'src']
	if '/igde/editors/conversation/' in vcxproj_str:
		return [root_dir / 'src' / 'deigde' / 'editors' / 'conversation' / 'src']
	if '/igde/editors/font/' in vcxproj_str:
		return [root_dir / 'src' / 'deigde' / 'editors' / 'font' / 'src']
	if '/igde/editors/gamedefinition/' in vcxproj_str:
		return [root_dir / 'src' / 'deigde' / 'editors' / 'gameDefinition' / 'src']
	if '/igde/editors/langpack/' in vcxproj_str:
		return [root_dir / 'src' / 'deigde' / 'editors' / 'langpack' / 'src']
	if '/igde/editors/particleemitter/' in vcxproj_str:
		return [root_dir / 'src' / 'deigde' / 'editors' / 'particleEmitter' / 'src']
	if '/igde/editors/project_testrunner/' in vcxproj_str:
		return [root_dir / 'src' / 'deigde' / 'editors' / 'project' / 'testrun']
	if '/igde/editors/project/' in vcxproj_str:
		return [root_dir / 'src' / 'deigde' / 'editors' / 'project' / 'src',
			root_dir / 'src' / 'deigde' / 'editors' / 'project' / 'minizip' / 'zip.c',
			root_dir / 'src' / 'deigde' / 'editors' / 'project' / 'minizip' / 'unzip.c',
			root_dir / 'src' / 'deigde' / 'editors' / 'project' / 'minizip' / 'ioapi.c']
	if '/igde/editors/rig/' in vcxproj_str:
		return [root_dir / 'src' / 'deigde' / 'editors' / 'rigeditor' / 'src']
	if '/igde/editors/skin/' in vcxproj_str:
		return [root_dir / 'src' / 'deigde' / 'editors' / 'skin' / 'src']
	if '/igde/editors/sky/' in vcxproj_str:
		return [root_dir / 'src' / 'deigde' / 'editors' / 'sky' / 'src']
	if '/igde/editors/speechanimation/' in vcxproj_str:
		return [root_dir / 'src' / 'deigde' / 'editors' / 'speechAnimation' / 'src']
	if '/igde/editors/synthesizer/' in vcxproj_str:
		return [root_dir / 'src' / 'deigde' / 'editors' / 'synthesizer' / 'src']
	if '/igde/editors/world/' in vcxproj_str:
		return [root_dir / 'src' / 'deigde' / 'editors' / 'world' / 'src']
	
	# Shared
	if '/shared/vulkan/' in vcxproj_str:
		return [root_dir / 'src' / 'shared' / 'vulkan' / 'src']
	if '/shared/fbx/' in vcxproj_str:
		return [root_dir / 'src' / 'modules' / 'combined' / 'fbx' / 'src' / 'shared']
	
	return []


def update_vcxproj_file(vcxproj_path: Path, dry_run: bool = False) -> bool:
	"""Update a single .vcxproj file with correct source and header files."""
	print(f"Processing: {vcxproj_path}")
	
	src_dirs = get_source_dir_from_vcxproj(vcxproj_path)
	if not src_dirs:
		print(f"  WARNING: Could not determine source directory for {vcxproj_path}")
		return False
	
	cpp_files = []
	header_files = []
	
	for src_dir in src_dirs:
		if not src_dir.exists():
			print(f"  WARNING: Source path does not exist: {src_dir}")
			return False
		
		if src_dir.is_file():
			if src_dir.suffix in {'.cpp', '.c'}:
				cpp_files.append(str(src_dir))
			elif src_dir.suffix in {'.h', '.hpp'}:
				header_files.append(str(src_dir))
		else:
			print(f"  Source directory: {src_dir}")
			
			# Find all source and header files
			cpp_files.extend(find_source_files(src_dir, {'.cpp', '.c'}))
			header_files.extend(find_source_files(src_dir, {'.h', '.hpp'}))
	
	cpp_files = [f for f in cpp_files if Path(f).name not in exclude_files]
	header_files = [f for f in header_files if Path(f).name not in exclude_files]
	
	print(f"  Found {len(cpp_files)} source files and {len(header_files)} header files")
	
	if len(cpp_files) == 0 and len(header_files) == 0:
		print(f"  WARNING: No source or header files found")
		return False
	
	# Parse XML
	try:
		tree = ET.parse(vcxproj_path)
		root = tree.getroot()
	except Exception as e:
		print(f"  ERROR: Failed to parse XML: {e}")
		return False
	
	project_dir = vcxproj_path.parent
	
	# Remove existing ClCompile and ClInclude ItemGroups
	for item_group in root.findall('msbuild:ItemGroup', NS):
		has_compile = item_group.find('msbuild:ClCompile', NS) is not None
		has_include = item_group.find('msbuild:ClInclude', NS) is not None
		if has_compile or has_include:
			root.remove(item_group)
	
	# Create new ItemGroup for source files if any exist
	if cpp_files:
		compile_group = ET.SubElement(root, 'ItemGroup')
		for cpp_file in cpp_files:
			rel_path = make_relative_path(cpp_file, project_dir)
			compile_elem = ET.SubElement(compile_group, 'ClCompile')
			compile_elem.set('Include', rel_path)
	
	# Create new ItemGroup for header files if any exist
	if header_files:
		include_group = ET.SubElement(root, 'ItemGroup')
		for header_file in header_files:
			rel_path = make_relative_path(header_file, project_dir)
			include_elem = ET.SubElement(include_group, 'ClInclude')
			include_elem.set('Include', rel_path)
	
	if dry_run:
		print(f"  DRY RUN: Would update {vcxproj_path}")
		return True
	
	# Write back to file
	try:
		# Format the XML nicely
		indent_xml(root)
		tree.write(vcxproj_path, encoding='utf-8', xml_declaration=True)
		print(f"  ✓ Updated successfully")
		return True
	except Exception as e:
		print(f"  ERROR: Failed to write XML: {e}")
		return False


def indent_xml(elem, level=0):
	"""Add pretty-printing indentation to XML."""
	i = "\n" + level * "  "
	if len(elem):
		if not elem.text or not elem.text.strip():
			elem.text = i + "  "
		if not elem.tail or not elem.tail.strip():
			elem.tail = i
		for child in elem:
			indent_xml(child, level + 1)
		if not child.tail or not child.tail.strip():
			child.tail = i
	else:
		if level and (not elem.tail or not elem.tail.strip()):
			elem.tail = i


def find_all_vcxproj_files() -> List[Path]:
	"""Find all .vcxproj files that should be updated."""
	vs_dir = Path(__file__).parent.resolve()
	
	vcxproj_files = []
	
	# Dragengine
	vcxproj_files.append(vs_dir / 'dragengine' / 'dragengine.vcxproj')
	
	# All modules
	modules_dir = vs_dir / 'modules'
	if modules_dir.exists():
		vcxproj_files.extend(modules_dir.rglob('*.vcxproj'))
	
	# All launchers
	launcher_dir = vs_dir / 'launcher'
	if launcher_dir.exists():
		for vcxproj in launcher_dir.rglob('*.vcxproj'):
			vcxproj_files.append(vcxproj)
	
	# IGDE
	igde_main = vs_dir / 'igde' / 'igde' / 'igde.vcxproj'
	if igde_main.exists():
		vcxproj_files.append(igde_main)
	
	# IGDE Shared
	igde_shared = vs_dir / 'igde' / 'shared' / 'igde_shared.vcxproj'
	if igde_shared.exists():
		vcxproj_files.append(igde_shared)
	
	# All IGDE editors
	editors_dir = vs_dir / 'igde' / 'editors'
	if editors_dir.exists():
		vcxproj_files.extend(editors_dir.rglob('*.vcxproj'))
	
	# Shared libraries
	shared_dir = vs_dir / 'shared'
	if shared_dir.exists():
		for vcxproj in shared_dir.rglob('*.vcxproj'):
			vcxproj_files.append(vcxproj)
	
	# Filter out external dependencies and non-existent files
	filtered = []
	for vcxproj in vcxproj_files:
		if not vcxproj.exists():
			continue
		# Skip extern projects
		if '/extern/' in str(vcxproj):
			continue
		# Skip enginedeal and demodules (meta projects)
		if vcxproj.name in ['enginedeal.vcxproj', 'demodules.vcxproj']:
			continue
		filtered.append(vcxproj)
	
	return sorted(set(filtered))


def main():
	import argparse
	
	parser = argparse.ArgumentParser(
		description='Update Visual Studio project files with correct source and header files'
	)
	parser.add_argument(
		'--dry-run',
		action='store_true',
		help='Show what would be updated without making changes'
	)
	parser.add_argument(
		'files',
		nargs='*',
		help='Specific .vcxproj files to update (default: all)'
	)
	
	args = parser.parse_args()
	
	if args.files:
		vcxproj_files = [Path(f) for f in args.files]
	else:
		vcxproj_files = find_all_vcxproj_files()
	
	print(f"Found {len(vcxproj_files)} project files to update")
	print()
	
	if args.dry_run:
		print("DRY RUN MODE - No files will be modified")
		print()
	
	success_count = 0
	fail_count = 0
	
	for vcxproj in vcxproj_files:
		try:
			if update_vcxproj_file(vcxproj, dry_run=args.dry_run):
				success_count += 1
			else:
				fail_count += 1
		except Exception as e:
			print(f"  ERROR: Unexpected error: {e}")
			fail_count += 1
		print()
	
	print("=" * 60)
	print(f"Summary: {success_count} succeeded, {fail_count} failed")
	
	return 0 if fail_count == 0 else 1


if __name__ == '__main__':
	sys.exit(main())
