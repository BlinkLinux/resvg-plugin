// Copyright (c) 2021 Xu Shaohua <shaohua@biofan.org>. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "svg_io_handler.h"

#include <QBuffer>
#include <QPainter>

#include "ResvgQt.h"

QT_BEGIN_NAMESPACE

SvgIOHandler::SvgIOHandler() : QImageIOHandler(), renderer_(new ResvgRenderer) {

}

SvgIOHandler::~SvgIOHandler() {

}

bool SvgIOHandler::canRead() const {
  if (!device()) {
    return false;
  }
  if (loaded_ && !read_done_) {
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
  if (read_done_ || load(device())) {
    bool xform = (clip_rect_.isValid() || scaled_size_.isValid() || scaled_clip_rect_.isValid());
    QSize finalSize = default_size_;
    QRectF bounds;
    if (xform && !default_size_.isEmpty()) {
      bounds = QRectF(QPointF(0, 0), QSizeF(default_size_));
      QPoint tr1, tr2;
      QSizeF sc(1, 1);
      if (clip_rect_.isValid()) {
        tr1 = -clip_rect_.topLeft();
        finalSize = clip_rect_.size();
      }
      if (scaled_size_.isValid()) {
        sc = QSizeF(qreal(scaled_size_.width()) / finalSize.width(),
                    qreal(scaled_size_.height()) / finalSize.height());
        finalSize = scaled_size_;
      }
      if (scaled_clip_rect_.isValid()) {
        tr2 = -scaled_clip_rect_.topLeft();
        finalSize = scaled_clip_rect_.size();
      }
      QTransform t;
      t.translate(tr2.x(), tr2.y());
      t.scale(sc.width(), sc.height());
      t.translate(tr1.x(), tr1.y());
      bounds = t.mapRect(bounds);
    }
    if (!finalSize.isEmpty()) {
      if (bounds.isEmpty() && back_color_.alpha() == 0) {
        *image = renderer_->renderToImage(finalSize);
      } else {
        // TODO(Shaohua):
//        *image = QImage(finalSize, QImage::Format_ARGB32_Premultiplied);
//        image->fill(backColor.rgba());
//        QPainter p(image);
//        p.setRenderHints(QPainter::SmoothPixmapTransform);
//        renderer->render(&p, bounds);
//        p.end();
        *image = renderer_->renderToImage(finalSize);
      }
    }
    read_done_ = true;
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
  switch (option) {
    case ImageFormat: {
      return QImage::Format_ARGB32_Premultiplied;
    }
    case Size: {
      const_cast<SvgIOHandler*>(this)->load(device());
      return default_size_;
    }
    case ClipRect: {
      return clip_rect_;
    }
    case ScaledSize: {
      return scaled_size_;
    }
    case ScaledClipRect: {
      return scaled_clip_rect_;
    }
    case BackgroundColor: {
      return back_color_;
    }
    default: {
      return {};
    }
  }
}

void SvgIOHandler::setOption(QImageIOHandler::ImageOption option, const QVariant& value) {
  switch (option) {
    case ClipRect: {
      clip_rect_ = value.toRect();
      break;
    }
    case ScaledSize: {
      scaled_size_ = value.toSize();
      break;
    }
    case ScaledClipRect: {
      scaled_clip_rect_ = value.toRect();
      break;
    }
    case BackgroundColor: {
      back_color_ = value.value<QColor>();
      break;
    }
    default: {
      break;
    }
  }
}

bool SvgIOHandler::supportsOption(QImageIOHandler::ImageOption option) const {
  switch (option) {
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

  if (loaded_) {
    return true;
  }
  if (format().isEmpty()) {
    canRead();
  }

  bool res = false;
  auto* buf = qobject_cast<QBuffer*>(device);
  if (buf) {
    const QByteArray& ba = buf->data();
    const QByteArray data = QByteArray::fromRawData(ba.constData() + buf->pos(), ba.size() - buf->pos());
    res = renderer_->load(data, ResvgOptions());
    buf->seek(ba.size());
  } else if (format() == kExtSvgz) {
    res = renderer_->load(device->readAll(), ResvgOptions());
  } else {
    res = renderer_->load(device->readAll(), ResvgOptions());
  }

  if (res) {
    default_size_ = renderer_->viewBox().size();
    loaded_ = true;
  }

  return loaded_;
}

QT_END_NAMESPACE