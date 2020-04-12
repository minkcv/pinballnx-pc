#ifndef PLUNGER_H_
#define PLUNGER_H_

#include <SFML/Graphics.hpp>
#include "Box2D/Box2D.h"
#include "util.h"
#include "SceneElement.h"

class Plunger {
    public:
        Plunger(SceneElement* root, b2World& world);
        void update();
    private:
        float m_halfWidth = 0.5f;
        float m_halfHeight = 0.1f;
        float m_startX = 0.6f;
        float m_startY = 6.9;
        b2Body* m_body;
        b2Body* m_constraint;
        b2PrismaticJoint* m_joint;
        RectangleShape* m_shape;
		SceneElement* m_elt;
};

#endif