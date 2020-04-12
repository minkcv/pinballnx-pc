#ifndef SCENE_ELEMENT_H_
#define SCENE_ELEMENT_H_

#include "list"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class SceneElement {
public:
	SceneElement(Transformable* sfObj);
	void add(SceneElement* elt);
	void remove(SceneElement* elt);
	void setLayer(int layer);
	int getLayer();
	Transformable* getSFObj();
	vector<SceneElement*> getChildren();
	void setTransform(Transform t);
	Transform getTransform();
private:
	int m_layer;
	Transformable* m_sfObj;
	Transform m_t;
	vector<SceneElement*> m_children;
};

#endif