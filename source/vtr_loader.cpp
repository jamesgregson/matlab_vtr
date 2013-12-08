
#include<vtkSmartPointer.h>
#include<vtkFloatArray.h>
#include<vtkDoubleArray.h>
#include<vtkPointData.h>
#include<vtkCellData.h>
#include<vtkRectilinearGrid.h>
#include<vtkXMLRectilinearGridWriter.h>
#include<vtkXMLRectilinearGridReader.h>
#include<vtkImageData.h>
#include<vtkXMLImageDataWriter.h>
#include<vtkXMLImageDataReader.h>

#include"vtr_loader.h"

void load_vtr( const char *filename, int *dim, double *aabb, field_data &point_data, field_data &cell_data ){
    typedef cl::CImg<double> image;

    vtkSmartPointer<vtkXMLRectilinearGridReader> reader = vtkSmartPointer<vtkXMLRectilinearGridReader>::New();
    
    reader->SetFileName( filename );
    reader->Update();
    
    auto vtr = reader->GetOutput();
    vtr->GetDimensions(dim);
    
    vtkSmartPointer<vtkDoubleArray> xcoords = vtkSmartPointer<vtkDoubleArray>::New();
    vtkSmartPointer<vtkDoubleArray> ycoords = vtkSmartPointer<vtkDoubleArray>::New();
    vtkSmartPointer<vtkDoubleArray> zcoords = vtkSmartPointer<vtkDoubleArray>::New();
    xcoords->DeepCopy( vtr->GetXCoordinates() );
    ycoords->DeepCopy( vtr->GetYCoordinates() );
    zcoords->DeepCopy( vtr->GetZCoordinates() );
    aabb[0] = xcoords->GetValue(0); aabb[1] = xcoords->GetValue(dim[0]-1);
    aabb[2] = ycoords->GetValue(0); aabb[3] = ycoords->GetValue(dim[1]-1);
    aabb[4] = zcoords->GetValue(0); aabb[5] = zcoords->GetValue(dim[2]-1);
    
    auto pd = vtr->GetPointData();
    for( int arr=0; arr<pd->GetNumberOfArrays(); arr++ ){
        auto tmp = vtkSmartPointer<vtkDoubleArray>::New();
        tmp->DeepCopy( pd->GetArray(arr) );
        tmp->SetName( pd->GetArray(arr)->GetName() );
        int nc = tmp->GetNumberOfComponents();
        image timage( dim[0], dim[1], dim[2], nc );
        for( int i=0; i<dim[0]; i++ ){
            for( int j=0; j<dim[1]; j++ ){
                for( int k=0; k<dim[2]; k++ ){
                    int ijk[] = { i, j, k };
                    for( int c=0; c<nc; c++ ){
                        vtkIdType id = vtr->ComputePointId( ijk );
                        timage(i,j,k,c) = tmp->GetComponent( id, c );
                    }
                }
            }
        }
        point_data[ std::string(tmp->GetName()) ] = timage;
    }
    
    auto cd = vtr->GetCellData();
    for( int arr=0; arr<cd->GetNumberOfArrays(); arr++ ){
        auto tmp = vtkSmartPointer<vtkDoubleArray>::New();
        tmp->DeepCopy( cd->GetArray(arr) );
        tmp->SetName( pd->GetArray(arr)->GetName() );
        int nc = tmp->GetNumberOfComponents();
        image timage( dim[0]-1, dim[1]-1, dim[2]-1, nc );
        for( int i=0; i<dim[0]-1; i++ ){
            for( int j=0; j<dim[1]-1; j++ ){
                for( int k=0; k<dim[2]-1; k++ ){
                    int ijk[] = { i, j, k };
                    vtkIdType id = vtr->ComputeCellId(ijk);
                    for( int c=0; c<nc; c++ ){
                        timage(i,j,k,c) = tmp->GetComponent( id, c );
                    }
                }
            }
        }
        cell_data[ std::string(tmp->GetName()) ] = timage;
    }
}

