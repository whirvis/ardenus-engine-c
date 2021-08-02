#ifndef AE_VORBIS_FILE_H_
#define AE_VORBIS_FILE_H_

#ifndef STB_VORBIS_HEADER_ONLY
	#define STB_VORBIS_HEADER_ONLY
	#include "stb_vorbis.c"
	#undef STB_VORBIS_HEADER_ONLY
#endif

typedef struct VorbisFile VorbisFile;

VorbisFile * VorbisFile_open(const char *path);

int VorbisFile_getALFormat(const VorbisFile *info);
int VorbisFile_getFrequencyHz(const VorbisFile *info);
int VorbisFile_getChannelCount(const VorbisFile *info);
int VorbisFile_getBitsPerSample(const VorbisFile *info);
int VorbisFile_readPCM(VorbisFile *info, int offset, char *buf, int len);

#endif
