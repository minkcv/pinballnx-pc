#include "SoundM.h"

SoundM::SoundM() : m_soundBuffers(), m_playing() {
	m_muted = false;
	for (size_t i = 0; i < m_soundNames.size(); i++) {
		SoundBuffer* soundBuffer = new SoundBuffer();
		soundBuffer->loadFromFile("data/" + m_soundNames.at(i) + ".wav");
		m_soundBuffers.push_back(soundBuffer);
	}
}

void SoundM::update() {
	for (size_t i = 0; i < m_playing.size(); i++) {
		if (m_playing.at(i)->getStatus() == SoundSource::Status::Stopped) {
			Sound* sound = m_playing.at(i);
			m_playing.erase(m_playing.begin() + i);
			delete sound;
		}
	}
}

Sound* SoundM::playSound(int id) {
	if (m_muted)
		return nullptr;
	Sound* playing = new Sound();
	playing->setBuffer(*m_soundBuffers.at(id));
	playing->play();
	m_playing.push_back(playing);
	return playing;
}

int SoundM::getId(string name) {
	return distance(m_soundNames.begin(), find(m_soundNames.begin(), m_soundNames.end(), name));
}

void SoundM::toggleMute() {
	m_muted = !m_muted;
}