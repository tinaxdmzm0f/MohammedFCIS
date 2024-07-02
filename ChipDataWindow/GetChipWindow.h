#pragma once

#include "chipdatawindow_global.h"
#include <QDialog>

namespace Ui { class GetChipWindow; };
class ChipDataWindow;

class CHIPDATAWINDOW_EXPORT GetChipWindow : public QDialog
{
	Q_OBJECT

public:
	GetChipWindow(QWidget *parent = Q_NULLPTR);
	~GetChipWindow();

protected slots:
	// ���ñ��ش�����ʾ��ʾ�������������
	void setLocalProxy();
	// �������������ʾ��ʾ
	void setNetProxy();
	// ��ȡ���ݣ��첽
	void getData();

private:
	void connect();

	Ui::GetChipWindow *ui;
};
