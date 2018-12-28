// OpenALPR sample for alprstreamgpu library
// Copyright 2017, OpenALPR Technology, Inc.

// System imports
#include <cstdlib>
#include <vector>
#include <string>
#include <string.h>
#include <sstream>
#include <unistd.h>

// Import used to list files in directory
#include <dirent.h>

// Import OpenALPR alprstreamgpu (also pulls in alprgpu.h and alpr.h)
// Object definitions for individual results are found in alpr.h
// Object definitions for group results are found in alprstreamgpu.h
#include <alprstream.h>

using namespace std;
using namespace alpr;

// helper prototypes
std::vector<std::string> list_files_in_dir(const char* dirPath);
void print_frame_results(vector<RecognizedFrame> rframes);
void print_group_results(std::vector<AlprGroupResult> groups);


int main(int argc, char** argv) {

  cout << "Initializing" << endl;
  const long STARTING_EPOCH_TIME_MS = 1500294710000;
  const std::string LICENSEPLATE_COUNTRY = "us";
  const std::string LICENSE_KEY = "";
  const std::string VIDEO_FILE = "/tmp/video.mp4";
  
  
  // Size of image buffer to maintain in stream queue -- This only matters if you are feeding
  // images/video into the buffer faster than can be processed (i.e., on a background thread)
  // Setting this to the batch size since we're feeding in images synchronously, so it's only needed to 
  // hold a single batch

  // Batch size and GPU ID set in openalpr.conf
  // Video buffer frames controls the number of frames to buffer in memory.  Must be >= gpu batch size
  const int VIDEO_BUFFER_SIZE = 15;
  
  // The stream will assume sequential frames.  If there is no motion from frame to frame, then 
  // processing can be skipped for some frames
  const bool USE_MOTION_DETECTION = true;

  AlprStream alpr_stream(VIDEO_BUFFER_SIZE, USE_MOTION_DETECTION);
  Alpr alpr(LICENSEPLATE_COUNTRY, "", "", LICENSE_KEY);

  if (!alpr.isLoaded())
  {
    cout << "Error loading OpenALPR library." << endl;
    exit(1);
  }
  cout << "Initialization complete" << endl;

  // It's important that the image dimensions are consistent within a batch and that you 
  // only drive OpenALPR with few various image sizes.  The memory for each image size is 
  // cached on the GPU for efficiency, and using many different image sizes will degrade performance

  
  alpr_stream.connect_video_file(VIDEO_FILE, STARTING_EPOCH_TIME_MS);

  while (alpr_stream.video_file_active() || alpr_stream.get_queue_size() > 0)
  {

    cout << "Queue size: " << alpr_stream.get_queue_size() << endl;

    // If processing on the CPU there is no benefit to a batch size > 1.
    int BATCH_SIZE = 10;

    // Process a batch as fast as we can, the batches won't necessarily be full

    vector<RecognizedFrame> frame_results = alpr_stream.process_batch(&alpr);
    print_frame_results(frame_results);
    
    // After each batch processing, we can check to see if any groups are ready
    // "Groups" form based on their timestamp and plate numbers on each stream
    // The stream object has configurable options for how long to wait before 
    // completing a plate group.  You may peek at the active list without popping.
    cout << "After batching there are: " << alpr_stream.peek_active_groups().size() << " active groups" << endl;
    
    std::vector<AlprGroupResult> group_results = alpr_stream.pop_completed_groups();
    print_group_results(group_results);
    

    // Sleep a little bit so we don't spin the CPU looping
    usleep(1000);
  }

  
  cout << "Done" << endl;
  
  return 0;
}



void print_frame_results(vector<RecognizedFrame> rframes)
{
  for (uint32_t frame_index = 0; frame_index < rframes.size(); frame_index++)
  {
    RecognizedFrame rf = rframes[frame_index];
    for (uint32_t i = 0; i < rf.results.plates.size(); i++)
    {
      cout << "Frame " << rf.frame_number << " result: " << rf.results.plates[i].bestPlate.characters << endl;
    }
  }
}

void print_group_results(std::vector<AlprGroupResult> groups)
{
  for (uint32_t group_index = 0; group_index < groups.size(); group_index++)
  {
    AlprGroupResult group = groups[group_index];

    cout << "Group (" << group.epoch_ms_time_start << " - " << group.epoch_ms_time_end << ") " << group.best_plate_number << endl;
  }
}
