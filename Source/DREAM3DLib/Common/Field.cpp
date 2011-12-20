/* ============================================================================
 * Copyright (c) 2010, Michael A. Jackson (BlueQuartz Software)
 * Copyright (c) 2010, Dr. Michael A. Groeber (US Air Force Research Laboratories)
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

#include "Field.h"
#include <iostream>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Field::Field() :
nucleus(0),
active(false),
numvoxels(0),
numneighbors(0),
newfieldname(0),
surfacefield(false),
outsideboundbox(false),
twinnewnumber(-1),
colonynewnumber(-1),
slipsystem(0),
phase(0),
centroidx(0.0),
centroidy(0.0),
centroidz(0.0),
aspectratio1(0.0),
aspectratio2(0.0),
omega3(0.0),
averagemisorientation(0.0),
kernelmisorientation(10000.0),
red(0.0),
green(0.0),
blue(0.0),
schmidfactor(0.0),
euler1(0.0),
euler2(0.0),
euler3(0.0),
axiseuler1(0.0),
axiseuler2(0.0),
axiseuler3(0.0),
volume(0.0),
equivdiameter(0.0),
radius1(0.0),
radius2(0.0),
radius3(0.0),
packquality(0.0)
{
#if CORRUPT_TEST
  test5 = NULL;
  test6 = NULL;
#endif
}

#define DELETE_VECTOR_POINTER(vec)\
  if (NULL != vec) {\
  vec->clear();\
  delete vec;\
  vec = NULL;\
  }

#define COPY_VECTOR_POINTER(src, dest, type)\
  if (NULL != src) {\
  dest = new std::vector<type>(0);\
  dest->assign(src->begin(), src->end());\
  }



#define CORRUPT_TEST_OUTPUT(var)\
    if (var != NULL) {\
      std::cout << "Grain: " << newgrainname << " " << #var << " over written with value " << std::endl;\
      printf("%p\n", var);\
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Field::~Field()
{
#if CORRUPT_TEST
  CORRUPT_TEST_OUTPUT(test5);
  CORRUPT_TEST_OUTPUT(test6);
#endif
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Field::deepCopy(Field::Pointer field)
{
  if (field.get() == this) { return; } // The pointers are the same just return
  nucleus = field->nucleus;
  active = field->active ;
  numvoxels = field->numvoxels ;
  numneighbors = field->numneighbors ;
  newfieldname = field->newfieldname ;
  surfacefield = field->surfacefield ;
  twinnewnumber = field->twinnewnumber;
  colonynewnumber = field->colonynewnumber;
  slipsystem = field->slipsystem;

  centroidx = field->centroidx ;
  centroidy = field->centroidy ;
  centroidz = field->centroidz ;
  omega3 = field->omega3 ;
  averagemisorientation = field->averagemisorientation ;
  kernelmisorientation = field->kernelmisorientation;
  red = field->red ;
  green = field->green ;
  blue = field->blue ;

  COPY_ARRAY_3(IPF, field);
  schmidfactor = field->schmidfactor ;
  euler1 = field->euler1 ;
  euler2 = field->euler2 ;
  euler3 = field->euler3 ;
  axiseuler1 = field->axiseuler1 ;
  axiseuler2 = field->axiseuler2 ;
  axiseuler3 = field->axiseuler3 ;
  volume = field->volume ;
  equivdiameter = field->equivdiameter ;
  radius1 = field->radius1 ;
  radius2 = field->radius2 ;
  radius3 = field->radius3 ;
  packquality = field->packquality;
  COPY_ARRAY_5(avg_quat, field);
  COPY_ARRAY_3(neighbordistfunc, field);
}


