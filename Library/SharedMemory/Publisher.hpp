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
#include <stdexcept>

#include "Library/SharedMemory/ChannelHeader.hpp"

template <typename MessageType>
class Publisher {
private:
    int shmID;          // Shared memory ID
    void* sharedMemory; // Pointer to the shared memory

public:
    Publisher(key_t key, size_t capacity);
    ~Publisher();

    void Init();

    void Publish(const MessageType& message);
};

template <typename MessageType>
Publisher<MessageType>::Publisher(key_t key, size_t capacity)
    : sharedMemoryKey(key) {
    SetUp();
}

template <typename MessageType>
Publisher<MessageType>::~Publisher() {
    // Detach from shared memory
    shmdt(sharedMemory);
}

template <typename MessageType>
void Publisher<MessageType>::Init() {
    // Create or attach to shared memory segment and initialize it

    // Try to create the shared memory segment
    shmID = shmget(sharedMemoryKey, sizeof(ChannelHeader<MessageType>) + sizeof(MessageType) * capacity, IPC_CREAT | IPC_EXCL | 0666);

    if (shmID == -1) {
        // If it already exists, throw an exception
        if (errno == EEXIST) {
            throw std::runtime_error("Shared memory segment already exists.");
        } else {
            perror("shmget");
            exit(1);
        }
    }

    // Attach to shared memory
    sharedMemory = shmat(shmID, NULL, 0);
    if (sharedMemory == (void*)-1) {
        perror("shmat");
        exit(1);
    }

    // Initialize the ChannelHeader if it's a new segment
    ChannelHeader<MessageType>* header = static_cast<ChannelHeader<MessageType>*>(sharedMemory);
    if (header->capacity == 0) {
        header->capacity = capacity;
        header->start = 0;
        header->end = 0;
        header->size = 0;
        pthread_mutex_init(&(header->writerMutex), NULL);
    }
}

template <typename MessageType>
void Publisher<MessageType>::Publish(const MessageType& message) {
    // Lock the writer mutex to ensure exclusive access
    ChannelHeader<MessageType>* header = static_cast<ChannelHeader<MessageType>*>(sharedMemory);
    pthread_mutex_lock(&(header->writerMutex));

    // Calculate the offset for the new data
    char* offset = static_cast<char*>(sharedMemory) + sizeof(ChannelHeader<MessageType>) + header->end * sizeof(MessageType);

    // Serialize and copy the message
    message.SerializeToArray(offset, sizeof(MessageType));

    // Update circular buffer pointers
    header->end = (header->end + 1) % header->capacity;

    if (header->size < header->capacity) {
        header->size++;
    } else {
        // Error: Buffer is full, print an error message
        std::cerr << "Error: Buffer is full. Overwriting data." << std::endl;
        // Update the start pointer to overwrite the oldest data
        header->start = (header->start + 1) % header->capacity;
    }

    // Unlock the writer mutex
    pthread_mutex_unlock(&(header->writerMutex));
}
