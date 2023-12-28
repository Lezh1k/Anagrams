#include "Mainwindow.h"
#include "ui_Mainwindow.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QtConcurrent/QtConcurrent>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->btn_try_parse->setEnabled(false);
  ui->btn_try_parse->setText("Initializing");
  const QString fp(":/anagrams/files/resources/rus_dictionary");
  parse_dct_file(fp);
  connect(ui->btn_try_parse, SIGNAL(released()), this,
          SLOT(btn_parse_released()));
}
//////////////////////////////////////////////////////////////

MainWindow::~MainWindow() { delete ui; }
//////////////////////////////////////////////////////////////

int MainWindow::parse_dct_file(const QString &path) {
  static const int MAX_LENGTH = 25;
  QFile f(path);
  if (!f.open(QFile::ReadOnly)) {
    ui->btn_try_parse->setText(f.errorString());
    return -1;
  }

  ui->btn_try_parse->setText("Loading");
  while (!f.atEnd()) {
    QByteArray arr = f.readLine(MAX_LENGTH);
    arr.truncate(arr.size() - 2);
    QString str(arr), sstr(arr);
    std::sort(sstr.begin(), sstr.end());

    if (m_dct_sorted_original.find(sstr) == m_dct_sorted_original.end())
      m_dct_sorted_original[sstr] = std::vector<QString>();

    m_dct_sorted_original[sstr].push_back(str);
  }
  f.close();
  ui->btn_try_parse->setEnabled(true);
  ui->btn_try_parse->setText("Parse");
  return 0;
}
//////////////////////////////////////////////////////////////

std::vector<std::vector<int>> combinations(int *arr, int len, int m) {
  std::vector<std::vector<int>> res;
  if (m == 2) {
    for (int i = 0; i < len - 1; ++i) {
      for (int j = i + 1; j < len; ++j) {
        std::vector<int> tmp;
        tmp.push_back(arr[i]);
        tmp.push_back(arr[j]);
        res.push_back(tmp);
      }
    }
    return res;
  }

  for (int i = 0; i <= len - m; ++i) {
    std::vector<std::vector<int>> tmp =
        combinations(&arr[i + 1], len - i - 1, m - 1);
    for (auto v = tmp.begin(); v != tmp.end(); ++v) {
      v->insert(v->begin(), arr[i]);
      res.push_back(*v);
    }
  }
  return res;
}
//////////////////////////////////////////////////////////////

std::vector<QString> MainWindow::try_find_anagrams(const QString &src) {
  QString sstr(src);
  std::sort(sstr.begin(), sstr.end());
  std::vector<QString> lst_result;
  int *arr = new int[sstr.size()];
  for (int i = 0; i < sstr.size(); ++i)
    arr[i] = i;

  for (int i = 2; i < sstr.size() - 1; ++i) {
    std::vector<std::vector<int>> lst_combinations =
        combinations(arr, sstr.size(), i);

    for (auto comb = lst_combinations.begin(); comb != lst_combinations.end();
         ++comb) {
      QString key;
      for (auto j = comb->begin(); j != comb->end(); ++j)
        key += sstr[*j];

      if (m_dct_sorted_original.find(key) == m_dct_sorted_original.end())
        continue;

      for (auto item = m_dct_sorted_original[key].begin();
           item != m_dct_sorted_original[key].end(); ++item) {
        if (std::find(lst_result.begin(), lst_result.end(), *item) !=
            lst_result.end())
          continue;
        lst_result.push_back(*item);
      } // for item
    }   // for comb
  }     // for i = 2; i < sstr.size() - 1; ++i

  do {
    if (m_dct_sorted_original.find(sstr) == m_dct_sorted_original.end())
      break;

    for (auto item = m_dct_sorted_original[sstr].begin();
         item != m_dct_sorted_original[sstr].end(); ++item) {
      if (std::find(lst_result.begin(), lst_result.end(), *item) !=
          lst_result.end())
        continue;
      lst_result.push_back(*item);
    } // for item
  } while (0);

  delete[] arr;
  return lst_result;
}
//////////////////////////////////////////////////////////////

void MainWindow::btn_parse_released() {
  QString src = ui->le_src_word->text();
  if (src.isEmpty())
    return;

  std::vector<QString> lst = try_find_anagrams(src);
  if (lst.empty())
    return;
  int len = lst[0].size();

  QString text;
  for (auto i = lst.begin(); i != lst.end(); ++i) {
    if (len != i->size()) {
      len = i->size();
      text += "**************************\n";
    }
    text += QString(" %1\n").arg(*i);
  }
  ui->te_result_words->setText(text);
}
//////////////////////////////////////////////////////////////
