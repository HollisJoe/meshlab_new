#include "editpaint.h"
#include <QPainter>
#include <QColorDialog>


Color4b to4b(QColor c) {
	return Color4b(c.red(),c.green(),c.blue(),100);
}

void ColorWid::setColor(QColor co) {
	bg=co;
}

void ColorWid::paintEvent(QPaintEvent * event ) {
	QPainter painter(this);
	painter.fillRect(QRect(0,0,width(),height()),bg);
	int col=bg.red()+bg.green()+bg.blue();
	if (col<150) painter.setPen(Qt::white);
	else painter.setPen(Qt::black);
	painter.drawRect(QRect(1,1,width()-3,height()-3));
}

void ColorWid::mousePressEvent ( QMouseEvent * event ) {
	QColor temp=QColorDialog::getColor(bg);
	if (temp.isValid()) bg=temp;
	update();
}

PaintToolbox::PaintToolbox(/*const QString & title,*/ QWidget * parent, Qt::WindowFlags flags) : QWidget(parent,flags | Qt::WindowStaysOnTopHint) {
	ui.setupUi(this);
	ui.front->setColor(Qt::black);
	ui.back->setColor(Qt::white);
	ui.front->setGeometry(10,10,40,35);
	ui.back->setGeometry(30,25,40,35);
	ui.switch_me->setGeometry(55,10,15,15);
	ui.set_bw->setGeometry(10,45,15,15);
	ui.fill_button->setEnabled(false);
	
}

Color4b PaintToolbox::getColor(Qt::MouseButton mouse) {
	switch (mouse) {
		case Qt::LeftButton: return to4b(ui.front->getColor());
		case Qt::RightButton: return to4b(ui.back->getColor());
		default: return to4b(ui.front->getColor());
	}
}

void PaintToolbox::on_pen_type_currentIndexChanged(QString value) {
	qDebug() << "textchange" << endl;
	on_pen_radius_valueChanged( ui.pen_radius->value());
}
void PaintToolbox::on_pen_radius_valueChanged(double value) {
	static double oldval=-1;
	if (ui.pen_type->currentText()=="pixel") {
		if ((double)((int)value)!=value) {
			if (oldval<value)
			ui.pen_radius->setValue((double)((int)value)+1);
			else ui.pen_radius->setValue((double)((int)value));
		}
	} else {
	}
	oldval=ui.pen_radius->value();
}
void PaintToolbox::on_switch_me_clicked() {
	QColor temp=ui.front->getColor();
	ui.front->setColor(ui.back->getColor());
	ui.back->setColor(temp);
	ui.front->update();
	ui.back->update();
}
void PaintToolbox::on_set_bw_clicked() {
	ui.front->setColor(Qt::black);
	ui.back->setColor(Qt::white);
	ui.front->update();
	ui.back->update();
}

void PaintToolbox::on_deck_slider_valueChanged(int value) {
	if (value!=ui.deck_box->value()) ui.deck_box->setValue(value);
}

void PaintToolbox::on_deck_box_valueChanged(int value) {
	if (value!=ui.deck_slider->value()) ui.deck_box->setValue((int)value);
}
