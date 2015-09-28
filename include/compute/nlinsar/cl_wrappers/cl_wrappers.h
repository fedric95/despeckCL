#ifndef CL_WRAPPERS_H
#define CL_WRAPPERS_H

#include <CL/cl.h>

#include "precompute_similarities_1st_pass.h"
#include "precompute_similarities_2nd_pass.h"
#include "precompute_patch_similarities.h"
#include "compute_weights.h"
#include "compute_number_of_looks.h"
#include "transpose.h"
#include "precompute_filter_values.h"
#include "compute_insar.h"
#include "smoothing.h"

namespace nlinsar {
    struct cl_wrappers {
        precompute_similarities_1st_pass precompute_similarities_1st_pass_routine;
        precompute_similarities_2nd_pass precompute_similarities_2nd_pass_routine;
        precompute_patch_similarities    precompute_patch_similarities_routine;
        compute_weights                  compute_weights_routine;
        compute_number_of_looks          compute_number_of_looks_routine;
        transpose                        transpose_routine;
        smoothing                        smoothing_routine;
        precompute_filter_values         precompute_filter_values_routine;
        compute_insar                    compute_insar_routine;

        cl_wrappers(cl::Context context,
                    const int search_window_size,
                    const int patch_size,
                    const int block_size);

        cl_wrappers(const cl_wrappers& other);
    };
}

#endif
