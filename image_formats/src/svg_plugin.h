// Copyright (c) 2021 Xu Shaohua <shaohua@biofan.org>. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef RESVG_PLUGIN_IMAGE_FORMATS_SRC_SVG_PLUGIN_H_
#define RESVG_PLUGIN_IMAGE_FORMATS_SRC_SVG_PLUGIN_H_

#include <QByteArray>
#include <QDebug>
#include <QIODevice>
#include <QImageIOHandler>
#include <QStringList>

class SvgPlugin : public QImageIOPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QImageIOHandlerFactoryInterface" FILE "svg.json")

 public:
  QStringList keys() const;

  Capabilities capabilities(QIODevice *device, const QByteArray &format) const override;

  QImageIOHandler *create(QIODevice *device, const QByteArray &format = QByteArray()) const override;
};

#endif  // RESVG_PLUGIN_IMAGE_FORMATS_SRC_SVG_PLUGIN_H_
