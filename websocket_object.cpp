#include "websocket_object.h"
#include <QDebug>

WebSocketObject::WebSocketObject()
{

}

WebSocketObject::~WebSocketObject() {}

void WebSocketObject::printData(const QVariant &data)
{
    qDebug() << data;
}
