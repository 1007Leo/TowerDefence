//#include "MouseListener.h"
//MouseListener::MouseListener()
//{
//}
//
//MouseListener::MouseListener(GameScene* scene)
//{
//	this->_mouseListener = EventListenerMouse::create();
//	_mouseListener->onMouseUp = CC_CALLBACK_1(MouseListener::onMouseUp, this);
//	//_eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);
//	this->scene = scene;
//}
//
//void MouseListener::onMouseUp(Event* event)
//{
//	EventMouse* e = (EventMouse*)event;
//	//const char* str = "Mouse Up detected, Key: ";
//	
//	//Vec2 tmp = this->scene->pixelToCoord({ e->getCursorX(), e->getCursorY() }, { 40, 40 });
//	const char* str = "Mouse Up detected, Key: ";
//	//str += tostr(e->getMouseButton());
//}
