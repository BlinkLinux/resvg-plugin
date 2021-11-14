// Copyright (c) 2021 Xu Shaohua <shaohua@biofan.org>. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "svg_io_handler.h"

QT_BEGIN_NAMESPACE

SvgIOHandler::SvgIOHandler() {

}

SvgIOHandler::~SvgIOHandler() {

}

bool SvgIOHandler::canRead() const {
  return false;
}

QByteArray SvgIOHandler::name() const {
  return QImageIOHandler::name();
}

bool SvgIOHandler::read(QImage* image) {
  return false;
}

bool SvgIOHandler::canRead(QIODevice* device) {
  return false;
}

QVariant SvgIOHandler::option(QImageIOHandler::ImageOption option) const {
  return QImageIOHandler::option(option);
}

void SvgIOHandler::setOption(QImageIOHandler::ImageOption option, const QVariant& value) {
  QImageIOHandler::setOption(option, value);
}

bool SvgIOHandler::supportsOption(QImageIOHandler::ImageOption option) const {
  return QImageIOHandler::supportsOption(option);
}

QT_END_NAMESPACE