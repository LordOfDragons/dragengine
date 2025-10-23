#!/bin/bash
cd build
find install \
  -name "*.h" \
  -or -name "*.hpp" \
  -or -name "*.ipp" \
  | sort >../header_file_list/modio
