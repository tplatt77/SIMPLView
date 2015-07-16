/* ============================================================================
* Copyright (c) 2009-2015 BlueQuartz Software, LLC
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
* Neither the name of BlueQuartz Software, the US Air Force, nor the names of its
* contributors may be used to endorse or promote products derived from this software
* without specific prior written permission.
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
* The code contained herein was partially funded by the followig contracts:
*    United States Air Force Prime Contract FA8650-07-D-5800
*    United States Air Force Prime Contract FA8650-10-D-5210
*    United States Prime Contract Navy N00173-07-C-2068
*
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


#include "RotateEulerRefFrame.h"

#ifdef DREAM3D_USE_PARALLEL_ALGORITHMS
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <tbb/blocked_range3d.h>
#include <tbb/partitioner.h>
#include <tbb/task_scheduler_init.h>
#endif

#include "DREAM3DLib/FilterParameters/AbstractFilterParametersReader.h"
#include "DREAM3DLib/FilterParameters/AbstractFilterParametersWriter.h"
#include "DREAM3DLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "DREAM3DLib/FilterParameters/FloatVec3FilterParameter.h"
#include "DREAM3DLib/FilterParameters/DoubleFilterParameter.h"
#include "DREAM3DLib/FilterParameters/SeparatorFilterParameter.h"
#include "OrientationLib/OrientationMath/OrientationMath.h"

#include "OrientationAnalysis/OrientationAnalysisConstants.h"

/**
 * @brief The RotateEulerRefFrameImpl class implements a threaded algorithm that rotates an array of Euler
 * angles about the supplied axis-angle pair.
 */
class RotateEulerRefFrameImpl
{
    float* m_CellEulerAngles;
    float angle;
    FloatVec3_t axis;
  public:
    RotateEulerRefFrameImpl(float* data, float rotAngle, FloatVec3_t rotAxis) :
      m_CellEulerAngles(data),
      angle(rotAngle),
      axis(rotAxis)
    {}
    virtual ~RotateEulerRefFrameImpl() {}

    void convert(size_t start, size_t end) const
    {
      float rotMat[3][3] = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
      FOrientArrayType om(9, 0.0f);
      FOrientTransformsType::ax2om(FOrientArrayType(axis.x, axis.y, axis.z, angle), om);
      om.toGMatrix(rotMat);

      float ea1 = 0, ea2 = 0, ea3 = 0;
      float g[3][3] = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } , { 0.0f, 0.0f, 0.0f } };
      float gNew[3][3] = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
      for (size_t i = start; i < end; i++)
      {
        ea1 = m_CellEulerAngles[3 * i + 0];
        ea2 = m_CellEulerAngles[3 * i + 1];
        ea3 = m_CellEulerAngles[3 * i + 2];
        FOrientArrayType om(9);
        FOrientTransformsType::eu2om(FOrientArrayType(ea1, ea2, ea3), om);
        om.toGMatrix(g);

        MatrixMath::Multiply3x3with3x3(g, rotMat, gNew);
        MatrixMath::Normalize3x3(gNew);
        // Because we are going to simply wrap the m_CellEulerAngles array, the new
        // Euler angles will be directly written to the m_CellEulerAngles array
        // at the proper spot
        FOrientArrayType eu( m_CellEulerAngles + (3 * i), 3);
        FOrientTransformsType::om2eu(FOrientArrayType(gNew), eu);
      }
    }

#ifdef DREAM3D_USE_PARALLEL_ALGORITHMS
    void operator()(const tbb::blocked_range<size_t>& r) const
    {
      convert(r.begin(), r.end());
    }
