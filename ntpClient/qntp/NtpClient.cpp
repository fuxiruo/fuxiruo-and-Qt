/* This file is part of QNtp, a library that implements NTP protocol.
 *
 * Copyright (C) 2011 Alexander Fokin <apfokin@gmail.com>
 *
 * QNtp is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * QNtp is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with QNtp. If not, see <http://www.gnu.org/licenses/>. */
#include "NtpClient.h"
#include <QHostAddress>
#include "NtpPacket.h"
#include "NtpReply.h"
#include "NtpReply_p.h"

NtpClient::NtpClient(QObject *parent): QObject(parent) {
    connect(&mSocket, SIGNAL(readyRead()), this, SLOT(OnReadyRead()));
}

NtpClient::NtpClient(const QString &host, quint16 bindPort, QObject *parent): QObject(parent) {
    init(host, bindPort);
}

void NtpClient::init(const QString &host, quint16 bindPort) {
    if(QAbstractSocket::ConnectedState == mSocket.state()){
        mSocket.close();
    }
    mSocket.connectToHost(host, bindPort);
}

QAbstractSocket::SocketState NtpClient::state()
{
    return mSocket.state();
}

NtpClient::~NtpClient() {

}

bool NtpClient::sendRequest() {
    if(mSocket.state() != QAbstractSocket::ConnectedState)
        return false;

    /* Initialize the NTP packet. */
    NtpPacket packet;
    memset(&packet, 0, sizeof(packet));
    packet.flags.mode = ClientMode;
    packet.flags.versionNumber = 4;
    packet.transmitTimestamp = NtpTimestamp::fromDateTime(QDateTime::currentDateTimeUtc());

    /* Send it. */
    if(mSocket.write(reinterpret_cast<const char *>(&packet), sizeof(packet)) < 0)
        return false;

    return true;
}

void NtpClient::OnReadyRead() {
    while (mSocket.bytesAvailable()) {
        NtpFullPacket packet;
        memset(&packet, 0, sizeof(packet));

        if(mSocket.read(reinterpret_cast<char *>(&packet), sizeof(packet)) < sizeof(NtpPacket))
            continue;

        QDateTime now = QDateTime::currentDateTime();

        /* Prepare reply. */
        NtpReplyPrivate *replyPrivate = new NtpReplyPrivate();
        replyPrivate->packet = packet;
        replyPrivate->destinationTime = now;
        NtpReply reply(replyPrivate);

        /* Notify. */
        Q_EMIT replyReceived(mSocket.peerAddress(), mSocket.peerPort(), reply);
    }
}

