#ifndef SCOREBOARD_H_
#define SCOREBOARD_H_

#include <SFML/Graphics.hpp>
#include "util.h"
#include <string.h>
#include "SceneElement.h"

using namespace std;
using namespace sf;

class Scoreboard {
    public:
        Scoreboard(SceneElement* root);
        void update(int currentBall, int maxBalls, uint64_t score, bool paused, string announce, string announceFlash);
        ~Scoreboard();
    private:
        SceneElement* m_ballsLeftText;
        SceneElement* m_scoreText;
        SceneElement* m_announce;
        SceneElement* m_background;
        string m_announceStr;
        int m_flashDelay = 6 * g_displayFrameRate / 60;
        int m_flashTimer;
        Font* m_font;
};
#endif