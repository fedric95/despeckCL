#include "boxcar_sub_image.h"

#include <iostream>

int boxcar_sub_image(cl::Context context,
                     boxcar_wrapper boxcar_routine,
                     insar_data& sub_insar_data,
                     const int window_width)
{
    // overlapped dimension, large enough to include the complete padded data to compute the similarities
    const int height_overlap = sub_insar_data.height;
    const int width_overlap  = sub_insar_data.width;
    const int n_elem_overlap = height_overlap*width_overlap;

    //***************************************************************************
    //
    // global buffers used by the kernels to exchange data
    //
    //***************************************************************************

    cl::Buffer device_raw_a1                  {context, CL_MEM_READ_ONLY  | CL_MEM_COPY_HOST_PTR, n_elem_overlap*sizeof(float), sub_insar_data.a1, NULL};
    cl::Buffer device_raw_a2                  {context, CL_MEM_READ_ONLY  | CL_MEM_COPY_HOST_PTR, n_elem_overlap*sizeof(float), sub_insar_data.a2, NULL};
    cl::Buffer device_raw_dp                  {context, CL_MEM_READ_ONLY  | CL_MEM_COPY_HOST_PTR, n_elem_overlap*sizeof(float), sub_insar_data.dp, NULL};

    cl::Buffer device_amp_filt                {context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, n_elem_overlap*sizeof(float), sub_insar_data.amp_filt, NULL};
    cl::Buffer device_phi_filt                {context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, n_elem_overlap*sizeof(float), sub_insar_data.phi_filt, NULL};
    cl::Buffer device_coh_filt                {context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, n_elem_overlap*sizeof(float), sub_insar_data.coh_filt, NULL};

    std::vector<cl::Device> devices;
    context.getInfo(CL_CONTEXT_DEVICES, &devices);
    cl::CommandQueue cmd_queue{context, devices[0]};

    //***************************************************************************
    //
    // executing routines and kernels
    //
    //***************************************************************************
    boxcar_routine.timed_run(cmd_queue,
                             device_raw_a1, device_raw_a2, device_raw_dp,
                             device_amp_filt, device_phi_filt, device_coh_filt,
                             height_overlap, width_overlap);
    
    //***************************************************************************
    //
    // copying back result and clean up
    //
    //***************************************************************************
    cmd_queue.enqueueReadBuffer(device_amp_filt, CL_TRUE, 0, n_elem_overlap*sizeof(float), sub_insar_data.amp_filt, NULL, NULL);
    cmd_queue.enqueueReadBuffer(device_phi_filt, CL_TRUE, 0, n_elem_overlap*sizeof(float), sub_insar_data.phi_filt, NULL, NULL);
    cmd_queue.enqueueReadBuffer(device_coh_filt, CL_TRUE, 0, n_elem_overlap*sizeof(float), sub_insar_data.coh_filt, NULL, NULL);

    return 0;
}
