#ifndef CONSTSET_HPP
#define CONSTSET_HPP

const QString SERVER_IP = "192.168.159.2";
const QString CLIENT_IP = "192.168.159.10";
const quint16 SERVER_PORT = 8995;
const quint16 CLIENT_PORT = 25000;
const double CENTER = 2060;
const double INCR = 0.0075;
const double RANGE = 30.72;
const double LSHIFT = INCR * CENTER;
const double RSHIFT = RANGE - LSHIFT;

const QString COMPORT = "COM8";
const int BAUDRATE = 115200;

#define DEG2RAD 0.01745329252;

#endif // CONSTSET_HPP
