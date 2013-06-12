/* === This file is part of Tomahawk Player - <http://tomahawk-player.org> ===
 *
 *   Copyright 2012, Christian Muehlhaeuser <muesli@tomahawk-player.org>
 *
 *   Tomahawk is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Tomahawk is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Tomahawk. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FLEXIBLETREEVIEW_H
#define FLEXIBLETREEVIEW_H

#include "ViewPage.h"
#include "PlaylistInterface.h"
#include "DllMacro.h"

class QStackedWidget;

class GridView;
class TrackView;
class ColumnView;
class TreeModel;
class PlaylistModel;
class FlexibleHeader;

class DLLEXPORT FlexibleTreeView : public QWidget, public Tomahawk::ViewPage
{
Q_OBJECT

public:
    enum FlexibleTreeViewMode
    { Columns = 0, Albums = 1, Flat = 2 };

    explicit FlexibleTreeView( QWidget* parent = 0, QWidget* extraHeader = 0 );
    ~FlexibleTreeView();

    virtual QWidget* widget() { return this; }
    virtual Tomahawk::playlistinterface_ptr playlistInterface() const;

    virtual QString title() const;
    virtual QString description() const;
    virtual QPixmap pixmap() const;

    virtual bool showInfoBar() const { return false; }
    virtual bool jumpToCurrentTrack();
    virtual bool isTemporaryPage() const;
    void setTemporaryPage( bool b );

    ColumnView* columnView() const { return m_columnView; }
    GridView* gridView() const { return m_gridView; }
    TrackView* trackView() const { return m_trackView; }

    void setGuid( const QString& guid );

    void setColumnView( ColumnView* view );
    void setGridView( GridView* view );
    void setTrackView( TrackView* view );

    void setTreeModel( TreeModel* model );

    void setPixmap( const QPixmap& pixmap );
    void setEmptyTip( const QString& tip );

public slots:
    void setCurrentMode( FlexibleTreeViewMode mode );
    virtual bool setFilter( const QString& pattern );

signals:
    void modeChanged( FlexibleTreeViewMode mode );
    void destroyed( QWidget* widget );

private slots:
    void onModelChanged();
    void onWidgetDestroyed( QWidget* widget );

private:
    FlexibleHeader* m_header;
    QPixmap m_pixmap;

    ColumnView* m_columnView;
    GridView* m_gridView;
    TrackView* m_trackView;

    TreeModel* m_model;
    QStackedWidget* m_stack;

    FlexibleTreeViewMode m_mode;
    bool m_temporary;
};

Q_DECLARE_METATYPE( FlexibleTreeView::FlexibleTreeViewMode );

#endif // FLEXIBLETREEVIEW_H
