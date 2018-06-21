alprstreamsample
-------------------

This is a basic example of running the OpenALPR AlprStream API.  This sample uses the C++ code which runs on Linux.  If you are running Windows, or need it to be cross-platform, use the C API.

Prerequisites:

    apt-get update && apt-get install cmake build-essential
    
    bash <(curl https://deb.openalpr.com/install)

Choose "install_sdk" from the menu


To run any of the samples:

  * Edit the cpp file and add your license key to the LICENSE_KEY variable.
  * Adjust the LICENSEPLATE_COUNTRY to your desired country used for license plate recognition
  * Run the following commands to build

    mkdir -p build
    cd build/
    cmake ..
    make -j4


alprstream_sample_videofile
-------------------------------

Opens a video file in a background thread to retrieve the data for processing.  On the main thread, we process the images and print the results.  Video file processing will never drop frames unless you explicitly call the "skip" function.  When processing on CPU, you may run the Alpr processing on multiple threads, make sure each thread has its own Alpr instance, and shares a single instance of the AlprStream object.
  
To run, follow the instructions above:
    
    * Update the path for VIDEO_FILE (default is /tmp/video.mp4)
    * ./alprstream_sample_videofile

alprstream_sample_videostream
-------------------------------

Opens a video stream URL in a background thread to retrieve the data for processing.  On the main thread, we process the images and print the results.  Video stream processing will drop frames to try and keep up with realtime.  When the queue is full, a random frame will be dropped to make room for the new one.  When processing on CPU, you may run the Alpr processing on multiple threads, make sure each thread has its own Alpr instance, and shares a single instance of the AlprStream object.

To run, follow the instructions above:
    
    * Edit the VIDEO_STREAM_URL and add the URL to your video stream
    * ./alprstream_sample_videostream

alprstream_sample_rawframe
-------------------------------

Sends raw pixel data into the AlprStream SDK that represent sequential images from a video source.  The format is 24-bit pixel data in BGR format.
This format is the native OpenCV Mat format.  We use OpenCV in this example to load the image files.  On Xenial, we use our own packaging of OpenCV 3.1 (apt-get install libalpropencv-dev from our repos).  On Bionic, we use standard OpenCV.

To run, follow the instructions above:
    
    * Make sure to place a number of images in the folder /tmp/imagebatchtest (if using GPU acceleration, make sure the images are the same width/height)
    * ./alprstream_sample_rawframe

