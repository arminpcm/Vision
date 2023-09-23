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
    int shm_id_;              // Shared memory ID
    void* shared_memory_;     // Pointer to the shared memory
    key_t shared_memory_Key;  // Key for the shared memory segment
    SubscriberMode mode_;    // Mode for reading messages
    std::function<void(const MessageType&)> callback_;  // Callback function

public:
    Subscriber(key_t key, SubscriberMode mode_, const std::function<void(const MessageType&)>& callback_);
    ~Subscriber();

    void Init();
    void SpinOnce();
};

template <typename MessageType>
Subscriber<MessageType>::Subscriber(key_t key, SubscriberMode mode_, const std::function<void(const MessageType&)>& callback_)
    : shared_memory_Key(key), mode_(mode), callback_(callback_) {
    Init();
}

template <typename MessageType>
Subscriber<MessageType>::~Subscriber() {
    // Detach from shared memory
    shmdt(shared_memory_);
}


template <typename MessageType>
void Subscriber<MessageType>::Init() {
    // Attach to shared memory segment

    // Try to attach to the shared memory segment
    shm_id_ = shmget(shared_memory_Key, 0, 0);

    if (shm_id_ == -1) {
        // If it doesn't exist, throw an exception
        if (errno == ENOENT) {
            throw std::runtime_error("Shared memory segment is not initialized.");
        } else {
            perror("shmget");
            exit(1);
        }
    }

    // Attach to shared memory
    shared_memory_ = shmat(shm_id_, NULL, 0);
    if (shared_memory_ == (void*)-1) {
        perror("shmat");
        exit(1);
    }
}

template <typename MessageType>
void Subscriber<MessageType>::SpinOnce() {
    // Lock the writer mutex to ensure data consistency
    ChannelHeader<MessageType>* header = static_cast<ChannelHeader<MessageType>*>(shared_memory_);
    pthread_mutex_lock(&(header->writer_mutex_));

    if (header->size_ > 0) {
        // Determine the index to read based on the mode
        size_t index = (mode_ == SubscriberMode::GET_LAST) ? ((header->end_ - 1 + header->capacity_) % header->capacity_) : header->start_;

        // Calculate the offset for reading
        char* offset = static_cast<char*>(shared_memory_) + sizeof(ChannelHeader<MessageType>) + index * sizeof(MessageType);

        // Deserialize and call the callback function
        MessageType message;
        message.ParseFromArray(offset, sizeof(MessageType));
        callback_(message);

        // Update the circular buffer pointers and size
        if (mode_ == SubscriberMode::GET_LAST) {
            header->size_--;
        } else {
            header->start_ = (header->start_ + 1) % header->capacity_;
            header->size_--;
        }
    }

    // Unlock the writer mutex
    pthread_mutex_unlock(&(header->writer_mutex_));
}
