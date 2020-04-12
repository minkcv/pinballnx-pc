#include "flipper.h"

Flipper::Flipper(SceneElement* root, b2World& world, int flipperID) {
	m_flipSound = new SoundBuffer();
	m_flipSound->loadFromFile("data/flip.wav");
	m_sound = new Sound();
	m_sound->setBuffer(*m_flipSound);
	m_flipSound2 = new SoundBuffer();
	m_flipSound2->loadFromFile("data/flip2.wav");
	m_sound2 = new Sound();
	m_sound2->setBuffer(*m_flipSound2);
    float yOffset = flipperID ? 0.02: -0.02;
    if (flipperID % 2 == 0) {
        m_key = Keyboard::LShift;
        m_rotateDirection = 1.0f;
    }
    else {
        m_key = Keyboard::RShift;
        m_rotateDirection = -1.0f;
    }
    m_x = m_positions.at(flipperID * 2);
    m_y = m_positions.at(flipperID * 2 + 1);
    b2BodyDef bd;
    bd.position.Set(m_x, m_y + yOffset);
    m_pivot = world.CreateBody(&bd);

    b2CircleShape circleShape;
    circleShape.m_radius = 0.18f;

#if DEBUG
    m_pivotShape = new CircleShape(0.18f * g_graphicsScale);
    m_pivotShape->setOrigin(Origin::Center);
    if (flipperID > 2)
        m_pivotShape->setFillColor(Color::Cyan);
    renderer->add(m_pivotShape);
#endif

    b2FixtureDef fd;
    fd.friction = 0.6f;
    fd.density = 1.0f;
    fd.shape = &circleShape;
    if (flipperID > 2) {
        fd.filter.maskBits = 1;
        fd.filter.categoryBits = 1;
    }
    else {
        fd.filter.maskBits = 1 << 2;
        fd.filter.categoryBits = 1 << 2;
    }
    m_pivot->CreateFixture(&fd);

    // Flipper body
    bd.type = b2_dynamicBody;
    bd.position.Set(m_x, m_y);
    bd.bullet = true;
    m_body = world.CreateBody(&bd);

    b2Vec2* vs;
    if (flipperID % 2 == 1)
        vs = getVertexArray(m_rightPoints);
    else
        vs = getVertexArray(m_leftPoints);
    fd.density = 10.0f;
    b2PolygonShape shape;
    shape.Set(vs, 8);
    fd.shape = &shape;
    m_body->CreateFixture(&fd);
    free(vs);

    b2RevoluteJointDef jd;
    jd.bodyA = m_pivot;
    jd.bodyB = m_body;
    jd.referenceAngle = 0;
    jd.enableMotor = true;
    jd.maxMotorTorque = 2000.0f;
    jd.motorSpeed = 0.0f;
    if (flipperID == 2) {
        m_lowerLimit += M_PI * 3.5 / 16;
        m_upperLimit += M_PI * 3.5 / 16;
    }
    jd.lowerAngle = m_lowerLimit;
    jd.upperAngle = m_upperLimit;
    jd.enableLimit = true;
    m_joint = (b2RevoluteJoint*)world.CreateJoint(&jd);

#if DEBUG
    m_cshape = new ConvexShape();
    m_cshape->getVertexArray()->setPrimitiveType(PrimitiveType::LineStrip);
    if (flipperID % 2 == 1)
        addPointsToShape(m_cshape, m_rightPoints);
    else
        addPointsToShape(m_cshape, m_leftPoints);
    if (flipperID > 2)
        m_cshape->setFillColor(Color::Cyan);
    renderer->add(m_cshape);
#else
	Texture* texture = new Texture();
	texture->loadFromFile("data/flipper.png");
    Sprite* sprite = new Sprite();
	sprite->setTexture(*texture);
	m_elt = new SceneElement(sprite);
    if (flipperID > 2)
        m_elt->setLayer(2);
    else
        m_elt->setLayer(6);
    root->add(m_elt);
#endif
}

void Flipper::update() {
    //float jointAngle = m_joint->GetJointAngle();
    //printf("%f %f %f\n", jointAngle, m_lowerLimit, m_upperLimit);
        
    if (Keyboard::isKeyPressed(m_key)) {
		if (!m_keyDown) {
			if (!g_muted)
				m_sound->play();
		}
        m_joint->SetMotorSpeed(-20.0f * m_rotateDirection);
		m_keyDown = true;
    }
    else {
        m_joint->SetMotorSpeed(20.0f * m_rotateDirection);
		if (m_keyDown) {
			if (!g_muted)
				m_sound2->play();
		}
		m_keyDown = false;
    }

    b2Vec2 position = m_body->GetPosition();
    float32 angle = m_body->GetAngle();
#if DEBUG
    m_cshape->setPosition(position.x * g_graphicsScale, position.y * g_graphicsScale);
    m_cshape->setRotation(angle * 180 / M_PI);
    // Pivot graphics
    b2Vec2 pivotPosition = m_pivot->GetPosition();
    m_pivotShape->setPosition(pivotPosition.x * g_graphicsScale, pivotPosition.y * g_graphicsScale);
#else
    m_elt->getSFObj()->setOrigin(16.0f, 16.0f);
    m_elt->getSFObj()->setPosition(position.x * g_graphicsScale, position.y * g_graphicsScale);
    if (m_rotateDirection < 0)
        angle += M_PI;
    m_elt->getSFObj()->setRotation(angle * 180 / M_PI);
#endif
}
