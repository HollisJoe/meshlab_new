include (../../shared.pri)

HEADERS = filter_dirt.h \
          particle.h \
          dirt_utils.h \
          $$VCGDIR/vcg/complex/trimesh/point_sampling.h

SOURCES = filter_dirt.cpp
TARGET = filter_dirt
