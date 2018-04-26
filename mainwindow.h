#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void NumberClicked();
    void PlusMinusClicked();
    void MultDivClicked();
    void EqualsClicked();
    void DecimalPointClicked();
    void SignClicked();
    void ClearClicked();
    void BackClicked();

/* inLinePlusMinusSign a inLineMultDivSig:
*  Sluzia na ulozenie nasledovnej operacii v poradi. Operacie Mult a Div maju prednost
*  pred Plus a Minus, preto su tieto premmne rozdelene do dvoch kategorii.
*  Vypocet sa vykona vzdy az po stlaceni dalsej operacie.
*/

private:
    Ui::MainWindow *ui;

    bool newNumber;
    double temporaryResult;
    double temporaryMultDivResult;

    QString inLinePlusMinusSign;
    QString inLineMultDivSign;

    void Calculator(double temporaryNumber, QString inLineSign);

};

#endif // MAINWINDOW_H
