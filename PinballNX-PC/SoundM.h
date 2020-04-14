#ifndef SOUNDM_H_
#define SOUNDM_H_

#include <SFML/Audio.hpp>
#include <vector>

using namespace std;
using namespace sf;

class SoundM {
public:
	SoundM();
	void update();
	Sound* playSound(int id);
	int getId(string name);
	void toggleMute();
private:
	bool m_muted;
	vector<SoundBuffer*> m_soundBuffers;
	vector<Sound*> m_playing;
	vector<Sound*> m_looping;
	vector<string> m_soundNames = {
		"bumper1",
		"bumper2",
		"bumper3",
		"spinner",
		"extra",
		"flip",
		"flip2",
		"gtarget1",
		"gtarget2",
		"jackpot",
		"multi",
		"lock",
		"lockup",
		"out",
		"ptarget1",
		"ptarget2",
		"trigger",
		"wheel"
	};
};

#endif
