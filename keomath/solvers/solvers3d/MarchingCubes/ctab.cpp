#include "ctab.h"
#include <QtGui>

cTab::cTab(Figuras tipo, QWidget *parent):
    QWidget(parent)
{
    this->tipo = tipo;
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    formula = new QLabel(this);
    formula->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    boton = new QPushButton("Usar",this);
    boton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    connect(boton,SIGNAL(clicked()),this,SLOT(boton_click()));

    mainLayout->addWidget(formula);
    switch(tipo)
    {
        
    case esfera:
        formula->setText("(x-x0)^2 + (y-y0)^2 + (z-z0)^2 = r^2");
        mainLayout->addLayout(constante("x0 = ",0.5));
        mainLayout->addLayout(constante("y0 = ",0.5));
        mainLayout->addLayout(constante("z0 = ",0.5));
        mainLayout->addLayout(constante("r = ",0.25));
        break;
    case elipsoide:
        formula->setText("((x-x0)/a)^2 + ((y-y0)/b)^2 + ((z-z0)/c)^2 = 1");
        mainLayout->addLayout(constante("x0 = ",0.5));
        mainLayout->addLayout(constante("y0 = ",0.5));
        mainLayout->addLayout(constante("z0 = ",0.5));
        mainLayout->addLayout(constante("a = ",0.3));
        mainLayout->addLayout(constante("b = ",0.4));
        mainLayout->addLayout(constante("c = ",0.35));
        break;
    case cilindro:
        formula->setText("(x-x0)^2 + (y-y0)^2 = r^2 | z0 < z < z1");
        mainLayout->addLayout(constante("x0 = ",0.5));
        mainLayout->addLayout(constante("y0 = ",0.5));
        mainLayout->addLayout(constante("z0 = ",0.25));
        mainLayout->addLayout(constante("z1 = ",0.75));
        mainLayout->addLayout(constante("r = ",0.25));
        break;
    case hiperboloide_1hoja:
        formula->setText("((x-x0)/a)^2 + ((y-y0)/b)^2 - ((z-z0)/c)^2 = 1");
        mainLayout->addLayout(constante("x0 = ",0.5));
        mainLayout->addLayout(constante("y0 = ",0.5));
        mainLayout->addLayout(constante("z0 = ",0.5));
        mainLayout->addLayout(constante("a = ",0.14));
        mainLayout->addLayout(constante("b = ",0.14));
        mainLayout->addLayout(constante("c = ",0.14));
        break;
    case hiperboloide_2hojas:
        formula->setText("((x-x0)/a)^2 + ((y-y0)/b)^2 - ((z-z0)/c)^2 = -1");
        mainLayout->addLayout(constante("x0 = ",0.5));
        mainLayout->addLayout(constante("y0 = ",0.5));
        mainLayout->addLayout(constante("z0 = ",0.5));
        mainLayout->addLayout(constante("a = ",0.14));
        mainLayout->addLayout(constante("b = ",0.14));
        mainLayout->addLayout(constante("c = ",0.14));
        break;
    case paraboloide_hiperbolico:
        formula->setText("((x-x0)/a)^2 - ((y-y0)/b)^2 = z-z0");
        mainLayout->addLayout(constante("x0 = ",0.5));
        mainLayout->addLayout(constante("y0 = ",0.5));
        mainLayout->addLayout(constante("z0 = ",0.5));
        mainLayout->addLayout(constante("a = ",0.3));
        mainLayout->addLayout(constante("b = ",0.3));
        break;
    }
    mainLayout->addWidget(boton);
    setLayout(mainLayout);
}

QHBoxLayout* cTab::constante(QString nombre, double valor)
{
    QString numero;
    QLabel *label;
    QLineEdit *edit;

    numero.setNum(valor);
    label = new QLabel(nombre,this);
    edit = new QLineEdit(numero,this);

    label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    edit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(label);
    constantes.append(edit);
    layout->addWidget(edit);
    return layout;
}

void cTab::boton_click()
{
    QList<double> lista;
    QLineEdit *temp;
    bool bien;
    foreach(temp,constantes)
    {
        
        lista.append(temp->text().toDouble(&bien));
        if(!bien)
        {
            
            return;
        }
    }
    switch(tipo)
    {
        
    case esfera:
        break;
    case elipsoide:
    case hiperboloide_1hoja:
    case hiperboloide_2hojas:
        if(lista.at(3) == 0 || lista.at(4) == 0 || lista.at(5) == 0)
        {
            
            return;
        }
        break;
    case cilindro:
        if(lista.at(2) >= lista.at(3))
        {
            
            return;
        }
        break;
    case paraboloide_hiperbolico:
        if(lista.at(3) == 0 || lista.at(4) == 0)
        {
            
            return;
        }
        break;
    }
    emit cambiar_formula(tipo,lista);
}
