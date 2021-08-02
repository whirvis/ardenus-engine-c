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

typedef struct VorbisFile {
	stb_vorbis *vorbis;
	stb_vorbis_info info;
	int m_alFormat;
	int expectedOffset;
} VorbisFile;

VorbisFile*
VorbisFile_open(const char *path) {
	int err = 0;
	
	VorbisFile *file = malloc(sizeof(*file));
	memset(file, 0, sizeof(*file));
	
	file->vorbis =
		stb_vorbis_open_filename(path, &err, NULL);
	if(err != VORBIS__no_error) {
		printf("VORBIS ISSUE: %d", err);
		exit(0);
	}
	
	file->info = stb_vorbis_get_info(file->vorbis);
	file->m_alFormat = alFormat(file->info);
	file->expectedOffset = 0;
}

int VorbisFile_getALFormat(const VorbisFile *file) {
	return file->m_alFormat;
}

int VorbisFile_getFrequencyHz(const VorbisFile *file) {
	return file->info.sample_rate;
}

int VorbisFile_getChannelCount(const VorbisFile *file) {
	return file->info.channels;
}

int VorbisFile_getBitsPerSample(const VorbisFile *file) {
	return 16;
}

int VorbisFile_readPCM(VorbisFile *file, int offset, char *buf, int len) {
	int channels = VorbisFile_getChannelCount(file);
	int bytesPerSample = VorbisFile_getBitsPerSample(file) / 8;
	
	/* only seek when necessary */
	if (file->expectedOffset != offset) {
		int sampleOffset = offset / (bytesPerSample * channels);
		stb_vorbis_seek(file->vorbis, sampleOffset);
		file->expectedOffset = offset;
	}
	
	int mallocAmt = len / 2;
	short *pcmBuf = calloc(mallocAmt, sizeof(short));
	int limit = stb_vorbis_get_samples_short_interleaved(
			file->vorbis, channels, pcmBuf, mallocAmt) * channels;
	if (limit == 0) {
		return -1;
	}
	
	int yuck = 0;
	int index = 0;
	int size = limit * 2;
	while (index < size) {
		short sample = pcmBuf[yuck++];
		buf[index++] = (ALbyte) (sample & 0xFF);
		buf[index++] = (ALbyte) ((sample >> 8) & 0xFF);
	}
	
	file->expectedOffset += size;
	return size;
}