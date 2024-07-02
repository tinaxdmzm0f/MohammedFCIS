#include "stdafx.h"
#include "GetChipWindow.h"
#include "ui_GetChipWindow.h"

GetChipWindow::GetChipWindow(QWidget *parent)
	: QDialog(parent),
	ui(new Ui::GetChipWindow)
{
	ui->setupUi(this);
	connect();
	this->ui->netProxyRadioButton->setChecked(true);
}

GetChipWindow::~GetChipWindow()
{
	delete ui;
}

void GetChipWindow::setLocalProxy()
{
	this->ui->noteLabel->setText(trUtf8(
		u8"˵�������ֻ������������ͬ��WiFi��������WiFi�ĸ߼����ã�ѡ���ֶ���������ַ���£�"));
	this->ui->proxyAddressLineEdit->setText(trUtf8(u8"��ַ:") + "192.168.3.1" + trUtf8(u8" �˿�:") + "8081");
}

void GetChipWindow::setNetProxy()
{
	this->ui->noteLabel->setText(trUtf8(
		u8"˵�������ֻ���������WiFi��������WiFi�ĸ߼����ã�����ѡ���Զ������������ַΪ��"));
	this->ui->proxyAddressLineEdit->setText("http://static.xuanxuan.tech/GF/GF.pac");
}

void GetChipWindow::getData()
{
}

void GetChipWindow::connect()
{
	QObject::connect(
		this->ui->getDataPushButton,
		&QPushButton::clicked,
		this,
		&GetChipWindow::getData
	);
	QObject::connect(
		this->ui->localProxyRadioButton,
		&QRadioButton::toggled,
		this,
		&GetChipWindow::setLocalProxy
	);
	QObject::connect(
		this->ui->netProxyRadioButton,
		&QRadioButton::toggled,
		this,
		&GetChipWindow::setNetProxy
	);
}
