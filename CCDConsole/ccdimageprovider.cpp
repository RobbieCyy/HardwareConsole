#include "ccdimageprovider.h"
//#include <QDebug>

CCDImageProvider::CCDImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
{

}

QPixmap CCDImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    int width = 512;
    int height = 512;

    if (size)
        *size = QSize(width, height);
    //QPixmap pixmap(requestedSize.width() > 0 ? requestedSize.width() : width,
    //               requestedSize.height() > 0 ? requestedSize.height() : height);
    //pixmap.fill(QColor(id).rgba());

    return QPixmap::fromImage(*m_image);
}

void CCDImageProvider::generateImage(unsigned char *imageData, int width, int height) {
    delete m_image;
    m_image = new QImage(imageData, width, height, QImage::Format_Indexed8);
}
