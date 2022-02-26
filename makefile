# # # # # # # # # # # # # # # # # # # #
# makefile targets unix-like systems. #
# # # # # # # # # # # # # # # # # # # #

  #############################
  #       export target       #
  # (make globally available) #
  #############################

export:
	if [ -d "/usr/local/include/sld/" ]; then rm -rf /usr/local/include/sld/; fi;
	mkdir -p /usr/local/include/sld/
	cp -r ./sld/* /usr/local/include/sld/
