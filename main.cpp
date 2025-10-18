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
    const float *in = (const float*)inputBuffer;
    (void) outputBuffer;

    if (inputBuffer == nullptr) {
        cerr << "No input detected." << endl;
    } else {
        for (unsigned long i = 0; i < framesPerBuffer; i++) {
            cout << in[i] << " ";
        }
        cout << endl;
    }

    // Stop after 5 seconds
    auto now = steady_clock::now();
    if (duration_cast<seconds>(now - startTime).count() >= 5) {
        return paComplete; // tells PortAudio to stop the stream
    }

    return paContinue;
}

int main() {
    Pa_Initialize();

    PaStream *stream;
    Pa_OpenDefaultStream(&stream,
                         1,
                         0,
                         paFloat32,
                         SAMPLE_RATE,
                         FRAMES_PER_BUFFER,
                         recordCallback,
                         nullptr);

    startTime = steady_clock::now();
    Pa_StartStream(stream);

    cout << "Recording for 5 seconds..." << endl;

    // Wait until the stream finishes
    while (Pa_IsStreamActive(stream) == 1) {
        this_thread::sleep_for(milliseconds(50));
    }

    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    cout << "Recording finished!" << endl;
    return 0;
}