#include "Audio.h"

Audio::Audio(string filename, bool loop) {
	this->loop = loop;

	// TODO: audio not working on emscripten
#ifndef __EMSCRIPTEN__
	if (loop) {
		// Uso la Librería Mixer - mp3
		Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096); // 2 canales
		mix = Mix_LoadMUS(filename.c_str());
	}
	else {
		// Uso SDL audio standard
		SDL_LoadWAV(filename.c_str(), &wavSpec, &wavBuffer, &wavLength);
		deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
	}
#endif
}

Audio::~Audio() {
#ifndef __EMSCRIPTEN__
	if (loop) {
		Mix_FreeMusic(mix);
		Mix_CloseAudio();
	}
	else {
		SDL_CloseAudioDevice(deviceId);
		SDL_FreeWAV(wavBuffer);
	}
#endif
}

void Audio::stopAndPlay() {
#ifndef __EMSCRIPTEN__
	if (loop) {
		Mix_PlayMusic(mix, -1);
		// -1 se repite sin parar
	}
	else {
		SDL_ClearQueuedAudio(deviceId); // siempre limpiar la cola
		SDL_QueueAudio(deviceId, wavBuffer, wavLength);
		SDL_PauseAudioDevice(deviceId, 0);
	}
#endif
}

void Audio::play() {
#ifndef __EMSCRIPTEN__
	if (loop) {
		Mix_PlayMusic(mix, -1);
		// -1 se repite sin parar
	}
	else {
		// hay más de 4 en cola
		if (SDL_GetQueuedAudioSize(deviceId) > wavLength * 4) {
			SDL_ClearQueuedAudio(deviceId); // limpiar
		}
		SDL_QueueAudio(deviceId, wavBuffer, wavLength);
		SDL_PauseAudioDevice(deviceId, 0);
	}
#endif
}
