#include <iostream>
#include <portaudio.h>
#include <chrono>
#include <thread>

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 512

using namespace std;
using namespace std::chrono;

// Global start time and stop flag
steady_clock::time_point startTime;

static int recordCallback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo* timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData)
{
    const float *in = (const float*)inputBuffer; // samples from mic.
    (void) outputBuffer; // prevent unused variable warnings.

    if (inputBuffer == nullptr) { // if there is no input buffer found
        cerr << "No input detected." << endl; // Error message that is immediately printed to the screen and endl is end line
    }

		// This segment just tells the code top stop running after 5 seconds.
    // Stop after 5 seconds
    auto now = steady_clock::now();
    if (duration_cast<seconds>(now - startTime).count() >= 5) {
        return paComplete; // tells PortAudio to stop the stream
    }

    return paContinue;
}

int main() {
    Pa_Initialize(); // loads the audio substem for your operating system.

    PaStream *stream; // variable where the stream data will go into.
    Pa_OpenDefaultStream(&stream,
                         1,		// input channels (mono)
                         0,		// Output channels
                         paFloat32, // Sample format
                         SAMPLE_RATE,
                         FRAMES_PER_BUFFER,
                         recordCallback,
                         nullptr); // No user data

    startTime = steady_clock::now(); 
    Pa_StartStream(stream); // begins audio processing on the stream. basically starts calling the my callback function continuously to process live audio data.

    cout << "Recording for 5 seconds..." << endl; // Outputs this line

    // Wait until the stream finishes
    while (Pa_IsStreamActive(stream) == 1) { // stop for 50 seconds when the stream is active.
        this_thread::sleep_for(milliseconds(50));
    }

    Pa_StopStream(stream); // Stop the stream.
    Pa_CloseStream(stream); // Close the stream.
    Pa_Terminate(); // Terminate all resources and free the resources. 

    cout << "Recording finished!" << endl;
    return 0;
}