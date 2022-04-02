#ifndef ANDORCCD_H
#define ANDORCCD_H

#include <QObject>

class AndorCCD : public QObject
{
    Q_OBJECT
public:
    explicit AndorCCD(QObject *parent = nullptr);
    ~AndorCCD();

private:
    unsigned int errorValue;
    char model[32]; // headmodel
    int m_minTemp;  // minimum temperature
    int	m_maxTemp;  // maximum temperature
    int m_lowGain;  // minimum temperature
    int	m_highGain;  // maximum temperature
    int m_gblXPixels; // number of horizontal pixels of the detector
    int	m_gblYPixels; // number of horizontal pixels of the detector
    bool m_keepAcquisition = false;
    bool m_dataProcessed = false;
    unsigned short m_data[512*512];

signals:
    void temperatureChange(float temperature, unsigned int status);
    void acquisitionTimingsChange(float exposure, float accumulate, float kinetic, unsigned int status);
    void finishAcquisition(unsigned short *data, int width, int height);
    void dataProcessed();

public slots:
    void setSpeed();
    void getEMGainRange();
    void coolerOn();
    void coolerOff();
    void openShutter();
    void closeShutter();
    void getTemperature();
    void getAcquisitionTimings();
    void setTemperature(int temperature);
    void setAcquisitionMode(int mode);
    void setExposureTime(float time);
    void setReadMode(int mode);
    void setImage(int hbin, int vbin, int hstart, int hend, int vstart, int vend);
    void setEMCCDGain(int gain);
    void setTriggerMode(int mode);
    void startAcquisitionAndGetData();
    void stopAcquisition();
    void setDataProcessed() { emit dataProcessed(); m_dataProcessed = true; };
    int minTemp() { return m_minTemp; };
    int	maxTemp() { return m_maxTemp; };
    int lowGain() { return m_lowGain; };
    int	highGain() { return m_highGain; };
    int detectorWidth() { return m_gblXPixels; };
    int	detectorHeight() { return m_gblYPixels; };
    bool keepAcquisition() { return m_keepAcquisition; };


};

#endif // ANDORCCD_H
