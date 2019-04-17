#ifndef COMMON_HPP
#define COMMON_HPP

#include "QApplication"

class Common : public QObject
{
    Q_OBJECT
    
public:
    
    Common(QObject *parent = nullptr);
    
protected:
    
    bool m_debug;
    
    QString m_program_name;
};

#endif // COMMON_HPP
