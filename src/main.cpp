#include "main_window.h"

#include "palette.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  app.setStyle(QStyleFactory::create("Fusion"));
  app.setPalette(dark_palette());
  MainWindow window;
  window.show();
  return app.exec();
}
