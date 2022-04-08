#include "datahandler.h"
//#include <QDebug>

DataHandler::DataHandler(QObject *parent) : QObject(parent)
{

}

void DataHandler::setCCD(AndorCCD *ccd) {
    m_ccd = ccd;
    connect(m_ccd, &AndorCCD::finishAcquisition, this, &DataHandler::generateImageData);
}

void DataHandler::setImageProvider(CCDImageProvider *imageProvider)
{
    m_imageProvider = imageProvider;
}

void DataHandler::setServer(SocketServer *server)
{
    m_server = server;
    hasServer = true;
}

void DataHandler::generateImageData(unsigned short *data, bool uploadData) {
    int width = m_ccd->hend() - m_ccd->hstart() + 1;
    int height = m_ccd->vend() - m_ccd->vstart() + 1;

    int temp;
    int min = 65535;
    int max = 0;
    QByteArray encode_data;
    for (int i=0; i<512*512; i++)
        m_imageData[i] = 0;
    for (int i=0; i<width*height; i++) {
        if(data[i]<min) {
            min = data[i];
        }
        if(data[i]>max) {
            max = data[i];
        }
        temp = (data[i]-lowCount) * 256 / (highCount-lowCount);
        if(temp < 0) {
            temp = 0;
        }
        else if(temp > 255) {
            temp = 255;
        }
        m_imageData[((i%width)+m_ccd->hstart()-1)*512+(i/width+m_ccd->vstart()-1)] = temp;
        if (uploadData) {
            char a = (data[i] >> 8) & 0xff;
            char b = data[i] & 0xff;
            encode_data.append(a);
            encode_data.append(b);
        }
    }
    if(m_autoScale) {
        lowCount = min;
        highCount = max;
    }
    m_imageProvider->generateImage(m_imageData, 512, 512);
    if (hasServer && uploadData) {
        m_server->sendData(encode_data);
    }
    m_ccd->setDataProcessed();
}
