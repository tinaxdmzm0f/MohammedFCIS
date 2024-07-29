#pragma once

#include "chip_global.h"
#include <QJsonObject>
#include <QIcon>
#include <QList>
#include <string>
#include <vector>
#include <map>

// ��Ϸ�ڵ�оƬ����
class CHIP_EXPORT GFChip
{
public:
	explicit GFChip(const QJsonObject& object);
	static GFChip fromJsonObject(const QJsonObject& object);
	QIcon icon() const;

	enum ChipClass
	{
		Class56 = 5061,
		Class551 = 5051,
		Class552 = 5052,
	};
	enum ChipColor
	{
		Orange = 1,
		Blue = 2,
	};
	struct Point
	{
		int x, y;
	};
	
	// оƬ���5061/5051/5052...
	int chipClass;
	// оƬ�ȼ�
	int level;
	// оƬ��ɫ
	int color;
	// оƬ��״ID
	int gridID;
	// С��װ����Ϣ��0δװ��
	int squad;
	// оƬλ��
	Point position;
	// оƬ��ת��˳ʱ�룬90��Ϊ��λ
	int rotate;
	// �˺�����
	int damageBlock;
	// װ�����
	int reloadBlock;
	// ���и���
	int hitBlock;
	// �Ʒ�����
	int defbreakBlock;
	// �˺���ֵ
	int damageValue;
	// װ����ֵ
	int reloadValue;
	// ������ֵ
	int hitValue;
	// �Ʒ���ֵ
	int defbreakValue;
	// ����
	bool locked;
	
private:
	GFChip();
	void calcValue();
};

QList<GFChip> getChips(const QJsonObject& obj);

// ÿ��оƬ�Ļ�������
class CHIP_EXPORT ChipConfig
{
public:
	typedef std::vector<std::string> Map;

	ChipConfig() = default;
	
	// оƬ��״ID
	int gridID;
	// оƬ���5061/5051/5052...
	int chipClass;
	// оƬ���
	int width;
	// оƬ�߶�
	int height;
	// оƬ����
	int blocks;

	// оƬ����
	std::string name;
	// оƬ��״
	Map map;

	// ͨ��gridID��ȡоƬ����
	static const ChipConfig& getConfig(int id);

	ChipConfig rotate90(int n = 1) const; // clockwise

private:
	explicit ChipConfig(const QJsonObject& object);

	static std::map<int, ChipConfig> configs_;
};

// оƬ��ƴͼ�ⷨ�еĲ���
struct ChipPuzzleOption
{
	int gridID;
	int x, y;
	// ˳ʱ����ת90�ȵĴ���
	int rotate;
	explicit ChipPuzzleOption(int _x = 0, int _y = 0, int _r = 0, int _no = 0) :gridID(_no), x(_x), y(_y), rotate(_r) {}
	explicit ChipPuzzleOption(const QJsonObject& object);
};