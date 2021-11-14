// Copyright (c) 2021 Xu Shaohua <shaohua@biofan.org>. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "svg_io_handler.h"

#include <QBuffer>

#include "ResvgQt.h"

QT_BEGIN_NAMESPACE

SvgIOHandler::SvgIOHandler() : QImageIOHandler(), renderer(new ResvgRenderer) {

}

SvgIOHandler::~SvgIOHandler() {

}

bool SvgIOHandler::canRead() const {
  if (!device()) {
    return false;
  }
  if (loaded && !readDone) {
    return true;
  }

  // Check magic number.
  QByteArray buf = device()->peek(8);
  if (buf.startsWith("\x1f\x8b")) {
    setFormat(kExtSvgz);
    return true;
  } else if (buf.contains("<?xml") || buf.contains("<svg") || buf.contains("<!--")) {
    setFormat(kExtSvg);
    return true;
  }
  return false;
}

QByteArray SvgIOHandler::name() const {
  return kExtSvg;
}

bool SvgIOHandler::read(QImage* image) {
  if (readDone || load(device())) {
    bool xform = (clipRect.isValid() || scaledSize.isValid() || scaledClipRect.isValid());
    QSize finalSize = defaultSize;
    QRectF bounds;
    if (xform && !defaultSize.isEmpty()) {
      bounds = QRectF(QPointF(0,0), QSizeF(defaultSize));
      QPoint tr1, tr2;
      QSizeF sc(1, 1);
      if (clipRect.isValid()) {
        tr1 = -clipRect.topLeft();
        finalSize = clipRect.size();
      }
      if (scaledSize.isValid()) {
        sc = QSizeF(qreal(scaledSize.width()) / finalSize.width(),
                    qreal(scaledSize.height()) / finalSize.height());
        finalSize = scaledSize;
      }
      if (scaledClipRect.isValid()) {
        tr2 = -scaledClipRect.topLeft();
        finalSize = scaledClipRect.size();
      }
      QTransform t;
      t.translate(tr2.x(), tr2.y());
      t.scale(sc.width(), sc.height());
      t.translate(tr1.x(), tr1.y());
      bounds = t.mapRect(bounds);
    }
    if (!finalSize.isEmpty()) {
      if (bounds.isEmpty() && backColor.alpha() == 0) {
        *image = renderer->renderToImage(finalSize);
      } else {
        *image = QImage(finalSize, QImage::Format_ARGB32_Premultiplied);
        image->fill(backColor.rgba());
        QPainter p(image);
        p.setRenderHints(QPainter::SmoothPixmapTransform);
//        renderer->render(&p, bounds);
        renderer->render(&p);
        p.end();
      }
    }
    readDone = true;
    return true;
  }

  return false;
}

// static
bool SvgIOHandler::canRead(QIODevice* device) {
  QByteArray buf = device->peek(8);
  return buf.startsWith("\x1f\x8b") || buf.contains("<?xml") || buf.contains("<svg") || buf.contains("<!--");
}

QVariant SvgIOHandler::option(QImageIOHandler::ImageOption option) const {
  switch(option) {
    case ImageFormat: {
      return QImage::Format_ARGB32_Premultiplied;
    }
    case Size: {
      const_cast<SvgIOHandler*>(this)->load(device());
      return defaultSize;
    }
    case ClipRect: {
      return clipRect;
    }
    case ScaledSize: {
      return scaledSize;
    }
    case ScaledClipRect: {
      return scaledClipRect;
    }
    case BackgroundColor: {
      return backColor;
    }
    default: {
      return {};
    }
  }
}

void SvgIOHandler::setOption(QImageIOHandler::ImageOption option, const QVariant& value) {
  switch(option) {
    case ClipRect: {
      clipRect = value.toRect();
      break;
    }
    case ScaledSize: {
      scaledSize = value.toSize();
      break;
    }
    case ScaledClipRect: {
      scaledClipRect = value.toRect();
      break;
    }
    case BackgroundColor: {
      backColor = value.value<QColor>();
      break;
    }
    default: {
      break;
    }
  }
}

bool SvgIOHandler::supportsOption(QImageIOHandler::ImageOption option) const {
  switch(option) {
    case ImageFormat:  // fall through
    case Size:  // fall through
    case ClipRect:  // fall through
    case ScaledSize:  // fall through
    case ScaledClipRect:  // fall through
    case BackgroundColor: {
      return true;
    }
    default: {
      return false;
    }
  }
}

bool SvgIOHandler::load(QIODevice* device) {
  if (!device) {
    return false;
  }

  if (loaded) {
    return true;
  }
  if (format().isEmpty()) {
    canRead();
  }

  bool res = false;
  auto *buf = qobject_cast<QBuffer*>(device);
  if (buf) {
    const QByteArray &ba = buf->data();
    res = renderer->load(QByteArray::fromRawData(ba.constData() + buf->pos(), ba.size() - buf->pos()));
    buf->seek(ba.size());
  } else if (format() == kExtSvgz) {
    res = renderer->load(device->readAll());
  } else {
    res = renderer->load(device->readAll());
  }

  if (res) {
    defaultSize = renderer->viewBox().size();
    loaded = true;
  }

  return loaded;
}

QT_END_NAMESPACE