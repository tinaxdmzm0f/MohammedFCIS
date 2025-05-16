#include "ChipSolver.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <vector>
#include <ctime>
#include "CodeX/CodeX.h"
#include <cstdlib>

constexpr auto QrcBase = ":/ChipSolver/Resources/";

ChipSolver::ChipSolver(QObject* parent):
	QThread(parent),
	useEquipped_(false),
	useLocked_(false),
	useAlt_(false),
	running_(false)
{
	Q_INIT_RESOURCE(ChipSolver);

	qRegisterMetaType<TargetBlock>("TargetBlock");
	
	QFile file(QString(QrcBase) + "squads.json");
	file.open(QIODevice::ReadOnly);
	configInfo_ = QJsonDocument::fromJson(file.readAll()).object();
	file.close();

	for(const auto& squad : configInfo_.keys())
	{
		file.setFileName(QrcBase + squad + ".json");
		file.open(QIODevice::ReadOnly);
		auto obj = QJsonDocument::fromJson(file.readAll()).object();
		file.close();
		
		// ���������Ϣ
		ChipViewInfo info;
		info.width = obj["width"].toInt();
		info.height = obj["height"].toInt();
		auto map = obj["map"].toArray();
		for(auto i = 0;i < map.size();++i)
		{
			info.map.emplace_back();
			for(const auto& it : map[i].toString().toLatin1())
			{
				info.map[i].push_back(-(it - '0'));
			}
		}
		squadView_[squad] = info;

		// ��ȡоƬƴͼ����
		const auto& configFile = configInfo_[squad].toObject();
		for(const auto& name : configFile.keys())
		{
			SquadConfig squadConfig;
			// ��װ�Ļ�����Ϣ��ʵ���Ϻͷ����޹أ�����û�ط����ˣ����õ����ٴ�
			squadConfig.blocks = obj["blocks"].toInt();
			squadConfig.optional = obj["optional"].toObject()[name].toInt();
			auto maxBlocks = obj["MaxBlocks"].toObject();
			squadConfig.maxValue.damageBlock = maxBlocks["damage"].toInt();
			squadConfig.maxValue.defbreakBlock = maxBlocks["def_break"].toInt();
			squadConfig.maxValue.hitBlock = maxBlocks["hit"].toInt();
			squadConfig.maxValue.reloadBlock = maxBlocks["reload"].toInt();
			auto maxValues = obj["MaxValues"].toObject();
			squadConfig.maxValue.damageValue = maxValues["damage"].toInt();
			squadConfig.maxValue.defbreakValue = maxValues["def_break"].toInt();
			squadConfig.maxValue.hitValue = maxValues["hit"].toInt();
			squadConfig.maxValue.reloadValue = maxValues["reload"].toInt();
			squadConfig.color = obj["color"].toInt();
			squadConfig.palindrome = obj["palindrome"].toInt();
			
			file.setFileName(QrcBase + configFile[name].toString());
			file.open(QIODevice::ReadOnly);
			auto configArray = QJsonDocument::fromJson(file.readAll()).array();
			file.close();
			for(const auto& it : configArray)
			{
				Config config;
				for(const auto& c : it.toArray())
				{
					config.push_back(ChipPuzzleOption(c.toObject()));
				}
				squadConfig.configs.push_back(config);
			}
			// name�Ƿ�����
			configs_[name] = squadConfig;
			maxValues_[squad] = squadConfig.maxValue;
		}
	}
}

QStringList ChipSolver::squadList() const
{
	return configInfo_.keys();
}

QStringList ChipSolver::configList(const QString& squad)
{
	targetSquadName_ = squad;
	return configInfo_[squad].toObject().keys();
}

GFChip ChipSolver::squadMaxValue(const QString& squad)
{
	return maxValues_[squad];
}

void ChipSolver::setTargetBlock(const TargetBlock& block)
{
	targetBlock_ = block;
}

void ChipSolver::setTargetConfig(const QString& name)
{
	targetConfigName_ = name;
}

void ChipSolver::setUseEquipped(bool b)
{
	useEquipped_ = b;
}

void ChipSolver::setUseLocked(bool b)
{
	useLocked_ = b;
}

