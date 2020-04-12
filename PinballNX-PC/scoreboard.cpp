#include "scoreboard.h"

Scoreboard::Scoreboard(SceneElement* root) {
    int backgroundHeight = 1280 - 1152;
	RectangleShape* rect = new RectangleShape(Vector2f(backgroundHeight, 768));
    rect->setPosition(1152, 0);
    rect->setFillColor(Color::Black);
	m_background = new SceneElement(rect);
    root->add(m_background);
    m_background->setLayer(-99);

    m_font = new Font();
    m_font->loadFromFile("data/advanced_dot_digital-7.ttf");
    Text* ballsLeftText = new Text();
	ballsLeftText->setString("BALL 1");
    ballsLeftText->setFont(*m_font);
    ballsLeftText->setCharacterSize(40);
    ballsLeftText->setPosition(1268, 5);
    ballsLeftText->setRotation(90);
	m_ballsLeftText = new SceneElement(ballsLeftText);
    root->add(m_ballsLeftText);

    Text* scoreText = new Text();
    scoreText->setFont(*m_font);
    scoreText->setCharacterSize(40);
    scoreText->setPosition(1268, 0);
    scoreText->setRotation(90);
	m_scoreText = new SceneElement(scoreText);
    root->add(m_scoreText);

    Text* announce = new Text();
    announce->setFont(*m_font);
    announce->setCharacterSize(40);
    announce->setPosition(1200, 350);
    announce->setRotation(90);
	m_announce = new SceneElement(announce);
    root->add(m_announce);
    m_announce->setLayer(-100);
}

void Scoreboard::update(int currentBall, int maxBalls, uint64_t score, bool paused, string announce, string announceFlash) {
    int announceWidth = 0;
    if (announceFlash.length() < 1) {
        m_announceStr = announce;
    }
    else {
        if (m_flashTimer < 0) {
            if (m_announceStr.compare(announce) == 0)
                m_announceStr = announceFlash;
            else
                m_announceStr = announce;
            m_flashTimer = m_flashDelay;
        }
        else
            m_flashTimer--;
    }
    ((Text*)m_announce->getSFObj())->setString(m_announceStr);
    if (m_announceStr.length() > 0) {
        int oneWidth = 0;
        for (size_t c = 0; c < m_announceStr.length(); c++) {
            if (oneWidth == 0)
                oneWidth = m_font->getGlyph(m_announceStr.at(c), 40, 0).advance;
            announceWidth += m_font->getGlyph(m_announceStr.at(c), 40, 0).advance;
        }
        int leftMargin = 768 / 2 - announceWidth / 2;
        // Align the characters with the above ones for the ball text
        leftMargin -= leftMargin % oneWidth;
        m_announce->setLayer(0);
        m_announce->getSFObj()->setPosition(1200, leftMargin);
    }
    else {
        m_announce->setLayer(-100);
    }

    if (paused) {
        m_announce->setLayer(0);
    }
    
    if (currentBall < maxBalls + 1)
        ((Text*)m_ballsLeftText->getSFObj())->setString("BALL " + std::to_string(currentBall));
    else {
        ((Text*)m_ballsLeftText->getSFObj())->setString("");
    }

    // Format score as "123,456,789" or "      1,234"
    std::string numWithCommas = std::to_string(score);
    int numCommas = 0;
    int insertPosition = numWithCommas.length() - 3;
    while (insertPosition > 0) {
        numWithCommas.insert(insertPosition, ",");
        insertPosition-=3;
        numCommas++;
    }
    int totalKerning = 0;
    for (size_t c = 0; c < numWithCommas.length() - 1; c++) {
        totalKerning += m_font->getKerning((Uint32)numWithCommas.at(c), numWithCommas.at(c + 1), 40);
    }
    int characterWidths = 0;
    for (size_t c = 0; c < numWithCommas.length(); c++) {
        characterWidths += m_font->getGlyph(numWithCommas.at(c), 40, 0).advance;
    }
    int width = totalKerning + characterWidths;
    m_scoreText->getSFObj()->setPosition(1268, 720 - width);
    ((Text*)m_scoreText->getSFObj())->setString(numWithCommas);
}

Scoreboard::~Scoreboard() {
    delete m_font;
}