#endif

};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
RotateEulerRefFrame::RotateEulerRefFrame() :
  AbstractFilter(),
  m_RotationAngle(0.0f),
  m_CellEulerAnglesArrayPath("", "", ""),
  m_CellEulerAngles(NULL)
{
  m_RotationAxis.x = 0.0f;
  m_RotationAxis.y = 0.0f;
  m_RotationAxis.z = 1.0f;

  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
RotateEulerRefFrame::~RotateEulerRefFrame()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RotateEulerRefFrame::setupFilterParameters()
{
  FilterParameterVector parameters;
  parameters.push_back(FloatVec3FilterParameter::New("Rotation Axis (ijk)", "RotationAxis", getRotationAxis(), FilterParameter::Parameter));
  parameters.push_back(DoubleFilterParameter::New("Rotation Angle (Degrees)", "RotationAngle", getRotationAngle(), FilterParameter::Parameter));

  {
    DataArraySelectionFilterParameter::DataStructureRequirements req;
    QVector<unsigned int> amTypes;
    amTypes.push_back(DREAM3D::AttributeMatrixType::Cell);
    amTypes.push_back(DREAM3D::AttributeMatrixType::Face);
    amTypes.push_back(DREAM3D::AttributeMatrixType::Edge);
    amTypes.push_back(DREAM3D::AttributeMatrixType::Vertex);
    req.amTypes = amTypes;
    req.daTypes = QVector<QString>(1, DREAM3D::TypeNames::Float);
    req.componentDimensions = QVector< QVector<size_t> >(1, QVector<size_t>(1, 3));
    parameters.push_back(DataArraySelectionFilterParameter::New("Euler Angles", "CellEulerAnglesArrayPath", getCellEulerAnglesArrayPath(), FilterParameter::RequiredArray, req));
  }
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RotateEulerRefFrame::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setCellEulerAnglesArrayPath(reader->readDataArrayPath("CellEulerAnglesArrayPath", getCellEulerAnglesArrayPath() ) );
  setRotationAxis( reader->readFloatVec3("RotationAxis", getRotationAxis() ) );
  setRotationAngle( reader->readValue("RotationAngle", getRotationAngle()) );
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int RotateEulerRefFrame::writeFilterParameters(AbstractFilterParametersWriter* writer, int index)
{
  writer->openFilterGroup(this, index);
  DREAM3D_FILTER_WRITE_PARAMETER(FilterVersion)
  DREAM3D_FILTER_WRITE_PARAMETER(CellEulerAnglesArrayPath)
  DREAM3D_FILTER_WRITE_PARAMETER(RotationAxis)
  DREAM3D_FILTER_WRITE_PARAMETER(RotationAngle)
  writer->closeFilterGroup();
  return ++index; // we want to return the next index that was just written to
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RotateEulerRefFrame::dataCheck()
{
  setErrorCondition(0);

  QVector<size_t> cDims(1, 3);
  m_CellEulerAnglesPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<float>, AbstractFilter>(this, getCellEulerAnglesArrayPath(), cDims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if( NULL != m_CellEulerAnglesPtr.lock().get() ) /* Validate the Weak Pointer wraps a non-NULL pointer to a DataArray<T> object */
  { m_CellEulerAngles = m_CellEulerAnglesPtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RotateEulerRefFrame::preflight()
{
  setInPreflight(true);
  emit preflightAboutToExecute();
  emit updateFilterParameters(this);
  dataCheck();
  emit preflightExecuted();
  setInPreflight(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RotateEulerRefFrame::execute()
{
  setErrorCondition(0);
  dataCheck();
  if(getErrorCondition() < 0) { return; }

  size_t totalPoints = m_CellEulerAnglesPtr.lock()->getNumberOfTuples();

  float rotAngle = m_RotationAngle * DREAM3D::Constants::k_Pi / 180.0f;

#ifdef DREAM3D_USE_PARALLEL_ALGORITHMS
  tbb::task_scheduler_init init;
  bool doParallel = true;
#endif

#ifdef DREAM3D_USE_PARALLEL_ALGORITHMS
  if (doParallel == true)
  {
    tbb::parallel_for(tbb::blocked_range<size_t>(0, totalPoints),
                      RotateEulerRefFrameImpl(m_CellEulerAngles, rotAngle, m_RotationAxis), tbb::auto_partitioner());

  }
  else
#endif
  {
    RotateEulerRefFrameImpl serial(m_CellEulerAngles, rotAngle, m_RotationAxis);
    serial.convert(0, totalPoints);
  }

  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer RotateEulerRefFrame::newFilterInstance(bool copyFilterParameters)
{
  RotateEulerRefFrame::Pointer filter = RotateEulerRefFrame::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString RotateEulerRefFrame::getCompiledLibraryName()
{ return OrientationAnalysisConstants::OrientationAnalysisBaseName; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString RotateEulerRefFrame::getGroupName()
{ return DREAM3D::FilterGroups::OrientationAnalysisFilters; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString RotateEulerRefFrame::getSubGroupName()
{ return DREAM3D::FilterSubGroups::ConversionFilters; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString RotateEulerRefFrame::getHumanLabel()
{ return "Rotate Euler Reference Frame"; }
