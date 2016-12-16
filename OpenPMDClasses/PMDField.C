/*****************************************************************************
*
* Copyright (c) 2000 - 2015, Lawrence Livermore National Security, LLC
* Produced at the Lawrence Livermore National Laboratory
* LLNL-CODE-442911
* All rights reserved.
*
* This file is  part of VisIt. For  details, see https://visit.llnl.gov/.  The
* full copyright notice is contained in the file COPYRIGHT located at the root
* of the VisIt distribution or at http://www.llnl.gov/visit/copyright.html.
*
* Redistribution  and  use  in  source  and  binary  forms,  with  or  without
* modification, are permitted provided that the following conditions are met:
*
*  - Redistributions of  source code must  retain the above  copyright notice,
*    this list of conditions and the disclaimer below.
*  - Redistributions in binary form must reproduce the above copyright notice,
*    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
*    documentation and/or other materials provided with the distribution.
*  - Neither the name of  the LLNS/LLNL nor the names of  its contributors may
*    be used to endorse or promote products derived from this software without
*    specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
* ARE  DISCLAIMED. IN  NO EVENT  SHALL LAWRENCE  LIVERMORE NATIONAL  SECURITY,
* LLC, THE  U.S.  DEPARTMENT OF  ENERGY  OR  CONTRIBUTORS BE  LIABLE  FOR  ANY
* DIRECT,  INDIRECT,   INCIDENTAL,   SPECIAL,   EXEMPLARY,  OR   CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
* SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
* CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
* LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
* OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
* DAMAGE.
*
*****************************************************************************/

// ***************************************************************************
//
// file PMDField.cpp
//
// Methods of the PMDField class
//
// Programmer: Mathieu Lobet
// Creation: Fri Oct 14 2016
//
// ***************************************************************************

#include "PMDField.h"

// ***************************************************************************
// Method: PMDField::PMDField
//
// Purpose:
//       Constructor
//
// Programmer: Mathieu Lobet
// Creation:   Fri Oct 14 2016
//
// Modifications:
//
// ***************************************************************************
PMDField::PMDField()
{
	unitSI=1;
	gridUnitSI=1;
	strcpy(this->name,"");
	strcpy(this->datasetPath,"");
	strcpy(this->groupPath,"");
	strcpy(this->unitsLabel,"");
	strcpy(this->axisLabels[0],"");
    strcpy(this->axisLabels[1],"");
    strcpy(this->axisLabels[2],"");
	strcpy(this->dataOrder,"");
}

// ***************************************************************************
// Method: PMDField::~PMDField
//
// Purpose:
//      Destructor
//
// Programmer: Mathieu Lobet
// Creation:   Fri Oct 14 2016
//
// Modifications:
//
// ***************************************************************************
PMDField::~PMDField()
{

}

