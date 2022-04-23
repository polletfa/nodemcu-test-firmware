#
# NodeMCU Test Firmware
#
# (c) 2022 Fabien Pollet <polletfa@posteo.de>
# MIT License (see LICENSE.md file)
#

# Set the board in the IDE or add the correct --board option here.
upload: build-frontend
	arduino --upload --verbose firmware/firmware.ino

build-frontend:
	npm run build --prefix frontend
	npm run deploy --prefix frontend

clean:
	rm -fr firmware/_gen
	npm run clean --prefix frontend

distclean: clean
	npm run distclean --prefix frontend
