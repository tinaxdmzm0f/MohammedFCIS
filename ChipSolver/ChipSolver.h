#pragma once

#include "chipsolver_global.h"
#include <QThread>
#include <QString>
#include <map>
#include <set>
#include <vector>
#include "Chip/Chip.h"

struct CHIPSOLVER_EXPORT TargetBlock
{
	// �˺�����
	int damageBlock;
	// �Ʒ�����
	int defbreakBlock;
	// ���ȸ���
	int hitBlock;
	// װ�����
	int reloadBlock;
	// ƫ�����
	int error;
	// ��󷽰�����
	int upper;

	explicit TargetBlock(int dmg = 0, int dbk = 0, int hit = 0, int r = 0,  int e = 0, int u = 1e9) :
		damageBlock(dmg), defbreakBlock(dbk), hitBlock(hit), reloadBlock(r), error(e), upper(u)
	{}
};

// һ����װ�����н�
typedef std::vector<Solution> SquadSolution;

class CHIPSOLVER_EXPORT ChipSolver : public QThread
{
	Q_OBJECT
public:
	explicit ChipSolver(QObject* parent = nullptr);
	~ChipSolver() = default;

	SquadSolution solutions;
	
	// ��װ�б�
	QStringList squadList() const;
	// ����װ�ķ����б�ͬʱ�����óɵ�ǰ��װ
	QStringList configList(const QString& squad);
	// ���ظ���װ���������
	GFChip squadMaxValue(const QString& squad);

	// ��һ������תΪ����ʾ��ͼ����Ϣ
	ChipViewInfo solution2ChipView(const Solution& solution);

public slots:
	// ����Ŀ�����
	void setTargetBlock(const TargetBlock& block);
	// ����Ŀ������÷���
	void setTargetConfig(const QString& name);
	void setUseEquipped(bool b);
	void setUseLocked(bool b);
	void stop();

signals:
	// ��������ٷֱ�
	void solvePercentChanged(int percent);
	// �ܷ�����
	void solveNumberChanged(int number);
	// ������ʱ
	void solveTimeChanged(double time);
	
protected:
	void run() override;

private:	
	// һ������
	typedef std::vector<ChipPuzzleOption> Config;
	// һ����װ����������
	struct SquadConfig
	{
		std::vector<Config> configs;
		// �ܸ���
		int blocks = 38;
		// �ɿո���
		int optional = 0;
		// ��ɫ
		int color;
		// �������
		GFChip maxValue;
	};
	// �������÷�����������Ϊkey
	std::map<QString, SquadConfig> configs_;
	// ��װ�����÷�����¼��Ϣ
	QJsonObject configInfo_;
	// ��װ�Ļ�������
	std::map<QString, ChipViewInfo> squadView_;
	// Ŀ����װ
	QString targetSquadName_;
	// Ŀ�귽��
	QString targetConfigName_;
	// Ŀ�����
	TargetBlock targetBlock_;
	// ��ʱ��¼Ŀ��
	TargetBlock tmpTarget_;
	// ��ʱ��¼����
	Solution tmpSolution_;
	// ��ʱѡ��ķ���
	Config tmpConfig_;
	// ��ǰ��װ�����ֵ
	GFChip tmpMaxValue_;
	// ȥ����set
	std::set<std::vector<int>> solutionSet_;
	// ��ɫ
	int tmpColor_;
	// ��һ���ϱ����������
	int lastSolveNumber_;
	// ��ʼ���е�ʱ��
	time_t t0_;
	// ʹ����װ��
	bool useEquipped_;
	// ʹ��������
	bool useLocked_;
	// ���б�־������ֹͣ����
	bool running_;
	// ��ǰоƬ
	std::vector<int> tmpChips_;

	//��鵱ǰоƬ�����Ƿ������ƴ�������Ҫ
	bool satisfyConfig(const Config& config);
	// ���оƬ�����Ƿ�����Ŀ��ĸ������Ҫ��ֻ�������磬�������true
	bool checkOverflow(const TargetBlock& target, const GFChip& chips) const;
	// �ݹ���ⷽ��
	void findSolution(int k);
};
