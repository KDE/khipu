
#ifndef GRAPH2D_H
#define GRAPH2D_H

#include "analitzaplot/plotview2d.h"


#endif

/*

#ifndef GRAPH2D_H
#define GRAPH2D_H


#include <QResizeEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QPixmap>
#include <QModelIndex>
#include <QTimer>



#include "keomath/function/function.h"

namespace Keomath
{


class FunctionsFilterProxyModel;

class Solver2D;

class ConstantSymbol
{
public:
    ConstantSymbol() {}
    ConstantSymbol(const QString &sign, const QString &numerator,
                   const QString &denominator, qreal value)
        : m_sign(sign)
        , m_numerator(numerator)
        , m_denominator(denominator)
        , m_value(value)
    {

    }
    ConstantSymbol(const ConstantSymbol &other)
        : m_sign(other.m_sign)
        , m_numerator(other.m_numerator)
        , m_denominator(other.m_denominator)
        , m_value(other.m_value)
    {

    }

    QString sign() const
    {
        return m_sign;
    }
    QString numerator() const
    {
        return m_numerator;
    }
    QString denominator() const
    {
        return m_denominator;
    }
    qreal value() const
    {
        return m_value;
    }

private:
    QString m_sign;
    QString m_numerator;
    QString m_denominator;
    qreal m_value;
};



class KEOMATH_EXPORT View2D : public QWidget
{
    Q_OBJECT


private:
    bool m_forceDrawFunction;
public:
    
    enum GraphMode
    {
        None=0,		
        Pan,		
        Selection	
    };



    
    explicit View2D(QWidget *parent = 0);



    
    ~View2D();


    void forceDrawFunctionsOnBuffer();

    void setFunctionsModel(FunctionsFilterProxyModel *functionsFilterProxyModel);


    QSize sizeHint() const
    {
        return QSize(100, 100);
    }

    
    bool toImage(const QString& path);







    
    void unselect();

    
    bool isFramed() const
    {
        return m_framed;
    }

    
    void setFramed(bool fr)
    {
        m_framed=fr;
    }

    
    bool isReadOnly() const
    {
        return m_readonly;
    }

    
    void setReadOnly(bool ro);

    
    void setKeepAspectRatio(bool ar);

    
    bool keepAspectRatio() const
    {
        return m_keepRatio;
    }

    
    QRectF definedViewport() const;

    void snapshotToClipboard();



public slots:
    


    
    void resetViewport()
    {
        setViewport(defViewport);
    }

    
    void zoomIn();

    
    void zoomOut();

    
    void updateFuncs( const QModelIndex & start, const QModelIndex& end );

    
    void setViewport(const QRectF& v, bool repaint=true);

    void setXAxisLabel(const QString &label);
    void setYAxisLabel(const QString &label);

    void improvedResolutionForCurrentViewport();

    
    void updateTickScale(QString m_tickScaleSymbol, qreal m_tickScaleSymbolValue,
                          int m_tickScaleNumerator,
                         int m_tickScaleDenominator);

    void setUseTickSymbols(bool flag)
    {
        m_tickScaleUseSymbols = flag;
        update();
    }
    void showHTicks(bool flag)
    {
        m_showHTicks = flag;
        update();
    }
    void showVTicks(bool flag)
    {
        m_showVTicks = flag;
        update();
    }
    void showHAxes(bool flag)
    {
        m_showHAxes = flag;
        update();
    }
    void showVAxes(bool flag)
    {
        m_showVAxes = flag;
        update();
    }
    void showRTang(bool flag)
    {
        m_showRTang = flag;
        update();
    }
    void showRNorm(bool flag)
    {
        m_showRNorm = flag;
        update();
    }
    void useCoorSys(int i)
    {
        m_useCoordSys = i;
        update();
    }





    void setGridColor(const QColor &color)
    {
        m_gridColor = color;
        update();
    }
    void setMeshGridShown(bool v)
    {
        m_meshGridShown = v;
        update();
    }

signals:
    
    void status(const QString &msg);

    void viewportChanged(const QRectF&);

    void viewClicked();

    void coordSysUsed(int i); 

private slots:
    void addFuncs( const QModelIndex & parent, int start, int end);
    void removeFuncs( const QModelIndex & parent, int start, int end);
    void timerWork();

private:
    static const QColor m_axeColor;
    static const QColor m_axe2Color;
    static const QColor m_derivativeColor;



    QString m_axisXLabel;
    QString m_axisYLabel;



    
    FunctionsFilterProxyModel *m_functionsFilterProxyModel;
    QPixmap buffer;

    
    
    
    
    
    





    QLabel *micepos;
    QPointF mark;
    void drawAxes(QPainter*, Function::Axe a);
    void drawPolarAxes(QPainter*);
    void drawCartesianAxes(QPainter*);
    QPointF toWidget(const QPointF &) const;
    QPointF fromWidget(const QPoint& p) const;
    QPointF toViewport(const QPoint& mv) const;
    QPointF calcImage(const QPointF& dp);
    QLineF slope(const QPointF& dp) const;
    QLineF slopeNormal(const QPointF& dn) const;
    double absF(double n1, double n2);

    double devFuncion(int tipo, double val);

    QLineF toWidget(const QLineF &) const;

    
    void paintEvent( QPaintEvent * );
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent * e );
    void wheelEvent(QWheelEvent *e);
    void resizeEvent(QResizeEvent *);
    GraphMode mode;
    QPoint press;
    QPoint last;
    QTimer *ah;
    int zw;
    int ubic;

    

    bool m_keepRatio;
    double rang_x, rang_y;
    QPointF ant;
    QRectF viewport;
    QRectF userViewport;
    QRectF defViewport;
    void drawFunctions(QPaintDevice*);
    void drawFunctionsArea(QPaintDevice*);
    void drawFunctionsAreaE(QPaintDevice*);
    double abs2(double n);

    void updateScale(bool repaint=true);

    void sendStatus(const QString& msg)
    {
        emit status(msg);
    }
    bool m_framed;
    bool m_readonly;
    QString m_posText;

    
    static QRect toBiggerRect(const QRectF&);

    
    qreal millimetersToPixels(qreal width_mm, QPaintDevice *device) const;


    
    bool m_showHTicks;
    bool m_showVTicks;


    bool m_showHAxes;
    bool m_showVAxes;
    bool m_showRTang;
    bool m_showRNorm;
    bool m_showR;
    double penTang;
    double penNorm;
    double liDomA;
    double lsDomA;
    int m_useCoordSys; 








    










private:


    int offX;
    int offY;

    QPoint lastpos;


    int getOffX()
    {
        return offX;
    }

    int getOffY()
    {
        return offY;
    }
    void setOffset(int _offX, int _offY)
    {
        offX = _offX;
        offY = _offY;


    }
    void shiftCanvas(int _xdiff, int _ydiff)
    {
        int _xoff = getOffX();
        int _yoff = getOffY();
        _xoff += _xdiff;
        _yoff += _ydiff;
        setOffset(_xoff, _yoff);


    }






private:
    QUuid m_spaceId;



    QMap<int, ConstantSymbol> m_constants;

private: 

    QString m_tickScaleSymbol;
    qreal m_tickScaleSymbolValue;
    bool m_tickScaleUseSymbols; 
    int m_tickScaleNumerator;
    int m_tickScaleDenominator;

    QColor m_gridColor;
    bool m_meshGridShown;





    QPointF lastMark;
    QPointF m_mousePos;


};

} 

#endif*/
