#pragma once

#include "chipsolver_global.h"
#include <QThread>
#include <QList>
#include <QMap>
#include <QString>
#include <QJsonObject>
#include "Chip/Chip.h"

// һ����н�
struct CHIPSOLVER_EXPORT Solution
{
	// ʹ�õ�оƬ����
	QList<ChipPuzzleOption> chips;
	// �õ�����װ������
	GFChip totalValue;
};

class CHIPSOLVER_EXPORT ChipSolver : public QThread
{
public:
	explicit ChipSolver(QObject* parent = nullptr);
	~ChipSolver() = default;

	// һ����װ�����н�
	typedef QList<Solution> SquadSolution;

	SquadSolution solutions;
	
	// ��װ�б�
	QStringList squads() const;
	// ����װ�ķ����б�ͬʱ�����óɵ�ǰ��װ
	QStringList configs(const QString& squad);

	struct TargetBlock
	{
		// �˺�����
		int damageBlock;
		// װ�����
		int reloadBlock;
		// ���и���
		int hitBlock;
		// �Ʒ�����
		int defbreakBlock;
		// ƫ�����
		int error;
		// ��󷽰�����
		int upper;

		explicit TargetBlock(int dmg = 0,int r = 0,int hit = 0,int dbk = 0,int e = 0,int u = 1e9):
			damageBlock(dmg), reloadBlock(r), hitBlock(hit),defbreakBlock(dbk),error(e),upper(u)
		{}
	};

	// ��һ������תΪ����ʾ��ͼ����Ϣ
	ChipViewInfo solution2ChipView(const Solution& solution);

public slots:
	// ����Ŀ�����
	void setTargetBlock(const TargetBlock& block);
	// ����Ŀ������÷���
	void setTargetConfig(const QString& name);

signals:
	void solvePercentChanged(int percent);
	
protected:
	void run() override;

private:
	// һ������
	typedef QList<ChipPuzzleOption> Config;
	// һ����װ����������
	struct SquadConfig
	{
		QList<Config> configs;
		// �ܸ���
		int blocks = 38;
		// �ɿո���
		int optional = 0;
		// �������
		GFChip maxValue;
	};
	// �������÷�����������Ϊkey
	QMap<QString, SquadConfig> configs_;
	// ��װ�����÷�����¼��Ϣ
	QJsonObject configInfo_;
	// ��װ�Ļ�������
	QMap<QString, ChipViewInfo> squadView_;
	// Ŀ����װ
	QString targetSquadName_;
	// Ŀ�귽��
	QString targetConfigName_;
	// Ŀ�����
	TargetBlock targetBlock_;
};
