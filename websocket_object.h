#ifndef WEBSOCKETOBJECT_H
#define WEBSOCKETOBJECT_H

#include <QObject>

class WebSocketObject: public QObject
{
    Q_OBJECT
public:
    WebSocketObject();    
    virtual ~WebSocketObject() override;
public slots:
    //this method could be called from js
    void printData(const QVariant& data);



};

#endif // WEBSOCKETOBJECT_H
