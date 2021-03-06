// Copyright (c) 2010, Jérôme Velut
// All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT OWNER ``AS IS'' AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
// NO EVENT SHALL THE COPYRIGHT OWNER BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
// OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "vtkChainSource.h"

#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"

#include <cstring>

vtkStandardNewMacro(vtkChainSource);

vtkChainSource::vtkChainSource()
{
    this->CellType = 0;
    this->Points = 0;
    this->rawPoints.clear();
    this->SetNumberOfInputPorts(0);
}

vtkChainSource::~vtkChainSource()
{
  this->ResetRawPoints();
}


int vtkChainSource::RequestInformation(
    vtkInformation *vtkNotUsed(request),
    vtkInformationVector **vtkNotUsed(inputVector),
    vtkInformationVector *outputVector)
{
    // get the info object
    //vtkInformation *outInfo = outputVector->GetInformationObject(0);
    //outInfo->Set(vtkStreamingDemandDrivenPipeline::MAXIMUM_NUMBER_OF_PIECES(),
    //             -1);
    return 1;
}


void vtkChainSource::AddRawPoint( double* pt )
{
  double* newPt = new double[3];
  std::memcpy( newPt, pt, 3 * sizeof( double ) );
  this->rawPoints.push_back( newPt );
  this->Modified();
}

std::vector<double*> vtkChainSource::GetRawPoints()
{
  return(this->rawPoints);
}

void vtkChainSource::ResetRawPoints(  )
{
  for( double* pt : this->rawPoints )
  {
    delete pt;
  }
  this->rawPoints.clear();
  this->Modified();
}


int vtkChainSource::RequestData(
    vtkInformation *vtkNotUsed(request),
    vtkInformationVector **vtkNotUsed(inputVector),
    vtkInformationVector *outputVector)
{
    // get the info object
    vtkInformation *outInfo = outputVector->GetInformationObject(0);

    // get the ouptut
    vtkPolyData *output = vtkPolyData::SafeDownCast(
                              outInfo->Get(vtkDataObject::DATA_OBJECT()));

    vtkPoints *newPoints;
    vtkCellArray *newLines;

    if (outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_PIECE_NUMBER()) > 0)
    {
        return 1;
    }

    newPoints = vtkPoints::New();
    newLines = vtkCellArray::New();

    if( this->Points )
    {
      newLines->InsertNextCell( this->Points->GetNumberOfPoints() );
      for( int id = 0; id < this->Points->GetNumberOfPoints(); id++ )//
        {
            vtkIdType newId = newPoints->InsertNextPoint(this->Points->GetPoint( id ));
            newLines->InsertCellPoint( newId );
        }
    }

    if( this->rawPoints.size() > 0 )
    {
      newLines->InsertNextCell( static_cast<int>(this->rawPoints.size()) );
      for( int id = 0; id < this->rawPoints.size(); id++ )//
      {
        vtkIdType newId = newPoints->InsertNextPoint( this->rawPoints[id] ) ;
        newLines->InsertCellPoint( newId );
      }
    }

    output->SetPoints(newPoints);
    newPoints->Delete();

    if( this->CellType == 1 )
    {
        output->SetLines(newLines);
    }
    else
    {
        output->SetVerts(newLines);
    }

    newLines->Delete();

    return 1;
}

void vtkChainSource::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os,indent);
}
