// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      sound.c
//      Purpose :   Sound system for runtime.
//      Date :      7th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include <runtime.h>

static SDL_AudioDeviceID audioDevice;
static SDL_AudioSpec audioSpec;

void (*m_writeData)(uint8_t* ptr, double data);
int (*m_calculateOffset)(int sample, int channel);

/**
 * @brief      Calculates the offset for s16 format
 *
 * @param[in]  sample   The sample count
 * @param[in]  channel  The channel number
 *
 * @return     The offset s 16.
 */
static int calculateOffset_s16(int sample, int channel) {
	return
		(sample * sizeof(int16_t) * audioSpec.channels) +
		(channel * sizeof(int16_t));
}

/**
 * @brief      Calculates the offset for f32 format
 *
 * @param[in]  sample   The sample count
 * @param[in]  channel  The channel offset
 *
 * @return     The offset f 32.
 */
static int calculateOffset_f32(int sample, int channel) {
	return
		(sample * sizeof(float) * audioSpec.channels) +
		(channel * sizeof(float));
}

/**
 * @brief      Writes the normalised data value (0-1) to the output stream
 *             matching the audio format
 *
 * @param      ptr   pointer into queue
 * @param[in]  data  Data
 */
static void writeData_s16(uint8_t* ptr, double data) {
	int16_t* ptrTyped = (int16_t*)ptr;
	double range = (double)INT16_MAX - (double)INT16_MIN;
	double dataScaled = data * range / 2.0;
	*ptrTyped = dataScaled;
}

/**
 * @brief      Writes the normalised data value (0-1) to the output stream
 *             matching the audio format
 *
 * @param      ptr   pointer into queue
 * @param[in]  data  Data
 */
static void writeData_f32(uint8_t* ptr, double data) {
	float* ptrTyped = (float*)ptr;
	*ptrTyped = data;
}

/**
 * @brief      Callback to repopulatoe sound buffer
 *
 * @param      userdata   User data passed
 * @param      stream    Stream address
 * @param[in]  len       The length size in bytes
 */
static void audioCallback(void* userdata,uint8_t* stream,int len) {
	// Unused parameters
	(void)userdata;
	(void)len;
	// Write data to the entire buffer by iterating through all samples and
	// channels.
	for (int sample = 0; sample < audioSpec.samples; ++sample) {
		#if ARTURO_PROCESS_SOUND==1
		double data = ((double)SNDGetChannelSample(0))/128.0;
		#else		
		double data = ((double)ApplicationGetChannelSample(0))/128.0;
		#endif
		// Write the same data to all channels
		for (int channel = 0; channel < audioSpec.channels; ++channel) {
			int offset = m_calculateOffset(sample, channel);
			uint8_t* ptrData = stream + offset;
			m_writeData(ptrData, data);
		}
	}
}

/**
 * @brief      Open the sound device
 */
void SOUNDOpen(void) {
	char *formatName = "<unknown>";

	// First define the specifications we want for the audio device
	SDL_AudioSpec desiredSpec;
	SDL_zero(desiredSpec);

	// Commonly used sampling frequency
	desiredSpec.freq = 44100;

	// Currently this program supports two audio formats:
	// - AUDIO_S16: 16 bits per sample
	// - AUDIO_F32: 32 bits per sample
	//
	// We need this flexibility because some sound cards do not support some
	// formats.

	// Higher bit depth means higher resolution the sound, lower bit depth
	// means lower resolution for the sound. Since we are just playing a simple
	// sine wave, 16 bits is fine.
	desiredSpec.format = AUDIO_S16;

	// Smaller buffer means less latency with the sound card, but higher CPU
	// usage. Bigger buffers means more latency with the sound card, but lower
	// CPU usage. 512 is fairly small, since I don't want a delay before a beep
	// is played.
	desiredSpec.samples = 512;

	// Since we are just playing a simple sine wave, we only need one channel.
	desiredSpec.channels = 1;

	// Set the callback (pointer to a function) to our callback. This function
	// will be called by SDL2 in a separate thread when it needs to write data
	// to the audio buffer. In other words, we don't control when this function
	// is called; SDL2 manages it.
	desiredSpec.callback = audioCallback;

	// When we open the audio device, we tell SDL2 what audio specifications we
	// desire. SDL2 will try to get these specifications when opening the audio
	// device, but sometimes the audio device does not support some of our
	// desired specifications. In that case, we have to be flexible and adapt
	// to what the audio device supports. The obtained specifications that the
	// audio device supports will be stored in `audioSpec`

	audioDevice = SDL_OpenAudioDevice(
		NULL, // device (name of the device, which we don't care about)
		0, // iscapture (we are not recording sound)
		&desiredSpec, // desired
		&audioSpec, // obtained
		0 // allowed_changes (allow any changes between desired and obtained)
	);

	if (audioDevice == 0) {
		SDL_Log("Failed to open audio: %s", SDL_GetError());
		// TODO: throw exception
	} else {
		switch (audioSpec.format) {
			case AUDIO_S16:
				m_writeData = writeData_s16;
				m_calculateOffset = calculateOffset_s16;
				formatName = "AUDIO_S16";
				break;
			case AUDIO_F32:
				m_writeData = writeData_f32;
				m_calculateOffset = calculateOffset_f32;
				formatName = "AUDIO_F32";
				break;
			default:
				SDL_Log("Unsupported audio format: %i", audioSpec.format);
				// TODO: throw exception
		}
		printf("Format %s %dHz\n",formatName,audioSpec.freq);
	}
}

/**
 * @brief      Close the sound device
 */
void SOUNDClose(void) {
	SDL_CloseAudioDevice(audioDevice);
}


/**
 * @brief      Start playing audio
 */
void SOUNDPlay(void) {
	SDL_PauseAudioDevice(audioDevice, 0);
}


/**
 * @brief      Stop playing audio
 */
void SOUNDStop(void) {
	SDL_PauseAudioDevice(audioDevice, 1);
}


//
//      Function that returns the sample rate in Hz of the implementeing hardware
//


/**
 * @brief      Get the sample rate
 *
 *             This returns the sample rate required by the sound system and is
 *             read by audio systems to know the signal to send
 *
 * @return     Sample rate in Hz
 */
int SNDGetSampleFrequency(void) {
    return audioSpec.freq;
}
