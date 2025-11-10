/*
 * Font Color
 * Copyright (c) 2019-present fdar0536
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef FONTDATABASE_HPP
#define FONTDATABASE_HPP

#include <vector>

#include "QAbstractItemModel"

#include "sqlhandler.hpp"

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

signals:

    void sortingDone();

public slots:

    void handle_sorting_done(QList<QString> &, QList<int> &);

protected:

    QHash<int, QByteArray> roleNames() const override;

private:

    int m_fontCount;

    SQLHandler *m_sql;

    void init();

    QString getString(const std::string &in);

    int main_process();

    QList<QString> m_fontFamilyList;

    QList<int> m_fontIndex;

    bool m_debug;

    QString m_program_name;
};

#endif // FONTDATABASE_HPP
