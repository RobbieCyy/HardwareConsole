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

void DataHandler::generateImageData(unsigned short *data, int width, int height) {
    int temp;
    int min = 65535;
    int max = 0;
    for (int i=0; i<512; i++)
        for(int j=0; j<512; j++) {
            if(data[i*512+j]<min) {
                min = data[i*512+j];
            }
            if(data[i*512+j]>max) {
                max = data[i*512+j];
            }
            temp = (data[i*512+j]-lowCount) * 256 / (highCount-lowCount);
            if(temp < 0) {
                temp = 0;
            }
            else if(temp > 255) {
                temp = 255;
            }
            m_imageData[i*512+j] = temp;
        }
    if(m_autoScale) {
        lowCount = min;
        highCount = max;
    }
    m_imageProvider->generateImage(m_imageData, width, height);
    m_ccd->setDataProcessed();
}
