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
#include <functional>

#include "Library/SharedMemory/ChannelHeader.hpp"

// Enum for mode selection
enum class SubscriberMode {
    GET_LAST,
    GET_FIRST
};


template <typename MessageType>
class Subscriber {
private:
    int shmID;              // Shared memory ID
    void* sharedMemory;     // Pointer to the shared memory
    key_t sharedMemoryKey;  // Key for the shared memory segment
    SubscriberMode mode;    // Mode for reading messages
    std::function<void(const MessageType&)> callback;  // Callback function

public:
    Subscriber(key_t key, SubscriberMode mode, const std::function<void(const MessageType&)>& callback);
    ~Subscriber();

    void Init();
    void SpinOnce();
};

template <typename MessageType>
Subscriber<MessageType>::Subscriber(key_t key, SubscriberMode mode, const std::function<void(const MessageType&)>& callback)
    : sharedMemoryKey(key), mode(mode), callback(callback) {
    Init();
}

template <typename MessageType>
Subscriber<MessageType>::~Subscriber() {
    // Detach from shared memory
    shmdt(sharedMemory);
}


template <typename MessageType>
void Subscriber<MessageType>::Init() {
    // Attach to shared memory segment

    // Try to attach to the shared memory segment
    shmID = shmget(sharedMemoryKey, 0, 0);

    if (shmID == -1) {
        // If it doesn't exist, throw an exception
        if (errno == ENOENT) {
            throw std::runtime_error("Shared memory segment is not initialized.");
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
}

template <typename MessageType>
void Subscriber<MessageType>::SpinOnce() {
    // Lock the writer mutex to ensure data consistency
    ChannelHeader<MessageType>* header = static_cast<ChannelHeader<MessageType>*>(sharedMemory);
    pthread_mutex_lock(&(header->writerMutex));

    if (header->size > 0) {
        // Determine the index to read based on the mode
        size_t index = (mode == SubscriberMode::GET_LAST) ? ((header->end - 1 + header->capacity) % header->capacity) : header->start;

        // Calculate the offset for reading
        char* offset = static_cast<char*>(sharedMemory) + sizeof(ChannelHeader<MessageType>) + index * sizeof(MessageType);

        // Deserialize and call the callback function
        MessageType message;
        message.ParseFromArray(offset, sizeof(MessageType));
        callback(message);

        // Update the circular buffer pointers and size
        if (mode == SubscriberMode::GET_LAST) {
            header->size--;
        } else {
            header->start = (header->start + 1) % header->capacity;
            header->size--;
        }
    }

    // Unlock the writer mutex
    pthread_mutex_unlock(&(header->writerMutex));
}
