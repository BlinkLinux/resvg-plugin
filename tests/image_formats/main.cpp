// Copyright (c) 2021 Xu Shaohua <shaohua@biofan.org>. All rights reserved.
// Use of this source is governed by GNU General Public License
// that can be found in the LICENSE file.

#include <QApplication>

#include <QLabel>

int main(int argc, char* argv[]) {
  QApplication application(argc, argv);

  QLabel label;
  label.resize(640, 480);
  QPixmap pixmap(":/images/logo.svg");
  label.setPixmap(pixmap);
  label.show();

  return QApplication::exec();
}