/* Kalkulacka - Juraj Sallai
 * Program rozlisuje medzi operaciami plus,minus a krat,deleno.
 * Uprednostnuje nasobenie a delenie vo vyraze
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pushbutton.h"
#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    newNumber = true;               // ak sa zadava novy operand, newNumber = true
    temporaryResult = 0.0;          // uklada medzivysledok pre operacie + a -
    temporaryMultDivResult = 0.0;   // uklada medzivysledok pre operacie * a /

    // Kontroluje stlacenie numerickych tlacitok, spusta NumberClicked()
    connect(ui->pushButtonNumberZero, &QPushButton::clicked,
            this, &MainWindow::NumberClicked);
    connect(ui->pushButtonNumberOne, &QPushButton::clicked,
            this, &MainWindow::NumberClicked);
    connect(ui->pushButtonNumberTwo, &QPushButton::clicked,
            this, &MainWindow::NumberClicked);
    connect(ui->pushButtonNumberThree, &QPushButton::clicked,
            this, &MainWindow::NumberClicked);
    connect(ui->pushButtonNumberFour, &QPushButton::clicked,
            this, &MainWindow::NumberClicked);
    connect(ui->pushButtonNumberFive, &QPushButton::clicked,
            this, &MainWindow::NumberClicked);
    connect(ui->pushButtonNumberSix, &QPushButton::clicked,
            this, &MainWindow::NumberClicked);
    connect(ui->pushButtonNumberSeven, &QPushButton::clicked,
            this, &MainWindow::NumberClicked);
    connect(ui->pushButtonNumberEight, &QPushButton::clicked,
            this, &MainWindow::NumberClicked);
    connect(ui->pushButtonNumberNine, &QPushButton::clicked,
            this, &MainWindow::NumberClicked);

    // Kontroluje stlacenie Plus a Minus. Spusta spolocnu PlusMinusClicked()
    connect(ui->pushButtonOperationPlus, &QPushButton::clicked,
            this, &MainWindow::PlusMinusClicked);
    connect(ui->pushButtonOperationMinus, &QPushButton::clicked,
            this, &MainWindow::PlusMinusClicked);

    // Kontroluje stlacenie Krat a Deleno. Spusta spolocnu MultDivClicked()
    connect(ui->pushButtonOperationMult, &QPushButton::clicked,
            this, &MainWindow::MultDivClicked);
    connect(ui->pushButtonOperationDiv, &QPushButton::clicked,
            this, &MainWindow::MultDivClicked);

    // Kontroluje stlacenie = , desatinna ciarka, zmena znamienka, Clear, Back
    // Spusta prislusne fcie
    connect(ui->pushButtonEquals, &QPushButton::clicked,
            this, &MainWindow::EqualsClicked);
    connect(ui->pushButtonDecimalPoint, &QPushButton::clicked,
            this, &MainWindow::DecimalPointClicked);
    connect(ui->pushButtonSign, &QPushButton::clicked,
            this, &MainWindow::SignClicked);
    connect(ui->pushButtonClear, &QPushButton::clicked,
            this, &MainWindow::ClearClicked);
    connect(ui->pushButtonBack, &QPushButton::clicked,
            this, &MainWindow::BackClicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}

// buttonNumber zisti identitu stlaceneho pushButton a ziska .text() do buttonNumberValue
// Do lineText sa vpise buttonNumberValue - numericka hodnota tlacitka

void MainWindow::NumberClicked() {
    PushButton *buttonText = (PushButton *)sender();
    QString buttonNumberValue = buttonText->text();

    // kontroluje aby sme nezapisali same nuly napr. 00000
    if (buttonNumberValue == "0" && ui->lineEdit->text() == "0") {
        return;
    }

    // kontroluje aby nebol prvy znak celeho cisla nula, t.j nechceme cislo 02 ale 2
    if (ui->lineEdit->text() == "0" && ui->lineEdit->text().length() == 1) {
        ui->lineEdit->setText("");
    }

    // Ak zadavame nove cislo, treba vymazat display (po stlaceni +,-,*,/,=)
    if (newNumber) {
        ui->lineEdit->clear();
        newNumber = false;
    }

    ui->lineEdit->setText(ui->lineEdit->text() + buttonNumberValue);

}

// Meni kladne a zaporne znamienko

void MainWindow::SignClicked() {
    QString textValue = ui->lineEdit->text();
    double numberValue = textValue.toDouble();

    if (numberValue > 0) {
        textValue.prepend("-");     // vloz "-" ak je hodnota mensia ako nula
    }
    else if (numberValue < 0) {     // odstran prvy znak ("-") ak je hodnota zaporna
        textValue.remove(0,1);
    }
    ui->lineEdit->setText(textValue);
}



// Vklada desatinnu ciarku a kontroluje aby nebola zadana viackrat

void MainWindow::DecimalPointClicked() {
    if (!ui->lineEdit->text().contains('.'))
    {
        ui->lineEdit->setText(ui->lineEdit->text() + ".");
    }
}


// Vymaze lineEdit (display) a vsetky doterajsie vypocty

void MainWindow::ClearClicked() {
    ui->lineEdit->clear();
    temporaryResult = 0.0;
    temporaryMultDivResult = 0.0;
    inLinePlusMinusSign.clear();
    inLineMultDivSign.clear();
}

// Krok spat
// Zisti dlzku cisla a vymaze posledny znak
// Ak ostane iba zaporny znak alebo prazdy text, nastavi primarne nulu

void MainWindow::BackClicked() {
    int numberLength = ui->lineEdit->text().length();
    QString textValue = ui->lineEdit->text().remove(numberLength-1, 1);

    ui->lineEdit->setText(textValue);
    if (ui->lineEdit->text() == "-" || ui->lineEdit->text().isEmpty()) {
        ui->lineEdit->setText("0");
    }
}


// Pocitanie docasnych vypoctov
// Funkcia volana z PlusMinusClicked(), MultDivClicked(), EqualsClicked()
// Berie docasny vysledok a znamienko operacie ako argument

void MainWindow::Calculator(double temporaryNumber, QString inLineSign) {
    if (inLineSign == "+") {
        temporaryResult += temporaryNumber;
    }
    else if (inLineSign == "-") {
        temporaryResult -= temporaryNumber;
    }
    else if (inLineSign == "*") {
        temporaryMultDivResult *= temporaryNumber;
    }
    else if (inLineSign == "/") {       // DORIESIT DELENIE NULOU
        temporaryMultDivResult /= temporaryNumber;
    }
}


// Kliknutie Krat a Deleno.
// buttonSign = znamieko zvolenej operacie (* alebo /)
// Tieto operacie maju prednost pred + a -,
// kontrolujeme teda iba oper. znamienko v inLineMultDivSign, vykona vypocet
// Nastavi newNumber na True aby sa po kliknuti na cislo zadavala nova hodnota

void MainWindow::MultDivClicked() {
    PushButton *buttonText = (PushButton *)sender();
    QString buttonSign = buttonText->text();
    double numberOnDisplay = ui->lineEdit->text().toDouble();

    if (!inLineMultDivSign.isEmpty()) {
        Calculator(numberOnDisplay, inLineMultDivSign);
    }
    else {
        temporaryMultDivResult = numberOnDisplay;
    }
    inLineMultDivSign = buttonSign;
    newNumber = true;
}



// Kliknutie na Plus a Minus
// buttonSign = znamieko zvolenej operacie (+ alebo -)
// Ak bola predosla operacia * alebo / (ulozene v inLineMultDivSign), vykona tento vypocet ako prvy
// Potom pokracuje s + a - operaciami
// Nastavi newNumber na True aby sa po kliknuti na cislo zadavala nova hodnota

void MainWindow::PlusMinusClicked() {

    PushButton *buttonText = (PushButton *)sender();
    QString buttonSign = buttonText->text();
    double numberOnDisplay = ui->lineEdit->text().toDouble();

    if (!inLineMultDivSign.isEmpty()) {
        Calculator(numberOnDisplay, inLineMultDivSign);
        numberOnDisplay = temporaryMultDivResult;
        temporaryMultDivResult = 0.0;
        inLineMultDivSign.clear();
    }
    if (!inLinePlusMinusSign.isEmpty()) {
        Calculator(numberOnDisplay, inLinePlusMinusSign);
    }
    else {
        temporaryResult = numberOnDisplay;
    }
    inLinePlusMinusSign = buttonSign;
    newNumber = true;
}


// Zobrazi vysledok
// Vid funckie PlusMinusClicked() a MultDivClicked()

void MainWindow::EqualsClicked() {
    double numberOnDisplay = ui->lineEdit->text().toDouble();

    if (!inLineMultDivSign.isEmpty()) {
        Calculator(numberOnDisplay, inLineMultDivSign);
        numberOnDisplay = temporaryMultDivResult;
        temporaryMultDivResult = 0.0;
        inLineMultDivSign.clear();
    }
    if (!inLinePlusMinusSign.isEmpty()) {
        Calculator(numberOnDisplay, inLinePlusMinusSign);
        inLinePlusMinusSign.clear();
    }
    else {
        temporaryResult = numberOnDisplay;
    }

    // vypis vysledok, vymaz docasny vypocet
    ui->lineEdit->setText(QString::number(temporaryResult));
    temporaryResult = 0.0;
    newNumber = true;
}

