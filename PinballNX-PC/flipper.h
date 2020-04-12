#ifndef FLIPPER_H_
#define FLIPPER_H_

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Box2D/Box2D.h"
#include "util.h"
#include "SceneElement.h"


using namespace std;
using namespace sf;

class Flipper {
    public:
        Flipper(SceneElement* root, b2World& world, int flipperID);
        void update();
    private:
        float m_rotateDirection;
        Keyboard::Key m_key;
		bool m_keyDown;
        float m_leftX = 0.68f;
        float m_leftY = 2.19f;
        float m_rightX = 0.68f;
        float m_rightY = 4.51f;
        float m_lowerLimit = -M_PI / 8;
        float m_upperLimit = M_PI / 8;
        float m_x;
        float m_y;
		SoundBuffer* m_flipSound;
		Sound* m_sound;
		SoundBuffer* m_flipSound2;
		Sound* m_sound2;
        vector<float> m_leftPoints = {-16, 0, -10, 93, -6, 99, -2.5, 100, 2.5, 100, 6, 99, 10, 93, 16, 0};
        vector<float> m_rightPoints = {-16, 0, -10, -93, -6, -99, -2.5, -100, 2.5, -100, 6, -99, 10, -93, 16, 0};
        vector<float> m_positions = {
            0.68f, 2.19f,
            0.68f, 4.51f,
            5.87f, 0.80f,
            3.48f, 3.92f
        };
        b2RevoluteJoint* m_joint;
        ConvexShape* m_cshape;
        SceneElement* m_elt;
        b2Body* m_body;
        b2Body* m_pivot;
        CircleShape* m_pivotShape;
};

#endif