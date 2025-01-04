#pragma once

#include "chip_global.h"
#include <QJsonObject>
#include <QPixmap>
#include <QList>
#include <string>
#include <vector>
#include <map>

// ��Ϸ�ڵ�оƬ����
class CHIP_EXPORT GFChip
{
public:
	explicit GFChip(const QJsonObject& object);
	GFChip();
	static GFChip fromJsonObject(const QJsonObject& object);
	QPixmap icon() const;
	QString name() const;
	QString squadName() const;

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

	// ��Ϸ��id
	int id;
	// ˳����
	int no;
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
	
	void calcValue();

	GFChip operator + (const GFChip& t) const;
	GFChip operator += (const GFChip& t);
	GFChip operator - (const GFChip& t) const;
	GFChip operator -= (const GFChip& t);
};

typedef std::vector<GFChip> Chips;

Chips CHIP_EXPORT getChips(const QJsonObject& obj);

// С�����תΪ��Ӧ���ƣ���Ҫ�ȴ�squad_with_user_info�ж�ȡid��ת��Ϊ1-6
QString CHIP_EXPORT squadString(int i);

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
	// ��ת������
	int direction;

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
struct CHIP_EXPORT ChipPuzzleOption
{
	int id;
	uint8_t x, y;
	// ˳ʱ����ת90�ȵĴ���
	uint8_t rotate;
	explicit ChipPuzzleOption(int _x = 0, int _y = 0, int _r = 0, int _no = 0) :id(_no), x(_x), y(_y), rotate(_r) {}
	explicit ChipPuzzleOption(const QJsonObject& object);
};

// оƬ�ڷ���Ϣ��0Ϊ�գ�>0ΪоƬ��ţ��±�+1),<0Ϊ�޷�ʹ��
struct CHIP_EXPORT ChipViewInfo
{
	int width;
	int height;
	std::vector<std::vector<int>> map;
};

// һ����н�
struct CHIP_EXPORT Solution
{
	// ʹ�õ�оƬ����
	std::vector<ChipPuzzleOption> chips;
	// �õ�����װ�����ԣ���levelΪ�ܵȼ���idΪ����ƫ�noΪ��ת����
	GFChip totalValue;

	bool operator <(const Solution& r) const;
};
