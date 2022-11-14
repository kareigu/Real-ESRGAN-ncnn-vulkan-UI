#include "main_window.h"

#include "palette.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  app.setStyle(QStyleFactory::create("Fusion"));
  app.setPalette(dark_palette());
  app.setApplicationName("realesrgan-ncnn-vulkan-ui");
  app.setOrganizationName("realesrgan-ncnn-vulkan-ui");
  MainWindow window;
  window.show();
  return app.exec();
}
