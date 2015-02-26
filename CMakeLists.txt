project(obs-openmax)

find_package(PkgConfig REQUIRED)
pkg_check_modules(OMX REQUIRED libomxil-bellagio)
include_directories(${OMX_INCLUDE_DIRS})
link_directories(${OMX_LIBRARY_DIRS})

set(obs-openmax_HEADERS
	obs-openmax.h)
	
set(obs-openmax_SOURCES
	obs-openmax-create.c
	obs-openmax-destroy.c
	obs-openmax-encode.c
	# obs-openmax-callback.c
	# obs-openmax-helpers.c
	obs-openmax.c
	obs-openmax-plugin-main.c
)

add_library(obs-openmax MODULE
	${obs-openmax_SOURCES}
	${obs-openmax_HEADERS})

target_link_libraries(obs-openmax
	libobs
	${OMX_LIBRARIES})

install_obs_plugin_with_data(obs-openmax data)