// ***************************************************************************
// Method: PMDField::ScanAttributes
//
// Purpose:
//      This method scans from a group or a dataset the attributes
//      that the current PMDField object needs.
//
// Programmer: Mathieu Lobet
// Creation:   Fri Oct 14 2016
//
// Modifications:
//
// ***************************************************************************
void PMDField::ScanAttributes(hid_t objectId)
{
#ifdef VERBOSE
    cerr << " PMDField::ScanAttributes: " << endl;
#endif

	int 	numAttr;
	int 	i;
	hid_t 	attrId;
	hid_t	attrType;
	hid_t	attrSpace;
	int 	ndims;
	hsize_t sdim[64];
	herr_t 	err;
	char 	name[64];

	// Number of attributes
	numAttr = H5Aget_num_attrs(objectId);

	// This solution with H5Aiterate2 does not work because GetAttributeInfo
	// needs to be static and therefore field attribute can not be modified
	// err = H5Aiterate2(objectId, H5_INDEX_NAME, H5_ITER_INC, NULL, 
    // GetAttributeInfo, NULL);
	// But I am not an expert in C++, Mathieu

	// iteration over the attributes
    for (i = 0; i < numAttr; i++)
    {
    	// Open the attribute using its loop id.
		attrId = H5Aopen_idx(objectId, (unsigned int)i );

		// Get the name
		H5Aget_name(attrId, 64, name );

		// The type of the attribute
		attrType  = H5Aget_type(attrId); 
		// Space
	    attrSpace = H5Aget_space(attrId);
	    // Number of dimensions
	    ndims = H5Sget_simple_extent_ndims(attrSpace);
	    //
	    err = H5Sget_simple_extent_dims(attrSpace, sdim, NULL);

		if (strcmp(name,"gridSpacing")==0)
		{
			SetGridSpacing(name, attrId, attrType, attrSpace);
		}
		else if (strcmp(name,"gridGlobalOffset")==0)
		{
			SetGridGlobalOffset(name, attrId, attrType, attrSpace);
		}
		else if (strcmp(name,"position")==0)
		{
			SetGridPosition(name, attrId, attrType, attrSpace);			
		}
		else if (strcmp(name,"axisLabels")==0)
		{
			SetAxisLabels(name, attrId, attrType, attrSpace);
		}
		else if (strcmp(name,"geometry")==0)
		{
			SetGeometry(name, attrId, attrType, attrSpace);
		}
		else if (strcmp(name,"unitSI")==0)
		{
			SetUnitSI(name, attrId, attrType, attrSpace);			
		}
		else if (strcmp(name,"gridUnitSI")==0)
		{
			SetGridUnitSI(name, attrId, attrType, attrSpace);				
		}
		else if (strcmp(name,"timeOffset")==0)
		{
			// To be implemented
		}		
		else if (strcmp(name,"unitDimension")==0)
		{
			SetUnitDimension(name, attrId, attrType, attrSpace);
		}
		else if (strcmp(name,"fieldBoundary")==0)
		{
			SetFieldBoundary(name, attrId, attrType, attrSpace);
		}		
		else if (strcmp(name,"dataOrder")==0)
		{
			SetDataOrder(name, attrId, attrType, attrSpace);
		}
		else if (strcmp(name,"fieldSolver")==0)
		{
			// To be implemented
		}
		else if (strcmp(name,"chargeCorrection")==0)
		{
			// To be implemented
		}
		else if (strcmp(name,"currentSmoothing")==0)
		{
			// To be implemented
		}
		H5Aclose(attrId);
	}
}

// ***************************************************************************
// Method: PMDField::SetGridDimensions
//
// Purpose:
//      This method opens the given dataset to determine the number of 
//      points/nodes each direction for the field grid.
//
// Programmer: Mathieu Lobet
// Creation:   Fri Oct 14 2016
//
// Modifications:
//
// ***************************************************************************
void PMDField::SetGridDimensions(hid_t datasetId)
{
    // Data space
    hid_t dataset_space = H5Dget_space(datasetId);
    // Data type
    hid_t dataset_type  = H5Dget_type(datasetId);
    // Storage size
    hsize_t dataset_storage_size = H5Dget_storage_size(datasetId);
    // Dimension from the data space
    hsize_t dims[3] ;
    hsize_t maxdims[3] ;
    this->ndims =  H5Sget_simple_extent_dims(dataset_space, dims, maxdims );

    if (ndims==3)
    {
        this->nbNodes[0] = dims[0];
        this->nbNodes[1] = dims[1];
        this->nbNodes[2] = dims[2];
    }
    else if (ndims==2)
    {
        this->nbNodes[0] = dims[0];
        this->nbNodes[1] = dims[1];
        this->nbNodes[2] = 1;
    }

}

// ***************************************************************************
// Method: PMDField::SetGridSpacing
//
// Purpose:
//      This method reads from the dataset attributes GridSpacing the grid
//      spacing (dx, dy, dz) of the field grid.
//
// Arguments:
//      name : name of the attribute
//      attrId : attribute Id for hdf5
//      attribute type : hdf5 attribute type
//      attribute space : hdf5 attribute space 
//
// Programmer: Mathieu Lobet
// Creation:   Fri Oct 14 2016
//
// Modifications:
// - Nov 11 2016 - Mathieu - add 2d case
//
// ***************************************************************************
void
PMDField::SetGridSpacing(char * name,
                         hid_t attrId,
                         hid_t attrType, 
                         hid_t attrSpace)
{
	herr_t 	err;
    if (H5T_FLOAT == H5Tget_class(attrType)) {

        int npoints = H5Sget_simple_extent_npoints(attrSpace);

        double * tmpArray = (double *)malloc(sizeof(double)*(int)npoints);

        err = H5Aread(attrId, attrType, tmpArray);

        if (npoints == 3) 
        {
            this->gridSpacing[0] = tmpArray[0];
            this->gridSpacing[1] = tmpArray[1];               
            this->gridSpacing[2] = tmpArray[2];
        }
        else if (npoints == 2)
        {
            this->gridSpacing[0] = tmpArray[0];
            this->gridSpacing[1] = tmpArray[1];               
            this->gridSpacing[2] = 0;
        }

        free(tmpArray);
    }
}

