/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2016 XMMS2 Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#ifndef __EQUALIZERWIDGET_H__
#define __EQUALIZERWIDGET_H__

#include <QWidget>
class QString;
class QVariant;
class QPixmap;
class QPaintEvent;
class QMouseEvent;

#include "pixmapslider.h"
class XConfig;
class Skin;
class PixmapButton;

//#include <iostream>

class EqualizerSlider : public PixmapSlider
{
	Q_OBJECT

	public:
		EqualizerSlider (QWidget*, int);

	signals:
		void numberedValueChanged (int value, int id);

	protected slots:
		void on_self_value_changed (int value);

	private:
		int m_id;
};

class EqualizerWidget : public QWidget
{
	Q_OBJECT

	public:
		EqualizerWidget(QWidget *parent);
		~EqualizerWidget();

		void setActive (bool);
		void paintEvent (QPaintEvent *event);
		void mouseDoubleClickEvent (QMouseEvent *event);

	public slots:
		void setPixmaps(Skin *skin);

	protected slots:
		void loadServerConfig ();
		void serverConfigValueChanged (QString key, QString value);
		void setEqualizerEnabled (bool enabled);
		void updateServerPreamp (int value);
		void updateServerBands (int value, int id);

	private:
		bool haveEqualizerEffect();

		bool m_active;
		XConfig *m_xconfig;
		QPixmap m_pixmap;
		QPixmap m_title;
		QPixmap m_graph;
		PixmapButton *m_closebtn;
		PixmapButton *m_shadebtn;
		PixmapButton *m_enable;
		PixmapButton *m_auto;
		PixmapButton *m_preset;
		PixmapSlider *m_preamp;
		EqualizerSlider *m_bands[10];
};


#endif
