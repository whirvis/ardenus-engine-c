#include "vorbis/VorbisFile.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <AL/al.h>

static VorbisFile_err
alFormat(const stb_vorbis_info *info, int *alFormat) {
	switch (info->channels) {
	case 1:
		*alFormat = AL_FORMAT_MONO16;
		return VORBIS_FILE_NO_ERROR;
	case 2:
		*alFormat = AL_FORMAT_STEREO16;
		return VORBIS_FILE_NO_ERROR;
	}
	return VORBIS_FILE_UNSUPPORTED_CHANNEL_COUNT;
}
