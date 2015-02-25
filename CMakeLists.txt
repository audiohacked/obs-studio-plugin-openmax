project(linux-openmax)

find_package(PkgConfig REQUIRED)
pkg_check_modules(OMX REQUIRED libomxil-bellagio)
include_directories(${OMX_INCLUDE_DIRS})
link_directories(${OMX_LIBRARY_DIRS})

set(linux-openmax_HEADERS
	openmax.h)
	
set(linux-openmax_SOURCES
	plugin-main.c
	openmax-encoder.c
	properties.c
	optional.c
	helpers.c
)

add_library(linux-openmax MODULE
	${linux-openmax_SOURCES}
	${linux-openmax_HEADERS})

target_link_libraries(linux-openmax
	libobs
	${OMX_LIBRARIES})

install_obs_plugin_with_data(linux-openmax data)
