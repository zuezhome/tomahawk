/* === This file is part of Tomahawk Player - <http://tomahawk-player.org> ===
 * 
 *   Copyright 2010-2011, Christian Muehlhaeuser <muesli@tomahawk-player.org>
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

#include "remoteioconnection.h"
#include <QFile>

RemoteIOConnection::RemoteIOConnection(Servent * s, FileTransferSession * fts)
    : Connection(s), m_fts(fts)
{
    qDebug() << "CTOR " << id() ;
}

RemoteIOConnection::~RemoteIOConnection()
{
    qDebug() << "DTOR " << id() ;
}

QString RemoteIOConnection::id() const
{
    return QString("RemoteIOConnection[%1]").arg(m_fts->fid());
}

void RemoteIOConnection::shutdown(bool wait)
{
    Connection::shutdown(wait);
    /*if(!wait)
    {
        Connection::shutdown(wait);
        return;
    }
    qDebug() << id() << " shutdown requested - waiting until we've received all data TODO";
    */
}

void RemoteIOConnection::setup()
{
    if(m_fts->type() == FileTransferSession::RECEIVING)
    {
        qDebug() << "RemoteIOConnection in RX mode";
        return;
    }

    qDebug() << "RemoteIOConnection in TX mode, fid:" << m_fts->fid();

    QString url("/tmp/test.mp3");
    qDebug() << "TODO map fid to file://, hardcoded for now";

    qDebug() << "Opening for transmission:" << url;
    m_readdev = QSharedPointer<QFile>(new QFile(url));
    m_readdev->open(QIODevice::ReadOnly);
    if(!m_readdev->isOpen())
    {
        qDebug() << "WARNING file is not readable: " << url;
        shutdown();
    }
    // send chunks within our event loop, since we're not in our own thread
    sendSome();
}

void RemoteIOConnection::handleMsg(QByteArray msg)
{
    Q_ASSERT(m_fts->type() == FileTransferSession::RECEIVING);
    m_fts->iodevice()->addData(msg);
    if(msg.length()==0) qDebug() << "Got 0len msg. end?";
}


Connection * RemoteIOConnection::clone()
{
    Q_ASSERT(false); return 0;
};


void RemoteIOConnection::sendSome()
{
    Q_ASSERT(m_fts->type() == FileTransferSession::SENDING);
    if(m_readdev->atEnd())
    {
        qDebug() << "Sent all. DONE";
        shutdown(true);
        return;
    }
    QByteArray ba = m_readdev->read(4096);
    //qDebug() << "Sending " << ba.length() << " bytes of audiofile";
    sendMsg(ba);
    QTimer::singleShot(0, this, SLOT(sendSome()));
}


