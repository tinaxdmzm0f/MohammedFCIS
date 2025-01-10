#pragma once
#pragma warning(disable:26812)
#include <QtWidgets/QMainWindow>
#include <QList>
#include <QMap>

namespace Ui { class CodeX; }
class GFChip;
class ChipSolver;
class ChipDataWindow;
class QProgressBar;
class QLabel;
class ChipTableModel;
class ChipTableDelegate;
class SolutionTableModel;
class SettingWindow;
class AboutDialog;

class CodeX : public QMainWindow
{
	Q_OBJECT

public:
	static CodeX* instance();

	// оƬ�б�
	Chips chips;
	// ��㰴��ɫ���࣬�ڲ㰴grid��ŷ����оƬ������ǿ��+20������
	std::map<int,std::map<int, std::vector<GFChip>>> gridChips;
	// ��С�ӷ������װ����оƬ
	std::map<int, std::vector<GFChip>> squadChips;
	
	// �����
	ChipSolver* solver_;

protected slots:
	void solve();
	void solveFinished();
	void selectSolution(int index);
	
private:
	CodeX(QWidget* parent = Q_NULLPTR);
	static CodeX* singleton;
	
	void connect();
	
	Ui::CodeX *ui;
	ChipDataWindow* chipDataWindow_;
	SettingWindow* settingWindow_;

	QProgressBar* progressBar_;
	QLabel* solveNumberLabel_;
	QLabel* timeLabel_;

	ChipTableModel* chipTableModel_;
	ChipTableDelegate* chipTableDelegate_;
	SolutionTableModel* solutionTableModel_;
	AboutDialog* aboutDialog_;
};
