
import sys
import xml.etree.ElementTree as ET

# Verify that the metainfo file at filepath has a first changelog entry matching version
def verify_metainfo_version(filepath: str, version: str) -> None:
	with open(filepath, 'r', encoding='utf-8') as f:
		def _local_name(tag: str) -> str:
			return tag.split('}', 1)[-1] if '}' in tag else tag
		
		tag = ET.parse(f).getroot().find('releases[1]/release[1]')
		if tag is None:
			raise ValueError(f'No <release> entries in {filepath}')
		
		version_is = tag.get('version')
		if version_is != version:
			raise ValueError(f'Version mismatch in {filepath}: expected {version!r}, found {version_is!r}')

# Verify all metainfo files have first changelog entry with matching version
def verify_metainfo_versions(version: str) -> None:
	verify_metainfo_version('src/launcher/gui/data/delauncher-gui.metainfo.xml', version)
	verify_metainfo_version('src/deigde/deigde/data/deigde.metainfo.xml', version)

cmd = sys.argv[1]
if cmd == 'verify_metainfo_versions':
	version = sys.argv[2]
	try:
		verify_metainfo_versions(version)
	except Exception as e:
		print(f"Verify Metainfo Versions: {e}")
		sys.exit(1)
	print("Verify Metainfo Versions: OK")
else:
	print(f"Unknown command: {cmd}")
	sys.exit(2)