// ***************************************************************************
// Method: PMDField::SetGridGlobalOffset
//
// Purpose:
//      This method reads from the dataset attributes GridGlobalOffset 
//      the relative offset of the field grid (usually relative to the 
//      rho grid).
//
// Arguments:
//      name : name of the attribute
//      attrId : attribute Id for hdf5
//      attribute type : hdf5 attribute type
//      attribute space : hdf5 attribute space 
//
// Programmer: Mathieu Lobet
// Creation:   Fri Oct 14 2016
//
// Modifications:
// - Nov 11 2016 - Mathieu - add 2d case
//
// ***************************************************************************
void
PMDField::SetGridGlobalOffset(char * name, 
                              hid_t attrId, 
                              hid_t attrType,
                              hid_t attrSpace)
{
	herr_t 	err;
    if (H5T_FLOAT == H5Tget_class(attrType)) {

        int npoints = H5Sget_simple_extent_npoints(attrSpace);

        double * tmpArray = (double *)malloc(sizeof(double)*(int)npoints);

        err = H5Aread(attrId, attrType, tmpArray);

        if (npoints==3)
        {
            this->gridGlobalOffset[0] = tmpArray[0];
            this->gridGlobalOffset[1] = tmpArray[1];              
            this->gridGlobalOffset[2] = tmpArray[2];
        }
        else if (npoints==2)
        {
            this->gridGlobalOffset[0] = tmpArray[0];
            this->gridGlobalOffset[1] = tmpArray[1];              
            this->gridGlobalOffset[2] = 0;
        }        

        free(tmpArray);
    }
}

// ***************************************************************************
// Method: PMDField::SetGridPosition
//
// Purpore:
//      This method reads from the dataset attributes GridPosition 
//      the origin of the grid (xmin, ymin, zmin).
//
// Arguments:
//      name : name of the attribute
//      attrId : attribute Id for hdf5
//      attribute type : hdf5 attribute type
//      attribute space : hdf5 attribute space 
//
// Programmer: Mathieu Lobet
// Creation:   Fri Oct 14 2016
//
// Modifications:
// - Nov 11 2016 - Mathieu - add 2d case
// ***************************************************************************
void
PMDField::SetGridPosition(char * name, 
                          hid_t attrId,
                          hid_t attrType,
                          hid_t attrSpace)
{
	herr_t 	err;
    if (H5T_FLOAT == H5Tget_class(attrType)) {

        int npoints = H5Sget_simple_extent_npoints(attrSpace);

        double * tmpArray = (double *)malloc(sizeof(double)*(int)npoints);

        err = H5Aread(attrId, attrType, tmpArray);

        if (npoints==3)
        {
            gridPosition[0] = tmpArray[0];
            gridPosition[1] = tmpArray[1];		        
            gridPosition[2] = tmpArray[2];
        }
        else if (npoints==2)
        {
            gridPosition[0] = tmpArray[0];
            gridPosition[1] = tmpArray[1];              
            gridPosition[2] = 0;
        }            
        free(tmpArray);
    }
}

// ***************************************************************************
// Method: PMDField::SetUnitSI
//
// Purpose:
//      this method captures the arrtibute unitSI from a group or a dataset.
//
// Arguments:
//      name : name of the attribute
//      attrId : attribute Id for hdf5
//      attribute type : hdf5 attribute type
//      attribute space : hdf5 attribute space 
//
// Programmer: Mathieu Lobet
// Creation:   Fri Oct 14 2016
//
// Modifications:
//
// ***************************************************************************
void 
PMDField::SetUnitSI(char * name, 
                    hid_t attrId,
                    hid_t attrType,
                    hid_t attrSpace)
{
	herr_t 	err;
    if (H5T_FLOAT == H5Tget_class(attrType)) {

        int npoints = H5Sget_simple_extent_npoints(attrSpace);

        err = H5Aread(attrId, attrType, &unitSI);

    }
}

// ***************************************************************************
// Method: PMDField::SetGridUnitSI
//
// Purpose:
//      This method reads from attribute GridUnitSI the factor to get the 
//      grid units in SI.
//
// Arguments:
//      name : name of the attribute
//      attrId : attribute Id for hdf5
//      attribute type : hdf5 attribute type
//      attribute space : hdf5 attribute space 
//
// Programmer: Mathieu Lobet
// Creation:   Fri Oct 14 2016
//
// Modifications:
//
// ***************************************************************************
void
PMDField::SetGridUnitSI(char * name, 
                        hid_t attrId, hid_t attrType, hid_t attrSpace)
{
	herr_t 	err;
    if (H5T_FLOAT == H5Tget_class(attrType)) {

        int npoints = H5Sget_simple_extent_npoints(attrSpace);

        err = H5Aread(attrId, attrType, &gridUnitSI);

    }
}

