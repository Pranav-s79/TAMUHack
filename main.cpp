#include <bits/stdc++.h>
#include <iostream>
#include <portaudio.h>

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 512

using namespace std;

// -------------------------------
// Author: Sidharth Kanchiraju 
// Date: 18/10/25
// Description: General C++ program template
// -------------------------------

// Type aliases
using ll = long long;
using ld = long double;
using pii = pair<int, int>;
using vi = vector<int>;


static int recordCallback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo* timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData)
{
    const float *in = (const float*)inputBuffer; // samples from mic.
    (void) outputBuffer; // Prevent unused variable warnings

    if (inputBuffer == nullptr) { // if there is no inputbuffer found
        std::cerr << "No input detected." << std::endl; // Error message that is immediately printed to the screen and endl is end line.
    } else {
        // Print the first few samples
        for (unsigned long i = 0; i < framesPerBuffer; i++) { // output each sample into a string. 
            std::cout << in[i] << " ";
        }
        std::cout << std::endl;
    }
    return paContinue;
}


// Entry point
int main() {
    Pa_Initialize(); // loads the audio subsystem for your operating system.

    PaStream *stream; // variable where the stream data will go into.
    Pa_OpenDefaultStream(&stream,
                         1,          // Input channels (mono)
                         0,          // Output channels
                         paFloat32,  // Sample format
                         SAMPLE_RATE,
                         FRAMES_PER_BUFFER,
                         recordCallback,
                         nullptr);   // No user data

    Pa_StartStream(stream); // begins audio processing on the stream. Basically Start calling my callback function continuously to process live audio data.

    std::cout << "Recording... Press Enter to stop.\n";
    std::cin.get();

    Pa_StopStream(stream); // Stops audio flow
    Pa_CloseStream(stream); // Closes the audio stream
    Pa_Terminate(); // frees the resoureces from memory

    return 0;
}