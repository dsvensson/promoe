/**
 *	This file is a part of Promoe, an XMMS2 Client
 *
 *	Copyright (C) 2005-2016 XMMS2 Team
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version 2
 *	of the License, or (at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 */

#include <xmmsclient/xmmsclient++.h>

#include "xconfig.h"
#include "xclient.h"

#include <QList>
#include <QVariant>

XConfig::XConfig (XClient *parent) : QObject (parent)
{
	m_ready = false;

	connect (parent, SIGNAL (gotConnection (XClient *)),
	         this, SLOT (on_connect (XClient *)));

	connect (parent, SIGNAL (disconnected (XClient *)),
	         this, SLOT (on_disconnect (XClient *)));

	if (parent->isConnected ()) {
		on_connect (parent);
	}
}

QString
XConfig::value_get (QString key)
{
	/* if XConfig is ready, the local cache should be in sync with the
	 * serverside configuration, otherwise the cache is empty */
	if (!m_ready) {
		return QString ();
	}
	return m_config_cache.value (key);
}

QStringList
XConfig::values_get (QRegExp key)
{
	QStringList ret;
	QHash < QString, QString >::ConstIterator it = m_config_cache.constBegin ();
	while (it != m_config_cache.constEnd ()) {
		if (key.exactMatch (it.key ())) {
			ret.append (it.value ());
		}
		++it;
	}
	return ret;
}

bool
XConfig::value_set (QString key, QString val)
{
	/* Only send change request to server from here
	 * update of local cache will be done through handle_config_value_changed
	 */
	if (!m_client->isConnected ()) {
		return false;
	}
	// Only send change, if the value really changed;
	if (val == value_get (key))
		return true;

	m_client->config ()->valueSet (key.toStdString (), val.toStdString ());

	return true;
}

bool
XConfig::value_register (QString key, QString defval)
{
	if (!m_client->isConnected ()) {
		return false;
	}
	m_client->config ()->valueRegister (key.toStdString (),
	                                    defval.toStdString ());

	return true;
}

void
XConfig::on_connect (XClient *client)
{
	client->config ()->valueList ()
	        (Xmms::bind (&XConfig::handle_config_value, this));

	client->config ()->broadcastValueChanged ()
	        (Xmms::bind (&XConfig::handle_config_value_changed, this));

	m_client = client;
}

void
XConfig::on_disconnect (XClient *client)
{
	/* We don't emit any signals here, as every class must be able to
	 * react on the configChanged signal, which will be fired for every
	 * configvalue on reonnect
	 */
	m_ready = false;
	m_config_cache.clear ();
}

bool
XConfig::handle_config_value (const Xmms::Dict &value)
{
	// FIXME: I should rework configuration handling, and perhaps I should
	// abolish the call to handle_config_value_changed here.
	bool ok = handle_config_value_changed (value);
	if (ok) {
		m_ready = true;
	}
	emit configLoaded ();
	return ok;
}

bool
XConfig::handle_config_value_changed (const Xmms::Dict &value)
{
	QHash <QString, QVariant> tmp = XClient::convert_dict(value); 

	QHash<QString, QVariant>::const_iterator i = tmp.constBegin ();
	while (i != tmp.constEnd ())
	{
		m_config_cache[i.key ()] = i.value ().toString ();
		emit configChanged (i.key (), i.value ().toString ());
		i++;
	}

	// TODO
	return true;
}

#include "xconfig.moc"
