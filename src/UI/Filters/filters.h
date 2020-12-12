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

class Filter : public QWidget {
Q_OBJECT

public:
    explicit Filter();

    QWidget* draw();

    void draw(QHBoxLayout *layout);

    [[nodiscard]] bool filter(const QModelIndex &index) const ;

    Types type;
    QString value;
private slots:
    void on_lineEdit_textChanged(const QString &new_value);

    void on_comboBox_edit(int new_type);
private:
    QComboBox *comboBox;
    QLineEdit *lineEdit;

signals:
        void changed();
};
#endif // Filters_H
