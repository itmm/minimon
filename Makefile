.PHONY: full clean

HX_SRCs = $(shell hx-srcs.sh)
SRCs = $(shell hx-files.sh $(HX_SRCs))

CXXFLAGS += -Wall -std=c++17 -Os

full: hx_run minimon

hx_run: $(HX_SRCs)
	@echo HX
	@hx
	@date >$@
	@make --no-print-directory minimon

$(SRCs): hx_run

clean:
	@echo RM
	@rm -f $(SRCs) minimon


minimon: minimon.cpp
