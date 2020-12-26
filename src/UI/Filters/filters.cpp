#include "filters.h"
#include "ProxyModel.h"
#include "QPushButton"
#include "QLabel"
#include <QCloseEvent>


FiltersView::FiltersView(ProxyModel* proxy, QWidget *parent)
    : QMainWindow(parent),
    proxy(proxy),
    ui(new Ui::FiltersView)
{
    ui->setupUi(this);

    for(Filter* filter: proxy->getFilters()){
        ui->verticalLayout_2->addWidget(filter->draw());
    }


    bar->addAction(andIcon);
    bar->addAction(orIcon);
    bar->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );

    connect(andIcon, SIGNAL(triggered(bool)), this, SLOT(handleAnd()));
    connect(orIcon, SIGNAL(triggered(bool)), this, SLOT(handleOr()));

    addToolBar(bar);
}

FiltersView::~FiltersView()
{
    delete ui;
}

void FiltersView::addFilter(Logic type) {
    auto* filter = new Filter(type);
    proxy->add_filter(filter);
    ui->verticalLayout_2->addWidget(filter->draw());
}

void FiltersView::handleAnd() {
    addFilter(Logic::And);
}

void FiltersView::handleOr() {
    addFilter(Logic::Or);
}

void Filter::draw(QHBoxLayout *layout) {
    comboBox_ = new QComboBox(this);
    comboBox_->setObjectName(QString::fromUtf8("comboBox"));
    comboBox_->addItem("Source");
    comboBox_->addItem("Destination");
    comboBox_->addItem("Length");
    comboBox_->addItem("Protocol");
//    comboBox->addItem("Info");
    comboBox_->setCurrentIndex(3);

    lineEdit_ = new QLineEdit(this);
    lineEdit_->setObjectName(QString::fromUtf8("lineEdit"));

    connect(lineEdit_, SIGNAL(textChanged(const QString &)), this, SLOT(on_lineEdit_textChanged(const QString &)));
    connect(comboBox_, SIGNAL(currentIndexChanged(int)), this, SLOT(on_comboBox_edit(int)));
    layout->addWidget(comboBox_);
    layout->addWidget(lineEdit_);
}

bool Filter::filter(const QModelIndex &index) const {
    QRegExp regExp(value, Qt::CaseInsensitive, QRegExp::Wildcard);
    return index.data().toString().contains(regExp);
}

void Filter::on_lineEdit_textChanged(const QString &new_value) {
    value = new_value;
    lineEdit->setText(value);

    if(lineEdit_ != nullptr){
        lineEdit_->setText(value);
    }

    emit changed();
}

void Filter::on_comboBox_edit(int new_type) {
    type = static_cast<Types>(new_type + 1);
    value = "";
    lineEdit->clear();
    lineEdit->setFocus();
    comboBox->setCurrentIndex(new_type);

    if (lineEdit_ != nullptr){
        lineEdit->clear();
        lineEdit->setFocus();
    }

    if(comboBox_ != nullptr){
        comboBox_->setCurrentIndex(new_type);
    }
    emit changed();
}

QWidget *Filter::draw() {
    auto layout_h = new QHBoxLayout(this);
    if(lineEdit_ == nullptr) {
        layout_h->addWidget(addButton);
    }
    layout_h->addWidget(comboBox);
    layout_h->addWidget(lineEdit);

    auto *w = new QWidget;
    w->setLayout(layout_h);
    if(lineEdit_ != nullptr)
        return w;

    auto* layout_v = new QVBoxLayout(this);
    auto* label = new QLabel(connect_with_prev == Logic::And ? "and" : "or");
    if(connect_with_prev == Logic::And)
        label->setStyleSheet("border-top:1px solid black;");
    layout_v->addWidget(label);
    layout_v->addWidget(w);

    auto* res = new QWidget;
    res->setLayout(layout_v);

    return res;
}

Filter::Filter(int connected) {
    type = static_cast<Types>(Types::Protocol + 1);
    connect_with_prev = static_cast<Logic>(connected);

    addButton = new QPushButton(QIcon(":images/stop.png"),"");
    QString buttonStyle = "QPushButton{border:none;}";
    addButton->setStyleSheet(buttonStyle);
    addButton->setIconSize(QSize(20,20));

    comboBox = new QComboBox(this);
    comboBox->setObjectName(QString::fromUtf8("comboBox"));
    comboBox->addItem("Source");
    comboBox->addItem("Destination");
    comboBox->addItem("Length");
    comboBox->addItem("Protocol");
//    comboBox->addItem("Info");
    comboBox->setCurrentIndex(3);

    lineEdit = new QLineEdit(this);
    lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

    connect(lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(on_lineEdit_textChanged(const QString &)));
    connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_comboBox_edit(int)));
}


