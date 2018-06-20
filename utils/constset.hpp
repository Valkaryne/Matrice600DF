#ifndef CONSTSET_HPP
#define CONSTSET_HPP

#include <QtMath>
#include <QVector>

const QString SERVER_IP = "192.168.1.37";
const QString CLIENT_IP = "192.168.1.34";
const quint16 SERVER_PORT = 8995;
const quint16 CLIENT_PORT = 25000;

const QString DATALINK = "COM6";
const QString SILICON = "COM5";
const QString COMPORT = SILICON;
const int BAUDRATE = 115200;

const double CENTER = 2060;
const double INCR = 0.0075;
const double RANGE = 30.72;
const double LSHIFT = INCR * CENTER;
const double RSHIFT = RANGE - LSHIFT;

const double CALIBRATION = 170.0;
const double PH2 = 140.0;
const double PH5 = 150.0;

#define DEG2RAD 0.01745329252;

#endif // CONSTSET_HPP
