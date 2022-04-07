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
    for (int i=0; i<width; i++)
        for(int j=0; j<height; j++) {
            if(data[i*height+j]<min) {
                min = data[i*height+j];
            }
            if(data[i*height+j]>max) {
                max = data[i*height+j];
            }
            temp = (data[i*height+j]-lowCount) * 256 / (highCount-lowCount);
            if(temp < 0) {
                temp = 0;
            }
            else if(temp > 255) {
                temp = 255;
            }
            m_imageData[i*height+j] = temp;
            if (uploadData) {
                char a = (data[i*height+j] >> 8) & 0xff;
                char b = data[i*height+j] & 0xff;
                encode_data.append(a);
                encode_data.append(b);
            }
        }
    if(m_autoScale) {
        lowCount = min;
        highCount = max;
    }
    m_imageProvider->generateImage(m_imageData, width, height);
    if (hasServer && uploadData) {
        m_server->sendData(encode_data);
    }
    m_ccd->setDataProcessed();
}
