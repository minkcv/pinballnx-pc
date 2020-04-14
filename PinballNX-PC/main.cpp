#include <math.h>
#include <SFML/Graphics.hpp>
#include "Box2D/Box2D.h"
#include "SceneElement.h"
#include "util.h"
#include "table.h"
#include "infoscreen.h"

using namespace std;

static bool drawSort(SceneElement* se1, SceneElement* se2) {
	return se1->getLayer() < se2->getLayer();
}

void subRender(RenderWindow* window, SceneElement* parent, SceneElement* elt, list<SceneElement*>* draws) {
	Transform combined;
	// TODO: this might not actually work for multiple levels of children
	if (parent != nullptr) {
		combined *= parent->getSFObj()->getTransform();
	}
	elt->setTransform(combined);
	draws->push_back(elt);
	for (int i = 0; i < elt->getChildren().size(); i++) {
		 subRender(window, elt, elt->getChildren().at(i), draws);
	}
}

void render(RenderWindow* window, SceneElement* root) {
	window->clear();
	list<SceneElement*>* draws = new list<SceneElement*>();
	subRender(window, nullptr, root, draws);
	std::stable_sort(draws->begin(), draws->end(), drawSort);
	for (list<SceneElement*>::iterator iter = draws->begin(); iter != draws->end(); iter++) {
		Drawable* dw = dynamic_cast<Drawable*>((*iter)->getSFObj());
		window->draw(*dw, (*iter)->getTransform());
	}
	window->display();
}



int main(int argc, char **argv) {
	bool rotate = false;
	bool fullscreen = false;
	bool rReleased = true;
	bool fReleased = true;
	bool mReleased = true;
	float sWidth = VideoMode::getDesktopMode().width;
	float sHeight = VideoMode::getDesktopMode().height;
	string title = "PinballNX-PC";
    // create the main renderer
	int noResize = Style::Close | Style::Titlebar;
	RenderWindow* window = new RenderWindow(VideoMode(1280, 720), title, noResize);
	window->setFramerateLimit(g_displayFrameRate);
    // Portrait graphics on desktop:
	RectangleShape* rootSF = new RectangleShape();
    SceneElement* root = new SceneElement(rootSF);

    // Gravity to the left
    b2Vec2 gravity(-7.5f, 0.0f);
    b2World world(gravity);

    // See important info in util.h about g_displayFrameRate
    float32 timeStep = 1.0f / g_displayFrameRate;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    Table table(root, world);
    InfoScreen infoScreen(root);
    // Start paused.
    infoScreen.show();

    // Do 10 ticks so that the physics objects can settle and 
    // the graphics can update positions and rotations to the physics objects.
    table.update();
    for (int i = 0; i < 10; i++) {
        table.update();
        world.Step(timeStep, velocityIterations, positionIterations);
		render(window, root);
    }

    bool paused = true;
    bool pauseReleased = true;
	bool focused = true;
    // main loop
    while (true) {
		Event event;
		window->pollEvent(event);

		if (event.type == Event::LostFocus)
			focused = false;
		if (event.type == Event::GainedFocus)
			focused = true;
		if (event.type == Event::Closed)
			break;
		if (!focused)
			continue;
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            break;
        }
        if (Keyboard::isKeyPressed(Keyboard::N)) {
            if (table.isGameOver()) {
                table.newGame();
            }
        }
		if (Keyboard::isKeyPressed(Keyboard::M)) {
			if (mReleased)
				g_sound->toggleMute();
			mReleased = false;
		}
		else
			mReleased = true;
		
		if (Keyboard::isKeyPressed(Keyboard::R)) {
			if (rReleased) {
				rotate = !rotate;
				window->close();
				delete window;
				rootSF->setScale(1, 1);
				if (rotate) {
					if (fullscreen) {
						window = new RenderWindow(VideoMode(sWidth, sHeight), title, Style::None);
						if (sHeight / sWidth > 1280.0 / 720.0)
							rootSF->setScale(sWidth / 720.0, sWidth / 720.0);
						else
							rootSF->setScale(sHeight / 1280.0, sHeight / 1280.0);
						rootSF->setPosition(0, sHeight);
					}
					else {
						window = new RenderWindow(VideoMode(720, 1280), title, noResize);
						rootSF->setPosition(0, 1280);
					}
					rootSF->setRotation(-90.0);
				}
				else {
					if (fullscreen) {
						window = new RenderWindow(VideoMode(sWidth, sHeight), title, Style::None);
						if (sWidth / sHeight > 1280.0 / 720.0)
							rootSF->setScale(sHeight / 720.0, sHeight / 720.0);
						else
							rootSF->setScale(sWidth / 1280.0, sWidth / 1280.0);
					}
					else
						window = new RenderWindow(VideoMode(1280, 720), title, noResize);
					rootSF->setPosition(0, 0);
					rootSF->setRotation(0.0);
				}
				window->setFramerateLimit(g_displayFrameRate);
			}
			rReleased = false;
		}
		else
			rReleased = true;
		
		if (Keyboard::isKeyPressed(Keyboard::F)) {
			if (fReleased) {
				fullscreen = !fullscreen;
				window->close();
				delete window;
				rootSF->setScale(1, 1);
				if (fullscreen) {
					if (rotate) {
						window = new RenderWindow(VideoMode(sWidth, sHeight), title, Style::None);
						if (sHeight / sWidth > 1280.0 / 720.0)
							rootSF->setScale(sWidth / 720.0, sWidth / 720.0);
						else
							rootSF->setScale(sHeight / 1280.0, sHeight / 1280.0);
						rootSF->setPosition(0, sHeight);
						rootSF->setRotation(-90.0);
					}
					else {
						window = new RenderWindow(VideoMode(sWidth, sHeight), title, Style::None);
						if (sWidth / sHeight > 1280.0 / 720.0)
							rootSF->setScale(sHeight / 720.0, sHeight / 720.0);
						else
							rootSF->setScale(sWidth / 1280.0, sWidth / 1280.0);
						rootSF->setPosition(0, 0);
						rootSF->setRotation(0.0);
					}
				}
				else {
					if (rotate) {
						window = new RenderWindow(VideoMode(720, 1280), title, noResize);
						rootSF->setRotation(-90.0);
						rootSF->setPosition(0, 1280);
					}
					else {
						window = new RenderWindow(VideoMode(1280, 720), title, noResize);
						rootSF->setRotation(0.0);
						rootSF->setPosition(0, 0);
					}
				}
				window->setFramerateLimit(g_displayFrameRate);
			}
			fReleased = false;
		}
		else
			fReleased = true;
		
        if (Keyboard::isKeyPressed(Keyboard::Space)) {
            if (pauseReleased) {
                if (paused)
                    infoScreen.hide();
                else
                    infoScreen.show();
                paused = !paused;
            }
            pauseReleased = false;
        }
        else {
            pauseReleased = true;
        }
		
        if (!paused) {
            table.update();
            world.Step(timeStep, velocityIterations, positionIterations);
        }
        table.updateScoreboard(paused);
		g_sound->update();
		render(window, root);
    }

    table.cleanup();

    // cleanup
    // will delete child's (textures, shapes, text..)
    //delete (renderer);
    
    return 0;
}
