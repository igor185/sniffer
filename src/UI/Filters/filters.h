#ifndef Filters_H
#define Filters_H

#include <IO/IO.h>
#include <QMainWindow>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QToolBar>

#include <QComboBox>



enum Types {
    Source,
    Destination,
    Length,
    Protocol
};

enum Logic {
    And,
    Or
};

class Filter : public QWidget {
Q_OBJECT

public:
    explicit Filter(int type = Logic::And);

    QWidget* draw();

    void draw(QHBoxLayout *layout);

    [[nodiscard]] bool filter(const QModelIndex &index) const ;

    Types type;
    Logic connect_with_prev;
    QString value;
    QPushButton *addButton;
private slots:
    void on_lineEdit_textChanged(const QString &new_value);

    void on_comboBox_edit(int new_type);
private:
    QComboBox *comboBox;
    QLineEdit *lineEdit;

    QComboBox *comboBox_ = nullptr;
    QLineEdit *lineEdit_ = nullptr;

signals:
        void changed();
};
#endif // Filters_H
