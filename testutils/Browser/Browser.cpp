// To build the source, set your drive letter Q: to the Qt SDK root,
// e.g., use "subst q: C:\QtSDK\Desktop\Qt\4.8.0\msvc2010" on the cmd line.
// This minimal source is derived from http://trac.webkit.org/wiki/QtWebKitTiling,
// for an example with more features see http://doc.qt.nokia.com/qt-maemo/maemo5-graphicswebview.html


#include <QApplication.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsWebView>
#include <QWebSettings>
#include <QNetworkProxy>

#include <windows.h>
#include <string>


const char * DEFAULT_URL  = "http://localhost/";
const char * DEFAULT_SIZE = "550x720";
const char * DEFAULT_PROXY = 0;


static void ShowUsage(const char * exeName) {

  const char * name = "";
  if (exeName) {
    name = strrchr(exeName, '\\');
    if (name) {
      name++;
    } else {
      name = exeName;
    }
  }

  std::string msg = "Usage:\n";
  msg += "  " + std::string(name) + " [-w###x###] [url]\n";
  msg += "  " + std::string(name) + " -h\n\n";
  msg += "Use -w###x### to specify the window size\n";
  MessageBoxA(NULL, msg.c_str(), name, MB_ICONINFORMATION);
}


class BGraphicsWebView : public QGraphicsWebView {
public:
  bool loadFinished(bool);
};


bool BGraphicsWebView::loadFinished(bool) {
  return 0;
}


int main(int argc, char *argv[]) {

  const char * url = DEFAULT_URL;
  const char * size = DEFAULT_SIZE;
  const char * proxy = DEFAULT_PROXY;

  for (int i = 1; i<argc; i++) {   
    if (argv[i][0]=='/' || argv[i][0]=='-') {
      if (argv[i][1]=='h' || argv[i][1]=='H' || argv[i][1]=='?') {
        ShowUsage(argv[0]);
        return 0;
      } 
      else if ((argv[i][1]=='w') || (argv[i][1]=='W')) {
        if ((argv[i][2]!=0) && (strchr(argv[1],'x')!=0) && (size==DEFAULT_SIZE)) {
          size = argv[i]+2;
        }
      }
      else if ((argv[i][1]=='p') || (argv[i][1]=='P')) {
        proxy = argv[i]+2;
      }
      else {
        ShowUsage(argv[0]);
        return 1;
      }
    } else {
      if (url==DEFAULT_URL) {
        url = argv[i];
      } else {
        ShowUsage(argv[0]);
        return 1;
      }
    }
  }

  QApplication app(argc, argv);
  if (proxy) {
    QUrl proxyUrl = proxy;
    QNetworkProxy netProxy(QNetworkProxy::HttpProxy, proxyUrl.host(), proxyUrl.port());
    QNetworkProxy::setApplicationProxy(netProxy);
  }

  QGraphicsScene scene;

  QGraphicsView view(&scene);
  view.setFrameShape(QFrame::NoFrame);
  view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  BGraphicsWebView webview;
  int width=0, height=0;
  if (2!=sscanf(size,"%ux%u", &width, &height)) {
    ShowUsage(argv[0]);
    return 1;
  }

  webview.resize(width, height);
  webview.load(QUrl(url));

  scene.addItem(&webview);
  view.setFixedSize(width, height);
  view.show();

  int ret = app.exec();

  return ret;
}
