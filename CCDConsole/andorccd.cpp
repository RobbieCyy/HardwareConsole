#include "andorccd.h"
#include "ATMCD32D.H"
#include <QDir>
#include <QDebug>

AndorCCD::AndorCCD(QObject *parent) : QObject(parent)
{
    char* dir = QDir::currentPath().toLatin1().data(); // Look in current working directory for driver files
    errorValue = Initialize(dir);  // Initialize driver in current directory
    if(errorValue != DRV_SUCCESS) {
        qDebug() << ("Initialize Error");
        return;
    }
    else {
        qDebug() << ("Initialize Succeed");
    }
    // Get Head Model
    errorValue=GetHeadModel(model);
    if(errorValue!=DRV_SUCCESS){
        qDebug() << ("Get Head Model information Error\n");
        return;
    }
    else {
        qDebug() << tr("Head Model %1\n").arg(model);
    }
    errorValue = GetDetector(&m_gblXPixels, &m_gblYPixels);
    if(errorValue!=DRV_SUCCESS){
        qDebug() << ("Get Detector information Error\n");
        return;
    }
    else {
        qDebug() << tr("Size of CCD: %1 x %2 pixels\n").arg(m_gblXPixels).arg(m_gblYPixels);
    }
    errorValue=GetTemperatureRange(&m_minTemp,&m_maxTemp);
    if(errorValue!=DRV_SUCCESS) {
        qDebug() << ("Get temparature range Error");
        return;
    }
    else {
        qDebug() << tr("The detector can be cooled between %1 and %2 in centigrade.").arg(m_minTemp).arg(m_maxTemp);
    }

    setSpeed();
    getEMGainRange();
}

AndorCCD::~AndorCCD()
{
    // Shut down camera
    ShutDown();
}

void AndorCCD::setSpeed()
{
    // Set Horizontal Speed to max
    float STemp = 0;
    float speed;
    int HSnumber = 0; // Horizontal Speed Index
    int ADnumber = 0; // AD Index
    int nAD, iAD, index, iSpeed;
    errorValue = GetNumberADChannels(&nAD);
    if (errorValue != DRV_SUCCESS){
        qDebug() << ("Get number AD Channel Error\n");
        return;
    }
    else {
        for (iAD = 0; iAD < nAD; iAD++) {
            GetNumberHSSpeeds(iAD, 0, &index);
            for (iSpeed = 0; iSpeed < index; iSpeed++) {
                GetHSSpeed(iAD, 0, iSpeed, &speed);
                if(speed > STemp){
                    STemp = speed;
                    HSnumber = iSpeed;
                    ADnumber = iAD;
                }
            }
        }
    }

    errorValue=SetADChannel(ADnumber);
    if(errorValue != DRV_SUCCESS){
        qDebug() << ("Set AD Channel Error\n");
        return;
    }

    errorValue=SetHSSpeed(0,HSnumber);
    if(errorValue != DRV_SUCCESS){
        qDebug() << ("Set Horizontal Speed Error\n");
        return;
    }
    else {
        qDebug() <<  tr("Horizontal speed set. Horizontal Speed set to %1 MHz\n").arg(STemp);
    }

    // Set Vertical speed to recommended
    int VSnumber;
    GetFastestRecommendedVSSpeed(&VSnumber, &speed);
    errorValue=SetVSSpeed(VSnumber);
    if(errorValue != DRV_SUCCESS) {
        qDebug() <<  tr("Set Vertical Speed Error\n");
        return;
    }
    else {
        qDebug() <<  tr("Vertical Speed set to %1 microseconds per pixel shift.\n").arg(speed);
    }
}

void AndorCCD::getEMGainRange() {
    errorValue=GetEMGainRange(&m_lowGain,&m_highGain);
    if(errorValue!=DRV_SUCCESS) {
        qDebug() << ("Get EM Gain range Error");
        return;
    }
    else {
        qDebug() << tr("The EM Gain can be set between %1 and %2.").arg(m_lowGain).arg(m_highGain);
    }
}

void AndorCCD::coolerOn()
{
    if(errorValue != DRV_SUCCESS) {
        return;
    }
    errorValue = CoolerON();
    if(errorValue != DRV_SUCCESS) {
        qDebug() << ("Failed to switched on temperature controller");
    }
    else {
        qDebug() << ("Temperature controller switched ON.");
    }
}

void AndorCCD::coolerOff()
{
    if(errorValue != DRV_SUCCESS) {
        return;
    }
    errorValue = CoolerOFF();
    if(errorValue != DRV_SUCCESS) {
        qDebug() << ("Failed to switched off temperature controller");
    }
    else {
        qDebug() << ("Temperature controller switched OFF.");
    }
}

void AndorCCD::openShutter()
{
    if(errorValue != DRV_SUCCESS) {
        return;
    }
    errorValue = SetShutter(0, 1, 50, 50); //the second 1 stands for Permanently Open
    if(errorValue!=DRV_SUCCESS) {
        qDebug() << ("Shutter error\n");
    }
    else {
        qDebug() << ("Shutters open.\n");
    }
}

