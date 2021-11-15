// Copyright (c) 2021 Xu Shaohua <shaohua@biofan.org>. All rights reserved.
// Use of this source is governed by MPL-2.0 License that can be found
// in the LICENSE file.


#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QLabel>
#include <QLibraryInfo>

int main(int argc, char* argv[]) {
  QApplication application(argc, argv);

  // Install librusty_resvg.so into $APP/plugins/imageformats/
  QDir app_dir = QCoreApplication::applicationDirPath();
  const QString plugin_path = app_dir.absoluteFilePath("plugins");
  QCoreApplication::addLibraryPath(plugin_path);

  QLabel label;
  label.resize(640, 480);
  QPixmap pixmap(":/images/circular-arrow-25.svg");
//  QPixmap pixmap(":/images/logo.svg");
  qDebug() << "pixmap size:" << pixmap.size();
  label.setPixmap(pixmap);
  label.show();

  return QApplication::exec();
}
