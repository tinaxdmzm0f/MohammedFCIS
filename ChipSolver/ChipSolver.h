#pragma once

#include "chipsolver_global.h"
#include <QThread>
#include <QString>
#include <map>
#include <set>
#include <vector>
#include <queue>
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
	// �����ʾ��������
	int showNumber;
	// �����㷽������
	int maxNumber;

	explicit TargetBlock(int dmg = 0, int dbk = 0, int hit = 0, int r = 0,  int e = 0, int u = 1e3,int m = 1e9) :
		damageBlock(dmg), defbreakBlock(dbk), hitBlock(hit), reloadBlock(r), error(e), showNumber(u),maxNumber(m)
	{}
};

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
	ChipViewInfo solution2ChipView(const Solution& solution, const QString& squad = "");

public slots:
	// ����Ŀ�����
	void setTargetBlock(const TargetBlock& block);
	// ����Ŀ������÷���
	void setTargetConfig(const QString& name);
	void setUseEquipped(bool b);
	void setUseLocked(bool b);
	void setUseAlt(bool b);
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
		// ��ת�Գƣ�0Ϊ���Գƣ�>0Ϊ��Ҫ��ת����
		int palindrome;
		// �������
		GFChip maxValue;
	};
	// �������÷�����������Ϊkey
	std::map<QString, SquadConfig> configs_;
	// ������װ�����ֵ
	std::map<QString, GFChip> maxValues_;
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
	// ��ʱѡ�����װ����
	SquadConfig tmpSquadConfig_;
	// ȥ����set
	std::set<std::vector<int>> solutionSet_;
	// ��һ���ϱ����������
	int lastSolveNumber_;
	// ��ʼ���е�ʱ��
	time_t t0_;
	// ʹ����װ��
	bool useEquipped_;
	// ʹ��������
	bool useLocked_;
	// ʹ�ñ�ѡ
	bool useAlt_;
	// ���б�־������ֹͣ����
	bool running_;
	// ��ǰоƬ
	std::vector<int> tmpChips_;
	// ���ȼ�����
	std::priority_queue<Solution> queue_;
	// ������
	int tmpSolutionNumber_;

	//��鵱ǰоƬ�����Ƿ������ƴ�������Ҫ
	bool satisfyConfig(const Config& config);
	// ���оƬ�����Ƿ�����Ŀ��ĸ������Ҫ��ֻ�������磬�������true
	bool checkOverflow(const TargetBlock& target, const GFChip& chips) const;
	// �ݹ���ⷽ��
	void findSolution(int k);
};
