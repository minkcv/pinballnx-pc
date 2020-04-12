#include "balllock.h"

BallLock::BallLock(SceneElement* root, b2World& world, int layerID, int shapeID, int releaseLocation, bool multi) {
    m_layerID = layerID;
    m_releaseLocation = releaseLocation;
    m_multi = multi;
    m_move = 0;
    m_locked = 0;
    if (m_multi) {
        for (int i = 0; i < m_capacity; i++) {
			Texture* texture = new Texture();
			texture->loadFromFile("data/pinball.png");
            Sprite* sprite = new Sprite();
			sprite->setTexture(*texture);
			SceneElement* elt = new SceneElement(sprite);
			sprite->setPosition(740 + i * m_xOffset, 490 + i * m_yOffset);
			elt->setLayer(-99);
            root->add(elt);
            m_elts.push_back(elt);
        }
    }

    vector<float> points = m_points.at(shapeID);
    b2Vec2* vs = getVertexArray(points);
    b2ChainShape chain;
    chain.CreateChain(vs, points.size() / 2);
    free(vs);
    vs = NULL;

    b2BodyDef bodyDef;
    m_body = world.CreateBody(&bodyDef);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &chain;
    fixtureDef.density = 0.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.isSensor = true;
    fixtureDef.filter.maskBits = 1 << m_layerID;
    fixtureDef.filter.categoryBits = 1 << m_layerID;
    m_fixture = m_body->CreateFixture(&fixtureDef);

#if DEBUG
    // Debug graphics
    m_cshape = new ConvexShape();
    addPointsToShape(m_cshape, points);
    m_cshape->getVertexArray()->setPrimitiveType(PrimitiveType::LineStrip);
    m_cshape->setFillColor(Color::Blue);
    renderer->add(m_cshape);
#endif
}

void BallLock::update() {
#if DEBUG
        m_cshape->setPosition(m_body->GetPosition().x * g_graphicsScale, m_body->GetPosition().y * g_graphicsScale);
#endif
    if (m_move != 0) {
        m_body->SetTransform(b2Vec2(m_move * m_xOffset / g_graphicsScale, m_move * m_yOffset / g_graphicsScale) + m_body->GetPosition(), m_body->GetAngle());

        if (m_move > 0 && m_locked < m_capacity) {
#if !DEBUG
            m_elts.at(m_locked)->setLayer(m_layerID * 2);
#endif
            m_locked++;
        }
        m_move = 0;
    }
}

b2Fixture* BallLock::getFixture() {
    return m_fixture;
}

void BallLock::trigger() {
    if (m_multi) {
        m_move = 1;
    }
}

bool BallLock::release() {
    bool released = m_locked > 0;
    if (m_multi && released) {
        m_elts.at(m_locked - 1)->setLayer(-99);
        m_move = -1;
        m_locked--;
    }
    return released;
}

int BallLock::getLocation() {
    return m_releaseLocation;
}

int BallLock::getNumLocked() {
    return m_locked;
}

void BallLock::reset() {
    if (m_multi) {
        m_move = 0;
        m_body->SetTransform(b2Vec2(-m_locked * m_xOffset / g_graphicsScale, -m_locked * m_yOffset / g_graphicsScale) + m_body->GetPosition(), m_body->GetAngle());
        for (int i = 0; i < m_capacity; i++) {
            m_elts.at(i)->setLayer(-99);
        }
        m_locked = 0;
    }
}

