#include "pathpicker.h"
#include <QHBoxLayout>

PathPicker::PathPicker(QWidget* parent) 
	: QWidget(parent) {

	m_path = new QLineEdit();
	m_browse_button = new QPushButton();
	m_browse_button->setText("&Browse");

	this->setLayout(new QHBoxLayout);
	this->layout()->addWidget(m_path);
	this->layout()->addWidget(m_browse_button);
}

PathPicker::~PathPicker() {

}