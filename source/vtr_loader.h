#ifndef VTR_LOADER_H
#define VTR_LOADER_H

#include<map>
#include<string>

#include"CImg.h"
namespace cl = cimg_library;

typedef std::map< std::string, cl::CImg<double> > field_data;

void load_vtr( const char *filename, int *dim, double *aabb, field_data &point_data, field_data &cell_data );

#endif