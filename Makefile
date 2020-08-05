.PHONY: clean

CXXFLAGS += -Wall

minimon: minimon.cpp

clean:
	rm -f minimon