void AndorCCD::closeShutter()
{
    if(errorValue != DRV_SUCCESS) {
        return;
    }
    errorValue = SetShutter(0, 2, 50, 50); //the second 2 stands for Permanently Close
    if(errorValue!=DRV_SUCCESS) {
        qDebug() << ("Shutter error\n");
    }
    else {
        qDebug() << ("Shutters close.\n");
    }
}

// This function returns the temperature in degrees of the detector.
// It also gives the status of cooling process.
void AndorCCD::getTemperature()
{
    float temperature;
    unsigned int status = GetTemperatureF(&temperature);
    emit temperatureChange(temperature, status);
}

// This function will return the current “valid” acquisition timing information.
void AndorCCD::getAcquisitionTimings()
{
    float exposure, accumulate, kinetic;
    unsigned int status = GetAcquisitionTimings(&exposure, &accumulate, &kinetic);
    emit acquisitionTimingsChange(exposure, accumulate, kinetic, status);
}

// This function will set the desired temperature of the detector.
void AndorCCD::setTemperature(int temperature)
{
    if(errorValue != DRV_SUCCESS || temperature > m_maxTemp || temperature < m_minTemp) {
        return;
    }
    errorValue = SetTemperature(temperature);
    if(errorValue != DRV_SUCCESS) {
        qDebug() << ("Unable to set temperature");
    }
    else {
        qDebug() << tr("Temperature set to %1 degree centigrade.").arg(temperature);
    }
}

// This function will set the acquisition mode to be used on the next StartAcquisition.
void AndorCCD::setAcquisitionMode(int mode)
{
    if(errorValue != DRV_SUCCESS) {
        return;
    }
    errorValue = SetAcquisitionMode(mode);
    if(errorValue != DRV_SUCCESS) {
        qDebug() << ("Acquisition mode not set.");
    }
    else {
        qDebug() << ("Acquisition mode set.");
    }
}

// This function will set the exposure time to the nearest valid value not less than the given value.
void AndorCCD::setExposureTime(float time)
{
    if(errorValue != DRV_SUCCESS) {
        return;
    }
    errorValue = SetExposureTime(time);
    if(errorValue != DRV_SUCCESS) {
        qDebug() << ("Exposure time declined.");
    }
    else {
        qDebug() << ("Exposure time Accepted.");
    }
}

// This function will set the readout mode to be used on the subsequent acquisitions.
void AndorCCD::setReadMode(int mode)
{
    if(errorValue != DRV_SUCCESS) {
        return;
    }
    errorValue = SetReadMode(mode);
    if(errorValue != DRV_SUCCESS) {
        qDebug() << ("Readout mode not set.");
    }
    else {
        qDebug() << ("Readout mode set.");
    }
}

// This function will set the horizontal and vertical binning to be used when taking a full resolution image.
void AndorCCD::setImage(int hbin, int vbin, int hstart, int hend, int vstart, int vend)
{
    if(errorValue != DRV_SUCCESS) {
        return;
    }
    errorValue = SetImage(hbin, vbin, hstart, hend, vstart, vend);
    if(errorValue != DRV_SUCCESS) {
        qDebug() << ("Parameters not accepted.");
    }
    else {
        qDebug() << ("Parameters accepted.");
    }
}

void AndorCCD::setEMCCDGain(int gain) {
    if(errorValue != DRV_SUCCESS) {
        return;
    }
    errorValue = SetEMCCDGain(gain);
    if(errorValue != DRV_SUCCESS) {
        qDebug() << ("Value for gain not accepted.");
    }
    else {
        qDebug() << ("Value for gain accepted.");
    }
}

// This function will set the trigger mode that the camera will operate in.
void AndorCCD::setTriggerMode(int mode)
{
    if(errorValue != DRV_SUCCESS) {
        return;
    }
    errorValue = SetTriggerMode(mode);
    if(errorValue != DRV_SUCCESS) {
        qDebug() << ("Trigger mode not set.");
    }
    else {
        qDebug() << ("Trigger mode set.");
    }
}

void AndorCCD::startAcquisitionAndGetData(bool uploadData)
{
    int status;
    errorValue = GetStatus(&status);
    if(errorValue != DRV_SUCCESS || status != DRV_IDLE || !m_dataProcessed) {
        return;
    }
    errorValue = StartAcquisition();
    if(errorValue!=DRV_SUCCESS) {
        qDebug() << tr("Acquisition failed： %1\n").arg(errorValue);
        return;
    }
    errorValue = WaitForAcquisition();
    if(errorValue!=DRV_SUCCESS) {
        qDebug() << tr("Acquisition Event not occurred\n");
        return;
    }
    errorValue = GetAcquiredData16(m_data, (m_hend-m_hstart+1)*(m_vend-m_vstart+1));
    if(errorValue!=DRV_SUCCESS) {
        qDebug() << tr("Data copy failed.\n");
    }
    else {
        //qDebug() << tr("Data copied.\n");
        m_dataProcessed = false;
        if (!uploadData) {
            m_keepAcquisition = true;
        }

        emit finishAcquisition(m_data, uploadData);
    }
}

void AndorCCD::stopAcquisition()
{
    m_keepAcquisition = false;
}
