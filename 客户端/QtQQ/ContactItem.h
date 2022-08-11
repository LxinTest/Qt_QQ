#pragma once

#include <QWidget>
#include "ui_ContactItem.h"

class ContactItem : public QWidget
{
	Q_OBJECT

public:
	ContactItem(QWidget *parent = nullptr);
	~ContactItem();

	void setUserName(const QString& userName);
	void setSignName(const QString& signName);
	void setHeadPixmap(const QPixmap& headPath);
	QString getUserNaame() const;
	QSize getHeadLabelSize() const;

private:
	void initControl();

private:
	Ui::ContactItem ui;
};
