/* ============================================================================
* Copyright (c) 2012 Michael A. Jackson (BlueQuartz Software)
* Copyright (c) 2012 Dr. Michael A. Groeber (US Air Force Research Laboratories)
* Copyright (c) 2012 Joseph B. Kleingers (Student Research Assistant)
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
* Neither the name of Michael A. Groeber, Michael A. Jackson, Joseph B. Kleingers,
* the US Air Force, BlueQuartz Software nor the names of its contributors may be
* used to endorse or promote products derived from this software without specific
* prior written permission.
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

#ifndef _DREAM3DApplication_H_
#define _DREAM3DApplication_H_

#include <QtCore/QObject>
#include <QtCore/QVector>

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>

#include "DREAM3DLib/DREAM3DLib.h"
//#include "DREAM3DLib/Plugin/IDREAM3DPlugin.h"

#define dream3dApp (static_cast<DREAM3DApplication *>(qApp))

class DSplashScreen;
class DREAM3D_UI;
class QPluginLoader;
class IDREAM3DPlugin;

class DREAM3DApplication : public QApplication
{
  Q_OBJECT

public:
  DREAM3DApplication(int & argc, char ** argv);
  virtual ~DREAM3DApplication();

  bool initialize(int argc, char* argv[]);

  QList<QWidget*> getDREAM3DWindowList();

  void registerDREAM3DWindow(QWidget* widget);
  void unregisterDREAM3DWindow(QWidget* widget);

protected:
  bool event(QEvent* event);

  DREAM3D_UI* getNewDREAM3DInstance();

protected slots:

  /**
   * @brief Updates the QMenu 'Recent Files' with the latest list of files. This
   * should be connected to the Signal QRecentFileList->fileListChanged
   * @param file The newly added file.
   */
  void updateRecentFileList(const QString &file);

  /**
  * @brief exitTriggered
  */
  void exitTriggered();

  /**
  * @brief activeWindowChanged
  */
  void activeWindowChanged(DREAM3D_UI* instance);

  // DREAM3D_UI slots
  void openRecentFile();

  void on_m_ActionNew_triggered();
  void on_m_ActionOpen_triggered();
  void on_m_ActionClearRecentFiles_triggered();
  void on_m_ActionShowIndex_triggered();
  void on_m_ActionCheck_For_Updates_triggered();
  void on_m_ActionLicense_Information_triggered();
  void on_m_ActionAbout_DREAM3D_triggered();
  void on_m_ActionExit_triggered();

private:
  QList<QWidget*>                 m_DREAM3DWidgetList;
  DREAM3D_UI*                     m_ActiveWindow;

  QMenuBar*                       m_GlobalMenu;
  QMenu*                          menuFile;
  QMenu*                          menu_RecentFiles;
  QMenu*                          menuView;
  QMenu*                          menuBookmarks;
  QMenu*                          menuHelp;
  QMenu*                          menuPipeline;
  QAction*                        actionShow_Filter_Library;
  QAction*                        actionShow_Prebuilt_Pipelines;
  QAction*                        actionOpen;
  QAction*                        actionNew;
  QAction*                        actionSavePipelineAs;
  QAction*                        actionShow_Favorites;
  QAction*                        actionClearPipeline;
  QAction*                        actionClearRecentFiles;
  QAction*                        actionShow_Issues;
  QAction*                        actionShow_Filter_List;
  QAction*                        actionShowIndex;
  QAction*                        actionLicense_Information;
  QAction*                        actionAbout_DREAM3D;
  QAction*                        actionCheck_For_Updates;
  QAction*                        actionExit;
  QAction*                        actionSaveAsNewFavorite;
  QAction*                        actionCopyCurrentFilter;
  QAction*                        actionAppendToExistingFavorite;
  QAction*                        actionUpdateFavorite;
  QAction*                        actionPasteCopiedFilter;
  QAction*                        actionRemoveCurrentFilter;
  QAction*                        actionPlugin_Information;
  QAction*                        actionSave;
  QAction*                        actionSaveAs;

  QString                         m_OpenDialogLastDirectory;

  bool                            show_splash;
  DSplashScreen*                  Splash;
  DREAM3D_UI*                     MainWindow;
  QVector<QPluginLoader*>         m_PluginLoaders;

  void initializeApplication();
  void initializeGlobalMenu();
  QVector<IDREAM3DPlugin*> loadPlugins();

  DREAM3DApplication(const DREAM3DApplication&); // Copy Constructor Not Implemented
  void operator=(const DREAM3DApplication&); // Operator '=' Not Implemented
};

#endif /* _DREAM3DApplication_H */

