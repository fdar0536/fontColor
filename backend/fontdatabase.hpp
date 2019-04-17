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
    
    //Ascending order
    Q_INVOKABLE bool font_asc_sort_init();
    
    Q_INVOKABLE int font_asc_sort_getID();
    
    Q_INVOKABLE QString font_asc_sort_getFamily();
    
    Q_INVOKABLE bool font_asc_sort_next();
    
    //Descending order
    Q_INVOKABLE bool font_desc_sort_init();
    
    Q_INVOKABLE int font_desc_sort_getID();
    
    Q_INVOKABLE QString font_desc_sort_getFamily();
    
    Q_INVOKABLE bool font_desc_sort_next();
    
private:
    
    int m_fontCount;
    
    QSqlQuery m_query;
    
    QSqlQuery m_query_asc;
    
    QSqlQuery m_query_desc;
    
    QSqlDatabase m_db;
    
    int exec_db_int(QSqlQuery &, QString &);
    
    bool exec_db_string(QSqlQuery &, QString &);
    
    bool exec_db_string(QSqlQuery &);
    
    int main_process();
    
    int font_sort_getID(QSqlQuery &);
    
    QString font_sort_getFamily(QSqlQuery &);
    
    bool font_sort_next(QSqlQuery &);
};

#endif // FONTDATABASE_HPP
