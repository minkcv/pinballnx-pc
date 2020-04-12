#ifndef INFOSCREEN_H_
#define INFOSCREEN_H_

#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include "SceneElement.h"

using namespace sf;
using namespace std;

class InfoScreen {
    public:
        InfoScreen(SceneElement* root);
        void show();
        void hide();
    private:
        vector<SceneElement*> m_textLines;
        SceneElement* m_shape;
        SceneElement* m_controlsImage;
		Font* m_font;
};
#endif