project(alprstreamsample)

set(CMAKE_BUILD_TYPE DEBUG)

cmake_minimum_required (VERSION 2.6)

        
ADD_DEFINITIONS(
    -std=c++11 
)
 



ADD_EXECUTABLE( alprstream_sample_videofile
  alprstream_sample_videofile.cpp 
)
TARGET_LINK_LIBRARIES(alprstream_sample_videofile
    alprstream
    openalpr
) 


ADD_EXECUTABLE( alprstream_sample_videostream
  alprstream_sample_videostream.cpp 
)
TARGET_LINK_LIBRARIES(alprstream_sample_videostream
    alprstream
    openalpr
) 

ADD_EXECUTABLE( alprstream_sample_rawframe
  alprstream_sample_rawframe.cpp 
)
TARGET_LINK_LIBRARIES(alprstream_sample_rawframe
    alprstream
    openalpr
    alpropencv
) 