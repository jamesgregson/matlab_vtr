#include<mex.h>

#include<CImg.h>
namespace cl=cimg_library;

#include"vtr_loader.h"

mxArray *to_array( const cl::CImg<double> &img ){
    mwSize dim[] = { img.width(), img.height(), img.depth(), img.spectrum() };
    if( img.depth() == 1 ){
        dim[2] = img.spectrum();
        dim[3] = 1;
    }
    mxArray *out = mxCreateNumericArray( 4, dim, mxDOUBLE_CLASS, mxREAL );
    double *ptr = mxGetPr( out );
    int cnt=0;
    for( int c=0; c<img.spectrum(); c++ ){
        for( int k=0; k<img.depth(); k++ ){
            for( int j=0; j<img.height(); j++ ){
                for( int i=0; i<img.width(); i++ ){
                    ptr[cnt] = img(i,j,k,c);
                    cnt++;
                }
            }
        }
    }
    return out;
}

mxArray *get_array( field_data &point_data, field_data &cell_data, std::string name ){
    for( field_data::iterator iter=point_data.begin(); iter!=point_data.end(); iter++ ){
        if( iter->first == name ){
            return to_array( iter->second );
        }
    }
    for( field_data::iterator iter=cell_data.begin(); iter!=cell_data.end(); iter++ ){
        if( iter->first == name ){
            return to_array( iter->second );
        }
    }
    return NULL;
}

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] ){
    // get the filename and print out the loading message
    char *filename = mxArrayToString( prhs[0] );
    mexPrintf( "loading: %s\n", filename );
    
    // arrays to hold the dimensions and bounding box as well
    // as field and cell data arrays to hold the contents
    int dim[3];
    double aabb[6];
    field_data point_data, cell_data;
    load_vtr( filename, dim, aabb, point_data, cell_data );
    
    // list the contents of the file if no arguments are supplied
    if( nrhs == 1 || nlhs != nrhs-1 ){
        // print out some debugging information
        mexPrintf( "  No arrays requested, listing file information...\n" );
        // print out some information about the file
        mexPrintf( "    dimensions:   [%d,%d,%d]\n", dim[0], dim[1], dim[2] );
        mexPrintf( "    bounding box: [%g,%g,%g,%g,%g,%g]\n", aabb[0], aabb[1], aabb[2], aabb[3], aabb[4], aabb[5] );
        
        if( !point_data.empty() ){
            mexPrintf("    Point Data:\n" );
            for( field_data::iterator iter=point_data.begin(); iter!=point_data.end(); iter++ ){
                mexPrintf( "      %-15s [%d,%d,%d,%d]\n", iter->first.c_str(), dim[0], dim[1], dim[2], iter->second.spectrum() );
            }
        }
        if( !cell_data.empty() ){
            mexPrintf( "    CellData:\n" );
            for( field_data::iterator iter=cell_data.begin(); iter!=cell_data.end(); iter++ ){
                mexPrintf( "      %-15s [%d,%d,%d,%d]\n", iter->first.c_str(), dim[0], dim[1], dim[2], iter->second.spectrum() );
            }
        }
        mexPrintf( "done.\n" );
    } else {
        for( int i=1; i<nrhs; i++ ){
            char *name = mxArrayToString( prhs[i] );
            std::string sname = name;
            mxFree( name );
            
            mxArray *var = get_array( point_data, cell_data, sname );
            if( var ){
                plhs[i-1] = var;
            } else {
                mexPrintf( "  Error, could not find variable '%s'\n", sname.c_str() );
                break;
            }
        }
        mexPrintf( "done.\n");
    }
    
    // free the filename
    mxFree( filename );    
}