// Copyright (c) 2021 Xu Shaohua <shaohua@biofan.org>. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "svg_plugin.h"

#include "svg_io_handler.h"

QT_BEGIN_NAMESPACE

QStringList SvgPlugin::keys() const {
  return {kExtSvg, kExtSvgz};
}

QImageIOPlugin::Capabilities SvgPlugin::capabilities(QIODevice* device, const QByteArray& format) const {
  if (format == kExtSvg || format == kExtSvgz) {
    return Capabilities(CanRead);
  }

  if (!format.isEmpty()) {
    return {};
  }

  Capabilities cap;
  if (device->isReadable() && SvgIOHandler::canRead(device)) {
    cap |= CanRead;
  }
  return cap;
}

QImageIOHandler* SvgPlugin::create(QIODevice* device, const QByteArray& format) const {
  auto* handler = new SvgIOHandler();
  handler->setDevice(device);
  handler->setFormat(format);
  return handler;
}

QT_END_NAMESPACE