/* ============================================================================
 * Copyright (c) 2012 Michael A. Jackson (BlueQuartz Software)
 * Copyright (c) 2012 Dr. Michael A. Groeber (US Air Force Research Laboratories)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of Michael A. Groeber, Michael A. Jackson, the US Air Force,
 * BlueQuartz Software nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  This code was written under United States Air Force Contract number
 *                           FA8650-07-D-5800
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef _QFILTERWIDGET_H_
#define _QFILTERWIDGET_H_

#include <QtCore/QStringList>
#include <QtCore/QSettings>
#include <QtCore/QUrl>

#include <QtGui/QFrame>
#include <QtGui/QMenu>


#include "DREAM3DLib/Common/AbstractFilter.h"
#include "DREAM3DLib/FilterParameters/FilterParameter.h"

#include "FilterWidgetsLib/FilterWidgetsLib.h"

#include "FilterWidgetsLib/ui_QFilterWidget.h"

// This needs to be defined
class QMouseEvent;
class AbstractFilter;
class QFormLayout;
class QLabel;
class QComboBox;

/**
 * @class QFilterWidget QFilterWidget.h FilterWidgets/QFilterWidget.h
 * @brief  This class is a subclass of the QGroupBox class and is used to display
 * Filter Options that the user can set. This class is capable of constructing a
 * default GUI widget set for each type of Filter Option that is available. If
 * the programmer needs more specialized widgets then they can simply subclass
 * this class and over ride or implement their custom code.
 * @author Michael A. Jackson for BlueQuartz Software
 * @date Jan 6, 2012
 * @version 1.0
 */
class FilterWidgetsLib_EXPORT QFilterWidget : public QFrame, private Ui::QFilterWidget
{
    Q_OBJECT
  public:
    QFilterWidget(QWidget* parent = NULL);
    QFilterWidget(AbstractFilter::Pointer filter, IObserver* observer = NULL, QWidget* parent = NULL);
    virtual ~QFilterWidget();


    virtual AbstractFilter::Pointer getFilter();

    virtual void setBorderColorStyle(QString s);
    virtual QString getBorderColorStyle();
    virtual bool isSelected();

    virtual QString getHumanLabel();
    virtual QString  getFilterGroup();
    virtual QString  getFilterSubGroup();

    static QString getOpenDialogLastDirectory() { return m_OpenDialogLastDirectory; }
    static void setOpenDialogLastDirectory(QString val) { m_OpenDialogLastDirectory = val; }

    virtual void getGuiParametersFromFilter(AbstractFilter* filt);

    QVector<QWidget*>& getFilterParameterWidgets();

    QWidget* getScrollWidgetContents();

  signals:
    void dragStarted(QFilterWidget* widget);
    void parametersChanged();

  public slots:


    virtual void setIsSelected(bool b);


    /**
      * @brief Sets the style of the Widget to indicate a selected or non-selected
      * state
      * @param selected Is the widget selected or not.
      */
    void changeStyle();

    /**
      *@brief
      */
    void updateWidgetStyle();

    /**
      * @brief Slot for a QTime to call to update the border of the widget in a
      * pulsing fashion
      */
    void setHasPreflightErrors(bool hasErrors);

    void setHasPreflightWarnings(bool hasWarnings);

  protected slots:
    void on_deleteBtn_clicked();
    void on_helpBtn_clicked();

  signals:
    void widgetSelected(QFilterWidget* w);
    void filterWidgetRemoved(QFilterWidget* widget);


  protected:
    virtual void mousePressEvent( QMouseEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent* event );
    virtual void mouseMoveEvent( QMouseEvent* event );



  private:
    QRect                     m_DeleteRect;
    QPoint                    dragStartPosition;
    QTimer*                   m_timer;
    unsigned char             m_CurrentBorderColorFactor;
    unsigned char             m_BorderIncrement;
    QString                   m_BorderColorStyle;
    bool                      m_IsSelected;
    bool                      m_HasPreflightErrors;
    bool                      m_HasPreflightWarnings;
    static QString            m_OpenDialogLastDirectory;
    AbstractFilter::Pointer   m_Filter;
    QVector<QWidget*>         m_FilterParameterWidgets;
    QWidget*                  m_BasicInputsScrollWidget;
    IObserver*                m_Observer;

    /**
     * @brief initialize Calls all the necessary initialization code for the widget
     * @param filter
     */
    void initialize(AbstractFilter::Pointer filter);

    QFilterWidget(const QFilterWidget&); // Copy Constructor Not Implemented
    void operator=(const QFilterWidget&); // Operator '=' Not Implemented
};

#endif /* _QFILTERWIDGET_H_ */
