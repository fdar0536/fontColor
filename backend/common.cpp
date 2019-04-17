#include "common.hpp"

Common::Common(QObject *parent) :
    QObject(parent),
    m_debug(false),
    m_program_name("")
{
    QStringList args = QApplication::arguments();
    if (args.count() > 1) //args.at(0) is program's file name
    {
        //arguments have been filterd in the main function
        m_debug = true;
        m_program_name = args.at(0);
    }
}
