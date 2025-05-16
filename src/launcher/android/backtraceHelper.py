#!/usr/bin/python3

import os
import re
import sys
import subprocess

#config = {'target': 'armv8Debug', 'task': 'mergeArmv8DebugNativeLibs'}
config = {'target': 'questDebug', 'task': 'mergeQuestDebugNativeLibs'}

binbase = os.path.expanduser(
    '~/Android/Sdk/ndk/27.0.12077973/toolchains/llvm/prebuilt/linux-x86_64/bin')
binnm = os.path.join(binbase, 'llvm-nm')
binaddr2line = os.path.join(binbase, 'llvm-symbolizer')

with open(sys.argv[1], 'r', encoding='utf-8') as f:
    content = f.readlines()

regexLine = re.compile(
    r'^.*?(\S+) ?\((\w*)(\+0x[0-9A-Fa-f]+)?\) ?\[(0x[0-9A-Fa-f]+)\].*\n$')
regexLine2 = re.compile(r'^([/A-Za-z0-9-_.]+) ?\[(0x[0-9A-Fa-f]+)\].*\n$')
regexLine3 = re.compile(r'^.*?([^\s(]+)\s?\(([^+]*)(\+(0x)?[0-9A-Fa-f]+)?\)\s?\n$')
regexLine3b = re.compile(r'^.*?([^\s(]+)\s?\(offset [^)]+\)\s?\(([^+]*)(\+(0x)?[0-9A-Fa-f]+)?\)\s?\n$')
regexLine4 = re.compile(r'^.*?pc\s+([0-9A-Fa-f]+)\s+([^\s]+)\n$')
regexLinePrefix = re.compile(r'^\[[A-Za-z0-9-_.: ]+\] ')
regexAppPath = re.compile(r'^/data/app/.*/ch\.dragondreams\.delauncher.*/lib/arm64-v8a/')
regexAppPath2 = re.compile(r'^/data/app/.*/ch\.dragondreams\.delauncher.*!')

def pathrepl(p):
    p = p.replace(
        '/data/data/ch.dragondreams.delauncher/files/dragengine',
        'build/release')
    p = regexAppPath.sub(
        'src/launcher/android/app/build/intermediates/merged_native_libs/{target}/{task}/out/lib/arm64-v8a/'.format(**config), p)
    p = regexAppPath2.sub(
        'src/launcher/android/app/build/intermediates/merged_native_libs/{target}/{task}/out/lib/arm64-v8a/'.format(**config), p)
    if p.endswith('.so.1'):
        p = p[:-2]
    return p

def run_console(*command):
    o = subprocess.check_output(command).decode('utf-8')
    while o and o[-1] == '\n':
        o = o[:-1]
    return o

enable_debug_print = False

def debug_print(*items):
    if enable_debug_print:
        print(items)

for line in content:
    if line[:3] == 'EE ' or line[:3] == 'WW ' or line[:3] == 'II ':
        line = line[3:]
    while True:
        replaced = regexLinePrefix.sub('', line)
        if replaced == line:
            break
        line = replaced
    if line[:11] == 'Backtrace: ':
        line = line[11:]

    debug_print(line)
    match = regexLine.match(line)
    nmdemangle = False
    if match:
        (binary, function, offset, address) = match.groups()
        debug_print('regexLine', binary, function, offset, address)
    else:
        match = regexLine2.match(line)
        if match:
            (binary, address) = match.groups()
            function = ''
            offset = 0
            debug_print('regexLine2', binary, address)
        else:
            match = regexLine3b.match(line)
            if match:
                (binary, function, offset) = match.groups()[0:3]
                offset = f'+0x{int(offset[1:]):x}'
                address = 0
                nmdemangle = True
                debug_print('regexLine3b', binary, function, offset)
            else:
                match = regexLine3.match(line)
                if match:
                    (binary, function, offset) = match.groups()[0:3]
                    offset = f'+0x{int(offset[1:]):x}'
                    address = 0
                    nmdemangle = True
                    debug_print('regexLine3', binary, function, offset)
                else:
                    match = regexLine4.match(line)
                    if match:
                        (address, binary) = match.groups()
                        address = f'0x{address}'
                        function = ''
                        offset = 0
                        debug_print('regexLine4', binary, address)
                    else:
                        continue

    binary = pathrepl(binary)
    #print([binary, function, offset, address])

    if offset:
        offset = int(offset[3:], 16)
        if binary[0] != '/':
            binary = run_console('realpath', binary)
        if function:
            output = run_console(binnm, '-C' if nmdemangle else '', binary).split('\n')
            for t in output:
                if function in t:
                    if not t or t[0] == ' ':
                        print(f'{binary}:??')
                    else:
                        base = int(t[:t.index(' ')], 16)
                        address = f'0x{base + offset:x}'
                        output = run_console(
                            binaddr2line, '-e', binary,
                            '-C', '-f', '-i', '-p', '-s', '-a',
                            address)
                        print(output)
                    break
        else:
            address = f'0x{offset:x}'
            output = run_console(
                binaddr2line, '-e', binary,
                '-C', '-f', '-i', '-p', '-s', '-a',
                address)
            print(output)
    else:
        if binary[0] != '/':
            binary = run_console('realpath', binary)
        output = run_console(
            binaddr2line, '-e', binary,
            '-C', '-f', '-i', '-p', '-s', '-a',
            address)
        print(output)
