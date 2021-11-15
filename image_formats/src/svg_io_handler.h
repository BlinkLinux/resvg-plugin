// Copyright (c) 2021 Xu Shaohua <shaohua@biofan.org>. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef RESVG_PLUGIN_IMAGE_FORMATS_SRC_SVG_IO_HANDLER_H_
#define RESVG_PLUGIN_IMAGE_FORMATS_SRC_SVG_IO_HANDLER_H_

#include <QByteArray>
#include <QIODevice>
#include <QImage>
#include <QImageIOHandler>
#include <QSharedPointer>
#include <QVariant>

QT_BEGIN_NAMESPACE

class ResvgRenderer;

class SvgIOHandler : public QImageIOHandler {
 public:
  SvgIOHandler();

  ~SvgIOHandler() override;

  bool canRead() const override;

  QByteArray name() const override;

  bool read(QImage* image) override;

  QVariant option(ImageOption option) const override;

  void setOption(ImageOption option, const QVariant& value) override;

  bool supportsOption(ImageOption option) const override;

  static bool canRead(QIODevice* device);

 private:
  bool load(QIODevice* device);

  QSharedPointer<ResvgRenderer> renderer_;

  QSize default_size_{};
  QRect clip_rect_{};
  QSize scaled_size_{};
  QRect scaled_clip_rect_{};
  bool loaded_{false};
  bool read_done_{false};
  QColor back_color_{};
};

constexpr const char kExtSvg[] = "svg";
constexpr const char kExtSvgz[] = "svgz";

#endif  // RESVG_PLUGIN_IMAGE_FORMATS_SRC_SVG_IO_HANDLER_H_