// ***************************************************************************
// Method: PMDField::SetGeometry
//
// Purpose:
//      This method reads from attribute geometry the type of grid.
//
// Arguments:
//      name : name of the attribute
//      attrId : attribute Id for hdf5
//      attribute type : hdf5 attribute type
//      attribute space : hdf5 attribute space 
//
// Programmer: Mathieu Lobet
// Creation:   Fri Oct 14 2016
//
// Modifications:
//
// ***************************************************************************
void
PMDField::SetGeometry(char * name, 
                      hid_t attrId, 
                      hid_t attrType,
                      hid_t attrSpace)
{
	herr_t 	err;
    if (H5T_STRING == H5Tget_class(attrType)) {

        int npoints = H5Sget_simple_extent_npoints(attrSpace);
        char tmpchar[64];

        err = H5Aread(attrId, attrType, &tmpchar);

        if (strstr(tmpchar,"cartesian")>0)
        {
        	strcpy(geometry,"cartesian");
        }
        else if (strstr(tmpchar,"thetaMode")>0)
        {
            strcpy(geometry,"thetaMode");
        }
    }
}

// ***************************************************************************
// Method: PMDField::SetAxisLabels
//
// Purpose:
//      This method reads from attribute axisLabel the labels for each axis.
//
// Arguments:
//      name : name of the attribute
//      attrId : attribute Id for hdf5
//      attribute type : hdf5 attribute type
//      attribute space : hdf5 attribute space 
//
// Programmer: Mathieu Lobet
// Creation:   Fri Oct 14 2016
//
// Modifications:
//
// ***************************************************************************
void
PMDField::SetAxisLabels(char * name,
                        hid_t attrId,
                        hid_t attrType,
                        hid_t attrSpace)
{
    int     iLabel;
    int     i;
    int     i0;
    char *  buffer;
	herr_t 	err;

    if (H5T_STRING == H5Tget_class(attrType)) {

        int npoints = H5Sget_simple_extent_npoints(attrSpace);

        // Buffer that will contains all the labels
        buffer = new char(npoints);

        // We put all the labels in buffer
        err = H5Aread(attrId, attrType, buffer);

        // then we parse buffer and fill this->axisLabels
        iLabel = 0;
        i0 = 0;
        for (i=0;i<npoints;i++)
        {
            this->axisLabels[iLabel][i-i0] = buffer[i];

            //cerr << buffer[i] << " " << iLabel << " " << i0 << endl;

            if (strcmp(&buffer[i],",")==0)
            {
                iLabel += 1;
                i0 = i+1;
            }
        }
        //cerr << "Labels: " << axisLabels[0] << " "
        //                   << axisLabels[1] << " " 
        //                   << axisLabels[2] << " " 
        //                   << endl;
    }
}

