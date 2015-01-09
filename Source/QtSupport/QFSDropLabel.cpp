/* ============================================================================
 * Copyright (c) 2011, Michael A. Jackson (BlueQuartz Software)
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
 * Neither the name of Michael A. Jackson nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
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
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "QFSDropLabel.h"

#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QUrl>
#include <QtCore/QTextStream>
#include <QtGui/QWidget>
#include <QtGui/QDragEnterEvent>




// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QFSDropLabel::QFSDropLabel(QWidget* parent)
  : QLabel(parent)
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void QFSDropLabel::changeStyleSheet(int style)
{
  QString styleSheet;
  QTextStream ss(&styleSheet);

  ss << "QLabel {";
#if defined(Q_OS_WIN)
  ss << "font: italic 9 pt \"Arial\";";
#elif defined(Q_OS_MAC)
  ss << "font: italic 12 pt \"Arial\";";
#else
  ss << "font: italic 10 pt \"Arial\";";
#endif

  if(style == FS_STANDARD_STYLE)
  {

  }
  else if(style == FS_DRAGGING_STYLE)
  {
    ss << "border: 2px solid rgb(34, 170, 46);";
    ss << "border-radius: 5px;";
  }
  else if(style == FS_DOESNOTEXIST_STYLE)
  {
    ss << "color: rgb(200, 50, 50); font: bold;";
  }
  else if(style == FS_WARNING_STYLE)
  {
    ss << "color: rgb(255, 140, 0); font: bold;";
  }


  ss << "}";

  setStyleSheet(styleSheet);
}



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void QFSDropLabel::dragEnterEvent(QDragEnterEvent* event)
{
  // accept just text/uri-list mime format
  if (event->mimeData()->hasFormat("text/uri-list"))
  {
    event->acceptProposedAction();
    changeStyleSheet(FS_DRAGGING_STYLE);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void QFSDropLabel::dragLeaveEvent(QDragLeaveEvent *event)
{
  changeStyleSheet(FS_STANDARD_STYLE);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void QFSDropLabel::dropEvent(QDropEvent* event)
{
  QList<QUrl> urlList;
  QString fName;
  QFileInfo info;

  if (event->mimeData()->hasUrls())
  {
    urlList = event->mimeData()->urls(); // returns list of QUrls
    // if just text was dropped, urlList is empty (size == 0)

    if ( urlList.size() > 0) // if at least one QUrl is present in list
    {
      fName = urlList[0].toLocalFile(); // convert first QUrl to local path
      fName = QDir::toNativeSeparators(fName);
      info.setFile( fName ); // information about file
      setText( fName ); // if is file, setText
      emit fileDropped(fName);
    }
  }
  changeStyleSheet(FS_STANDARD_STYLE);

  event->acceptProposedAction();
}

