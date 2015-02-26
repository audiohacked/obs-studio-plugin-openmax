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

#include "obs-openmax.h"

static bool obs_openmax_encode(void *data, struct encoder_frame *frame,
		struct encoder_packet *packet, bool *received_packet)
{
	struct obs_openmax *obs_omxil = data;
	
	//fill input buffer with frame
	//tell the encoder to do its work by stating input buffer is done
	//wait we have a full output buffer

	return true;
}
