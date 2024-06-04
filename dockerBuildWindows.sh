#!/bin/bash
USER_UID=$(id -u)
docker run -it --rm \
  --volume=`realpath .`:/sources \
  compile-windows-crosscompile \
  /bin/bash
