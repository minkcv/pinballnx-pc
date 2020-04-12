#include "gtarget.h"

GTarget::GTarget(SceneElement* root, b2World& world, int targetGroupID, int layerID, bool sameSprites, bool collide, bool dontReset) {
    m_layerID = layerID;
    m_collide = collide;
    m_dontReset = dontReset;
	m_sound = new SoundBuffer();
	if (collide)
		m_sound->loadFromFile("data/gtarget1.wav");
	else
		m_sound->loadFromFile("data/gtarget2.wav");
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
        fixtureDef.isSensor = !collide;
        fixtureDef.filter.maskBits = 1 << layerID;
        fixtureDef.filter.categoryBits = 1 << layerID;
        b2Fixture* fixture = body->CreateFixture(&fixtureDef);
        m_fixtures.push_back(fixture);
        m_isPressed.push_back(false);
        m_dropTimers.push_back(0);
    #if DEBUG
        // Debug graphics
        ConvexShape* cshape = new ConvexShape();
        addPointsToShape(cshape, points);
        cshape->getVertexArray()->setPrimitiveType(PrimitiveType::LineStrip);
        renderer->add(cshape);
    #else
		Texture* textureEnabled = new Texture();
        if (sameSprites)
			textureEnabled->loadFromFile("data/gtarget" + to_string(targetGroupID) + "-0" + "enabled.png");
        else
			textureEnabled->loadFromFile("data/gtarget" + to_string(targetGroupID) + "-" + to_string(s) + "enabled.png");
		Sprite* spriteEnabled = new Sprite();
		spriteEnabled->setTexture(*textureEnabled);
		SceneElement* eltEnabled = new SceneElement(spriteEnabled);
        eltEnabled->setLayer(layerID * 2);
        eltEnabled->getSFObj()->setPosition(m_positions.at(targetGroupID).at(s * 2), m_positions.at(targetGroupID).at(s * 2 + 1));
        root->add(eltEnabled);
        m_eltsEnabled.push_back(eltEnabled);

		Texture* textureDisabled = new Texture();
        if (sameSprites)
			textureDisabled->loadFromFile("data/gtarget" + to_string(targetGroupID) + "-0" + "disabled.png");
        else
			textureDisabled->loadFromFile("data/gtarget" + to_string(targetGroupID) + "-" + to_string(s) + "disabled.png");
		Sprite* spriteDisabled = new Sprite();
		spriteDisabled->setTexture(*textureDisabled);
		SceneElement* eltDisabled = new SceneElement(spriteDisabled);
        eltDisabled->setLayer(-99);
        eltDisabled->getSFObj()->setPosition(m_positions.at(targetGroupID).at(s * 2), m_positions.at(targetGroupID).at(s * 2 + 1));
        m_eltsDisabled.push_back(eltDisabled);
        root->add(eltDisabled);
    #endif
    }
}

void GTarget::update() {
	for (size_t i = 0; i < m_sounds.size(); i++) {
		if (m_sounds.at(i)->getStatus() == SoundSource::Status::Stopped) {
			Sound* sound = m_sounds.at(i);
			m_sounds.erase(m_sounds.begin() + i);
			delete sound;
		}
	}
    if (m_frameCurrent < m_hitFrames) {
        m_frameCurrent++;
    }
    else if (m_frameCurrent == m_hitFrames) {
        reset();
        m_frameCurrent++;
    }
    for (size_t i = 0; i < m_dropTimers.size(); i++) {
        if (m_dropTimers.at(i) > 0) {
            m_dropTimers[i] = m_dropTimers[i] - 1;
            if (m_dropTimers.at(i) == 0) {
                b2Filter fd;
                fd.maskBits = 0;
                fd.categoryBits = 0;
                m_fixtures.at(i)->SetFilterData(fd);
            }
        }
    }
}

vector<b2Fixture*> GTarget::getFixtures() {
    return m_fixtures;
}

bool GTarget::press(size_t targetID) {
	Sound* sound = new Sound();
	sound->setBuffer(*m_sound);
	if (!g_muted) {
		sound->play();
		m_sounds.push_back(sound);
	}
    m_isPressed[targetID] = true;
    if (m_collide)
        m_dropTimers[targetID] = m_dropTime;
#if !DEBUG
    m_eltsEnabled.at(targetID)->setLayer(-99);
    m_eltsDisabled.at(targetID)->setLayer(m_layerID * 2);
#endif
    size_t numPressed = 0;
    for (size_t i = 0; i < m_isPressed.size(); i++) {
        if (m_isPressed.at(i))
            numPressed++;
    }
    if (numPressed == m_isPressed.size()) {
        if (!m_dontReset)
            m_frameCurrent = 0;
        return true;
    }
    return false;
}

bool GTarget::isPressed(size_t targetID) {
    return m_isPressed.at(targetID);
}

void GTarget::reset() {
    for (size_t i = 0; i < m_isPressed.size(); i++) {
        m_isPressed[i] = false;
        if (m_collide) {
            b2Filter fd;
            fd.maskBits = 1 << m_layerID;
            fd.categoryBits = 1 << m_layerID;
            m_fixtures.at(i)->SetFilterData(fd);
        }
#if !DEBUG
        m_eltsEnabled.at(i)->setLayer(m_layerID * 2);
        m_eltsDisabled.at(i)->setLayer(-99);
#endif
    }
}