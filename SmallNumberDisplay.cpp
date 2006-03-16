#include "SmallNumberDisplay.h"
#include "Skin.h"

SmallNumberDisplay::SmallNumberDisplay (QWidget *parent, int w) : PixWidget (parent)
{
	m_w = w;
	setMinimumSize (w, 6);
	setMaximumSize (w, 6);
	m_pixmap = QPixmap (w, 6);
}

void
SmallNumberDisplay::setPixmaps (Skin *skin)
{
	drawNumber ();
}

void
SmallNumberDisplay::setNumber (int num, int len)
{
	snprintf (m_nums, len+1, "%02d", num);
	m_num = len;

	drawNumber ();

	update ();
}

void
SmallNumberDisplay::drawNumber ()
{
	Skin *skin = Skin::getInstance ();

	QPainter paint;
	paint.begin (&m_pixmap);
	paint.drawPixmap (m_pixmap.rect (),
					  skin->getItem (Skin::TEXTBG),
					  m_pixmap.rect ());

	for (int i = 0; i < m_num; i++) {
		paint.drawPixmap (QRect (i*5, 0, 4, 6),
						  skin->getLetter (m_nums[i]),
						  skin->getLetter (m_nums[i]).rect ());
	}

	paint.end ();
}

