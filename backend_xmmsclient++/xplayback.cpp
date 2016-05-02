/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2016 XMMS2 Team
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#include <xmmsclient/xmmsclient++.h>

#include "xplayback.h"
#include "xclient.h"

#include <xmmsclient/xmmsclient++/playback.h>
#include <xmmsclient/xmmsclient++/playlist.h>

#include <QtDebug>

#include <QStyle>

XPlayback::XPlayback (XClient *client)
{
	m_client = client;

	connect (client, SIGNAL (gotConnection (XClient *)),
	         this, SLOT (on_connect (XClient *)));

	if (client->isConnected ()) {
		on_connect (client);
	}
}

void
XPlayback::on_connect (XClient *client)
{
	client->playback ()->getStatus ()
	        (Xmms::bind (&XPlayback::playback_status, this));
	client->playback ()->broadcastStatus ()
	        (Xmms::bind (&XPlayback::playback_status, this));

	client->playback ()->volumeGet ()
			(Xmms::bind (&XPlayback::volume_changed, this));
	client->playback ()->broadcastVolumeChanged ()
	        (Xmms::bind (&XPlayback::volume_changed, this));
}

void
XPlayback::play ()
{
	if (!m_client->isConnected ()) return;

	m_client->playback ()->start ();
}

void
XPlayback::pause ()
{
	if (!m_client->isConnected ()) return;

	m_client->playback ()->pause ();
}

void
XPlayback::toggle_pause ()
{
	if (m_status == XMMS_PLAYBACK_STATUS_PLAY) {
		pause ();
	} else if (m_status == XMMS_PLAYBACK_STATUS_PAUSE) {
		play ();
	}

}

void
XPlayback::stop ()
{
	if (!m_client->isConnected ()) return;

	m_client->playback ()->stop ();
}

void
XPlayback::prev ()
{
	if (!m_client->isConnected ()) return;

	m_client->playlist ()->setNextRel (-1);
	m_client->playback ()->tickle ();
}

void
XPlayback::next ()
{
	if (!m_client->isConnected ()) return;

	m_client->playlist ()->setNextRel (1);
	m_client->playback ()->tickle ();
}

void
XPlayback::setPos (int pos)
{
	if (!m_client->isConnected ()) return;

	m_client->playlist ()->setNext (pos) ();
	m_client->playback ()->tickle () ();
}

/*
 * Seeking
 */
void
XPlayback::seekMs (uint milliseconds)
{
	if (!m_client->isConnected ()) return;

	m_client->playback ()->seekMs (milliseconds);
}

void
XPlayback::seekMsRel (int milliseconds)
{
	if (!m_client->isConnected ()) return;

	m_client->playback ()->seekMsRel (milliseconds);
}

/*
 * Status signals
 */
bool
XPlayback::playback_status (const Xmms::Playback::Status &status)
{
	m_status = status;

	emit playbackStatusChanged (status);
	return true;
}

/*
 * Volume
 */

bool
XPlayback::volume_changed (const Xmms::Dict &volDict)
{
	QHash<QString, QVariant> levels = XClient::convert_dict (volDict);

	VolumeMap volumes;
	QStringList keys = levels.keys();
	foreach (QString key, keys) {
		volumes.insert (key, levels.value (key).toInt());
	}
	emit volumeChanged (volumes);
}


void
XPlayback::setVolume (QString key, int volume)
{
	m_client->playback ()->volumeSet (key.toStdString(), volume);
}

#include "xplayback.moc"
