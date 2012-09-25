/********************************************************************************
** Form generated from reading UI file 'config.ui'
**
** Created: Sun 9. Sep 18:48:15 2012
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIG_H
#define UI_CONFIG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *labelPassword;
    QLineEdit *editPass;
    QLineEdit *editLogin;
    QLabel *label;
    QLineEdit *editDir;
    QLabel *labelLogin;
    QCheckBox *checkBoxPublish;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *applyButton;
    QPushButton *cancelButton;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(328, 176);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Form->sizePolicy().hasHeightForWidth());
        Form->setSizePolicy(sizePolicy);
        Form->setWindowTitle(QString::fromUtf8("Config"));
        gridLayoutWidget = new QWidget(Form);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 10, 301, 121));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(14);
        gridLayout->setVerticalSpacing(4);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        labelPassword = new QLabel(gridLayoutWidget);
        labelPassword->setObjectName(QString::fromUtf8("labelPassword"));
        labelPassword->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(labelPassword, 1, 0, 1, 1);

        editPass = new QLineEdit(gridLayoutWidget);
        editPass->setObjectName(QString::fromUtf8("editPass"));

        gridLayout->addWidget(editPass, 1, 1, 1, 1);

        editLogin = new QLineEdit(gridLayoutWidget);
        editLogin->setObjectName(QString::fromUtf8("editLogin"));

        gridLayout->addWidget(editLogin, 0, 1, 1, 1);

        label = new QLabel(gridLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label, 3, 0, 1, 1);

        editDir = new QLineEdit(gridLayoutWidget);
        editDir->setObjectName(QString::fromUtf8("editDir"));

        gridLayout->addWidget(editDir, 3, 1, 1, 1);

        labelLogin = new QLabel(gridLayoutWidget);
        labelLogin->setObjectName(QString::fromUtf8("labelLogin"));
        labelLogin->setLayoutDirection(Qt::LeftToRight);
        labelLogin->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(labelLogin, 0, 0, 1, 1);

        checkBoxPublish = new QCheckBox(gridLayoutWidget);
        checkBoxPublish->setObjectName(QString::fromUtf8("checkBoxPublish"));
        checkBoxPublish->setChecked(true);

        gridLayout->addWidget(checkBoxPublish, 2, 1, 1, 1);

        horizontalLayoutWidget = new QWidget(Form);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(30, 130, 261, 41));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(25);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        applyButton = new QPushButton(horizontalLayoutWidget);
        applyButton->setObjectName(QString::fromUtf8("applyButton"));

        horizontalLayout->addWidget(applyButton);

        cancelButton = new QPushButton(horizontalLayoutWidget);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        horizontalLayout->addWidget(cancelButton);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        labelPassword->setText(QApplication::translate("Form", "Password:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Form", "Screenshot directory:", 0, QApplication::UnicodeUTF8));
        labelLogin->setText(QApplication::translate("Form", "Login:", 0, QApplication::UnicodeUTF8));
        checkBoxPublish->setText(QApplication::translate("Form", "Publish screenshot", 0, QApplication::UnicodeUTF8));
        applyButton->setText(QApplication::translate("Form", "Apply", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("Form", "Cancel", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Form);
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIG_H