// ***************************************************************************
// Method: PMDField::GetUnitDimension
//
// Purpose:
//      This method reads the UnitDimension attributes and generates
//      the unitsLabel paramerer.
//
// Arguments:
//      name : name of the attribute
//      attrId : attribute Id for hdf5
//      attribute type : hdf5 attribute type
//      attribute space : hdf5 attribute space 
//
// Programmer: Mathieu Lobet
// Creation:   Fri Oct 14 2016
//
// Modifications:
//
// ***************************************************************************
void PMDField::SetUnitDimension(char * name, 
                                hid_t attrId,
                                hid_t attrType,
                                hid_t attrSpace)
{
	herr_t 	err;
	int 	i;
	char 	buffer[64];
	char	units[8];
	int 	firstunits = 0;

    if (H5T_FLOAT == H5Tget_class(attrType)) {

        int npoints = H5Sget_simple_extent_npoints(attrSpace);

        double * tmpArray = (double *)malloc(sizeof(double)*(int)npoints);

        err = H5Aread(attrId, attrType, tmpArray);

        strcpy(unitsLabel,"");

        for(i=0;i<7;i++)
        {

	        if (int(tmpArray[i])!=0)
	        {

	        	if (firstunits==0)
	        	{
	        		strcpy(units,"");
	        		firstunits=1;
	        	}
	        	else
	        	{
	        		strcpy(units,".");
	        	}

	        	// List of SI units
	        	switch(i)
	        	{
	        	// Distance, meter
	        	case 0:
	        		strcat(units,"m");
	        		break;
	        	// ass, kg
	        	case 1:
	        		strcat(units,"kg");
	        		break;
	        	// time, second
	        	case 2:
	        		strcat(units,"s");
	        		break;
	        	// Electric Current, Ampere
	            case 3:
	        		strcat(units,"A");
	        		break;
	        	// Temperature, Kelvin
	            case 4:
	        		strcat(units,"K");
	        		break;
				//amount of substance, mole
	            case 5:
	        		strcat(units,"mol");
	        		break;
				//luminous intensity, candela
	            case 6:
	        		strcat(units,"candela");
	        		break;
	        	}
				//amount of substance, mole
				//luminous intensity, candela

	        	if (int(tmpArray[i]) == 1)
	        	{
	        		sprintf(buffer, units, int(tmpArray[i]));
	        	}
	        	else
	        	{
	        		strcat(units,"^%d");
	        		sprintf(buffer, units, int(tmpArray[i]));
	        	}
	        	// Creation of the unitsLabel
	        	strcat(unitsLabel,buffer);
	        }        	
        }       
        free(tmpArray);
    }
}

// ***************************************************************************
// Method: PMDField::SetFieldBoundary
//
// Purpose:
//      This method reads the field boundaries
//
// Arguments:
//      name : name of the attribute
//      attrId : attribute Id for hdf5
//      attribute type : hdf5 attribute type
//      attribute space : hdf5 attribute space 
//
// Programmer: Mathieu Lobet
// Creation:   Fri Oct 14 2016
//
// Modifications:
//
// ***************************************************************************
void
PMDField::SetFieldBoundary(char * name, 
                           hid_t attrId,
                           hid_t attrType, 
                           hid_t attrSpace)
{
	herr_t 	err;
    if (H5T_STRING == H5Tget_class(attrType)) {

    	// Number of elements
        int npoints = H5Sget_simple_extent_npoints(attrSpace);

        //cout << " Number of elements: " << npoints << endl;

        err = H5Aread(attrId, attrType, this->fieldBoundary);

    }
}

// ***************************************************************************
// Method: PMDField::SetFieldBoundary
//
// Purpose:
//      This method reads the field boundaries
//
// Arguments:
//      name : name of the attribute
//      attrId : attribute Id for hdf5
//      attribute type : hdf5 attribute type
//      attribute space : hdf5 attribute space 
//
// Programmer: Mathieu Lobet
// Creation:   Fri Oct 14 2016
//
// Modifications:
//
// ***************************************************************************
void
PMDField::SetDataOrder(char * name, 
                       hid_t attrId, 
                       hid_t attrType, 
                       hid_t attrSpace)
{
	herr_t 	err;
    if (H5T_STRING == H5Tget_class(attrType)) {

    	// Number of elements
        int npoints = H5Sget_simple_extent_npoints(attrSpace);

        //cout << " Number of elements: " << npoints << endl;

        err = H5Aread(attrId, attrType, this->dataOrder);

    }
}

// ***************************************************************************
// Method: PMDField::GetNumValues
//
// Purpose:
//      This method returns the total number of nodes/points that 
//      compose a grid.
//
// Arguments:
//      name : name of the attribute
//      attrId : attribute Id for hdf5
//      attribute type : hdf5 attribute type
//      attribute space : hdf5 attribute space 
//
// Programmer: Mathieu Lobet
// Creation:   Fri Oct 14 2016
//
// Modifications:
// - Nov 11 2016 - Mathieu - add 2d case
//
// ***************************************************************************
int PMDField::GetNumValues()
{
    if (this->ndims==3)
    {
        return this->nbNodes[0]*this->nbNodes[1]*this->nbNodes[2];
    }
    else if (this->ndims==2)
    {
        return this->nbNodes[0]*this->nbNodes[1];
    }
    return 0;
}

