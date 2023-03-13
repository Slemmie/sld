# make contents globally aviailable
export:
	rm -rf /usr/local/include/sld/
	mkdir -p /usr/local/include/sld/
	cp -r ./sld/* /usr/local/include/sld/
