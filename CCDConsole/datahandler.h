#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <QObject>
#include "andorccd.h"
#include "ccdimageprovider.h"
#include "socketserver.h"

class DataHandler : public QObject
{
    Q_OBJECT
public:
    explicit DataHandler(QObject *parent = nullptr);

private:
    AndorCCD *m_ccd = nullptr;
    CCDImageProvider *m_imageProvider = nullptr;
    SocketServer *m_server = nullptr;
    bool hasServer = false;
    bool m_autoScale = true;
    int lowCount = 0;
    int highCount = 65535;
    unsigned char m_imageData[512*512];

signals:

public slots:
    void setCCD(AndorCCD *ccd);
    void setServer(SocketServer *server);
    void setImageProvider(CCDImageProvider *imageProvider);
    void generateImageData(unsigned short *data, bool uploadData);
};

#endif // DATAHANDLER_H
