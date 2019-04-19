#ifndef FONTDATABASE_HPP
#define FONTDATABASE_HPP

#include <vector>

#include "QSqlDatabase"
#include "QSqlQuery"
#include "QAbstractItemModel"

using namespace std;

class FontDatabase : public QAbstractItemModel
{
    Q_OBJECT
    
    Q_PROPERTY(int fontCount READ fontCount)
    
public:
    
    enum FontRoles
    {
        IdRole = Qt::UserRole + 1,
        FamilyRole
    };
    
    explicit FontDatabase(QObject *parent = nullptr);
    
    ~FontDatabase() override;
    
    int fontCount() const;
    
    Q_INVOKABLE QString getFontFamily(int);
    
    Q_INVOKABLE QString getFontFilePath(int);
    
    Q_INVOKABLE QString getFontFileName(int);
    
    Q_INVOKABLE QString getFontStyle(int);
    
    Q_INVOKABLE void font_asc_sort();
    
    Q_INVOKABLE void font_desc_sort();
    
    Q_INVOKABLE int getCurrentFontIndex(int);
    
    Q_INVOKABLE QString getCurrentFontFamily(int);
    
    //pure virtual functions
    Q_INVOKABLE int rowCount(const QModelIndex & = QModelIndex()) const override;
    
    Q_INVOKABLE int columnCount(const QModelIndex & = QModelIndex()) const override;
    
    Q_INVOKABLE QVariant data(const QModelIndex &, int = Qt::DisplayRole) const override;
    
    QModelIndex index(int, int, const QModelIndex & = QModelIndex()) const override;
    
    QModelIndex parent(const QModelIndex &) const override;
    
protected:
    
    QHash<int, QByteArray> roleNames() const override;
    
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
    
    QList<QString> m_fontFamilyList;
    
    QList<int> m_fontIndex;
    
    bool m_debug;
    
    QString m_program_name;
};

#endif // FONTDATABASE_HPP