void ChipSolver::setUseAlt(bool b)
{
	useAlt_ = b;
}

void ChipSolver::stop()
{
	if(this->isRunning())
		running_ = false;
}

ChipViewInfo ChipSolver::solution2ChipView(const Solution& solution, const QString& squad)
{
	auto view = squadView_[targetSquadName_];
	if(squad != "")
	{
		view = squadView_[squad];
	}
	for(auto i = 0;i < solution.chips.size();++i)
	{
		const auto& chip = solution.chips[i];
		const auto& chipConfig = ChipConfig::getConfig(CodeX::instance()->chips[chip.no].gridID).rotate90(chip.rotate);
		for(auto x = 0;x < chipConfig.width;++x)
		{
			for(auto y = 0;y < chipConfig.height;++y)
			{
				if(chipConfig.map[y][x] == '1')
				{
					view.map[y + chip.y][x + chip.x] = i + 1;
				}
			}
		}
	}
	typedef std::vector<std::vector<int>> Map;
	auto rotate = [](const Map& map) -> Map
	{
		Map r;
		std::vector<int> row;
		row.resize(map.size(), 0);
		r.resize(map[0].size(), row);
		for (auto i = 0u; i < r.size(); ++i)
		{
			for (auto j = 0u; j < r[i].size(); ++j)
			{
				r[i][j] = map[map.size() - j - 1][i];
			}
		}
		return r;
	};
	for(auto i = 0;i < solution.totalValue.rotate;++i)
	{
		view.map = rotate(view.map);
	}
	return view;
}

void ChipSolver::run()
{
	const auto& plans = configs_[targetConfigName_];
	running_ = true;
	lastSolveNumber_ = 0;
	tmpTarget_ = targetBlock_;
	tmpTarget_.error += plans.optional; // ���Ӷ���Ŀɿո�
	int percent = 0;
	solutions.clear();
	queue_ = std::priority_queue<Solution>();
	t0_ = clock();
	tmpChips_.resize(8, 0);
	tmpSquadConfig_ = configs_[targetConfigName_];
	tmpSolutionNumber_ = 0;
	for(auto i = 0;i < plans.configs.size();++i)
	{
		// ���㵱ǰ���Ȱٷֱ�
		int per = round((i + 1) * 100.0 / plans.configs.size());
		if(per > percent)
		{
			percent = per;
			emit solvePercentChanged(percent);
		}

		// ��ǰ���÷���
		tmpConfig_ = plans.configs[i];
		
		if(!satisfyConfig(plans.configs[i]))
		{
			continue;
		}
		// ��ʱ��¼����
		tmpSolution_.chips.resize(8, ChipPuzzleOption());

		solutionSet_.clear();
		findSolution(0);
		emit solveNumberChanged(lastSolveNumber_ = tmpSolutionNumber_);
		auto t1 = clock();
		emit solveTimeChanged(double(t1 - t0_) / CLOCKS_PER_SEC);
		if (tmpSolutionNumber_ >= targetBlock_.maxNumber)
			break;
	}
	while(!queue_.empty())
	{
		solutions.push_back(queue_.top());
		queue_.pop();
	}
	emit solvePercentChanged(100);
}

bool ChipSolver::satisfyConfig(const Config& config)
{
	QMap<int, int> require;
	bool ans = true;
	for(const auto& it : config)
	{
		++require[it.no];
	}
	for(const auto& it : require.keys())
	{
		ans = ans && (CodeX::instance()->gridChips[tmpSquadConfig_.color][it].size() >= require[it]);
	}
	return ans;
}

bool ChipSolver::checkOverflow(const TargetBlock& target, const GFChip& chips) const
{
	int over = 0;
	over += std::max(0, chips.defbreakBlock - target.defbreakBlock);
	over += std::max(0, chips.damageBlock - target.damageBlock);
	over += std::max(0, chips.reloadBlock - target.reloadBlock);
	over += std::max(0, chips.hitBlock - target.hitBlock);
	return over > target.error;
}

