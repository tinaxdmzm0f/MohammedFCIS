#pragma once

#include "chipdatawindow_global.h"
#include <QDialog>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QProcess>

namespace Ui { class GetChipWindow; };
class ChipDataWindow;

class CHIPDATAWINDOW_EXPORT GetChipWindow : public QDialog
{
	Q_OBJECT

public:
	GetChipWindow(QWidget *parent = Q_NULLPTR);
	~GetChipWindow();

	void init();

signals:
	void sendChipJsonObject(const QJsonObject& object);

protected slots:
	// ���ñ��ش�����ʾ��ʾ�������������
	void setLocalProxy();
	// �������������ʾ��ʾ
	void setNetProxy();
	// ��ȡ���ݣ��첽
	void getData();

	// �ӷ��������ܵ�����
	void recvData(QNetworkReply* reply);

	// ���̴߳���
	void processError(QProcess::ProcessError error);
	// ���߳����ݿɶ�
	void processDataReady();
	// �������ش���
	void startLocalProxy();

	void closeEvent(QCloseEvent*) override;

private:
	void connect();
	// �����ӷ��������߱���������ж�ȡ��оƬ��Ϣ
	bool parseChipData(const QByteArray& data);

	// ɱ���Ѿ����ڵĽ���
	void killProcess();

	Ui::GetChipWindow *ui;
	QNetworkRequest* request_;
	QNetworkAccessManager* accessManager_;
	QProcess* process_;
	QString localProxyAddr_, localProxyPort_;
};
