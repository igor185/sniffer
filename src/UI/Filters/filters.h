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
    Source = 0,
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
    explicit Filter(QVBoxLayout* layout_parent, int type = Logic::And);

    QWidget* draw();

    void draw(QHBoxLayout *layout);

    bool filter(sockets::base_socket *sock) const ;

    Types type;
    int type2 = 0;
    Logic connect_with_prev;
    QString value;
    QPushButton *addButton;
    QVBoxLayout* layout;
    QWidget * res = new QWidget;
private slots:
    void on_lineEdit_textChanged(const QString &new_value);

    void on_comboBox_edit(int new_type);
    void on_comboBox2_edit(int new_type);

    void delete_filter();
private:
    QComboBox *comboBox;
    QComboBox *comboBox2;
    QLineEdit *lineEdit;

    QComboBox *comboBox_ = nullptr;
    QComboBox *comboBox2_ = nullptr;
    QLineEdit *lineEdit_ = nullptr;

    void fillOptions();
signals:
        void changed();
        void remove(Filter*);
};
#endif // Filters_H
