#include "SceneElement.h"

SceneElement::SceneElement(Transformable* sfObj) : m_children(), m_sfObj(sfObj), m_t(Transform::Identity) {
	m_layer = 0;
}

void SceneElement::add(SceneElement* elt) {
	m_children.push_back(elt);
}

void SceneElement::remove(SceneElement* elt) {
	m_children.erase(std::find(m_children.begin(), m_children.end(), elt));
}

void SceneElement::setLayer(int layer) {
	m_layer = layer;
}

int SceneElement::getLayer() {
	return m_layer;
}

Transformable* SceneElement::getSFObj() {
	return m_sfObj;
}

vector<SceneElement*> SceneElement::getChildren() {
	return m_children;
}

void SceneElement::setTransform(Transform t) {
	m_t = t;
}

Transform SceneElement::getTransform() {
	return m_t;
}