// ***************************************************************************
// Method: PMDField::GetDomainProperties
//
// Purpose:
//      This method returns the properties of the required block when the
//      fields are readed by block (parallel)
//
// Programmer: Mathieu Lobet
// Creation:   Mon Nov 14 2016
//
// Arguments:
//      blockDim : number of domains to divide the field
//      blockId : index of the block
//      fieldBlock : structure containing the properties of the block
//
// Modifications:
//
// ***************************************************************************
int
PMDField::GetBlockProperties(int blockDim, 
                             int blockId,
                             fieldBlockStruct * fieldBlock)
{

#ifdef VERBOSE
    cerr << "PMDField::GetBlockProperties"
         << "(blockDim=" << blockDim 
         << ", blockId=" << blockId
         << ")" << endl;
#endif

    // Parameter declaration
    int r;                          // division rest

    // Copy the name of the dataset
    strcpy(fieldBlock->dataSetPath,this->datasetPath);

    // 3D datasets
    if (this->ndims==3)
    {
        // Set the dimension of the block
        fieldBlock->ndims=this->ndims;

        // Computation of the number of Nodes
        // We divide the field dataset into blockDim domains 
        // in the last direction
        fieldBlock->nbNodes[0] = this->nbNodes[0] / blockDim;
        r = this->nbNodes[0]%blockDim;
        if (blockId < r )
        {
            fieldBlock->nbNodes[0] += 1;
        }

        fieldBlock->nbNodes[1] = this->nbNodes[1];
        fieldBlock->nbNodes[2] = this->nbNodes[2];

        // Computation of minimum idexes
        if (blockId < r )
        {
            fieldBlock->minNode[0] = blockId*fieldBlock->nbNodes[0];
        }
        else
        {
            fieldBlock->minNode[0] = r*(fieldBlock->nbNodes[0]+1) 
                                   + (blockId - r)*fieldBlock->nbNodes[0];
        }
        fieldBlock->minNode[1] = 0;
        fieldBlock->minNode[2] = 0;

        // Adjust so that the domains have a common node
        if (blockId > 0)
        {
            fieldBlock->nbNodes[0] += 1;
        }        
        if (blockId > 0)
        {
            fieldBlock->minNode[0] -= 1;
        }

        // Total number of nodes
        fieldBlock->nbTotalNodes = fieldBlock->nbNodes[0]
                                   *fieldBlock->nbNodes[1]
                                   *fieldBlock->nbNodes[2];

        // Computation of maximum indexes
        fieldBlock->maxNode[0] = fieldBlock->minNode[0] 
                               + fieldBlock->nbNodes[0] -1;
        fieldBlock->maxNode[1] = fieldBlock->minNode[1] 
                               + fieldBlock->nbNodes[1] -1;
        fieldBlock->maxNode[2] = fieldBlock->minNode[2] 
                               + fieldBlock->nbNodes[2] -1;
    }
    // 2D datasets
    else if (this->ndims==2)
    {

        // Set the dimension of the block
        fieldBlock->ndims=this->ndims;

        // Computation of the number of Nodes
        // We divide the field dataset into blockDim domains 
        // in the last direction
        fieldBlock->nbNodes[0] = this->nbNodes[0] / blockDim;
        r = this->nbNodes[0]%blockDim;
        if (blockId < r )
        {
            fieldBlock->nbNodes[0] += 1;
        }

        fieldBlock->nbNodes[1] = this->nbNodes[1];
        fieldBlock->nbNodes[2] = 1;

        // Computation of minimum idexes
        if (blockId < r )
        {
            fieldBlock->minNode[0] = blockId*fieldBlock->nbNodes[0];
        }
        else
        {
            fieldBlock->minNode[0] = r*(fieldBlock->nbNodes[0]+1) 
                                   + (blockId - r)*fieldBlock->nbNodes[0];
        }
        fieldBlock->minNode[1] = 0;
        fieldBlock->minNode[2] = 0;

        // Adjust so that the domains have a common node
        if (blockId > 0)
        {
            fieldBlock->nbNodes[0] += 1;
        }        
        if (blockId > 0)
        {
            fieldBlock->minNode[0] -= 1;
        }

        // Total number of nodes
        fieldBlock->nbTotalNodes = fieldBlock->nbNodes[0]
                                 * fieldBlock->nbNodes[1];

        // Computation of maximum indexes
        fieldBlock->maxNode[0] = fieldBlock->minNode[0] 
                               + fieldBlock->nbNodes[0] -1;
        fieldBlock->maxNode[1] = fieldBlock->minNode[1] 
                               + fieldBlock->nbNodes[1] -1;
        fieldBlock->maxNode[2] = 0;

    }
    else
    {
        cerr << " This dimension, " 
             << this->ndims 
             << ", can not be read in parallel" 
             << endl;
        return -1;
    }

    // Return 0 if no error
    return 0;
}
