#pragma once

#include <QLabel>
#include <QPropertyAnimation>

class RootContatItem  : public QLabel
{
	Q_OBJECT

	//箭头角度
	//增加一个 动态属性
	Q_PROPERTY(int rotation READ rotation WRITE setRotation)

public:
	RootContatItem(bool hasArrow = true, QWidget *parent = nullptr);
	~RootContatItem();

public:
	void setText(const QString& title);
	void setExpanded(bool expand);

private:
	int rotation();
	void setRotation(int rotation);

protected:
	void paintEvent(QPaintEvent* event);

private:
	QPropertyAnimation *m_animation;//箭头旋转的时候，需要使用动画类
	QString m_titleText;			//显示的文本
	int m_rotation;					//箭头的角度
	bool m_hasArrow;				//是否有箭头
};
