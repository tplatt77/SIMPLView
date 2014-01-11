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
 *                           FA8650-10-D-5210
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "InputPathWidget.h"

#include <QtCore/QMetaProperty>
#include <QtCore/QDir>

#include <QtGui/QFileDialog>

#include "FilterWidgetsLib/Widgets/moc_InputPathWidget.cxx"


// Initialize private static member variable
QString InputPathWidget::m_OpenDialogLastDirectory = "";
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
InputPathWidget::InputPathWidget(FilterParameter* parameter, AbstractFilter* filter, QWidget* parent) :
  QWidget(parent),
  m_Filter(filter),
  m_FilterParameter(parameter)
{
  setupUi(this);
  setupGui();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
InputPathWidget::~InputPathWidget()
{}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void InputPathWidget::setupGui()
{
  if (m_FilterParameter != NULL)
  {
    inputPathWidgetLabel->setText(m_FilterParameter->getHumanLabel() );
  }
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void InputPathWidget::on_inputPathWidgetLineEdit_textChanged(const QString& text)
{
 bool ok = m_Filter->setProperty(m_FilterParameter->getPropertyName().toStdString().c_str(), text);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void InputPathWidget::on_inputPathWidgetPushButton_clicked()
{
  QString currentPath = m_Filter->property(m_FilterParameter->getPropertyName().toStdString().c_str()).toString();
  if(currentPath.isEmpty() == true)
  {
    currentPath = m_OpenDialogLastDirectory;
  }
  QString Ftype = m_FilterParameter->getFileType();
  QString ext = m_FilterParameter->getFileExtension();
  QString s = Ftype + QString(" Files (") + ext + QString(");;All Files(*.*)");
  QString defaultName = currentPath + QDir::separator() + "Untitled";
  QString file = QFileDialog::getExistingDirectory(this,
                                                   tr("Select Input Folder"),
                                                   defaultName,
                                                   QFileDialog::ShowDirsOnly);

  if(true == file.isEmpty())
  {
    return;
  }
  bool ok = false;
  file = QDir::toNativeSeparators(file);
  // Store the last used directory into the private instance variable
  QFileInfo fi(file);
  m_OpenDialogLastDirectory = fi.path();

  on_inputPathWidgetLineEdit_textChanged(file);
}
