#ifndef CTAB_H
#define CTAB_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include "formulas.h"

class cTab : public QWidget
{
    Q_OBJECT
public:
    cTab(Figuras tipo, QWidget *parent = 0);
private:
    QLabel *formula;
    QList<QLineEdit*> constantes;
    QPushButton *boton;
    Figuras tipo;

    QHBoxLayout* constante(QString nombre, double valor = 0);
signals:
    void cambiar_formula(Figuras tipo, QList<double> constantes);
public slots:
    void boton_click();
};

#endif 
