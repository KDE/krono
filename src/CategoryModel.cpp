/**
 * Copyright (C) 2011 Trever Fischer <tdfischer@fedoraproject.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include "CategoryModel.h"

CategoryModel::CategoryModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int CategoryModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_categories.size();
}

QVariant CategoryModel::data(const QModelIndex &idx, int role) const
{
    if (idx.isValid() && idx.row() >= 0 && idx.row() < m_categories.size() && idx.column() == 0) {
        Category row = m_categories[idx.row()];
        switch (role) {
            case Qt::DisplayRole:
                return row.description;
            case Qt::DecorationRole:
                return row.icon;
            case URIRole:
                return row.uri;
        }
    }
    return QVariant();
}

void CategoryModel::addCategory(const Category &cat)
{
    beginInsertRows(QModelIndex(), m_categories.size(), m_categories.size());
    m_categories << cat;
    endInsertRows();
}

#include "CategoryModel.moc"
