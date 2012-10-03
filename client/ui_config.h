/********************************************************************************
** Form generated from reading UI file 'config.ui'
**
** Created: Wed 3. Oct 08:33:30 2012
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
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QComboBox *comboImageType;
    QComboBox *comboSourcesType;
    QLabel *labelPassword;
    QLabel *labelLogin;
    QCheckBox *checkBoxLangDialogShow;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *applyButton;
    QPushButton *cancelButton;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(308, 151);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Form->sizePolicy().hasHeightForWidth());
        Form->setSizePolicy(sizePolicy);
        Form->setWindowTitle(QString::fromUtf8("Config"));
        gridLayoutWidget = new QWidget(Form);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 10, 291, 91));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(14);
        gridLayout->setVerticalSpacing(4);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        comboImageType = new QComboBox(gridLayoutWidget);
        comboImageType->setObjectName(QString::fromUtf8("comboImageType"));

        gridLayout->addWidget(comboImageType, 0, 1, 1, 1);

        comboSourcesType = new QComboBox(gridLayoutWidget);
        comboSourcesType->setObjectName(QString::fromUtf8("comboSourcesType"));
        comboSourcesType->setEditable(false);

        gridLayout->addWidget(comboSourcesType, 1, 1, 1, 1);

        labelPassword = new QLabel(gridLayoutWidget);
        labelPassword->setObjectName(QString::fromUtf8("labelPassword"));
        labelPassword->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(labelPassword, 1, 0, 1, 1);

        labelLogin = new QLabel(gridLayoutWidget);
        labelLogin->setObjectName(QString::fromUtf8("labelLogin"));
        labelLogin->setLayoutDirection(Qt::LeftToRight);
        labelLogin->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(labelLogin, 0, 0, 1, 1);

        checkBoxLangDialogShow = new QCheckBox(gridLayoutWidget);
        checkBoxLangDialogShow->setObjectName(QString::fromUtf8("checkBoxLangDialogShow"));
        checkBoxLangDialogShow->setChecked(true);

        gridLayout->addWidget(checkBoxLangDialogShow, 2, 1, 1, 1);

        horizontalLayoutWidget = new QWidget(Form);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(30, 100, 261, 41));
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

        comboImageType->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        comboImageType->clear();
        comboImageType->insertItems(0, QStringList()
         << QApplication::translate("Form", "png", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Form", "jpg", 0, QApplication::UnicodeUTF8)
        );
        comboSourcesType->clear();
        comboSourcesType->insertItems(0, QStringList()
         << QApplication::translate("Form", "Plain text", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Form", "C++", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Form", "Pascal", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Form", "Python", 0, QApplication::UnicodeUTF8)
        );
        labelPassword->setText(QApplication::translate("Form", "Language:", 0, QApplication::UnicodeUTF8));
        labelLogin->setText(QApplication::translate("Form", "Image type", 0, QApplication::UnicodeUTF8));
        checkBoxLangDialogShow->setText(QApplication::translate("Form", "Show language select dialog", 0, QApplication::UnicodeUTF8));
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
