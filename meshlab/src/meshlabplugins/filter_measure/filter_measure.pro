include (../../shared.pri)

HEADERS       += filter_measure.h
SOURCES       += filter_measure.cpp 
TARGET         = filter_measure
SHORTTARGET = $$TARGET
PRE_TARGETDEPS += filter_measure.xml

macx:QMAKE_POST_LINK = "cp "$$SHORTTARGET".xml ../../distrib/plugins/lib"$$TARGET".xml"
#INSTALLS += sample_xmlfilter.xml



