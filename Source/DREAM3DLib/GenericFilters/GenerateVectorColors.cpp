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
#include "GenerateVectorColors.h"


#include "DREAM3DLib/Math/MatrixMath.h"
#include "DREAM3DLib/Math/OrientationMath.h"
#include "DREAM3DLib/Math/MatrixMath.h"
#include "DREAM3DLib/Utilities/ColorTable.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
GenerateVectorColors::GenerateVectorColors() :
  AbstractFilter(),
  m_DataContainerName(DREAM3D::Defaults::VolumeDataContainerName),
  m_CellAttributeMatrixName(DREAM3D::Defaults::CellAttributeMatrixName),
  m_SelectedVectorArrayName(""),
  m_VectorsArrayName(""),
  m_Vectors(NULL),
  m_CellVectorColorsArrayName(DREAM3D::CellData::VectorColor),
  m_CellVectorColors(NULL),
  m_GoodVoxelsArrayName(DREAM3D::CellData::GoodVoxels),
  m_GoodVoxels(NULL)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
GenerateVectorColors::~GenerateVectorColors()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateVectorColors::setupFilterParameters()
{
  FilterParameterVector parameters;
  {
    FilterParameter::Pointer parameter = FilterParameter::New();
    parameter->setHumanLabel("Vector Array Name");
    parameter->setPropertyName("SelectedVectorArrayName");
    parameter->setWidgetType(FilterParameterWidgetType::SingleArraySelectionWidget);
    parameter->setValueType("QString");
    parameter->setUnits("");
    parameters.push_back(parameter);
  }
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateVectorColors::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  /* Code to read the values goes between these statements */
  /* FILTER_WIDGETCODEGEN_AUTO_GENERATED_CODE BEGIN*/
  setSelectedVectorArrayName( reader->readString( "SelectedVectorArrayName", getSelectedVectorArrayName() ) );
  /* FILTER_WIDGETCODEGEN_AUTO_GENERATED_CODE END*/
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int GenerateVectorColors::writeFilterParameters(AbstractFilterParametersWriter* writer, int index)
{
  writer->openFilterGroup(this, index);
  /* Place code that will write the inputs values into a file. reference the
   AbstractFilterParametersWriter class for the proper API to use. */
  writer->writeValue("SelectedVectorArrayName", getSelectedVectorArrayName() );
  writer->closeFilterGroup();
  return ++index; // we want to return the next index that was just written to
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateVectorColors::dataCheck()
{
  setErrorCondition(0);

  VolumeDataContainer* m = getDataContainerArray()->getPrereqDataContainer<VolumeDataContainer, AbstractFilter>(this, getDataContainerName(), false);
  if(getErrorCondition() < 0 || NULL == m) { return; }
  AttributeMatrix::Pointer cellAttrMat = m->getPrereqAttributeMatrix<AbstractFilter>(this, getCellAttributeMatrixName(), -301);
  if(getErrorCondition() < 0 || NULL == cellAttrMat.get() ) { return; }

  if(m_SelectedVectorArrayName.isEmpty() == true)
  {
    setErrorCondition(-11000);
    notifyErrorMessage(getHumanLabel(), "An array from the Volume DataContainer must be selected.", getErrorCondition());
  }
  else
  {

    QString dcName;
    QString amName;
    QString daName;

    QStringList tokens = m_SelectedVectorArrayName.split(DREAM3D::PathSep);
    // We should end up with 3 Tokens
    if(tokens.size() != 3)
    {
      setErrorCondition(-11002);
      QString ss = QObject::tr("The path to the Attribute Array is malformed. Each part should be separated by a '|' character.");
      notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    }
    else
    {
      dcName = tokens.at(0);
      amName = tokens.at(1);
      daName = tokens.at(2);

      DataContainer::Pointer dc = getDataContainerArray()->getDataContainer(dcName);
      if(NULL == dc.get())
      {
        setErrorCondition(-11003);
        QString ss = QObject::tr("The DataContainer '%1' was not found in the DataContainerArray").arg(dcName);
        notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
        return;
      }

      AttributeMatrix::Pointer attrMat = dc->getAttributeMatrix(amName);
       if(NULL == attrMat.get())
      {
        setErrorCondition(-11004);
        QString ss = QObject::tr("The AttributeMatrix '%1' was not found in the DataContainer '%2'").arg(amName).arg(dcName);
        notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
        return;
      }

      m_VectorsArrayName = daName;

      QVector<size_t> dims(1, 3);
      m_VectorsPtr = cellAttrMat->getPrereqArray<DataArray<float>, AbstractFilter>(this, m_VectorsArrayName, -303, dims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
      if( NULL != m_VectorsPtr.lock().get() ) /* Validate the Weak Pointer wraps a non-NULL pointer to a DataArray<T> object */
      { m_Vectors = m_VectorsPtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */
    }
  }

  QVector<size_t> dims(1, 3);
  m_CellVectorColorsPtr = cellAttrMat->createNonPrereqArray<DataArray<uint8_t>, AbstractFilter, uint8_t>(this, m_CellVectorColorsArrayName, 0, dims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if( NULL != m_CellVectorColorsPtr.lock().get() ) /* Validate the Weak Pointer wraps a non-NULL pointer to a DataArray<T> object */
  { m_CellVectorColors = m_CellVectorColorsPtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */
  // The good voxels array is optional, If it is available we are going to use it, otherwise we are going to create it
  dims[0] = 1;
  m_GoodVoxelsPtr = cellAttrMat->getPrereqArray<DataArray<bool>, AbstractFilter>(NULL, m_GoodVoxelsArrayName, -304, dims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if( NULL != m_GoodVoxelsPtr.lock().get() ) /* Validate the Weak Pointer wraps a non-NULL pointer to a DataArray<T> object */
  { m_GoodVoxels = m_GoodVoxelsPtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */
  else {m_GoodVoxels = NULL;}
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateVectorColors::preflight()
{
  emit preflightAboutToExecute();
  emit updateFilterParameters(this);
  dataCheck();
  emit preflightExecuted();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void GenerateVectorColors::execute()
{
  int err = 0;
  QString ss;
  setErrorCondition(err);

  dataCheck();
  if(getErrorCondition() < 0) { return; }

  VolumeDataContainer* m = getDataContainerArray()->getDataContainerAs<VolumeDataContainer>(getDataContainerName());

  int64_t totalPoints = m->getAttributeMatrix(getCellAttributeMatrixName())->getNumTuples();

  bool missingGoodVoxels = true;
  if (NULL != m_GoodVoxels)
  {
    missingGoodVoxels = false;
  }

  int phase;
  size_t index = 0;

  DREAM3D::Rgb argb = 0x00000000;

  // Write the IPF Coloring Cell Data
  for (int64_t i = 0; i < totalPoints; i++)
  {
    index = i * 3;
    m_CellVectorColors[index] = 0;
    m_CellVectorColors[index + 1] = 0;
    m_CellVectorColors[index + 2] = 0;


    float dir[3];
    float r, g, b;
    DREAM3D::Rgb argb;
    if(missingGoodVoxels == true || m_GoodVoxels[i] == true)
    {
      dir[0] = m_Vectors[index+0];
      dir[1] = m_Vectors[index+1];
      dir[2] = m_Vectors[index+2];
      MatrixMath::Normalize3x1(dir);
      if(dir[2] < 0) MatrixMath::Multiply3x1withConstant(dir, -1);
      float trend = atan2f(dir[1], dir[0])*(180.0/DREAM3D::Constants::k_Pi);
      float plunge = acosf(dir[2])*(180.0/DREAM3D::Constants::k_Pi);
      if(trend < 0.0) trend += 360.0;
      if(trend <= 120.0)
      {
        r = 255.0*((120.0-trend)/120.0);
        g = 255.0*(trend/120.0);
        b = 0.0;
      }
      if(trend > 120.0 && trend <= 240.0)
      {
        trend -= 120.0;
        r = 0.0;
        g = 255.0*((120.0-trend)/120.0);
        b = 255.0*(trend/120.0);
      }
      if(trend > 240.0 && trend < 360.0)
      {
        trend -= 240.0;
        r = 255.0*(trend/120.0);
        g = 0.0;
        b = 255.0*((120.0-trend)/120.0);
      }
      float deltaR = 255.0-r;
      float deltaG = 255.0-g;
      float deltaB = 255.0-b;
      r += (deltaR*((90.0-plunge)/90.0));
      g += (deltaG*((90.0-plunge)/90.0));
      b += (deltaB*((90.0-plunge)/90.0));
      if(r > 255.0) r = 255.0;
      if(g > 255.0) g = 255.0;
      if(b > 255.0) b = 255.0;
      argb = RgbColor::dRgb(r, g, b, 255);
      m_CellVectorColors[index] = RgbColor::dRed(argb);
      m_CellVectorColors[index + 1] = RgbColor::dGreen(argb);
      m_CellVectorColors[index + 2] = RgbColor::dBlue(argb);
    }
  }

  /* Let the GUI know we are done with this filter */
  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer GenerateVectorColors::newFilterInstance(bool copyFilterParameters)
{
  /*
  * FlattenMethod
  */
  GenerateVectorColors::Pointer filter = GenerateVectorColors::New();
  if(true == copyFilterParameters)
  {
    filter->setSelectedVectorArrayName( getSelectedVectorArrayName() );
  }
  return filter;
}