SPI overlay to use GPIO pins to select eight chips
==================================================

dtc -@ -I dts -O dtb -o spi-cs-extend.dtbo spi-cs-extend.dts

Copy 'spi-cs-extend.dtbo' to /boot/overlays

Add 'dtoverlay=spi-cs-extend' to /boot/config.txt

Reference to 

mcbridejc/Adding SPI chip selects on Raspberry Pi.md

https://gist.github.com/mcbridejc/d060602e892f6879e7bc8b93aa3f85be

