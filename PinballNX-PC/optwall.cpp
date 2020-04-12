#include "optwall.h"

OptWall::OptWall(SceneElement* root, b2World& world, int optWallId, int layerID) {
    m_layerID = layerID;

    vector<float> points = m_WallShapes.at(optWallId);

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
    fixtureDef.filter.maskBits = 1 << layerID;
    fixtureDef.filter.categoryBits = 1 << layerID;
    m_fixture = body->CreateFixture(&fixtureDef);

#if DEBUG
    // Debug graphics
    m_shapeEnabled = new ConvexShape();
    addPointsToShape(m_shapeEnabled, points);
    m_shapeEnabled->getVertexArray()->setPrimitiveType(PrimitiveType::LineStrip);
    if (layerID == 3)
        m_shapeEnabled->setFillColor(Color::Red);
    if (layerID == 0)
        m_shapeEnabled->setFillColor(Color::Cyan);
    renderer->add(m_shapeEnabled);
    m_shapeEnabled->setLayer(layerID * 2 + 1);
#else
	Texture* textureEnabled = new Texture();
	textureEnabled->loadFromFile("data/optwall" + std::to_string(optWallId) + "enabled.png");
	Sprite* spriteEnabled = new Sprite();
	spriteEnabled->setTexture(*textureEnabled);
	m_eltEnabled = new SceneElement(spriteEnabled);
    root->add(m_eltEnabled);
    m_eltEnabled->setLayer(layerID * 2 + 1);
    m_eltEnabled->getSFObj()->setPosition(m_positions.at(optWallId * 2), m_positions.at(optWallId * 2 + 1));
	Texture* textureDisabled = new Texture();
	textureDisabled->loadFromFile("data/optwall" + std::to_string(optWallId) + "disabled.png");
	Sprite* spriteDisabled = new Sprite();
	spriteDisabled->setTexture(*textureDisabled);
	m_eltDisabled = new SceneElement(spriteDisabled);
    root->add(m_eltDisabled);
    m_eltDisabled->setLayer(-100);
    m_eltDisabled->getSFObj()->setPosition(m_positions.at(optWallId * 2), m_positions.at(optWallId * 2 + 1));
#endif
    // Default state is disabled
    disable();
}

void OptWall::enable() {
    m_isEnabled = true;
    b2Filter filterData = m_fixture->GetFilterData();
    filterData.maskBits = 1 << m_layerID;
    filterData.categoryBits = 1 << m_layerID;
    m_fixture->SetFilterData(filterData);
#if !DEBUG
    m_eltEnabled->setLayer(m_layerID * 2 + 1);
    m_eltDisabled->setLayer(-99);
#else
    m_shapeEnabled->setLayer(m_layerID * 2 + 1);
#endif
}

void OptWall::disable() {
    m_isEnabled = false;
    b2Filter filterData = m_fixture->GetFilterData();
    filterData.maskBits = 0;
    filterData.categoryBits = 0;
    m_fixture->SetFilterData(filterData);
#if !DEBUG
    m_eltEnabled->setLayer(-99);
    m_eltDisabled->setLayer(m_layerID * 2 + 1);
#else
    m_shapeEnabled->setLayer(-100);
#endif
}

void OptWall::toggle() {
    if (isEnabled())
        disable();
    else
        enable();
}

bool OptWall::isEnabled() {
    return m_isEnabled;
}
