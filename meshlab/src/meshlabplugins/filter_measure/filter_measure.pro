include (../../shared.pri)

HEADERS       = filter_measure.h

SOURCES       = filter_measure.cpp \ 
		../../meshlab/filterparameter.cpp \
		$$GLEWCODE

TARGET        = filter_measure

CONFIG       += opengl



