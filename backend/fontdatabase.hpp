#ifndef FONTDATABASE_HPP
#define FONTDATABASE_HPP

#include <vector>

#include "QSqlDatabase"
#include "QSqlQuery"

#include "common.hpp"

using namespace std;

class FontDatabase : public Common
{
    Q_OBJECT
    
    Q_PROPERTY(int fontCount READ fontCount)
    
public:
    
    explicit FontDatabase(QObject *parent = nullptr);
    
    ~FontDatabase();
    
    int fontCount() const;
    
    Q_INVOKABLE QString getFontFamily(int);
    
    Q_INVOKABLE QString getFontFilePath(int);
    
    Q_INVOKABLE QString getFontFileName(int);
    
    Q_INVOKABLE QString getFontStyle(int);
    
private:
    
    int m_fontCount;
    
    QSqlQuery m_query;
    
    QSqlDatabase m_db;
    
    int exec_db_int(QSqlQuery &, QString &);
    
    bool exec_db_string(QSqlQuery &, QString &);
    
    bool exec_db_string(QSqlQuery &);
    
    int main_process();
};

#endif // FONTDATABASE_HPP
