#include "boxee.h"

using namespace core;

Boxee &Boxee::instance()
{
    static Boxee m_instance;
    return m_instance;
}
