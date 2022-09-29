#include "pathpicker.h"
#include <QHBoxLayout>
#include <QFileDialog>

PathPicker::PathPicker(const QString& title, QWidget* parent)
	: QWidget(parent) {
	m_title = new QLabel();
	m_title->setText(QString("%1:").arg(title));
	m_title->setFixedWidth(45);
	m_init();
}

PathPicker::PathPicker(QWidget* parent) 
	: QWidget(parent) {
	m_init();
}

void PathPicker::m_init() {
	m_path = new QLineEdit();
	m_browse_button = new QPushButton();
	m_browse_button->setText(tr("&Browse"));

	connect(m_browse_button, &QPushButton::released, this, [&] {
		auto path = QFileDialog::getOpenFileName(this, tr("Select image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
		m_path->setText(path);
	});

	this->setLayout(new QHBoxLayout);

	if (m_title)
		this->layout()->addWidget(m_title);

	this->layout()->addWidget(m_path);
	this->layout()->addWidget(m_browse_button);
}

PathPicker::~PathPicker() {

}