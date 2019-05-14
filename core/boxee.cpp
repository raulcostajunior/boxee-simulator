#include "boxee.h"

#include <QString>

using namespace core;

Boxee &Boxee::instance()
{
    static Boxee m_instance;
    return m_instance;
}

void Boxee::setBootTimeSecs(uint8_t bootTime)
{
    // TODO: write body
}

void Boxee::setShutdownTimeSecs(uint8_t shutdownTime)
{
    // TODO: write body
}

void Boxee::setPassword(const QString &password)
{
    // TODO: write body
}

void Boxee::setHttpPort(uint16_t httpPort)
{
    // TODO: write body
}

void Boxee::setState(State state)
{
    // TODO: write body
}

void Boxee::setMediaType(MediaType mediaType)
{
    // TODO: write body
}

void Boxee::powerOn()
{
    // TODO: write body
}

void Boxee::powerOff()
{
    // TODO: write body
}
