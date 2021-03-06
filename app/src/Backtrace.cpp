#include "msgui/Backtrace.h"
#include "msgui/Frame.h"
#include "msgwidget/highlighter/HL_CPP.h"

#include <mredit/Label.h>

#include <QBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QFileInfo>

#define FILENAME_ROLE 9552

namespace msgui {

Backtrace::Backtrace(QWidget *parent) :
	QTreeWidget(parent), _backtrace()
{
	setColumnCount(3);
	setHeaderLabels(QStringList() << "#" << "Name" << "Kind" << "Source");
	header()->resizeSection(0, 10);
	header()->resizeSection(1, 330);
	header()->resizeSection(2, 280);
	header()->resizeSection(3, 170);

	connect(this, &QTreeWidget::itemClicked, this, &Backtrace::btItemClicked);
	connect(header(), &QHeaderView::sectionResized, this, &Backtrace::columnResized);
}

void Backtrace::setBacktrace(msglib::cmd::backtrace::ptr backtrace)
{
	if (backtrace)
	{
		clear();
		int ct = 0;
		for (auto &b : backtrace->frames) {
			QTreeWidgetItem *item = new QTreeWidgetItem(this);

			item->setData(3, FILENAME_ROLE, b->source_location);

			item->setText(0, QString("#%1").arg(ct));
			//item->setTextAlignment(0, Qt::AlignCenter);
			ct++;

			mredit::Label *namelbl = new mredit::Label(this);
			namelbl->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
			new msgwidget::highlighter::HL_CPP(namelbl->document());
			namelbl->setPlainText(b->name);
			setItemWidget(item, 1, namelbl);

			item->setText(2, b->kind);
			QFileInfo fi(b->source_location);
			item->setText(3, fi.fileName());
		}
	}
	_backtrace = backtrace;
}

void Backtrace::btItemClicked(QTreeWidgetItem *item, int column)
{
	if (item)
	{
		emit showFileAndLine(item->data(3, FILENAME_ROLE).toString());
	}
}

void Backtrace::columnResized(int logicalIndex, int oldSize, int newSize)
{
	scheduleDelayedItemsLayout();
}

}