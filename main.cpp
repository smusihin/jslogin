#include <QApplication>
#include <QtWebKitWidgets/QWebView>
#include <QtWebKitWidgets/QWebFrame>
#include <QtWebKitWidgets/QWebPage>
#include <QWebElement>
#include <QThread>
#include <QDebug>
#include <QObject>
#include <QTimer>

#include "access_manager.h"
#include "websocket_object.h"


const char * const url =  "https://example.com";
const QString login = "your_login";
const char * const password = "12345678";
const int wait_loading_ms = 10000;

const QStringList keys = {"session_id",
                          "login_name",
                          "device_id",
                          "public_value",
                          "evidence_message",
                          "Sec-WebSocket-Key",
                          "ticket"};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug()<<QSslSocket::supportsSsl()
          <<QSslSocket::sslLibraryBuildVersionString()
          <<QSslSocket::sslLibraryVersionString();

    auto view = new QWebView;
    view->show();

    AccessManager manager;
    auto settings = view->page()->settings();
    view->page()->setNetworkAccessManager(&manager) ;
    view->setMouseTracking(true);
    settings->setAttribute(QWebSettings::JavascriptEnabled, true);
    settings->setAttribute(QWebSettings::DeveloperExtrasEnabled,true);
    settings->setAttribute(QWebSettings::JavascriptCanOpenWindows,true);
    settings->setAttribute(QWebSettings::LocalStorageEnabled,true);

    QObject::connect(&manager, &AccessManager::paramsLoaded,
            [](const QStringList & params)
    {
        for(const auto &param: params)
        {
            auto keyAndValue = param.split('=');
            if(keyAndValue.size() == 2)
            {
                auto key = keyAndValue[0];
                auto value = keyAndValue[1];
                if(keys.contains(key))
                {
                    qDebug() << key << " = " << value;
                }
            }
        }
    });

    view->load(QUrl(url));

   QTimer::singleShot(wait_loading_ms,[&view](){
       auto frame = view->page()->mainFrame();
       frame->evaluateJavaScript("n = window.Ext.ComponentManager.all['bp-window-login-1018'].controller; "
                                    "i=n.getView(); r=i.down('form').getForm(); o=r.getValues();"
                                   "o.login='"+login+"';o.password='"+password+"';"
                                    "BP.ux.data.Authorization.initAuthorization({authType:'SRP6a',params:o||{},scope:n.getView()},n.loadAuthorizationSuccess,n.loadAuthorizationFailure)");
        });

   auto sendByWebsocket = [&view](const QString & data){
       auto frame = view->page()->mainFrame();
       frame->evaluateJavaScript("globalWebSocket.send(\""+ data +"\");");
   };

   sendByWebsocket("your_data");


   WebSocketObject wsObject;

   QTimer::singleShot(5* wait_loading_ms,[&view, &wsObject](){
       auto frame = view->page()->mainFrame();

       //access to websocket
       qDebug() << "websocket url: " << frame->evaluateJavaScript("globalWebSocket.url");

       frame->addToJavaScriptWindowObject("wsObject", &wsObject); //bridge js - c++
       //push data from js websocket to c++ object
       //js websocket documentation: https://developer.mozilla.org/ru/docs/Web/API/WebSocket
       frame->evaluateJavaScript("globalWebSocket.addEventListener('message', function(event)"
                                 "{"
                                    "wsObject.printData(event.data);"
                                 "})");
       });

    return a.exec();
}
