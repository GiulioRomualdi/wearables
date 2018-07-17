/*
 * Copyright (C) 2018 Istituto Italiano di Tecnologia (IIT)
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * GNU Lesser General Public License v2.1 or any later version.
 */

#include "XSensMVNDriver.h"
#include "XSensMVNDriverImpl.h"

using namespace xsensmvn;

/* -------------------------- *
 *  Construtors / Destructors *
 * -------------------------- */

XSensMVNDriver::XSensMVNDriver(const DriverConfiguration conf)
    : m_dataSample(new DriverDataSample)
    , m_dataMutex(new std::mutex)
    , m_pimpl(nullptr)
{
    m_pimpl.reset(new XSensMVNDriverImpl(conf, m_dataSample, m_dataMutex));
};

XSensMVNDriver::~XSensMVNDriver()
{
    m_pimpl.reset(nullptr);
}

/* ---------- *
 *  Functions *
 * ---------- */

bool XSensMVNDriver::configureAndConnect()
{
    return m_pimpl->configureAndConnect();
}

bool XSensMVNDriver::terminate()
{
    return m_pimpl->cleanAndClose();
}

bool XSensMVNDriver::setMinimumAcceptableCalibrationQuality(
    const xsensmvn::CalibrationQuality quality) const
{
    return m_pimpl->m_calibrator->setMinimumAcceptableCalibrationQuality(quality);
}

const xsensmvn::CalibrationQuality& XSensMVNDriver::getMinimumAcceptableCalibrationQuality() const
{
    return m_pimpl->m_calibrator->getMinimumAcceptableCalibrationQuality();
}

// void XSensMVNDriver::cacheData()
//{
//    m_dataSample = std::move(*m_pimpl->m_lastProcessedDataSample);
//    xsInfo << "data moved from private to public";
//}

const DriverDataSample& XSensMVNDriver::getDataSample()
{
    //    cacheData();
    std::lock_guard<std::mutex> readLock(*m_dataMutex);
    return *m_dataSample;
}

const LinkDataVector& XSensMVNDriver::getLinkDataSample()
{
    std::lock_guard<std::mutex> readLock(*m_dataMutex);
    return m_dataSample->links;
}

const SensorDataVector& XSensMVNDriver::getSensorDataSample()
{
    std::lock_guard<std::mutex> readLock(*m_dataMutex);
    return m_dataSample->sensors;
}

const JointDataVector& XSensMVNDriver::getJointDataSample()
{
    std::lock_guard<std::mutex> readLock(*m_dataMutex);
    return m_dataSample->joints;
}

// double XSensMVNDriver::getSampleAbsoluteTime() const
//{
//    return m_dataSample.absoluteTime;
//}

// double XSensMVNDriver::getSampleRelativeTime() const
//{
//    return m_dataSample.relativeTime;
//}

std::vector<std::string> XSensMVNDriver::getSuitLinkLabels() const
{
    return m_pimpl->getSuitLinkLabels();
}

std::vector<std::string> XSensMVNDriver::getSuitSensorLabels() const
{
    return m_pimpl->getSuitSensorLabels();
}

std::vector<std::string> XSensMVNDriver::getSuitJointLabels() const
{
    return m_pimpl->getSuitJointLabels();
}

DriverStatus XSensMVNDriver::getStatus() const
{
    return m_pimpl->getDriverStatus();
}

/* ------------------------------------------ *
 *  IXsensMVNControl Interface Implementation *
 * ------------------------------------------ */

bool XSensMVNDriver::startAcquisition()
{
    return m_pimpl->startAcquisition();
}

bool XSensMVNDriver::stopAcquisition()
{
    return m_pimpl->stopAcquisition();
}

bool XSensMVNDriver::calibrate(const std::string& calibrationType)
{
    // No need to check if calibrationType is empty, the check is inside the implementation
    return m_pimpl->calibrate(calibrationType);
}

bool XSensMVNDriver::abortCalibration()
{
    return m_pimpl->m_calibrator->abortCalibration();
}

bool XSensMVNDriver::setBodyDimensions(const std::map<std::string, double>& bodyDimensions)
{
    return m_pimpl->m_calibrator->setBodyDimensions(bodyDimensions);
}

bool XSensMVNDriver::getBodyDimensions(std::map<std::string, double>& dimensions) const
{
    return m_pimpl->m_calibrator->getBodyDimensions(dimensions);
}

bool XSensMVNDriver::getBodyDimension(const std::string bodyName, double& dimension) const
{
    return m_pimpl->m_calibrator->getBodyDimension(bodyName, dimension);
}