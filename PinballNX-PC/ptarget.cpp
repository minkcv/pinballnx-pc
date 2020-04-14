#include "ptarget.h"

PTarget::PTarget(SceneElement* root, b2World& world, int targetGroupID, int layerID) {
    m_layerID = layerID;
    m_flashTimer = 0;
    m_currentTarget = 0;
    m_frame = 0;
	m_soundId1 = g_sound->getId("ptarget1");
	m_soundId2 = g_sound->getId("ptarget2");

    vector<vector<float>> shapes = m_targetGroups.at(targetGroupID);
    for (size_t s = 0; s < shapes.size(); s++) {
        vector<float> points = shapes.at(s);
        b2Vec2* vs = getVertexArray(points);
        b2ChainShape chain;
        chain.CreateChain(vs, points.size() / 2);
        free(vs);
        vs = NULL;

        b2BodyDef bodyDef;
        b2Body* body = world.CreateBody(&bodyDef);
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &chain;
        fixtureDef.density = 0.0f;
        fixtureDef.friction = 0.3f;
        fixtureDef.isSensor = true;
        fixtureDef.filter.maskBits = 0xFFFF;
        fixtureDef.filter.categoryBits = 0xFFFF;
        b2Fixture* fixture = body->CreateFixture(&fixtureDef);
        m_fixtures.push_back(fixture);
    #if DEBUG
        // Debug graphics
        ConvexShape* cshape = new ConvexShape();
        addPointsToShape(cshape, points);
        cshape->getVertexArray()->setPrimitiveType(PrimitiveType::LineStrip);
        renderer->add(cshape);
    #else
		Texture* textureEnabled = new Texture();
		textureEnabled->loadFromFile("data/ptarget" + to_string(targetGroupID) + "-" + to_string(s) + "enabled.png");
		Sprite* spriteEnabled = new Sprite();
		spriteEnabled->setTexture(*textureEnabled);
		SceneElement* eltEnabled = new SceneElement(spriteEnabled);
        eltEnabled->setLayer(-99);
        eltEnabled->getSFObj()->setPosition(m_positions.at(targetGroupID).at(s * 2), m_positions.at(targetGroupID).at(s * 2 + 1));
        root->add(eltEnabled);
        m_eltsEnabled.push_back(eltEnabled);
		Texture* textureDisabled = new Texture();
		textureDisabled->loadFromFile("data/ptarget" + to_string(targetGroupID) + "-" + to_string(s) + "disabled.png");
		Sprite* spriteDisabled = new Sprite();
		spriteDisabled->setTexture(*textureDisabled);
		SceneElement* eltDisabled = new SceneElement(spriteDisabled);
        eltDisabled->setLayer(layerID * 2);
        eltDisabled->getSFObj()->setPosition(m_positions.at(targetGroupID).at(s * 2), m_positions.at(targetGroupID).at(s * 2 + 1));
        m_eltsDisabled.push_back(eltDisabled);
		root->add(eltDisabled);
    #endif
    }
}

vector<b2Fixture*> PTarget::getFixtures() {
    return m_fixtures;
}

void PTarget::update() {
    if (m_flashTimer < m_flashFrames)
        m_flashTimer++;
    else if (m_flashTimer == m_flashFrames) {
        m_flashTimer = 0;
        if (m_frame == 0) {
            m_frame = 1;
#if !DEBUG
            m_eltsEnabled.at(m_currentTarget)->setLayer(-99);
            m_eltsDisabled.at(m_currentTarget)->setLayer(m_layerID * 2);
#endif
        }
        else {
            m_frame = 0;
#if !DEBUG
            m_eltsEnabled.at(m_currentTarget)->setLayer(m_layerID * 2);
            m_eltsDisabled.at(m_currentTarget)->setLayer(-99);
#endif
        }
    }
}

bool PTarget::isCurrent(size_t targetID) {
    return targetID == m_currentTarget;
}

bool PTarget::press(size_t targetID) {
	
    if (targetID == m_currentTarget) {
#if !DEBUG
        m_eltsEnabled.at(targetID)->setLayer(-99);
        m_eltsDisabled.at(targetID)->setLayer(m_layerID * 2);
#endif
        m_frame = 0;
        m_currentTarget++;
        if (m_currentTarget == m_fixtures.size()) {
            m_currentTarget = 0;
			g_sound->playSound(m_soundId2);
        }
		else {
			g_sound->playSound(m_soundId1);
		}
    }
    return m_currentTarget == 0;
}

void PTarget::reset() {
    for (size_t i = 0; i < m_fixtures.size(); i++) {
#if !DEBUG
        m_eltsDisabled.at(i)->setLayer(m_layerID * 2);
        m_eltsEnabled.at(i)->setLayer(-99);
#endif
    }
    m_currentTarget = 0;
}