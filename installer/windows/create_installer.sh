#!/bin/bash

version="$1"
if [ "${version}" == "" ]; then
	version="9999"
fi

# clear build directory
rm -rf build unpacked || exit 1

# unpack. both can beunpacked into the same directory
unzip -d unpacked ../../archive/build/dragengine-windows64*.zip
unzip -d unpacked ../../archive/build/dragengine-dev-windows64*.zip
unzip -d unpacked ../../archive/build/deigde-windows64*.zip
unzip -d unpacked ../../archive/build/deigde-dev-windows64*.zip

# dragengine installer
docker run --rm -i -v "$PWD:/work" --user $(id -u):$(id -g) \
 compile-windows-installer /bin/bash -c \
  " \
     echo \"xclient:x:`id -g`:\" >>/etc/group \
  && echo \"xclient:x:`id -u`:`id -g`:xclient:/home/xclient:/bin/bash\" >>/etc/passwd \
  && echo \"xclient::14082:0:99999:7:::\" >>/etc/shadow \
  && sudo chown -R `id -u` /home/xclient \
  && cd /work \
  && sed -e 's/%VERSION%/${version}/g' dragengine64.iss.in >dragengine64.iss \
  && wine-x11-run iscc dragengine64.iss \
  " || exit 1

# dragengine development installer
docker run --rm -i -v "$PWD:/work" --user $(id -u):$(id -g) \
 compile-windows-installer /bin/bash -c \
  " \
     echo \"xclient:x:`id -g`:\" >>/etc/group \
  && echo \"xclient:x:`id -u`:`id -g`:xclient:/home/xclient:/bin/bash\" >>/etc/passwd \
  && echo \"xclient::14082:0:99999:7:::\" >>/etc/shadow \
  && sudo chown -R `id -u` /home/xclient \
  && cd /work \
  && sed -e 's/%VERSION%/${version}/g' dragengine64-dev.iss.in >dragengine64-dev.iss \
  && wine-x11-run iscc dragengine64-dev.iss \
  " || exit 1

# deigde installer
docker run --rm -i -v "$PWD:/work" --user $(id -u):$(id -g) \
 compile-windows-installer /bin/bash -c \
  " \
     echo \"xclient:x:`id -g`:\" >>/etc/group \
  && echo \"xclient:x:`id -u`:`id -g`:xclient:/home/xclient:/bin/bash\" >>/etc/passwd \
  && echo \"xclient::14082:0:99999:7:::\" >>/etc/shadow \
  && sudo chown -R `id -u` /home/xclient \
  && cd /work \
  && sed -e 's/%VERSION%/${version}/g' deigde64.iss.in >deigde64.iss \
  && wine-x11-run iscc deigde64.iss \
  " || exit 1

# deigde development installer
docker run --rm -i -v "$PWD:/work" --user $(id -u):$(id -g) \
 compile-windows-installer /bin/bash -c \
  " \
     echo \"xclient:x:`id -g`:\" >>/etc/group \
  && echo \"xclient:x:`id -u`:`id -g`:xclient:/home/xclient:/bin/bash\" >>/etc/passwd \
  && echo \"xclient::14082:0:99999:7:::\" >>/etc/shadow \
  && sudo chown -R `id -u` /home/xclient \
  && cd /work \
  && sed -e 's/%VERSION%/${version}/g' deigde64-dev.iss.in >deigde64-dev.iss \
  && wine-x11-run iscc deigde64-dev.iss \
  " || exit 1

# clean up
rm -rf unpacked || exit 1
