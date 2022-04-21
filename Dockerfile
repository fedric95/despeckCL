FROM docker.io/library/ubuntu:20.04


RUN apt-get update
RUN apt-get upgrade -y

ENV DEBIAN_FRONTEND=noninteractive


RUN apt-get install -y git
RUN apt-get install -y swig
RUN apt-get install -y libboost-all-dev
RUN apt-get install -y libgsl-dev
RUN apt-get install -y libgl-dev
RUN apt-get install -y nvidia-cuda-toolkit

RUN apt-get install -y opencl-headers
RUN apt-get install -y ocl-icd-libopencl1
RUN apt-get install -y build-essential
RUN apt-get install -y build-essential cmake protobuf-compiler
RUN apt-get install -y python-numpy

COPY . /opt/despeckCL/
WORKDIR /opt/despeckCL/

RUN mkdir build && cd build && cmake ../

RUN cat /opt/despeckCL/build/CMakeFiles/CMakeOutput.log

#RUN apt-get install -y libgl-dev && apt-get install -y clfft-client

RUN cd build && make -j4


RUN cd build && make test
RUN cd build make install