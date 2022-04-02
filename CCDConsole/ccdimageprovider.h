#ifndef CCDIMAGEPROVIDER_H
#define CCDIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include "andorccd.h"

class CCDImageProvider : public QQuickImageProvider
{
public:
    CCDImageProvider();
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;

private:
    QImage *m_image = new QImage(512, 512, QImage::Format_Indexed8);

signals:

public slots:
    void generateImage(unsigned char *imageData, int width, int height);
};

#endif // CCDIMAGEPROVIDER_H
