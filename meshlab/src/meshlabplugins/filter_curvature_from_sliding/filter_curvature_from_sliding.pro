include (../../shared.pri)

HEADERS       = filter_curvature_from_sliding.h

SOURCES       = filter_curvature_from_sliding.cpp \ 
		../../meshlab/filterparameter.cpp
TARGET        = filter_curvature_from_sliding

INCLUDEPATH += ../../../../code
win32-msvc2005:LIBS	+= ../../../../code/lib/levmar/lib/levmar.lib