#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <QObject>
#include "andorccd.h"
#include "ccdimageprovider.h"

class DataHandler : public QObject
{
    Q_OBJECT
public:
    explicit DataHandler(QObject *parent = nullptr);

private:
    AndorCCD *m_ccd = nullptr;
    CCDImageProvider *m_imageProvider = nullptr;
    bool m_autoScale = true;
    int lowCount = 0;
    int highCount = 65535;
    unsigned char m_imageData[512*512];

signals:

public slots:
    void setCCD(AndorCCD *ccd);
    void setImageProvider(CCDImageProvider *imageProvider);
    void generateImageData(unsigned short *data, int width, int height);
};

#endif // DATAHANDLER_H
