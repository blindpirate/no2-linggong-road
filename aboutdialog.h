#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QtGui>
#include<QtWidgets>
namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();
    
private slots:
    void on_pushButton_clicked();

    void on_label_linkActivated(const QString &link);

private:
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
