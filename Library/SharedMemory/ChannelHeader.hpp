// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.

#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <cstring>

struct ChannelHeader {
    size_t capacity;      // Capacity of the channel
    size_t start;         // Start position of circular buffer
    size_t end;           // End position of circular buffer
    size_t size;          // Current size of the channel
    pthread_mutex_t writerMutex; // Mutex for the writer
};
