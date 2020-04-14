#include "infoscreen.h"

InfoScreen::InfoScreen(SceneElement* root) {
    RectangleShape* shape = new RectangleShape(Vector2f(500, 600));
    shape->setPosition(440, 60);
    shape->setFillColor(Color::Black);
	m_shape = new SceneElement(shape);
    m_shape->setLayer(-99);
    root->add(m_shape);
	Texture* texture = new Texture();
	texture->loadFromFile("data/controls.png");
	Sprite* sprite = new Sprite();
	texture->setSmooth(false);
	sprite->setTexture(*texture);
	m_controlsImage = new SceneElement(sprite);
    m_controlsImage->getSFObj()->setPosition(480, 60);
    m_controlsImage->getSFObj()->setScale(3.0, 3.0);
    m_controlsImage->setLayer(-99);
    root->add(m_controlsImage);

	m_font = new Font();
	m_font->loadFromFile("data/texgyreheros-regular.ttf");
    vector<string> infoText = {
        "PINBALLNX  BY  MINKCV - VERSION  1.1"
	};
    for (size_t t = 0; t < infoText.size(); t++) {
        Text* text = new Text();
		text->setFont(*m_font);
		text->setString(infoText.at(t));
        text->setCharacterSize(30);
        text->setPosition(920 - t * 35, 80);
        text->setRotation(90);
		SceneElement* textElt = new SceneElement(text);
        root->add(textElt);
        textElt->setLayer(-100);
        m_textLines.push_back(textElt);
    }
}

void InfoScreen::show() {
    for (size_t t = 0; t < m_textLines.size(); t++) {
        m_textLines.at(t)->setLayer(100);
    }
    m_controlsImage->setLayer(99);
    m_shape->setLayer(99);
}

void InfoScreen::hide() {
    for (size_t t = 0; t < m_textLines.size(); t++) {
        m_textLines.at(t)->setLayer(-100);
    }
    m_shape->setLayer(-99);
    m_controlsImage->setLayer(-100);
}
