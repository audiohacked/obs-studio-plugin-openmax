/******************************************************************************
    Copyright (C) 2015 by Sean Nelson <audiohacked@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
#include "openmax.h"

#define TEXT_COMPONENT_NAME obs_module_text("OMXComponent_name")
#define TEXT_COMPONENT_DEFAULT "OMX.mesa.video_encode.avc"

void openmax_defaults(obs_data_t *settings)
{
	obs_data_set_default_string(settings, "component_name", "OMX.mesa.video_encode.avc");
}
	
obs_properties_t *openmax_properties(void *unused)
{
	UNUSED_PARAMETER(unused);

	obs_properties_t *props = obs_properties_create();

	obs_properies_add_text(props, "component_name", TEXT_COMPONENT_NAME, TEXT_COMPONENT_DEFAULT);
	return props;
}
