#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:
  Ui::MainWindow *ui;  
  std::map<QString, std::vector<QString> > m_dct_sorted_original;

  int parse_dct_file(const QString& path);
  std::vector<QString> try_find_anagrams(const QString& src);

private slots:
  void btn_parse_released();
};

#endif // MAINWINDOW_H