void ChipSolver::findSolution(int k)
{
	if (!running_)
		return;
	if (k >= tmpConfig_.size())
	{
		if (solutionSet_.count(tmpChips_) > 0)
			return;
		solutionSet_.insert(tmpChips_);
		auto t = tmpSolution_.chips.size();
		tmpSolution_.chips.resize(k);
		tmpSolution_.totalValue.no = 0;
		tmpSolution_.totalValue.id = 0;
		tmpSolution_.totalValue.exp = 0;
		tmpSolution_.totalValue.rotate = 0;
		for(const auto& it : tmpSolution_.chips)
		{
			const auto& chip = CodeX::instance()->chips[it.no];
			auto r = chip.rotate % ChipConfig::getConfig(chip.gridID).direction;
			tmpSolution_.totalValue.no += int(it.rotate != r);
			tmpSolution_.totalValue.exp += chip.exp;
		}
		if(tmpSquadConfig_.palindrome > 0)
		{
			// ö��������ת����
			for(auto i = tmpSquadConfig_.palindrome;i < 4;i += tmpSquadConfig_.palindrome)
			{
				int sum = 0;
				// ��ÿ��оƬ����һ����ת
				for (const auto& it : tmpSolution_.chips)
				{
					const auto& chip = CodeX::instance()->chips[it.no];
					auto r = chip.rotate + i;
					r %= ChipConfig::getConfig(chip.gridID).direction; // ����оƬ����Գ�����
					sum += int(r != it.rotate);
				}
				if (sum < tmpSolution_.totalValue.no)
				{
					tmpSolution_.totalValue.no = sum;
					tmpSolution_.totalValue.rotate = i;
				}
			}
		}
		tmpSolution_.totalValue.id += std::min(0, tmpSolution_.totalValue.defbreakValue - tmpSquadConfig_.maxValue.defbreakValue);
		tmpSolution_.totalValue.id += std::min(0, tmpSolution_.totalValue.damageValue - tmpSquadConfig_.maxValue.damageValue);
		tmpSolution_.totalValue.id += std::min(0, tmpSolution_.totalValue.reloadValue - tmpSquadConfig_.maxValue.reloadValue);
		tmpSolution_.totalValue.id += std::min(0, tmpSolution_.totalValue.hitValue - tmpSquadConfig_.maxValue.hitValue);
		queue_.push(tmpSolution_);
		if(queue_.size() > targetBlock_.showNumber)
		{
			queue_.pop();
		}
		tmpSolution_.chips.resize(t);
		++tmpSolutionNumber_;

		if(tmpSolutionNumber_ - lastSolveNumber_ > 1000)
		{
			emit solveNumberChanged(lastSolveNumber_ = tmpSolutionNumber_);
			auto t1 = clock();
			emit solveTimeChanged(double(t1 - t0_) / CLOCKS_PER_SEC);
			if(tmpSolutionNumber_ > targetBlock_.maxNumber)
			{
				running_ = false;
			}
		}
		
		return;
	}
	//��ȡ��ǰ�����ͺŵ�оƬ�б�
	auto& chips = CodeX::instance()->gridChips[tmpSquadConfig_.color][tmpConfig_[k].no];
	// �ȱ�����һ����оƬ���ã���������id
	tmpSolution_.chips[k] = tmpConfig_[k];
	for (auto& chip : chips)
	{
		if (chip.squad & 0x8000) //��ʹ��
			continue;
		if ((chip.locked && !useLocked_)// �������Ҳ�ʹ��������
			|| (chip.squad && !useEquipped_))// ��װ���Ҳ�ʹ����װ��
		{
			continue;
		}
		// ��ʹ���Ҳ�ʹ����װ��
		if(CodeX::instance()->chipUsed(chip.no) && !useAlt_)
		{
			continue;
		}
		// ����˲�����Ҫ��
		if (checkOverflow(tmpTarget_, tmpSolution_.totalValue + chip))
		{
			continue;
		}
		chip.squad |= 0x8000; //����λ��1������С�ӺŶ�������
		tmpSolution_.totalValue += chip;
		tmpSolution_.chips[k].no = chip.no; // ����id
		tmpChips_[k] = chip.no;
		findSolution(k + 1);
		chip.squad &= ~0x8000; // ����λ��0
		tmpSolution_.totalValue -= chip;
	}
}
