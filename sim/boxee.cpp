#include "boxee.h"

using namespace sim;

Boxee &Boxee::instance()
{
    static Boxee m_instance;
    return m_instance;
}
