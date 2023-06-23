#if !defined(WAV_CPP)


#include "common.h"


#pragma pack(push, 1)
struct wav_riff {
	u32 id;
	u32 size;
	u32 format;
};
struct wav_format {
	u32 id;
	u32 size;
	u16 format;
	u16 channelCount;
	u32 sampleRate;
	u32 byteRate;
	u16 blockAlign;
	u16 bitsPerSample;
};
struct wav_data_info {
	u32 id;
	u32 size;
};
#pragma pack(pop)




struct Sound {
	u32 size;
	u16 bytesPerSample;
	u16 samplesPerSecond;
	u16 channels;
	s16* data;
};







Sound LoadWAV(File_Result file_result) {
	Sound snd = {};
	u8* contents = (u8*)file_result.contents;
	wav_riff* riffChunk = ConsumeData(&contents, wav_riff);
	if (FOURCC("WAVE") == riffChunk->format) {
		wav_format* formatChunk = ConsumeData(&contents, wav_format);
		wav_data_info* infoChunk = ConsumeData(&contents, wav_data_info);
        
		s16* data = (s16*)ConsumeDataSize(&contents, infoChunk->size);
        
		snd.bytesPerSample = formatChunk->bitsPerSample / 8;
		Assert(snd.bytesPerSample == 2);
		snd.channels = formatChunk->channelCount;
		snd.samplesPerSecond = (u16)formatChunk->sampleRate;
		snd.size = infoChunk->size;
		snd.data = (s16*)malloc(snd.size);
		u32 sampleSize = snd.bytesPerSample * snd.channels;
		u32 samples = snd.size / sampleSize;
		for (u32 sampleIndex = 0; sampleIndex < samples; sampleIndex++) {
			for (u32 channelIndex = 0; channelIndex < snd.channels; channelIndex++) {
				u32 index = (sampleIndex * snd.channels) + channelIndex;
				*(snd.data + index) = *(data + index);
			}
		}
		return snd;
	}
	else {
		//TODO: invalid format.
		CRASH;
		return snd;
	}
}




int main(int argc, char **args) {
    File_Result file_result = read_file("test.wav");
    if (file_result.content_size) {
        Sound sound = LoadWAV(file_result);
    }
}



#define WAV_CPP
#endif