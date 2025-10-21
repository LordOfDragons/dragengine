#!/bin/bash
cd build
find install -name "*.h*" | sort >../header_file_list/modio
